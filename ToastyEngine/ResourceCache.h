#pragma once

#ifndef RESOURCE_CACHE
#define RESOURCE_CACHE

#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Diagnostics.h"

#include <unordered_map>
#include <stb_image.h>
#include <optional>
#include <fstream>

namespace ResourceCache {

    // Caches
	inline std::unordered_map<std::string, size_t> textureCache{};
	inline std::unordered_map<std::string, MeshPtr> meshCache{};
	inline std::unordered_map<std::string, ModelPtr> modelCache{};
	inline std::unordered_map<std::string, MaterialPtr> materialCache{};
    inline std::unordered_map<std::string, Shader> shaderCache{};

    inline void releaseTexture(const size_t& id) {
        glDeleteTextures(1, &id);
    }

    inline void releaseVertexBuffer(const size_t& id) {
        glDeleteVertexArrays(1, &id);
    }

    inline void releaseShader(const size_t& id) {
        glDeleteProgram(id);
    }

    inline void releaseAll() {
        // Release all textures in a material

        for (const auto& i : materialCache) {
            i.second->release();
        }
        materialCache.clear();
            
        // Release models (and all their meshes)
        for (const auto& j : modelCache) {
            j.second->release();
        }
        modelCache.clear();
            
        for (const auto& j : shaderCache) {
            releaseShader(j.second.id);
        }
        shaderCache.clear();
    }

    inline std::optional<ModelPtr> getModel(const std::string_view path) {
        Diagnostics::Log("[{}] Looking for cached model {}...", __FUNCTION__, path);
        const auto res = modelCache.find(path.data());
        if (res != modelCache.end())
            return res->second;
        return {};
    }

    inline ModelPtr loadModel(const std::string_view path) {
        const auto cachedId = getModel(path);
        if (cachedId) return *cachedId;

        Diagnostics::Log("[{}] Loading model {}...", __FUNCTION__, path);

        return modelCache.try_emplace(path.data(), std::make_shared<Model>(path.data())).first->second;
    }

    inline std::optional<size_t> getTexture(const std::string_view path) {
        Diagnostics::Log("[{}] Looking for cached texture {}...", __FUNCTION__, path);
        const auto res = textureCache.find(path.data());
        if (res != textureCache.end())
            return res->second;
        return {};
    }

	inline size_t loadTexture(const std::string_view path) {
        const auto cachedId = getTexture(path);
        if (cachedId) return *cachedId;

        Diagnostics::Log("[{}] Loading texture {}...", __FUNCTION__, path);

        size_t id;
        glGenTextures(1, &id);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format = GL_RED;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            Diagnostics::Throw("Texture at path '{}' could not be loaded.", path);
        }

        stbi_image_free(data);

        return textureCache.try_emplace(path.data(), id).first->second;
	}

    inline std::optional<Shader> getShader(const std::string_view name) {
        Diagnostics::Log("[{}] Looking for cached shader {}...", __FUNCTION__, name);
        const auto res = shaderCache.find(name.data());
        if (res != shaderCache.end())
            return res->second;
        return {};
    }

    inline Shader loadShader(
        const std::string_view name,
        const std::string_view vertPath,
        const std::string_view fragPath,
        const std::string_view geoPath
    ) {
        const auto cachedShader = getShader(name);
        if (cachedShader) return *cachedShader;

        Diagnostics::Log("[{}] Loading shader {}...", __FUNCTION__, name);

        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vShaderFile.open(vertPath.data());
            fShaderFile.open(fragPath.data());
            std::stringstream vShaderStream, fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            // if geometry shader path is present, also load a geometry shader
            if (geoPath.data() != "") {
                std::ifstream gShaderFile;
                gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

                gShaderFile.open(geoPath.data());
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& err) {
            Diagnostics::Throw("Shader file could not be read: {}.", err.what());
        }

        return shaderCache.try_emplace(
            name.data(),
            Shader(vertexCode.c_str(), fragmentCode.c_str(), geometryCode != "" ? geometryCode.c_str() : nullptr)
        ).first->second;
    }

    inline std::optional<MaterialPtr> getMaterial(const std::string_view name) {
        const auto res = materialCache.find(name.data());
        if (res != materialCache.end())
            return res->second;
        return {};
    }

    inline MaterialPtr createMaterial(
        const std::string_view name,
        const std::string_view diffusePath,
        const std::string_view normalPath,
        const std::string_view specularPath,
        const std::string_view displacementPath
    ) {
        const auto cachedName = getMaterial(name);
        if (cachedName) return *cachedName;

        Diagnostics::Log("[{}] Creating material {}...", __FUNCTION__, name);

        return materialCache.try_emplace(
            name.data(),
            std::make_shared<Material>(
                Material(name, diffusePath, normalPath, specularPath, displacementPath)
            )
        ).first->second;
    }
};

#endif
