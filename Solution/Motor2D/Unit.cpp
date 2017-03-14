#include "Unit.h"
#include "Render.h"
#include "Application.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include "p2Log.h"
#include "math.h"
#include "Map.h"
#include "Application.h"
#include "Collision.h"
#include "Textures.h"
#include "p2Defs.h"
#include "Scene.h"

Unit::Unit(int posX, int posY)
{
	entityPosition.x = posX;
	entityPosition.y = posY;

	//Anim Idle//
	for (int i = 0; i < 5; i++) {
		Animation idle;
		for (int j = 0; j < 10; j++) {
			idle.PushBack({ 56*j,49*i,56,49 });
		}
		idle.speed = 0.2f;
		idleAnimations.push_back(idle);
		if (i != 0 && i != 5 - 1) {
			idle.flip = SDL_FLIP_HORIZONTAL;
			idleAnimations.push_back(idle);
		}
	}

	//Anim Moving//
	for (int i = 0; i < 5; i++) {
		Animation move;
		for (int j = 0; j < 10; j++) {
			move.PushBack({ 60*j,52*i,60,52 });
		}
		move.speed = 0.2f;
		movingAnimations.push_back(move);
		if (i != 0 && i != 5 - 1) {
			move.flip = SDL_FLIP_HORIZONTAL;
			movingAnimations.push_back(move);
		}
	}

	unitIdleTexture = App->tex->Load("textures/Units/ElvenArcher/ElvenArcherIdle.png");
	unitMoveTexture = App->tex->Load("textures/Units/ElvenArcher/ElvenArcherMove.png");

	entityTexture = unitIdleTexture;

	currentDirection = RIGHT; // starting direction
	SetAnim(currentDirection);

	SDL_Rect r = currentAnim->GetCurrentFrame();
	SDL_Rect colliderRect = { entityPosition.x - (r.w / 2.5f), entityPosition.y - (r.h / 2.5f), r.w / 1.25f, r.h };
	COLLIDER_TYPE colliderType;

	colliderType = COLLIDER_FRIENDLY_UNIT;

	collider = App->collision->AddCollider(colliderRect, colliderType, App->entityManager);

	isSelected = false;

}

Unit::~Unit()
{
}

bool Unit::Update(float dt)
{
	switch (state) {
	case UNIT_MOVING:
		Move(dt);
		break;

	}

	return true;
}

bool Unit::Draw()
{
	SDL_Rect r = currentAnim->GetCurrentFrame();
	collider->rect.x = entityPosition.x - (r.w / 2.5f);
	collider->rect.y = entityPosition.y - (r.h / 2.5f);
	
	if (isSelected) {
		App->render->DrawCircle(entityPosition.x, entityPosition.y + (r.h / 2), 10, 255, 255, 255, 255);
		App->render->Blit(entityTexture, entityPosition.x - (r.w / 2), entityPosition.y - (r.h / 2), &r, currentAnim->flip);
	}
	else {
		App->render->Blit(entityTexture, entityPosition.x - (r.w / 2), entityPosition.y - (r.h / 2), &r, currentAnim->flip);
	}
	
	return true;
}

void Unit::SetDestination()
{
	iPoint target;
	App->input->GetMousePosition(target.x, target.y);
	target = App->map->WorldToMap(target.x - App->render->camera.x, target.y - App->render->camera.y);

	iPoint origin = App->map->WorldToMap(entityPosition.x, entityPosition.y);
	App->pathfinding->CreatePath(origin, target, path);

	if (path.size() > 0) {
		SetState(UNIT_MOVING);
		destinationReached = false;
		if (path.front() == origin) {
			if (path.size() > 1) {
				destinationTile = path.begin()._Ptr->_Next->_Myval;
				path.remove(path.begin()._Ptr->_Next->_Myval);
			}
		}
		else {
			destinationTile = path.front();
		}
		path.erase(path.begin());
	}
}

void Unit::Move(float dt)
{
	CalculateVelocity();
	LookAt();

	if (!destinationReached) {

		fPoint vel = (velocity * (unitMovementSpeed + 100)) * dt;
		roundf(vel.x);
		roundf(vel.y);
		entityPosition.x += int(vel.x);
		entityPosition.y += int(vel.y);

		if (entityPosition.DistanceNoSqrt(destinationTileWorld) < 1) {
			if (path.size() > 0) {
				destinationTile = path.front();
				path.erase(path.begin());
			}
			else {
				destinationReached = true;
				SetState(UNIT_IDLE);
			}
		}
	}
}

void Unit::CalculateVelocity()
{

	destinationTileWorld = App->map->MapToWorld(destinationTile.x + 1, destinationTile.y);
	velocity.x = destinationTileWorld.x - entityPosition.x;
	velocity.y = destinationTileWorld.y - entityPosition.y;

	velocity.Normalize();
}

void Unit::LookAt()
{

	float angle = atan2f(velocity.y, velocity.x) * RADTODEG;

	if (angle < 22.5 && angle > -22.5)
		direction = RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		direction = DOWN_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		direction = DOWN;
	else if (angle >= 112.5 && angle <= 157.5)
		direction = DOWN_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		direction = LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		direction = UP_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		direction = UP;
	else if (angle >= -67.5 && angle <= -22.5)
		direction = UP_RIGHT;

	if (direction != currentDirection)
	{
		currentDirection = direction;
		SetAnim(currentDirection);
	}
}

void Unit::SetState(unitState newState)
{
	switch (newState) {
	case UNIT_IDLE:
		this->state = UNIT_IDLE;
		SetAnim(currentDirection);
		entityTexture = unitIdleTexture;
		break;
	case UNIT_MOVING:
		this->state = UNIT_MOVING;
		SetAnim(currentDirection);
		entityTexture = unitMoveTexture;
		break;
	}
}

void Unit::SetAnim(unitDirection currentDirection) {

	switch (state) {
	case UNIT_IDLE:
		currentAnim = &idleAnimations[currentDirection];
		break;
	case UNIT_MOVING:
		currentAnim = &movingAnimations[currentDirection];
		break;
	}
}
