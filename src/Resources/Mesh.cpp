
#include <Mesh.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <ResourceManager.hpp>

#include <Logger.hpp>

using namespace models;

void Mesh::Load(const aiScene* scene,
                const aiMesh* mesh,
                aiMatrix4x4 transform,
                std::optional<std::string> relativePath) {
  // Load the material
  material = Material(scene, mesh);

  const auto num_vertices = mesh->mNumVertices;
  const auto color_channels = mesh->GetNumColorChannels();
  const auto has_normals = mesh->HasNormals();
  const auto has_tangents_and_bitangents = mesh->HasTangentsAndBitangents();

  logging::Logger::LogDebug("Mesh has " + std::to_string(num_vertices) +
                            " vertices.");

  logging::Logger::LogDebug("Mesh contains " + std::to_string(color_channels) +
                            " color channels.");

  logging::Logger::LogDebug("Mesh has normals: " + std::to_string(has_normals));

  logging::Logger::LogDebug("Mesh has tangents and bitangets: " +
                            std::to_string(has_tangents_and_bitangents));

  int print = 0;

  for (unsigned int i = 0; i < num_vertices; ++i) {
    double per_done = (100 * i) / num_vertices;

    if (per_done == print) {
      logging::Logger::LogDebug(std::to_string(per_done) + "%");
      print += 5;
    }

    VertexType vert;

    glm::mat4 trans = aiMatrix4x4ToGlmMat4(transform);

    glm::vec4 tmp;
    tmp.x = mesh->mVertices[i].x;
    tmp.y = mesh->mVertices[i].y;
    tmp.z = mesh->mVertices[i].z;
    tmp.w = 1;
    tmp = trans * tmp;
    vert.Position = tmp;

    if (has_normals) {
      glm::vec4 tmp;
      tmp.x = mesh->mNormals[i].x;
      tmp.y = mesh->mNormals[i].y;
      tmp.z = mesh->mNormals[i].z;
      tmp.w = 1;
      tmp = trans * tmp;
      vert.Normal = tmp;
    }

    if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0]) {
      // a vertex can contain up to 8 different texture coordinates. We thus
      // make the assumption that we won't use models where a vertex can have
      // multiple texture coordinates so we always take the first set (0).
      vert.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vert.TexCoords.y = mesh->mTextureCoords[0][i].y;
    } else {
      vert.TexCoords.x = 0.0;
      vert.TexCoords.y = 0.0;
    }

    if (has_tangents_and_bitangents) {
      // tangent
      vert.Tangent.x = mesh->mTangents[i].x;
      vert.Tangent.y = mesh->mTangents[i].y;
      vert.Tangent.z = mesh->mTangents[i].z;

      // bitangent
      vert.Bitangent.x = mesh->mBitangents[i].x;
      vert.Bitangent.y = mesh->mBitangents[i].y;
      vert.Bitangent.z = mesh->mBitangents[i].z;
    }

    // Set the vertex color, if defined
    aiColor4D vertex_color = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    if (mesh->HasVertexColors(0) && mesh->mColors[0]) {
      vertex_color = *mesh->mColors[0];
    }
    vert.Color.r = vertex_color.r;
    vert.Color.g = vertex_color.g;
    vert.Color.b = vertex_color.b;
    vert.Color.w = vertex_color.a;

    vertices.push_back(vert);
  }

  logging::Logger::LogDebug("Scene HasMaterials: " +
                            std::to_string(scene->HasMaterials()));
  if (scene->HasMaterials()) {
    // TODO: Handle n > 1
    auto material = scene->mMaterials[0];

    auto manager = resources::ResourceManager::GetManager();

    std::vector<std::shared_ptr<Texture>> diffuseMaps = manager.LoadTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse", relativePath);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<std::shared_ptr<Texture>> specularMaps = manager.LoadTextures(
        material, aiTextureType_SPECULAR, "texture_specular", relativePath);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. Normal maps
    std::vector<std::shared_ptr<Texture>> normalMaps = manager.LoadTextures(
        material, aiTextureType_HEIGHT, "texture_normal", relativePath);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<std::shared_ptr<Texture>> heightMaps = manager.LoadTextures(
        material, aiTextureType_AMBIENT, "texture_height", relativePath);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  }

  logging::Logger::LogDebug("Mesh has " + std::to_string(mesh->mNumFaces) +
                            " faces");

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // retrieve all indices of the face and store them in the indices vector
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  logging::Logger::LogDebug("Mesh has " + std::to_string(indices.size()) +
                            " indices");

  // set up the buffers
  Setup();
}

void Mesh::Setup() {
  // create buffers/arrays
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // A great thing about structs is that their memory layout is sequential for
  // all its items. The effect is that we can simply pass a pointer to the
  // struct and it translates perfectly to a glm::vec3/2 array which again
  // translates to 3/2 floats which translates to a byte array.
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
               &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  int idx = 0;

  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Normal));

  // vertex color
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Color));

  // vertex texture coords
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, TexCoords));

  // vertex tangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Tangent));

  // vertex bitangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType),
                        (void*)offsetof(VertexType, Bitangent));

  glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram& shader) const {
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i);  // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures[i]->Type();

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);  // transfer unsigned int to stream
    else if (name == "texture_normal")
      number = std::to_string(normalNr++);  // transfer unsigned int to stream
    else if (name == "texture_height")
      number = std::to_string(heightNr++);  // transfer unsigned int to stream

    // now set the sampler to the correct texture unit
    auto location =
        glGetUniformLocation(shader.getHandle(), (name + number).c_str());

    glUniform1i(location, i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures[i]->Id());
  }

  // Set up material properties
  shader.setUniform("material.ambient", material.Color.to_vec4(AMBIENT));
  shader.setUniform("material.diffuse", material.Color.to_vec4(DIFFUSE));
  shader.setUniform("material.specular", material.Color.to_vec4(SPECULAR));
  shader.setUniform("material.shininess", material.Shininess);
  shader.setUniform("material.shininess_strength", material.ShininessStrength);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}