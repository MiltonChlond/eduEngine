#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.h"
#include "MeshComponent.h"

inline void AnimationSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<AnimationComponent, MeshComponent>();
	for (auto entity : view)
	{
		auto& mesh = view.get<MeshComponent>(entity);
		auto& aniComp = view.get<AnimationComponent>(entity);
		auto meshLock = mesh.mesh.lock();

		aniComp.time += dt;

		if (aniComp.playAnimation)
		{
			aniComp.animTimer += dt;
			meshLock->animate(aniComp.anim, aniComp.animTimer);

			if (aniComp.animTimer >= aniComp.animDuration)
			{
				aniComp.playAnimation = false;
				aniComp.animTimer = 0.0f;
			}
			continue;
		}

		if (aniComp.useLayering)
		{ 
			meshLock->animateBlend(aniComp.baseAnim, aniComp.secAnim, aniComp.time, aniComp.time, aniComp.branch);
		} 
		else if (aniComp.useSpeed)
		{
			aniComp.blendFactor = aniComp.speed / aniComp.maxSpeed;
			meshLock->animateBlend(aniComp.baseAnim, aniComp.secAnim, aniComp.time, aniComp.time, aniComp.blendFactor); //speed controls blendFactor
		}
		else
		{
			meshLock->animateBlend(aniComp.baseAnim, aniComp.secAnim, aniComp.time, aniComp.time, aniComp.blendFactor); //speed does not control blendfactor
		}
	}
}