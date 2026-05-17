#pragma once

#include "TransformComponent.h"
#include "AABBColliderComponent.h"
#include "SphereColliderComponent.h"
#include "ShapeRenderer.hpp"
#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "glmcommon.hpp"
#include <vector>

inline glm::vec3 CreatePoint(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 right)
{
	glm::vec3 point = pos + forward + up + right;
	return point;
}

inline void DrawAABBColliders(entt::registry& registry, ShapeRendererPtr& shapeRenderer)
{
	auto view = registry.view<TransformComponent, AABBColliderComponent>();

	for (auto entity : view)
	{
		auto& transform = registry.get<TransformComponent>(entity);
		auto& AABB = registry.get<AABBColliderComponent>(entity);

		std::vector<glm::vec3> points;
		
		glm::vec3 forward = glm::vec3(0, 0, AABB.zWidth);
		glm::vec3 up = glm::vec3(0, AABB.yWidth, 0);
		glm::vec3 right = glm::vec3(AABB.xWidth , 0, 0);

		points.push_back(CreatePoint(transform.pos, forward, up, right));
		points.push_back(CreatePoint(transform.pos, forward, up, -right));
		points.push_back(CreatePoint(transform.pos, -forward, up, right));
		points.push_back(CreatePoint(transform.pos, -forward, up, -right));
		points.push_back(CreatePoint(transform.pos, forward, -up, right));
		points.push_back(CreatePoint(transform.pos, forward, -up, -right));
		points.push_back(CreatePoint(transform.pos, -forward, -up, right));
		points.push_back(CreatePoint(transform.pos, -forward, -up, -right));

		shapeRenderer->push_states(ShapeRendering::Color4u::Green);

		//top horizontal lines
		shapeRenderer->push_line(points[0], points[1]);
		shapeRenderer->push_line(points[0], points[2]);
		shapeRenderer->push_line(points[3], points[1]);
		shapeRenderer->push_line(points[3], points[2]);

		//vertical lines
		shapeRenderer->push_line(points[0], points[4]);
		shapeRenderer->push_line(points[1], points[5]);
		shapeRenderer->push_line(points[2], points[6]);
		shapeRenderer->push_line(points[3], points[7]);

		//bottom horizonta lines
		shapeRenderer->push_line(points[4], points[5]);
		shapeRenderer->push_line(points[4], points[6]);
		shapeRenderer->push_line(points[7], points[5]);
		shapeRenderer->push_line(points[7], points[6]);

		shapeRenderer->pop_states<ShapeRendering::Color4u>();
	}
}

inline void DrawSphereColliders(entt::registry& registry, ShapeRendererPtr shapeRenderer)
{
	auto view = registry.view<TransformComponent, SphereColliderComponent>();

	int segments = 20; // amount of lines per circle per axis

	for (auto entity : view)
	{
		auto& transform = registry.get<TransformComponent>(entity);
		auto& sphere = registry.get<SphereColliderComponent>(entity);

        glm::vec3 center = transform.pos;
        float radius = sphere.radius;

		shapeRenderer->push_states(ShapeRendering::Color4u::Blue);

        for (int i = 0; i < segments; i++)
        {
            float t0 = glm::two_pi<float>() * (float)i / (float)segments;
            float t1 = glm::two_pi<float>() * (float)(i + 1) / (float)segments;

            glm::vec3 p0 =
            {
                center.x + cos(t0) * radius,
                center.y + sin(t0) * radius,
                center.z
            };

            glm::vec3 p1 =
            {
                center.x + cos(t1) * radius,
                center.y + sin(t1) * radius,
                center.z
            };

            shapeRenderer->push_line(p0, p1);
        }

        for (int i = 0; i < segments; i++)
        {
            float t0 = glm::two_pi<float>() * (float)i / (float)segments;
            float t1 = glm::two_pi<float>() * (float)(i + 1) / (float)segments;

            glm::vec3 p0 =
            {
                center.x + cos(t0) * radius,
                center.y,
                center.z + sin(t0) * radius
            };

            glm::vec3 p1 =
            {
                center.x + cos(t1) * radius,
                center.y,
                center.z + sin(t1) * radius
            };

            shapeRenderer->push_line(p0, p1);
        }

        for (int i = 0; i < segments; i++)
        {
            float t0 = glm::two_pi<float>() * (float)i / (float)segments;
            float t1 = glm::two_pi<float>() * (float)(i + 1) / (float)segments;



            glm::vec3 p0 =
            {
                center.x,
                center.y + cos(t0) * radius,
                center.z + sin(t0) * radius
            };

            glm::vec3 p1 =
            {
                center.x,
                center.y + cos(t1) * radius,
                center.z + sin(t1) * radius
            };

            shapeRenderer->push_line(p0, p1);
        }
	}
}