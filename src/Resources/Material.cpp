#include <Material.hpp>

using namespace models;

const aiMaterial* GetMaterial(const aiScene* scene, const aiMesh* mesh) {
    return scene->mMaterials[mesh->mMaterialIndex];
}