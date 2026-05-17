
#include <entt/entt.hpp>
#include "glmcommon.hpp"
#include "imgui.h"
#include "Log.hpp"
#include "Game.hpp"

#include "TransformComponent.h"
#include "LinearVelocityComponent.h"
#include "MeshComponent.h"
#include "NPCController.h"
#include "PlayerControllerComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"

#include "AABBColliderComponent.h"
#include "SphereColliderComponent.h"

#include "RenderSystem.h"
#include "PlayerControllerSystem.h"
#include "PlayerRotationSystem.h"
#include "NPCControllerSystem.h"
#include "MovementSystem.h"
#include "CameraPerspectiveSystem.h"
#include "CameraMovementController.h"
#include "AnimationSystem.h"
#include "AnimationSpeedSystem.h"
#include "DrawBoneGizmoSystem.h"

#include "CollisionResponseSystem.h"
#include "CollisionDetectionSystem.h"
#include "DrawCollidersSystem.h"
#include "DamageSystem.h"
#include "QuestSystem.h"
#include "HorseSystem.h"

bool Game::init()
{
    forwardRenderer = std::make_shared<eeng::ForwardRenderer>();
    forwardRenderer->init("shaders/phong_vert.glsl", "shaders/phong_frag.glsl");

    shapeRenderer = std::make_shared<ShapeRendering::ShapeRenderer>();
    shapeRenderer->init();

    // Do some entt stuff
    entity_registry = std::make_shared<entt::registry>();

    // Grass
    grassMesh = std::make_shared<eeng::RenderableMesh>();
    grassMesh->load("assets/grass/grass_trees_merged.fbx", false);

    // Horse
    horseMesh = std::make_shared<eeng::RenderableMesh>();
    horseMesh->load("assets/Animals/Horse.fbx", false);

    // Character
    characterMesh = std::make_shared<eeng::RenderableMesh>();

    //animation character
    animationMesh = std::make_shared<eeng::RenderableMesh>();
    animationMesh->load("assets/ExoRed/exo_red.fbx");
    animationMesh->load("assets/ExoRed/idle (2).fbx", true);
    animationMesh->load("assets/ExoRed/walking.fbx", true);
    animationMesh->removeTranslationKeys("mixamorig:Hips");

    foodMesh = std::make_shared<eeng::RenderableMesh>();
    foodMesh->load("assets//ExoRed/exo_red.fbx");

    drawSkeleton = false;

    // Amy 5.0.1 PACK FBX
    characterMesh->load("assets/Amy/Ch46_nonPBR.fbx");
    characterMesh->load("assets/Amy/idle.fbx", true);
    characterMesh->load("assets/Amy/walking.fbx", true);
    characterMesh->load("assets/Amy/waving.fbx", true);
    // Remove root motion
    characterMesh->removeTranslationKeys("mixamorig:Hips");

    CreatePlayer();
    CreateCamera();
    CreateGrass();
    //CreateNPC();
    CreateHorse();
    CreateAnimationCharacter();
    CreateFood();

    //listener
    listenerID = eventQueue.Subscribe([this](const EventType& event)
        {
            if (std::holds_alternative<CollisionEvent>(event))
            {
                CollisionEvent colEvent = std::get<CollisionEvent>(event);
                CollisionResponseSystem(*entity_registry, colEvent);
            }
            else if (std::holds_alternative<TriggerEvent>(event))
            {
                TriggerEvent triggerEvent = std::get<TriggerEvent>(event);
                TriggerResponseSystem(*entity_registry, triggerEvent);
            }
            else if (std::holds_alternative<DamageEvent>(event))
            {
                DamageEvent damageEvent = std::get<DamageEvent>(event);
                DamageSystem(*entity_registry, damageEvent);
            }
            else if (std::holds_alternative<QuestEvent>(event))
            {
                QuestEvent questEvent = std::get<QuestEvent>(event);
                QuestSystem(*entity_registry, questEvent, currentInput);
            }
            else if (std::holds_alternative<HorseFedEvent>(event))
            {
                HorseFedEvent fedEvent = std::get<HorseFedEvent>(event);
                HorseSystem(*entity_registry, fedEvent);
            }
        });
    return true;
}

void Game::CreatePlayer()
{
    player = entity_registry->create();
    entity_registry->emplace<TransformComponent>(player, glm::vec3{0.0f, 0.0f, 0.0f },
                                                         glm::vec3{0.0f, 0.0f, 0.0f },
                                                         glm::vec3{0.03f, 0.03f, 0.03f},
                                                         "Player");
    entity_registry->emplace<MeshComponent>(player, characterMesh );
    entity_registry->emplace<LinearVelocityComponent>(player, glm::vec3{0, 0, 0}, 100.0f);
    entity_registry->emplace<PlayerControllerComponent>(player);

    eeng::AnimationBranchDesc branchDesc;
    branchDesc.root_node_name = "mixamorig:Spine";
    branchDesc.mode = eeng::AnimationBranchDesc::Mode::IncludeSubtree;
    entity_registry->emplace<AnimationComponent>(player, 1, 2, 0.0f, 0.0f, 0.0f, true, false, 0.0f, branchDesc, false, 3, 0.0f, 2.0f);

    entity_registry->emplace<SphereColliderComponent>(player, 8.0f, false);
    entity_registry->emplace<AABBColliderComponent>(player, 5.0f, 5.0f, 5.0f, false);
    entity_registry->emplace<HealthComponent>(player, 100, 100);
    entity_registry->emplace<QuestComponent>(player);
}

void Game::CreateCamera()
{
    camera = entity_registry->create();
    entity_registry->emplace<CameraComponent>(camera, true, glm_aux::vec3_000, glm_aux::vec3_010, 1.0f, 500.0f, 60.0f, 0.0f, -glm::pi<float>() / 8, glm::ivec2(-1, -1));
    entity_registry->emplace<TransformComponent>(camera, glm::vec3{0.0f, 0.0f, -2.0f },
                                                         glm::vec3{0.0f, 0.0f, 0.0f },
                                                         glm::vec3{0.0f, 0.0f, 0.0f },
                                                         "Camera");
    entity_registry->emplace<CameraMovementComponent>(camera);
    isThirdPersonCam = true;
}

void Game::CreateGrass()
{
    grass = entity_registry->create();
    entity_registry->emplace<TransformComponent>(grass, glm::vec3{0.0f, 0.0f, 0.0f },
                                                        glm::vec3{0.0f, 0.0f, 0.0f },
                                                        glm::vec3{100.0f, 100.0f, 100.0f },
                                                        "Grass");
    entity_registry->emplace<MeshComponent>(grass, grassMesh );
}

void Game::CreateNPC()
{
    npc = entity_registry->create();
    entity_registry->emplace<TransformComponent>(npc, glm::vec3{0.5f, 0.0f, 0.0f},
                                                      glm::vec3{0.0f, 0.0f, 0.0f },
                                                      glm::vec3{0.03f, 0.03f, 0.03f},
                                                      "NPC");
    entity_registry->emplace<MeshComponent>(npc, characterMesh);
    entity_registry->emplace<LinearVelocityComponent>(npc, glm::vec3{ 0, 0, 0 }, 20.0f);
    entity_registry->emplace<NPCController>(npc);

    entity_registry->emplace<SphereColliderComponent>(npc, 8.0f, false);
    entity_registry->emplace<AABBColliderComponent>(npc, 5.0f, 5.0f, 5.0f, false);

    stopMoving = false;
}

void Game::CreateHorse()
{
    horse = entity_registry->create();
    entity_registry->emplace<TransformComponent>(horse, glm::vec3{30.0f, 0.0f, -35.0f },
                                                        glm::vec3{0.0f, 35.0f, 0.0f },
                                                        glm::vec3{0.01f, 0.01f, 0.01f},
                                                        "Horse");
    entity_registry->emplace<MeshComponent>(horse, horseMesh);

    eeng::AnimationBranchDesc branchDesc;
    branchDesc.root_node_name = "mixamorig:Spine";
    branchDesc.mode = eeng::AnimationBranchDesc::Mode::IncludeSubtree;
    entity_registry->emplace<AnimationComponent>(horse, 11, 0, 0.0f, 0.0f, 0.0f, false, false, 0.0f, branchDesc, false, 3, 0.0f, 4.0f);

    entity_registry->emplace<SphereColliderComponent>(horse, 8.0f, true);
    entity_registry->emplace<AABBColliderComponent>(horse, 5.0f, 5.0f, 5.0f, true);
    entity_registry->emplace<HorseComponent>(horse);
}

void Game::CreateAnimationCharacter()
{
    animationCharacter = entity_registry->create();
    entity_registry->emplace<MeshComponent>(animationCharacter, animationMesh);
    entity_registry->emplace<TransformComponent>(animationCharacter, glm::vec3{0.0f, 0.0f, 5.0f },
                                                    glm::vec3{0.0f, 0.0f, 0.0f },
                                                    glm::vec3{0.03f, 0.03f, 0.03f},
                                                    "AnimationCharacter");
    eeng::AnimationBranchDesc branchDesc;
    branchDesc.root_node_name = "mixamorig:Spine";
    branchDesc.mode = eeng::AnimationBranchDesc::Mode::IncludeSubtree;
    
    entity_registry->emplace<AnimationComponent>(animationCharacter, 1, 2, 0.0f, 0.0f, 0.0f, false, true, 0.0f, branchDesc);
}

void Game::CreateFood()
{
    food = entity_registry->create();
    entity_registry->emplace<TransformComponent>(food, glm::vec3{ -30.0f, 0.0f, 5.0f },
                                                        glm::vec3{ 0.0f, 0.0f, 0.0f },
                                                        glm::vec3{ 0.03f, 0.03f, 0.03f },
                                                        "AnimationCharacter");
    entity_registry->emplace<MeshComponent>(food, foodMesh);
    entity_registry->emplace<SphereColliderComponent>(food, 8.0f, true);
    entity_registry->emplace<AABBColliderComponent>(food, 5.0f, 5.0f, 5.0f, true);
    entity_registry->emplace<FoodComponent>(food);
}

void Game::update(
    float time,
    float deltaTime,
    InputManagerPtr input)
{
    currentInput = input;
   
    NPCContollerSystem(*entity_registry);

    PlayerControllerSystem(*entity_registry, currentInput);
    PlayerRotationSystem(*entity_registry, currentInput);

    MovementSystem(*entity_registry, deltaTime);

    CameraPerspectiveSystem(*entity_registry, currentInput);
    TransformComponent& playerTransform = entity_registry->get<TransformComponent>(player);
    CameraMovementController(*entity_registry, currentInput, deltaTime, playerTransform);

    CollisionDetectionSystem(*entity_registry);

    DrawAABBColliders(*entity_registry, shapeRenderer);
    DrawSphereColliders(*entity_registry, shapeRenderer);

    eventQueue.BroadCast();

    AnimationSpeedSystem(*entity_registry);
    AnimationSystem(*entity_registry, deltaTime);
    DrawBoneGizmoSystem(*entity_registry, drawSkeleton, shapeRenderer);

}

void Game::render(
    float time,
    int windowWidth,
    int windowHeight)
{
    renderUI();

    drawcallCount = RenderSystem(*entity_registry, forwardRenderer, windowWidth, windowHeight, pointlight.pos, pointlight.color);

    // Draw shape batches (lines etc)
    auto cameras = entity_registry->view<CameraComponent, TransformComponent>();
    
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
        return;
    }

    auto& cam = cameras.get<CameraComponent>(camera);
    auto& transform = cameras.get<TransformComponent>(camera);
    const float aspectRatio = float(windowWidth) / windowHeight;

    glm::mat4 v = glm::lookAt(transform.pos, cam.lookAt, cam.up);
    glm::mat4 p = glm::perspective(glm::radians(cam.fov), aspectRatio, cam.nearPlane, cam.farPlane);

    shapeRenderer->render(p * v);
    shapeRenderer->post_render();
}

void Game::renderUI()
{
    // Begin game info ImGui window
    ImGui::Begin("Game Info");

    ImGui::Text("Drawcall count %i", drawcallCount);

    // Color picker for light color
    if (ImGui::ColorEdit3("Light color",
        glm::value_ptr(pointlight.color),
        ImGuiColorEditFlags_NoInputs))
    {
    }

    if (auto players = entity_registry->view<PlayerControllerComponent, TransformComponent, LinearVelocityComponent>()) //if player
    {
        ImGui::Separator();
        ImGui::Text("Player Entities");
        for (auto player : players)
        {
            ImGui::PushID(int(player));
            auto& playerCtrl = players.get<PlayerControllerComponent>(player);
            ImGui::SliderFloat("Player Movement Speed", &playerCtrl.speed, 10, 100);

            auto& transform = players.get<TransformComponent>(player);
            ImGui::SliderFloat("Player Size: X", &transform.scale.x, 0.01, 1);
            ImGui::SliderFloat("Player Size: Y", &transform.scale.y, 0.01, 1);
            ImGui::SliderFloat("Player Size: Z", &transform.scale.z, 0.01, 1);
            ImGui::PopID();
        }
    }

    if (auto npcs = entity_registry->view<NPCController, TransformComponent>())
    {
        ImGui::Separator();
        ImGui::Text("NPC Entities");
        ImGui::Checkbox("Stop NPC Movement", &stopMoving);
        for (auto npc : npcs)
        {
            auto& NPCContoller = npcs.get<NPCController>(npc);
            if (stopMoving)
                NPCContoller.isMoving = false;
            else
                NPCContoller.isMoving = true;
        }
    }

    ImGui::End(); // end info window

    ImGui::Begin("Quest"); //quest window
    auto& questComponent = entity_registry->get<QuestComponent>(player);
    ImGui::Text(questComponent.questText);
    ImGui::End();

    ImGui::Begin("Animation"); //animation window
    auto view = entity_registry->view<AnimationComponent, TransformComponent, MeshComponent>();
    for (auto entity : view)
    {
        ImGui::PushID((int)entity);

        auto& transform = view.get<TransformComponent>(entity);
        auto& aniComp = view.get<AnimationComponent>(entity);
        auto& mesh = view.get<MeshComponent>(entity);
        auto meshLock = mesh.mesh.lock();
        int nbrOfAnims = meshLock->getNbrAnimations() - 1;

        ImGui::Text("Entity: %s", transform.name.c_str());
        ImGui::SliderInt("Primary Animation", &aniComp.baseAnim, 0, nbrOfAnims);
        ImGui::SliderInt("Secondary Animation", &aniComp.secAnim, 0, nbrOfAnims);
        ImGui::SliderFloat("Blend Factor: %f", &aniComp.blendFactor, 0 , 1);
        ImGui::Text("Blend Factor: %f", aniComp.blendFactor);
        ImGui::Text("Speed: %f", aniComp.speed);
        ImGui::Checkbox("Use Speed", &aniComp.useSpeed);
        ImGui::Checkbox("Use Layering", &aniComp.useLayering);
        ImGui::Separator();

        ImGui::PopID();
    }
    ImGui::End();
}

void Game::destroy()
{

}