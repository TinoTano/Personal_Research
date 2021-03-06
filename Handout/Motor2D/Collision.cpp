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

	matrix[COLLIDER_FRIENDLY_UNIT][COLLIDER_FRIENDLY_UNIT] = false;

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
	//TODO 3 : Create a new Quadtree
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

	// TODO 3 & TODO 4

	quadTreeChecks = 0;
	quadTreeTimer.Start();

	double quadTreeTime = quadTreeTimer.ReadMs();


	//TODO 4

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
