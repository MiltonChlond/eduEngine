#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "InputManager.hpp"

inline void FreeCameraSystem(entt::registry& registry, InputManagerPtr& input, float dt)
{
	auto cameras = registry.view<CameraComponent, TransformComponent>();
	for (auto camera : cameras)
	{
		auto& cameraComp = cameras.get<CameraComponent>(camera);
		auto& cameraTran = cameras.get<TransformComponent>(camera);

		if (cameraComp.cameraMode != CameraMode::FreeCam) continue;

		auto mouse = input->GetMouseState();
		glm::ivec2 mouseCurr = { mouse.x, mouse.y };

		glm::ivec2 deltaMouse = { 0, 0 };
		if (cameraComp.mouse_xy_prev.x >= 0 && mouse.leftButton)
		{
			deltaMouse = mouseCurr - cameraComp.mouse_xy_prev;
			cameraComp.yaw -= deltaMouse.x * cameraComp.sensitivity;
			cameraComp.pitch -= deltaMouse.y * cameraComp.sensitivity;
			cameraComp.pitch = glm::clamp(cameraComp.pitch, -glm::radians(89.0f), glm::radians(89.0f));
		}
		
		cameraComp.mouse_xy_prev = mouseCurr;

		glm::vec3 forward;
		forward.x = cos(cameraComp.pitch) * sin(cameraComp.yaw);
		forward.y = sin(cameraComp.pitch);
		forward.z = cos(cameraComp.pitch) * cos(cameraComp.yaw);
		forward = glm::normalize(forward);

		using Key = eeng::InputManager::Key;
		bool W = input->IsKeyPressed(Key::W);
		bool A = input->IsKeyPressed(Key::A);
		bool S = input->IsKeyPressed(Key::S);
		bool D = input->IsKeyPressed(Key::D);
		bool space = input->IsKeyPressed(Key::Space);
		bool ctrl = input->IsKeyPressed(Key::LeftCtrl);

		glm::vec3 flatForward;
		flatForward.x = sin(cameraComp.yaw);
		flatForward.y = 0;
		flatForward.z = cos(cameraComp.yaw);

		glm::vec3 up = glm_aux::vec3_010;
		glm::vec3 right = glm::normalize(glm::cross(flatForward, up));
		
		glm::vec3 movement = forward * ((W ? 1.0f : 0.0f) + (S ? -1.0f : 0.0f)) + right * ((A ? -1.0f : 0.0f) + (D ? 1.0f : 0.0f)) + up * ((space ? 1.0f : 0.0f) + (ctrl ? -1.0f : 0.0f));
		cameraTran.pos += movement * cameraComp.moveSpeed * dt;
		cameraComp.lookAt = cameraTran.pos + forward;
	}
}