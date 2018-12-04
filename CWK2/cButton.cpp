/*
=================
- cButton.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cButton.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cButton::cButton() : cSprite()
{
}


/*
=================================================================
Update the sprite position
=================================================================
*/


gameState cButton::update(gameState theCurrentGameState, gameState newGameState, SDL_Point theAreaClicked)
{
	SDL_Point areaClicked = theAreaClicked;
	if (areaClicked.x > this->getSpritePos().x && areaClicked.x < (this->getSpritePos().x + this->getSpriteDimensions().w) && areaClicked.y > this->getSpritePos().y && areaClicked.y < (this->getSpritePos().y + this->getSpriteDimensions().h))
	{
		this->buttonClickedRC.x = (int)(areaClicked.x - this->getSpritePos().x) / this->getSpriteDimensions().w;
		this->buttonClickedRC.y = (int)(areaClicked.y - this->getSpritePos().y) / this->getSpriteDimensions().h;
		this->clicked = true;
		return newGameState;
	}
	return theCurrentGameState;
}

bool cButton::getClicked()
{
	return this->clicked;
}
void cButton::setClicked(bool state)
{
	this->clicked = state;
}