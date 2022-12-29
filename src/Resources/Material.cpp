#include <Logger.hpp>
#include <Material.hpp>
#include <ResourceManager.hpp>

using namespace models;

static std::map<aiTextureType, std::string> texturesToAdd = {
    {aiTextureType_DIFFUSE, "diffuse"},
    {aiTextureType_SPECULAR, "specular"},
    {aiTextureType_AMBIENT, "ambient"},
    {aiTextureType_EMISSIVE, "emissive"},
    {aiTextureType_SHININESS, "shininess"},
    {aiTextureType_LIGHTMAP, "light_map"},
    {aiTextureType_NORMALS, "height"},
    {aiTextureType_HEIGHT, "height"},
};

Material::Material(const aiScene* scene,
                   const aiMesh* mesh,
                   const std::optional<std::string> relativePath)
    : _material{scene->mMaterials[mesh->mMaterialIndex]},
      _relativePath{relativePath} {
  logging::Logger::LogInfo("Mesh is using material " +
                           std::to_string(mesh->mMaterialIndex));
  SetColor();
  SetShininess();
  SetShininessStrength();
  LoadTextures();
}

void Material::SetShininessStrength() {
  float shininess_strength = 1.0f;
  _material->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
  logging::Logger::LogDebug("Material has shininess strength " +
                            std::to_string(shininess_strength));
  ShininessStrength = shininess_strength;
}

void Material::SetShininess() {
  float shininess = 1.0f;
  _material->Get(AI_MATKEY_SHININESS, shininess);
  logging::Logger::LogDebug("Material has shininess " +
                            std::to_string(shininess));
  Shininess = shininess;
}

void Material::SetColor() {
  MaterialColor result;

  // Diffuse component
  if (AI_SUCCESS !=
      aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &result.diffuse)) {
    result.diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
        "Mesh does not have a diffuse color. Using default: Red=" +
        std::to_string(result.diffuse.r) +
        " Green=" + std::to_string(result.diffuse.g) +
        " Blue=" + std::to_string(result.diffuse.b));
  } else {
    logging::Logger::LogDebug(
        "Mesh has a diffuse color: Red=" + std::to_string(result.diffuse.r) +
        " Green=" + std::to_string(result.diffuse.g) +
        " Blue=" + std::to_string(result.diffuse.b));
  }

  // Specular component
  if (AI_SUCCESS != aiGetMaterialColor(_material, AI_MATKEY_COLOR_SPECULAR,
                                       &result.specular)) {
    result.specular = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
        "Mesh does not have a ambient color. Using default: Red=" +
        std::to_string(result.specular.r) +
        " Green=" + std::to_string(result.specular.g) +
        " Blue=" + std::to_string(result.specular.b));
  } else {
    logging::Logger::LogDebug(
        "Mesh has a ambient color: Red=" + std::to_string(result.specular.r) +
        " Green=" + std::to_string(result.specular.g) +
        " Blue=" + std::to_string(result.specular.b));
  }

  // Ambient component
  if (AI_SUCCESS !=
      aiGetMaterialColor(_material, AI_MATKEY_COLOR_AMBIENT, &result.ambient)) {
    result.ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
        "Mesh does not have a ambient color. Using default: Red=" +
        std::to_string(result.ambient.r) +
        " Green=" + std::to_string(result.ambient.g) +
        " Blue=" + std::to_string(result.ambient.b));
  } else {
    logging::Logger::LogDebug(
        "Mesh has a ambient color: Red=" + std::to_string(result.ambient.r) +
        " Green=" + std::to_string(result.ambient.g) +
        " Blue=" + std::to_string(result.ambient.b));
  }

  Color = result;
}

void Material::LoadTextures() {
  auto manager = resources::ResourceManager::GetManager();
  for (auto kvp : texturesToAdd) {
    auto maps = manager.LoadTextures(this->_material, kvp.first, kvp.second,
                                     this->_relativePath);
    this->_textures.insert(this->_textures.end(), maps.begin(), maps.end());
  }
}

void Material::Draw(ShaderProgram& shader) const {
  if (this->_textures.size() > 0) {
    shader.setUniform("material.isTextured", 1);
  } else {
    shader.setUniform("material.isTextured", 0);
  }

  for (int i = 0; i < this->_textures.size(); i++) {
    std::string type = this->_textures[i]->Type();
    if (type == "diffuse") {
      shader.setUniform("material.diffuseTex", i);
    } else if (type == "specular") {
      shader.setUniform("material.specularTex", i);
    }

    // active proper texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, this->_textures[i]->Id());
  }
}
