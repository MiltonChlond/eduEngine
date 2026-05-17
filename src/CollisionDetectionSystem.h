#pragma once

#include "SphereColliderComponent.h"
#include "AABBColliderComponent.h"
#include "TransformComponent.h"
#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "EventQueue.h"

#include "Log.hpp"

struct BVHNode
{
	glm::vec3 center = {0, 0, 0};
	float radius = 0;
	BVHNode* right = nullptr;
	BVHNode* left = nullptr;
	entt::entity entity = entt::null;
}; 

inline BVHNode* CreateParentNode(BVHNode* a, BVHNode* b)
{
	BVHNode* parent = new BVHNode();
	float distance = glm::distance(a->center, b->center);
	glm::vec3 direction = b->center - a->center;
	glm::vec3 norm = direction / distance;

	glm::vec3 p1 = a->center - norm * a->radius;
	glm::vec3 p2 = b->center + norm * b->radius;

	parent->center = (p1 + p2) * 0.5f;
	parent->radius = glm::distance(p1, p2) * 0.5f;

	parent->left = a;
	parent->right = b;

	return parent;
}

inline float DistanceBetweenSpheres(BVHNode* a, BVHNode* b)
{
	float centerDistance = glm::distance(a->center, b->center);
	float surfaceDistance = centerDistance - (a->radius + b->radius);

	return std::max(0.0f, surfaceDistance);
}

inline std::vector<std::pair<BVHNode*, BVHNode*>>FindPairs(std::vector<BVHNode*> openList, float maxDistance)
{
	std::vector<std::pair<BVHNode*, BVHNode*>>allPairs;
	std::vector<BVHNode*>available = openList;

	while (!available.empty())
	{
		BVHNode* current = available.back();

		available.pop_back();

		float closestDistance = maxDistance;
		BVHNode* bestMatch = nullptr;

		int bestIndex = -1;

		for (int i = 0; i < available.size(); i++)
		{
			float distance = DistanceBetweenSpheres(current, available[i]);

			if (distance < closestDistance)
			{
				closestDistance = distance;
				bestMatch = available[i];
				bestIndex = i;
			}
		}

		if (bestMatch)
		{
			available.erase(available.begin() + bestIndex);
		}

		allPairs.push_back({ current, bestMatch });
	}

	return allPairs;
}

inline BVHNode* BroadPhase(entt::registry& registry) //bottom up broad phase
{
	std::vector<BVHNode*> openList;

	auto spheres = registry.view<SphereColliderComponent, TransformComponent>();

	for (auto entity : spheres)
	{
		auto& transform = spheres.get<TransformComponent>(entity);
		auto& collider = spheres.get<SphereColliderComponent>(entity);

		BVHNode* node = new BVHNode();

		node->center = transform.pos;
		node->radius = collider.radius;
		node->entity = entity;

		openList.push_back(node);
	}

	float maxDistance = FLT_MAX;

	while (openList.size() > 1)
	{
		auto pairs = FindPairs(openList, maxDistance);
		openList.clear();

		for (auto pair : pairs)
		{
			BVHNode* parent;
			if (pair.second)
			{
				parent = CreateParentNode(pair.first, pair.second);

				parent->left = pair.first;
				parent->right = pair.second;
				openList.push_back(parent);
			}
			else
			{
				openList.push_back(pair.first);
			}
		}
		maxDistance = FLT_MAX;
	}

	return openList[0];
}

inline bool SphereSphereCheck(BVHNode* a, glm::vec3 center, float radius) //broad phase
{
	float distance = glm::distance(a->center, center);
	float collisionDistance = a->radius + radius;
	if (distance < collisionDistance)
	{
		return true;
	}
	return false;
}


inline std::vector<entt::entity> TraverseBVH(BVHNode* rootNode, glm::vec3 center, float radius)
{
	std::vector<entt::entity> possibleCollisions;

	if (!rootNode)
		return possibleCollisions;

	if (!SphereSphereCheck(rootNode, center, radius))
		return possibleCollisions;

	if (!rootNode->right && !rootNode->left)
	{
		possibleCollisions.push_back(rootNode->entity);
		return possibleCollisions;
	}

	auto leftRecurse = TraverseBVH(rootNode->left, center, radius);
	possibleCollisions.insert(possibleCollisions.end(), leftRecurse.begin(), leftRecurse.end());

	auto rightRecurse = TraverseBVH(rootNode->right, center, radius);
	possibleCollisions.insert(possibleCollisions.end(), rightRecurse.begin(), rightRecurse.end());

	return possibleCollisions;
}

inline bool AABBAABBCheck(AABBColliderComponent a1, TransformComponent t1, AABBColliderComponent a2, TransformComponent t2) //narrow phase
{
	float distanceX = glm::distance(t1.pos.x, t2.pos.x);
	float distanceY = glm::distance(t1.pos.y, t2.pos.y);
	float distanceZ = glm::distance(t1.pos.z, t2.pos.z);

	if (distanceX > a1.xWidth + a2.xWidth) return false;

	if (distanceY > a1.yWidth + a2.yWidth) return false;

	if (distanceZ > a1.zWidth + a2.zWidth) return false;

	return true;
}

inline void ResolveCollision(TransformComponent& t1, TransformComponent& t2, SphereColliderComponent s1, SphereColliderComponent s2)
{
	glm::vec3 collisionNormal = glm::normalize(t1.pos - t2.pos);
	float penetration = (s1.radius + s2.radius) - glm::length(t1.pos - t2.pos);
	t1.pos += collisionNormal * penetration * 0.5f;
	t2.pos -= collisionNormal * penetration * 0.5f;
}

inline void DeleteBVH(BVHNode* node)
{
	if (!node)return;

	if(node->left)
		DeleteBVH(node->left);
	if(node->right)
		DeleteBVH(node->right);
	delete node;
}

inline void CollisionDetectionSystem(entt::registry& registry)
{
	BVHNode* root = BroadPhase(registry); //create bvh tree

	auto entities = registry.view<SphereColliderComponent, TransformComponent, AABBColliderComponent>();
	for (auto entity : entities)
	{
		auto& sphere = entities.get<SphereColliderComponent>(entity);
		auto& transform = entities.get<TransformComponent>(entity);
		auto& AABB = entities.get<AABBColliderComponent>(entity);

		std::vector<entt::entity> possibleCollisions = TraverseBVH(root, transform.pos, sphere.radius); //find possible collisions
		for (auto other : possibleCollisions) //check collision from possible ones
		{
			if (other == entity) continue;
			
			auto& otherAABB = entities.get<AABBColliderComponent>(other);
			auto& otherTransform = entities.get<TransformComponent>(other);

			if(AABBAABBCheck(AABB, transform, otherAABB, otherTransform))
			{
				auto& otherSphere = entities.get<SphereColliderComponent>(other);

				if (otherAABB.isTrigger && AABB.isTrigger)//both trigger, do nothing
				{
					continue;
				}
				
				if (otherAABB.isTrigger)
				{
					TriggerEvent triggerEvent = {other, entity};
					eventQueue.QueueEvent(triggerEvent);
					continue;
				}
				else if (AABB.isTrigger)
				{
					TriggerEvent triggerEvent = {entity, other};
					eventQueue.QueueEvent(triggerEvent);
					continue;
				}
				
				ResolveCollision(transform, otherTransform, sphere, otherSphere);
				CollisionEvent collisionEvent = {entity, other};
				eventQueue.QueueEvent(collisionEvent);
			}
		}
	}
	DeleteBVH(root);
}