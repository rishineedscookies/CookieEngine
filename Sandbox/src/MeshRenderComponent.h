#pragma once

#define MESH_RENDER_ID 5

struct MeshRenderComponent
{
	struct Cookie::Model* Model;
	class Cookie::Material* Material;
};
