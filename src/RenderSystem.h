#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ForwardRenderer.hpp"
#include "CameraComponent.h"

inline int RenderSystem(entt::registry& registry, eeng::ForwardRendererPtr& renderer, int windowWidth, int windowHeight, glm::vec3 lightPos, glm::vec3 lightColor)
{
	auto cameras = registry.view<CameraComponent, TransformComponent>();
	entt::entity camera;
	bool foundCam = false;
	for (auto cam : cameras)
	{
		auto& camComp = cameras.get<CameraComponent>(cam);
		if (camComp.isActive)
		{
			camera = cam;
			foundCam = true;
			break;
		}
	}
	if (!foundCam)
	{
		return 0;
	}

	auto& cameraComp = cameras.get<CameraComponent>(camera);
	auto& cameraTran = cameras.get<TransformComponent>(camera);

	const float aspectRatio = float(windowWidth) / windowHeight;
	glm::mat4 P = glm::perspective(glm::radians(cameraComp.fov), aspectRatio, cameraComp.nearPlane, cameraComp.farPlane);
	glm::mat4 V = glm::lookAt(cameraTran.pos, cameraComp.lookAt, cameraComp.up);

	renderer->beginPass(P, V, lightPos, lightColor, cameraTran.pos);

	auto entities = registry.view<TransformComponent, MeshComponent>();

	for (auto entity : entities)
	{
		auto& transform = entities.get<TransformComponent>(entity);
		auto& mesh = entities.get<MeshComponent>(entity);
		
		auto meshShared = mesh.mesh.lock();
		if (!meshShared)
		{
			continue;
		}
		
		glm::mat4 matrix = glm_aux::TRS(transform.pos, transform.rot.y, glm_aux::vec3_010, transform.scale);

		renderer->renderMesh(meshShared, matrix);
	}

	return(renderer->endPass());
}