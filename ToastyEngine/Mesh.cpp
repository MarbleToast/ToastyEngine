#include "Mesh.h"

Mesh::Mesh(
	std::vector<MeshVertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures
): vertices(vertices), indices(indices), textures(textures) {
	setupMesh();
}

void Mesh::Draw(Shader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int displacementNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

        // retrieve texture number (the N in diffuse_textureN)
        Texture::TextureType type = textures[i].type;
        std::string name;

        switch (type) {
            case Texture::TextureType::DIFFUSE:
                name = "texture_diffuse" + std::to_string(diffuseNr++);
                break;
            case Texture::TextureType::NORMAL:
                name = "texture_normal" + std::to_string(specularNr++);
                break;
            case Texture::TextureType::SPECULAR:
                name = "texture_specular" + std::to_string(diffuseNr++);
                break;
            case Texture::TextureType::DISPLACEMENT:
                name = "texture_displacement" + std::to_string(diffuseNr++);
                break;
            default:
                break;
        }   

        shader.setInt(name.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
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
