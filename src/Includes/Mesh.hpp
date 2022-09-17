
#pragma once

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

class Mesh {
 private:
  std::vector<VertexType> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<models::Texture>> textures;

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
            glm::mat4 transform,
            std::optional<std::string> relativePath = std::nullopt);

  /// @brief Draw the mesh to the screen.
  /// @param shader The shader we want to use when drawing.
  void Draw(ShaderProgram& shader) const;
};
}  // namespace models