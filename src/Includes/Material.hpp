#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <glm/vec4.hpp>

namespace models {

  enum MaterialColorType {
    DIFFUSE,
    SPECULAR,
    AMBIENT,
    NONE
  };

  struct MaterialColor {
    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    const glm::vec4 to_vec4(MaterialColorType type) const {
      switch(type) {
        case DIFFUSE: 
          return glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
        case SPECULAR:
          return glm::vec4(specular.r, specular.g, specular.b, specular.a);
        case AMBIENT:
          return glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
        default: 
          return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Return white as a default
      }
    }
  };
  
  class Material{
    public:
      MaterialColor Color;
      float Shininess;
      float ShininessStrength;

      Material(){};
      Material(const aiScene*, const aiMesh*);

    private:
      const aiMaterial* _material;
      const aiScene* _scene;
      const aiMesh* _mesh;

    protected:
      const MaterialColor GetColor();
      const float GetShininess();
      const float GetShininessStrength();
  };
}