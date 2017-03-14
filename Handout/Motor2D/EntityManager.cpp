#include "EntityManager.h"
#include "p2Log.h"
#include "Collision.h"
#include "Application.h"
#include "Scene.h"
#include "Unit.h"
#include "Render.h"
#include "Pathfinding.h"

EntityManager::EntityManager() : Module()
{
	name = "entityManager";
	nextID = 1;
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Awake(pugi::xml_node & config)
{
	

	return true;
}

bool EntityManager::Start()
{
	LOG("Starting EntityManager");
	bool ret = true;
	return ret;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	App->input->GetMousePosition(mouseX, mouseY);
	mouseX -= App->render->camera.x;
	mouseY -= App->render->camera.y;

	for (list<Unit*>::iterator it = friendlyUnitList.begin(); it != friendlyUnitList.end(); it++) {
		(*it)->Update(dt);
		(*it)->Draw();
	}

	// TODO 5
	/*
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {

		for (list<Unit*>::iterator it = friendlyUnitList.begin(); it != friendlyUnitList.end(); it++) {
			if (mouseX < (*it)->entityPosition.x + ((*it)->collider->rect.w / 2) && mouseX > (*it)->entityPosition.x - ((*it)->collider->rect.w / 2) &&
				mouseY < (*it)->entityPosition.y + ((*it)->collider->rect.h / 2) && mouseY > (*it)->entityPosition.y - ((*it)->collider->rect.h / 2)) {
					(*it)->isSelected = true;
					selectedUnit = *it;
					for (list<Unit*>::iterator it2 = friendlyUnitList.begin(); it2 != friendlyUnitList.end(); it2++) {
						if (*it != *it2) {
							if ((*it2)->isSelected) {
								(*it2)->isSelected = false;
							}
						}
					}
					break;
			}
			else {
				if ((*it)->isSelected) {
					(*it)->isSelected = false;
					selectedUnit = nullptr;
				}
			}
		}

		if (selectedUnit == nullptr) {
			iPoint target;
			target = App->map->WorldToMap(mouseX, mouseY);

			if (App->pathfinding->IsWalkable(target)) {
				CreateUnit(mouseX, mouseY);
			}
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
		if (selectedUnit != nullptr) {
			selectedUnit->SetDestination();
		}
	}
	*/

	return true;
}

bool EntityManager::PostUpdate()
{
	if (removeUnitList.size() > 0) {
		list<Unit*>::iterator i = removeUnitList.begin();

		while (i != removeUnitList.end())
		{
			list<Unit*>::iterator unitToDestroy = i;
			++i;
			DestroyEntity((*unitToDestroy));
		}

		removeUnitList.clear();
	}

	return true;
}

bool EntityManager::CleanUp()
{
	LOG("Freeing EntityManager");

	for (list<Unit*>::iterator it = friendlyUnitList.begin(); it != friendlyUnitList.end(); it++) {
		RELEASE((*it));
	}
	friendlyUnitList.clear();

	for (list<Unit*>::iterator it = removeUnitList.begin(); it != removeUnitList.end(); it++) {
		RELEASE((*it));
	}
	removeUnitList.clear();

	return true;
}

Unit* EntityManager::CreateUnit(int posX, int posY)
{
	Unit* unit = new Unit(posX, posY);
	unit->entityID = nextID;
	nextID++;
	friendlyUnitList.push_back(unit);

	return unit;
}

void EntityManager::DeleteUnit(Unit* unit, bool isEnemy)
{
	if (unit != nullptr) {
		removeUnitList.push_back(unit);
		friendlyUnitList.remove(unit);
	}
}

void EntityManager::OnCollision(Collider * c1, Collider * c2)
{
	
}


void EntityManager::DestroyEntity(Entity * entity)
{
	if (entity != nullptr) {
		list<Unit*>::iterator it = removeUnitList.begin();

		while (it != removeUnitList.end())
		{
			if (*it == entity)
			{
				removeUnitList.remove(*it);
				delete entity;
				return;
			}
			++it;
		}
	}
}