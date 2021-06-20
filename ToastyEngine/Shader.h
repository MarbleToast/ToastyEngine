#pragma once

#include "Matrix.h"
#include <string>

class Shader
{
public:
	size_t id;

	Shader(
		const char* vertPath,
		const char* fragPath,
		const char* geoPath = nullptr
	);

	void activate();

	void setInt(const std::string& name, const int& value);
	void setMat4(const std::string& name, const Mat4& mat);
	void setFloat(const std::string& name, const float& value);


private:
	void errorCheck(size_t shader, const char* type);
};

