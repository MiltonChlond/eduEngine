#pragma once

#include "glmcommon.hpp"

struct TransformComponent
{
	glm::vec3 pos = {0, 0, 0};
	glm::vec3 rot = {0, 0, 0};
	glm::vec3 scale = {1, 1, 1};

	std::string name;
};
