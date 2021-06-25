#include "Renderer.h"
#include "ResourceCache.h"

#include "include/glm/gtc/matrix_transform.hpp"


Renderer::Renderer()
{
}

void Renderer::AddModel(ModelPtr model, const std::string_view shaderName, Transform transform) {
	Diagnostics::Log("[{}] Adding model {} to render queue", __FUNCTION__, model->modelName);
	size_t instanceCount = 0;
	for (const auto i : renderQueue) {
		if (i.first->identifier == model->modelName) ++instanceCount;
	}
	renderQueue.try_emplace(
		new GameObject((instanceCount > 0 ? model->modelName + std::to_string(instanceCount) : model->modelName), transform, model),
		shaderName.data()
	);
}

void Renderer::Draw(Window* window, Camera* camera) {

	for (const auto i : renderQueue) {

		if (i.first->renderModel != nullptr) {

			auto shaderResult = ResourceCache::getShader(i.second);
			if (shaderResult) {
				shaderResult->activate();

				shaderResult->setMat4("projection", camera->getFrustrum());
				shaderResult->setMat4("view", camera->getViewMatrix());

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, i.first->transform.position);
				model = glm::rotate(model, i.first->transform.rotation.x, i.first->up);
				model = glm::rotate(model, i.first->transform.rotation.y, i.first->right);
				model = glm::scale(model, i.first->transform.scale);
				shaderResult->setMat4("model", model);

				
				i.first->renderModel->Draw(*shaderResult);
			}
			else {
				Diagnostics::Throw("[{}] Shader {} could not be fetched", __FUNCTION__, i.second);
			}
		}
		else {
			Diagnostics::Log("[WARNING][{}] Modelless object {} in render queue", __FUNCTION__, i.second);
			continue;
		}
	}
}

void Renderer::release() {
	renderQueue.clear();
}
