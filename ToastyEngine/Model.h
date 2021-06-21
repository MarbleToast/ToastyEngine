#pragma once
#include "Texture.h"
#include "Mesh.h"

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(const std::string_view path) {
            loadModel(path);
        }
        void Draw(Shader& shader) noexcept;
        void release();
    private:
        // model data
        std::vector<Mesh> meshes;
        std::vector<size_t> texturesLoaded;

        std::string directory;

        void loadModel(const std::string_view path) throw();
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        
};

using ModelPtr = std::shared_ptr<Model>;

