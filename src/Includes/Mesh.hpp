
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

#include <Material.hpp>
#include <Shader.hpp>
#include <Texture.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace models {

struct VertexType {
  // Vertice
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec4 Color;

  // Texture
  glm::vec2 TexCoords;
  glm::vec3 Bitangent;
  glm::vec3 Tangent;
};

inline glm::mat4 aiMatrix4x4ToGlmMat4(const aiMatrix4x4 from) {
  glm::mat4 to;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      to[row][col] = from[row][col];
    }
  }

  return to;
}

class Mesh {
 private:
  std::vector<VertexType> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<models::Texture>> textures;
  glm::mat4 globalTransform;

  unsigned int VAO, VBO, EBO;

  void Setup();

 public:
  Material material;
  /// @brief Loads the mesh data from the scene and assimp mesh object.
  /// @param scene The assimp scene object.
  /// @param transform The mesh transformation.
  /// @param mesh The assimp mesh object.
  void Load(const aiScene* scene,
            const aiMesh* mesh,
            aiMatrix4x4 transform,
            std::optional<std::string> relativePath = std::nullopt);

  /// @brief Draw the mesh to the screen.
  /// @param shader The shader we want to use when drawing.
  void Draw(ShaderProgram& shader) const;
};
}  // namespace models