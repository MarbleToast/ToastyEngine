#include "Mesh.h"

Mesh::Mesh(
	std::vector<MeshVertex> vertices,
	std::vector<unsigned int> indices,
	MaterialPtr material
): vertices(vertices), indices(indices), material(material) {
	setupMesh();
}

void Mesh::Draw(Shader& shader) {
    for (size_t i = 0; i < 4;) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

        std::string textureName;

        switch (i) {
            case Texture::TextureType::DIFFUSE:
                textureName = "texture_diffuse";
                break;
            case Texture::TextureType::NORMAL:
                textureName = "texture_normal";
                break;
            case Texture::TextureType::SPECULAR:
                textureName = "texture_specular";
                break;
            case Texture::TextureType::DISPLACEMENT:
                textureName = "texture_displacement";
                break;
            default:
                break;
        }   
        const auto texture = material->getMaterialTexture((Texture::TextureType)i);
        shader.setInt(textureName.c_str(), texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        ++i;
    }
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, textureCoords));

    // vertex normals
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, tangent));

    // vertex texture coords
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, bitangent));

    glBindVertexArray(0);
}
