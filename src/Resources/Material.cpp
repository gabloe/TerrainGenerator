#include <Material.hpp>
#include <Logger.hpp>

using namespace models;

Material::Material(const aiScene* scene, const aiMesh* mesh)
  : _material{scene->mMaterials[mesh->mMaterialIndex]} {

  SetColor();
  SetShininess();
  SetShininessStrength();
}

void Material::SetShininessStrength() {
  float shininess_strength = 1.0f;
  _material->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength);
  logging::Logger::LogDebug("Material has shininess strength " + std::to_string(shininess_strength));
  ShininessStrength = shininess_strength;
}

void Material::SetShininess() {
  float shininess = 1.0f;
  _material->Get(AI_MATKEY_SHININESS, shininess);
  logging::Logger::LogDebug("Material has shininess " + std::to_string(shininess));
  Shininess = shininess;
}

const MaterialColor Material::SetColor() {
  MaterialColor result;

  // Diffuse component
  if (AI_SUCCESS != aiGetMaterialColor(_material, AI_MATKEY_COLOR_DIFFUSE, &result.diffuse)) {
    result.diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
      "Mesh does not have a diffuse color. Using default: Red=" + std::to_string(result.diffuse.r) +
      " Green=" + std::to_string(result.diffuse.g) +
      " Blue=" + std::to_string(result.diffuse.b));
  } else {
    logging::Logger::LogDebug(
      "Mesh has a diffuse color: Red=" + std::to_string(result.diffuse.r) +
      " Green=" + std::to_string(result.diffuse.g) +
      " Blue=" + std::to_string(result.diffuse.b));
  }

  // Specular component
  if (AI_SUCCESS != aiGetMaterialColor(_material, AI_MATKEY_COLOR_SPECULAR, &result.specular)) {
    result.specular = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
      "Mesh does not have a ambient color. Using default: Red=" + std::to_string(result.specular.r) +
      " Green=" + std::to_string(result.specular.g) +
      " Blue=" + std::to_string(result.specular.b));
  } else {
    logging::Logger::LogDebug(
      "Mesh has a ambient color: Red=" + std::to_string(result.specular.r) +
      " Green=" + std::to_string(result.specular.g) +
      " Blue=" + std::to_string(result.specular.b));
  }

  // Ambient component
  if (AI_SUCCESS != aiGetMaterialColor(_material, AI_MATKEY_COLOR_AMBIENT, &result.ambient)) {
    result.ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    logging::Logger::LogDebug(
      "Mesh does not have a ambient color. Using default: Red=" + std::to_string(result.ambient.r) +
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
