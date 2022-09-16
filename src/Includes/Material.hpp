#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace models {

    const aiMaterial* GetMaterial(const aiScene*, const aiMesh*)


    aiColor4D material_diffuse_color = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
    if (AI_SUCCESS !=
            aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &material_diffuse_color) &&
        mesh->HasVertexColors(0) && mesh->mColors[0]) {
        material_diffuse_color = *mesh->mColors[0];

        logging::Logger::LogDebug(
            "Mesh does not have a diffuse color, using default: Red=" +
            std::to_string(material_diffuse_color.r) +
            " Green=" + std::to_string(material_diffuse_color.g) +
            " Blue=" + std::to_string(material_diffuse_color.b));
    } else {
        logging::Logger::LogDebug(
            "Mesh has a diffuse color: Red=" + std::to_string(material_diffuse_color.r) +
            " Green=" + std::to_string(material_diffuse_color.g) +
            " Blue=" + std::to_string(material_diffuse_color.b));
    }
}