#include <Logger.hpp>
#include <Material.hpp>
#include <ResourceManager.hpp>

using namespace models;

std::string to_string(aiShadingMode mode) {
  switch (mode) {
    case aiShadingMode::aiShadingMode_Blinn:
      return "Blinn";
    case aiShadingMode::aiShadingMode_CookTorrance:
      return "CookTorrance";
    case aiShadingMode::aiShadingMode_Flat:
      return "Flat";
    case aiShadingMode::aiShadingMode_Fresnel:
      return "Fresnel";
    case aiShadingMode::aiShadingMode_Gouraud:
      return "Gouraud";
    case aiShadingMode::aiShadingMode_Minnaert:
      return "Minnaert";
    case aiShadingMode::aiShadingMode_NoShading:
      return "NoShading";
    case aiShadingMode::aiShadingMode_OrenNayar:
      return "OrenNayar";
    case aiShadingMode::aiShadingMode_PBR_BRDF:
      return "PBR_BRDF";
    case aiShadingMode::aiShadingMode_Phong:
      return "Phong";
    case aiShadingMode::aiShadingMode_Toon:
      return "Toon";
    default:
      return "UNKNOWN";
  }
}

float ReadFloat(aiMaterial* material,
                const char* key,
                unsigned int type,
                unsigned int index,
                std::string name,
                float defaultValue) {
  float result;
  if (material->Get(key, type, index, result) != AI_SUCCESS) {
    logging::Logger::LogInfo(name + " not provided.");
    return defaultValue;
  }
  logging::Logger::LogInfo(name + " set to " + std::to_string(result));

  return result;
}

int ReadInt(aiMaterial* material,
            const char* key,
            unsigned int type,
            unsigned int index,
            std::string name,
            int defaultValue) {
  int result;
  if (AI_SUCCESS != aiGetMaterialInteger(material, key, type, index, &result)) {
    logging::Logger::LogInfo(name + " not set.");
    return defaultValue;
  }

  logging::Logger::LogInfo(name + " set to " + std::to_string(result));
  return result;
}

void ReadColor(aiMaterial* material,
               const char* colorName,
               unsigned int type,
               unsigned int index,
               aiColor4D& output,
               std::string name,
               aiColor4D defaultValue) {
  if (AI_SUCCESS !=
      aiGetMaterialColor(material, colorName, type, index, &output)) {
    output = defaultValue;
    logging::Logger::LogDebug(name +
                              " color not found on mesh. Setting to default.");
  }

  logging::Logger::LogDebug(name + " color: Red=" + std::to_string(output.r) +
                            " Green=" + std::to_string(output.g) +
                            " Blue=" + std::to_string(output.b));
}

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
  ShininessStrength = ReadFloat(_material, AI_MATKEY_SHININESS_STRENGTH,
                                "ShininessStrength", 1.0f);
}

void Material::SetShininess() {
  Shininess = ReadFloat(_material, AI_MATKEY_SHININESS, "Shininess", 1.0f);
}

void Material::SetOpacity() {
  Opacity = ReadFloat(_material, AI_MATKEY_OPACITY, "Opacity", 1.0f);
}

void Material::SetShadingMode() {
  ShadingMode = (aiShadingMode)ReadInt(_material, AI_MATKEY_SHADING_MODEL,
                                       "Shading model", aiShadingMode_Gouraud);

  // Have to log here because the generic logging in ReadInt does not provide
  // enough information.
  logging::Logger::LogInfo("Shading mode set to " + to_string(ShadingMode));
}

void Material::SetColor() {
  MaterialColor result;

  ReadColor(_material, AI_MATKEY_COLOR_DIFFUSE, result.diffuse, "Diffuse",
            aiColor4D(1.0f, 1.0f, 1.0f, 1.0f));

  ReadColor(_material, AI_MATKEY_COLOR_SPECULAR, result.specular, "Specular",
            aiColor4D(1.0f, 1.0f, 1.0f, 1.0f));

  ReadColor(_material, AI_MATKEY_COLOR_AMBIENT, result.ambient, "Ambient",
            aiColor4D(1.0f, 1.0f, 1.0f, 1.0f));

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
