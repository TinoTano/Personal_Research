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
	UNIT_IDLE, UNIT_MOVING, UNIT_ATTACKING, UNIT_DEAD
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
	unitDirection direction;
	float unitMovementSpeed;
	list<iPoint> path;
	bool destinationReached = true;
	fPoint velocity;
	iPoint destinationTile;
	iPoint destinationTileWorld;
	unitDirection currentDirection;
	SDL_Texture* unitIdleTexture;
	SDL_Texture* unitMoveTexture;
	unitState state;

public:
	bool isSelected;

	//Animations
	vector<Animation> idleAnimations;
	vector<Animation> movingAnimations;
	vector<Animation> attackingAnimations;
	vector<Animation> dyingAnimations;

	Animation* currentAnim = nullptr;

};

#endif // !__UNIT_H__


