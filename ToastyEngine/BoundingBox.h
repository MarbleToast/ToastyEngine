#pragma once
#include "Vector.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct BoundingBox {
	Vec3 min;
	Vec3 max;
	bool operator<(const aiAABB& rhs) {
		return rhs.mMin.x > this->min.x || rhs.mMin.y > this->min.y || rhs.mMin.z > this->min.z;
	};
	bool operator>(const aiAABB& rhs) {
		return rhs.mMax.x < this->max.x || rhs.mMax.y < this->max.y || rhs.mMax.z < this->max.z;
	};
	bool operator<(const BoundingBox& rhs) {
		return rhs.min.x > this->min.x || rhs.min.y > this->min.y || rhs.min.z > this->min.z;
	};
	bool operator>(const BoundingBox& rhs) {
		return rhs.max.x < this->max.x || rhs.max.y < this->max.y || rhs.max.z < this->max.z;
	};
};