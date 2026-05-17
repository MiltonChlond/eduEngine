#pragma once

struct AnimationComponent
{
	int baseAnim = 1;
	int secAnim = 2;

	float time = 0;

	float speed = 0;
	float maxSpeed = 0;
	bool useSpeed = false;

	bool useLayering = false;

	float blendFactor = 0;
	eeng::AnimationBranchDesc branch;

	bool playAnimation = false; //used to override the animationsystem and the rest of the animation component to allow the entity to play a specific animation (anim) for a specific amount of time
	int anim = 1;
	float animTimer = 0.0f;
	float animDuration = 2.0f;
};