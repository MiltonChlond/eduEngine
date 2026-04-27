#pragma once

#include "RenderableMesh.hpp"
#include <memory>

struct MeshComponent
{
	std::weak_ptr<eeng::RenderableMesh> mesh;
};