
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

inline glm::mat4 aiMatrix4x4ToGlmMat4(const aiMatrix4x4 from) {
  glm::mat4 to;

  to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
  to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
  to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
  to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

  return to;
}

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
            aiMatrix4x4 transform,
            std::optional<std::string> relativePath = std::nullopt);

  /// @brief Draw the mesh to the screen.
  /// @param shader The shader we want to use when drawing.
  void Draw(ShaderProgram& shader) const;
};
}  // namespace models