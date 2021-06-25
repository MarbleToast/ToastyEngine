#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Window.h"

#include <vector>
#include <unordered_map>

class Renderer {
private:
	std::unordered_map<GameObject*, const std::string> renderQueue = {};
	
public:
	Renderer();

	void AddModel(ModelPtr model, const std::string_view shaderName, Transform transform);
	void Draw(Window* window, Camera* camera);
	void release();
};

