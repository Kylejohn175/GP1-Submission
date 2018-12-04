/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"

class cRocket : public cSprite
{
private:
	int rocketVelocity;
	int move;

public:
	cRocket();
	void update(double deltaTime);		// Rocket update method
	void setRocketVelocity(int rocketVel);   // Sets the velocity for the rocket
	int getRocketVelocity();				 // Gets the rocket velocity
	void setRocketMove(int rocketMove);   // Sets the rocket move Value
	int getRocketMove();				 // Gets the rocket move value
}; 
#endif