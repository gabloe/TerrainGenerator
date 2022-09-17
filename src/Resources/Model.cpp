
#include <Logger.hpp>
#include <Model.hpp>

#include <sstream>
#include <stdexcept>

using namespace models;

void Model::Load(std::string fileName) {
  auto constexpr flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                         aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(fileName, flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error{"Could not read the model file: " + path};
  }

  this->path = fileName;

  logging::Logger::LogDebug("Processing root node");
  ProcessNode(scene->mRootNode, scene, aiMatrix4x4{});

  logging::Logger::LogInfo("Model " + fileName + " loaded successfully");
}

void Model::Draw(ShaderProgram& shader) const {
  // LogPainful
  //  std::cout << "Rendering model " << this->path << std::endl;
  for (unsigned int i = 0; i < meshes.size(); i++) {
    meshes[i].Draw(shader);
  }
}

void Model::ProcessNode(aiNode* node,
                        const aiScene* scene,
                        aiMatrix4x4 tranformationMatrix) {
  auto nodeTransformation = tranformationMatrix * node->mTransformation;
  auto transposedNodeMatrix = nodeTransformation.Transpose();
  glm::mat4 transform;

  std::stringstream ss;
  for (int row = 0; row < 4; row++) {
    ss << "{";
    for (int col = 0; col < 4; col++) {
      transform[row][col] = transposedNodeMatrix[row][col];
      ss << transform[row][col] << (col == 3 ? "" : ", ");
    }
    ss << "}\n";
  }

  logging::Logger::LogInfo("transform:\n" + ss.str());

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    logging::Logger::LogDebug("Loading mesh " + std::to_string(i));
    Mesh mesh;
    mesh.Load(scene, scene->mMeshes[i], transform, this->path);
    this->meshes.push_back(mesh);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    logging::Logger::LogDebug("Processing child node " + std::to_string(i));
    ProcessNode(node->mChildren[i], scene, nodeTransformation);
  }
}