#pragma once

#include "GameObject.h"

#include <vector>

class Renderer {
private:
	std::vector<GameObject*> renderQueue;
public:
	Renderer();
};

