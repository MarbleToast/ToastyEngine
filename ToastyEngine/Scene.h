#pragma once
#include "GameObject.h"

struct ModelInstance {
	ModelPtr model;
	Transform transform;
};

class Scene {
	std::vector<ModelInstance> models;

public:
	const std::string name;
	void addModel(const ModelPtr& model, Vec3 position, Vec2 rotation);
};

