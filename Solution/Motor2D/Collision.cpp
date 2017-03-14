#include "Collision.h"
#include "Application.h"
#include "Input.h"
#include "p2Log.h"
#include "Render.h"
#include "Window.h"
#include "QuadTree.h"
#include "p2Defs.h"
#include "EntityManager.h"

Collision::Collision() : Module()
{
	name = "collision";

	matrix[COLLIDER_FRIENDLY_UNIT][COLLIDER_FRIENDLY_UNIT] = true;

}


Collision::~Collision()
{
}

bool Collision::Awake(pugi::xml_node &)
{
	return true;
}

bool Collision::Start()
{
	//TODO 3
	uint w, h;
	App->win->GetWindowSize(w, h);
	quadTree = new QuadTree({ -App->render->camera.x,0,(int)w,(int)h });
	return true;
}

bool Collision::PreUpdate()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
	}

	return true;
}

bool Collision::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug) {
		DebugDraw();
	}

	////// Brute Force method //////

	Collider *c1;
	Collider *c2;

	bruteForceChecks = 0;
	bruteForceTimer.Start();
	for (list<Collider*>::iterator col1 = colliders.begin(); col1 != colliders.end(); col1++) {
		c1 = (*col1);

		for (list<Collider*>::iterator col2 = colliders.begin(); col2 != colliders.end(); col2++) {
			c2 = (*col2);
			if (c1 != c2) {
				if (c1->CheckCollision(c2->rect) == true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
				bruteForceChecks++;
			}
		}
	}
	double bruteForceTime = bruteForceTimer.ReadMs();


	////// Quad Tree method //////

	//TODO 3

	quadTreeTimer.Start();

	quadTree->ClearTree();

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		quadTree->Insert(*it);
	}

	quadTreeChecks = 0;

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		potentialCollisionList.clear();
		quadTree->Retrieve(potentialCollisionList, *it);
		if (potentialCollisionList.size() > 0) {
			for (list<Collider*>::iterator it2 = potentialCollisionList.begin(); it2 != potentialCollisionList.end(); it2++) {
				if ((*it)->CheckCollision((*it2)->rect)) {
					if (matrix[(*it)->type][(*it2)->type] && (*it)->callback)
						(*it)->callback->OnCollision((*it), (*it2));

					if (matrix[(*it2)->type][(*it)->type] && (*it2)->callback)
						(*it2)->callback->OnCollision((*it2), (*it));
				}
				quadTreeChecks++;
				//TODO 4
				App->render->DrawQuad((*it2)->rect, 0, 255, 0, 255, false);
				App->render->DrawLine((*it)->rect.x + (*it)->rect.w / 2, (*it)->rect.y + (*it)->rect.h / 2, (*it2)->rect.x + (*it2)->rect.w / 2, (*it2)->rect.y + (*it2)->rect.h / 2, 255, 255, 255, 255);
			}
		}
	}

	double quadTreeTime = quadTreeTimer.ReadMs();
	
	//TODO 4
	nodeList.clear();
	quadTree->GetNodes(nodeList);

	for (int i = 0; i < nodeList.size(); i++) {
		if (nodeList[i] != nullptr) {
			App->render->DrawQuad(nodeList[i]->nodeRect, 255, 255, 255, 255, false);
		}
	}

	static char title[350];
	sprintf_s(title, "Colliders: %d | Brute force checks: %d Time: %.5f ms | Quadtree checks: %d Time: %.5f ms ",
		colliders.size(), bruteForceChecks, bruteForceTime, quadTreeChecks, quadTreeTime);
	App->win->SetTitle(title);

	

	return true;
}

bool Collision::CleanUp()
{
	LOG("Freeing colliders");
	list<Collider*>::reverse_iterator it = colliders.rbegin();

	while (it != colliders.rend())
	{
		RELEASE(*it);
		++it;
	}

	colliders.clear();

	return true;
}

Collider * Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module * callback)
{
	Collider* ret = new Collider(rect, type, callback);
	colliders.push_back(ret);

	return ret;
}

void Collision::DeleteCollider(Collider * collider)
{
	collider->to_delete = true;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (bool)SDL_HasIntersection(&rect, &r);
}

void Collision::DebugDraw()
{

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		if ((*it) == nullptr) {
			continue;
		}

		App->render->DrawQuad((*it)->rect, 255, 255, 255, 255, false);
	}
}
