#pragma once

#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Diagnostics.h"

#include <unordered_map>
#include <stb_image.h>

namespace ResourceCache {
	inline std::unordered_map<std::string, size_t> textureCache{};
	inline std::unordered_map<std::string, MeshPtr> meshCache{};
	inline std::unordered_map<std::string, ModelPtr> modelCache{};
	inline std::unordered_map<std::string, MaterialPtr> materialCache{};

	inline size_t loadTexture(const std::string_view path) {
        const auto res = textureCache.find(path.data());
        if (res != textureCache.end()) 
            return res->second;

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
};

