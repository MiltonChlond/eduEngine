#pragma once

struct PlayerControllerComponent
{
	glm::ivec2 mouse_xy_prev{ -1, -1 };
	float sensitivity = 0.005f;
	float speed = 50;
};