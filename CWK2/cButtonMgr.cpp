/*
=================
cButtinMgr.cpp
- CPP file for class definition - IMPLEMENTATION
- CPP file for the ButtonMgr class
=================
*/
#include "cButtonMgr.h"
cButtonMgr* cButtonMgr::pInstance = NULL;

/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cButtonMgr* cButtonMgr::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cButtonMgr();
	}
	return cButtonMgr::pInstance;
}

/*
=================
- Data constructor initializes the buttonMgr object
- Is always called, thus ensures all buttonMgr objects are in a consistent state.
=================
*/
cButtonMgr::cButtonMgr()
{

}

/*
=================
- Destructor.
=================
*/
cButtonMgr::~cButtonMgr()
{
	deleteBtn();
}

void cButtonMgr::add(LPCSTR btnName, cButton* theBtn)
{
	if (!getBtn(btnName))
	{
		//cButton * newBtn = new cButton();
		//newTxt->loadTexture(theFilename, theSDLRenderer);
		gameBtns.insert(make_pair(btnName, theBtn));
	}
}

void cButtonMgr::deleteBtn()
{
	for (map<LPCSTR, cButton*>::iterator btn = gameBtns.begin(); btn != gameBtns.end(); ++btn)
	{
		delete btn->second;
	}
}

/*
=================
- return the texture.
=================
*/
cButton* cButtonMgr::getBtn(LPCSTR btnName)        // return the texture.
{
	map<LPCSTR, cButton*>::iterator btn = gameBtns.find(btnName);
	if (btn != gameBtns.end())
	{
		return btn->second;
	}
	else
	{
		return NULL;
	}
}
