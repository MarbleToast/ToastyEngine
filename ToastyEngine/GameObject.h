#pragma once
#include "Vector.h"
#include "Model.h"
#include <string>
#include <string_view>

const float DEFAULT_YAW = -90.f;
const float DEFAULT_PITCH = 0.f;

struct Transform {
	// Position in world space.
	Vec3 position;
	Vec2 rotation;
};

class GameObject {
protected:
	/*
		Parent object.
		All movement is shared from the parent to the child object(but not vice versa).
	*/ 
	GameObject* parent = nullptr;

	// Model to be rendered.
	Model* renderModel = nullptr;
	
	Transform transform;

	// Vector pointing forward relative to object's rotation.
	Vec3 front;
	// Vector pointing right relative to object's rotation.
	Vec3 right;
	// Vector pointing up relative to object's rotation.
	Vec3 up;

	// Vector pointing up in world space.
	Vec3 worldUp = Vec3(0.f, 1.f, 0.f);
	
	// Recalculates relative direction vectors based on object's pitch and yaw.
	void updateVectors();
public:
	const std::string identifier;

	GameObject(const std::string_view id, Vec3 pos);
	
	virtual void update();
	virtual void handleInput();

	constexpr GameObject* getParent() const;
	void setParent(GameObject& obj);
};

