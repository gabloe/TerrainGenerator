#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <glm/vec4.hpp>

#include <Shader.hpp>
#include <Texture.hpp>

#include <memory>

namespace models {

enum MaterialColorType { DIFFUSE, SPECULAR, AMBIENT, NONE };

struct MaterialColor {
  aiColor4D diffuse;
  aiColor4D specular;
  aiColor4D ambient;
  const glm::vec4 to_vec4(MaterialColorType type) const {
    switch (type) {
      case DIFFUSE:
        return glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
      case SPECULAR:
        return glm::vec4(specular.r, specular.g, specular.b, specular.a);
      case AMBIENT:
        return glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
      default:
        return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // Return white as a default
    }
  }
};

class Material {
 public:
  MaterialColor Color;
  float Shininess;
  float ShininessStrength;
  float Opacity;

  Material(){};
  Material(const aiScene*, const aiMesh*, const std::optional<std::string>);

  void Draw(ShaderProgram&) const;

 private:
  std::optional<std::string> _relativePath;
  std::vector<std::shared_ptr<models::Texture>> _textures;
  aiMaterial* _material;
  aiShadingMode ShadingMode;

  void SetColor();
  void SetShininess();
  void SetShininessStrength();
  void LoadTextures();
  void SetOpacity();
  void SetShadingMode();
};
}  // namespace models