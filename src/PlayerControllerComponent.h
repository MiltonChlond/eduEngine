#pragma once

struct PlayerControllerComponent
{
	float speed = 50;
	glm::ivec2 mouse_xy_prev{ -1, -1 };
	float sensitivity = 0.005f;
};