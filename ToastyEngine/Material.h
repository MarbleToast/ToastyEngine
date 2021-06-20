#pragma once
#include <string_view>
#include "Texture.h"
class Material {
	const Texture materialTextures[4];

	public:
		std::string_view name;
		Material(
			const std::string_view name,
			const std::string_view diffusePath,
			const std::string_view specularPath,
			const std::string_view normalPath,
			const std::string_view heightPath
		);
};

using MaterialPtr = std::shared_ptr<Material>;

