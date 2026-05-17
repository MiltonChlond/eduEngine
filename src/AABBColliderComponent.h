#pragma once

#include "glmcommon.hpp"

struct AABBColliderComponent
{
	float xWidth;
	float yWidth;
	float zWidth;
	bool isTrigger = false;
};