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

namespace ResourceCache {
	inline std::unordered_map<std::string, size_t> textureCache{};
	inline std::unordered_map<std::string, MeshPtr> meshCache{};
	inline std::unordered_map<std::string, ModelPtr> modelCache{};
	inline std::unordered_map<std::string, MaterialPtr> materialCache{};

    inline void releaseTexture(const size_t& id) {
        glDeleteTextures(1, &id);
    }

    inline void releaseVertexBuffer(const size_t& id) {
        glDeleteVertexArrays(1, &id);
    }

    inline void releaseAll() {
        // Release all textures in a material
        for (auto i : materialCache) 
            i.second->release();

        // Release models (and all their meshes)
        for (auto j : modelCache)
            j.second->release();
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
