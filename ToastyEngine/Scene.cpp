#include "Scene.h"

void Scene::addModel(const ModelPtr& model, Vec3 position, Vec2 rotation) {
	models.push_back({ model, {position, rotation} });
}
