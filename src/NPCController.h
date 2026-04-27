#pragma once

struct NPCController
{
	bool isMoving = true;
	glm::vec3 targetPos{0, 0, 0};
	float speed = 10;
	float reachDistance = 0.5f;
};