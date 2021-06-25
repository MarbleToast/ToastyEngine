#pragma once
#include "Vector.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

#include <string>
#include <vector>

struct MeshVertex {
    Vec3 position;

    Vec2 textureCoords;

    Vec3 normal;
    Vec3 tangent;
    Vec3 bitangent;
};

class Mesh {
    public:
        // mesh data
        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;
        MaterialPtr material;

        Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, MaterialPtr material);
        void Draw(Shader& shader);
        void release();

        unsigned int VAO;

    private:
        //  render data
        unsigned int VBO, EBO;

        void setupMesh();
};

using MeshPtr = std::shared_ptr<Mesh>;