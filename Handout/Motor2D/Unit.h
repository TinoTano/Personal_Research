#ifndef __UNIT_H__
#define __UNIT_H__

#include "p2Point.h"
#include "Entity.h"
#include "Animation.h"
#include <list>
#include <vector>
#include "Input.h"
#include "Map.h"

enum unitState
{
	UNIT_IDLE, UNIT_MOVING
};

enum unitDirection {
	DOWN, DOWN_LEFT, DOWN_RIGHT, LEFT, RIGHT, UP_LEFT, UP_RIGHT, UP
};

class Unit : public Entity
{
public:
	Unit(int posX, int posY);
	~Unit();

	bool Update(float dt);
	bool Draw();

	void SetDestination();
	void Move(float dt);
	void CalculateVelocity();
	void LookAt();
	void SetAnim(unitDirection currentDirection);
	void SetState(unitState state);

private:
	unitDirection direction = RIGHT;
	float unitMovementSpeed = 0;
	list<iPoint> path;
	bool destinationReached = true;
	fPoint velocity = { 0,0 };
	iPoint destinationTile = { 0,0 };
	iPoint destinationTileWorld = { 0,0 };
	unitDirection currentDirection = RIGHT;
	SDL_Texture* unitIdleTexture = nullptr;
	SDL_Texture* unitMoveTexture = nullptr;
	unitState state = UNIT_IDLE;

public:
	bool isSelected;

	//Animations
	vector<Animation> idleAnimations;
	vector<Animation> movingAnimations;

	Animation* currentAnim = nullptr;

};

#endif // !__UNIT_H__


