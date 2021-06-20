#include <stb_image.h>

#include "Model.h"
#include "Diagnostics.h"

void Model::Draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes.size();) {
        meshes[i].Draw(shader);
        ++i;
    }
}

void Model::loadModel(const std::string_view path) {
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
    std::vector<Texture> textures;

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

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::TextureType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::TextureType::NORMAL);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, Texture::TextureType::DISPLACEMENT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<size_t> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::TextureType internalType) {
    std::vector<size_t> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type);) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string fullPath = directory + '/' + str.C_Str();

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size();) {
            if (std::strcmp(texturesLoaded[j], fullPath.c_str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
            ++j;
        }

        if (!skip) {   
            // if texture hasn't been loaded already, load it

            Texture texture(fullPath.c_str(), internalType);
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
        ++i;
    }
    return textures;
}
