#include <stb_image.h>

#include "Model.h"
#include "Diagnostics.h"
#include "ResourceCache.h"

void Model::Draw(Shader& shader) noexcept {
    for (unsigned int i = 0; i < meshes.size();) {
        meshes[i].Draw(shader);
        ++i;
    }
}

void Model::loadModel(const std::string_view path) throw() {
	Assimp::Importer assimp;
	const aiScene* scene = assimp.ReadFile(
        std::string{ path },
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices
    );

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Diagnostics::Throw("Model could not be loaded: {}", assimp.GetErrorString());
    }
    else { 
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));
        directory += '/';
        processNode(scene->mRootNode, scene); 
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes;) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        ++i;
    }
    for (size_t i = 0; i < node->mNumChildren;) {
        processNode(node->mChildren[i], scene);
        ++i;
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<size_t> textures;

    for (size_t i = 0; i < mesh->mNumVertices;) {
        MeshVertex vertex;
        Vec3 vector;
        
        // vertices
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            Vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.textureCoords = vec;

            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
            vertex.textureCoords = { 0.0f, 0.0f };

        vertices.push_back(vertex);
        ++i;
    }

    for (unsigned int i = 0; i < mesh->mNumFaces;) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices;) {
            indices.push_back(face.mIndices[j]);
            ++j;
        }
        ++i;
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::string materialName = material->GetName().C_Str();
    
    const auto cachedMaterial = ResourceCache::getMaterial(materialName);
    if (cachedMaterial) {
        return Mesh(vertices, indices, *cachedMaterial);
    }

    aiString diffuseTexName;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexName) == AI_FAILURE) {
        Diagnostics::Throw("[{}] Diffuse texture get returned failure.", __FUNCTION__);
    }

    aiString normalTexName;
    if (!material->GetTexture(aiTextureType_HEIGHT, 0, &normalTexName) == AI_FAILURE) {
        Diagnostics::Throw("[{}] Normal map texture get returned failure.", __FUNCTION__);
    }

    aiString specularTexName;
    if (!material->GetTexture(aiTextureType_SPECULAR, 0, &specularTexName) == AI_FAILURE) {
        Diagnostics::Throw("[{}] Specular map texture get returned failure.", __FUNCTION__);
    }

    aiString displacementTexName;
    if (!material->GetTexture(aiTextureType_SHININESS, 0, &displacementTexName) == AI_FAILURE) {
        Diagnostics::Throw("[{}] Displacement texture get returned failure.", __FUNCTION__);
    }

    MaterialPtr newMaterial = ResourceCache::createMaterial(
        materialName,
        directory + diffuseTexName.C_Str(),
        normalTexName.length == 0 ? "" : directory + normalTexName.C_Str(),
        specularTexName.length == 0 ? "" : directory + specularTexName.C_Str(),
        displacementTexName.length == 0 ? "" : directory + displacementTexName.C_Str()
    );

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, newMaterial);
}

void Model::release() {
    for (auto& m : meshes) {
        m.release();
    }
}
