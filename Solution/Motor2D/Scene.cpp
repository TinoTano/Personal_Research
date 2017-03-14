#include "Application.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "PathFinding.h"
#include "Scene.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Unit.h"
#include "QuadTree.h"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	if (App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	elvenArcher = App->entityManager->CreateUnit(-10, 128);
	elvenArcher = App->entityManager->CreateUnit(10, 180);
	elvenArcher = App->entityManager->CreateUnit(-75, 270);
	elvenArcher = App->entityManager->CreateUnit(-385, 275);
	elvenArcher = App->entityManager->CreateUnit(-500, 320);
	/*elvenArcher = App->entityManager->CreateUnit(-400, 530);
	elvenArcher = App->entityManager->CreateUnit(-345, 530);
	elvenArcher = App->entityManager->CreateUnit(-185, 650);
	elvenArcher = App->entityManager->CreateUnit(227, 465);*/

	App->render->camera.x = 600;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}