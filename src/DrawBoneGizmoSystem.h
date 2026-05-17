#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "ShapeRenderer.hpp"

inline void DrawBoneGizmoSystem(entt::registry& registry, bool drawSkeleton, ShapeRendererPtr& shapeRenderer)
{
    if (drawSkeleton)
    {
        float axisLen = 25.0f;
        auto view = registry.view<MeshComponent, TransformComponent>();
        for (auto entity : view)
        {
            auto& m = view.get<MeshComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);
            auto mesh = m.mesh.lock();

            if (!mesh) continue;

            //mesh matrix
            glm::mat4 global = glm_aux::TRS(transform.pos, transform.rot.y, { 0, 1, 0 }, transform.scale);
            

            for (int i = 0; i < mesh->boneMatrices.size(); i++)
            {
                auto IBinverse = glm::inverse(mesh->m_bones[i].inversebind_tfm);
                glm::mat4 boneMatrix = global * mesh->boneMatrices[i] * IBinverse;

                glm::vec3 pos = glm::vec3(boneMatrix[3]);
                glm::vec3 right = (glm::vec3(boneMatrix[0]));
                glm::vec3 up = (glm::vec3(boneMatrix[1]));
                glm::vec3 fwd = (glm::vec3(boneMatrix[2]));

                shapeRenderer->push_states(ShapeRendering::Color4u::Red);
                shapeRenderer->push_line(pos, pos + right * axisLen);

                shapeRenderer->push_states(ShapeRendering::Color4u::Green);
                shapeRenderer->push_line(pos, pos + up * axisLen);

                shapeRenderer->push_states(ShapeRendering::Color4u::Blue);
                shapeRenderer->push_line(pos, pos + fwd * axisLen);

                shapeRenderer->pop_states<ShapeRendering::Color4u>();
                shapeRenderer->pop_states<ShapeRendering::Color4u>();
                shapeRenderer->pop_states<ShapeRendering::Color4u>();
            }
        }
        
    }
	
}