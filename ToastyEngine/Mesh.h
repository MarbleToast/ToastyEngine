#pragma once
#include "Vector.h"
#include "Shader.h"
#include "Texture.h"

#include <string>
#include <vector>

struct PosVertex {
    Vec3 position;
};

struct TexCoordVertex {
    Vec2 texCoords;
};

struct DiffVertex {
    Vec3 normal;
    Vec3 tangent;
    Vec3 bitangent;
};

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
        std::vector<Texture> textures;

        Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);

        unsigned int VAO;

    private:
        //  render data
        unsigned int VBO, EBO;

        void setupMesh();
};

using MeshPtr = std::shared_ptr<Mesh>;