
#include <Logger.hpp>
#include <Model.hpp>

#include <sstream>
#include <stdexcept>

using namespace models;

void Model::Load(std::string fileName) {
  auto constexpr flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                         aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                         aiProcess_MakeLeftHanded;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(fileName, flags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error{"Could not read the model file: " + path};
  }

  this->path = fileName;

  // Scale it back up
  logging::Logger::LogInfo("Processing root node");
  {
    auto scale = scene->mRootNode->mTransformation[0][0] == 0.0
                     ? 1.0 / scene->mRootNode->mTransformation[2][2]
                     : 1.0 / scene->mRootNode->mTransformation[0][0];

    aiMatrix4x4 mul;
    mul[0][0] = scale;
    mul[1][1] = scale;
    mul[2][2] = scale;
    scene->mRootNode->mTransformation = mul * scene->mRootNode->mTransformation;
  }

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

void Model::ProcessNode(aiNode* node, const aiScene* scene, aiMatrix4x4 mat) {
  // Parent * Me (This is what the assimp tool does)
  auto modelMatrix = mat * node->mTransformation;

  logging::Logger::LogInfo("Meshes: " + std::to_string(node->mNumMeshes));
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    logging::Logger::LogInfo("Loading mesh " +
                             std::to_string(node->mMeshes[i]));
    Mesh mesh;
    mesh.Load(scene, scene->mMeshes[node->mMeshes[i]], modelMatrix, this->path);
    this->meshes.push_back(mesh);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    logging::Logger::LogInfo("Processing child node " + std::to_string(i));
    ProcessNode(node->mChildren[i], scene, modelMatrix);
  }
}