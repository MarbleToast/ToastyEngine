#pragma once
#include <string_view>
#include "Texture.h"
#include <array>

class Material {
	std::array<size_t, 4> materialTextures;

	public:
		std::string_view name;
		Material(
			const std::string_view name,
			const std::string_view diffusePath,
			const std::string_view normalPath,
			const std::string_view specularPath,
			const std::string_view displacementPath
		);

		size_t getMaterialTexture(const Texture::TextureType type) const noexcept;
};

using MaterialPtr = std::shared_ptr<Material>;

