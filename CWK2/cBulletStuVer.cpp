/*
=================
cBullet.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cBullet.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cBullet::cBullet() : cSprite()
{
	this->bulletVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cBullet::update(double deltaTime)
{
	auto rads = PI / 180.0f * this->getSpriteRotAngle();

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (float)(cos(rads));
	direction.Y = (float)(sin(rads));

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(direction.X * this->bulletVelocity * this->getSpriteTranslation().x * deltaTime);
	currentSpritePos.y += (int)(direction.Y * this->bulletVelocity * this->getSpriteTranslation().y * deltaTime);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });

	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
  Sets the velocity for the Bullet
=================================================================
*/
void cBullet::setBulletVelocity(int bulletVel)
{
	this->bulletVelocity = bulletVel;
}
/*
=================================================================
  Gets the Bullet velocity
=================================================================
*/
int cBullet::getBulletVelocity()
{
	return this->bulletVelocity;
}
