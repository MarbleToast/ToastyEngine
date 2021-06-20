#pragma once
#include <string>
#include <memory>

class Texture
{
public:
	const enum class TextureType {
		DIFFUSE,
		NORMAL,
		SPECULAR,
		DISPLACEMENT
	};
	size_t id;
	std::string path;
	TextureType type;

	Texture(const std::string& path, const TextureType& type);

	constexpr bool operator==(const Texture& t) const { return id == t.id; };
};

using TexturePtr = std::shared_ptr<Texture>;


