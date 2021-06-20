#include "Material.h"
#include "ResourceCache.h"

Material::Material(
	const std::string_view name,
	const std::string_view diffusePath,
	const std::string_view normalPath,
	const std::string_view specularPath,
	const std::string_view displacementPath
) 
	: materialTextures{
		ResourceCache::loadTexture(diffusePath),
		normalPath == "" ? 0 : ResourceCache::loadTexture(normalPath),
		specularPath == "" ? 0 : ResourceCache::loadTexture(specularPath),
		displacementPath == "" ? 0 : ResourceCache::loadTexture(displacementPath)
	},
	name(name) 
{}

size_t Material::getMaterialTexture(const Texture::TextureType type) const noexcept {
	return materialTextures[type];
}