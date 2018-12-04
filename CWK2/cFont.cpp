/*
==========================================================================
cFont.cpp
==========================================================================
*/
//#pragma warning ( disable : 4996 )
#include "cFont.h"
using namespace std;

/*
==========================================================================
Default Constructor
==========================================================================
*/
cFont::cFont()
{
	theFont = NULL;
}
/*
==========================================================================
Destructor
==========================================================================
*/
cFont::~cFont()
{
	//Close the font that was used
	TTF_CloseFont(theFont);
}
/*
==========================================================================
Load the desiered font
==========================================================================
*/
bool cFont::loadFont(LPCSTR fontFileName, int fontSize)
{
	theFont = TTF_OpenFont(fontFileName, fontSize);

	if (theFont == NULL)
	{
		cout << " Failed to load font : " << SDL_GetError() << endl;
		return false;
	}
	return true;
}

/*
==========================================================================
get the pointer to the font
==========================================================================
*/
TTF_Font* cFont::getFont()
{
	return theFont;
}

/*
==========================================================================
Render the text using the desired font
==========================================================================
*/
SDL_Texture* cFont::createTextTexture(SDL_Renderer* theRenderer, LPCSTR text, textType txtType, SDL_Color txtColour, SDL_Color txtBkgd)
{
	SDL_Texture* theTxtTexture = NULL;
	SDL_Surface* theTxtSurface = NULL;

	switch (txtType)
	{
	    case textType::solid:
		{
			theTxtSurface = TTF_RenderText_Solid(theFont, text, txtColour);
		}
		break;
		case textType::blended:
		{
			theTxtSurface = TTF_RenderText_Blended(theFont, text, txtColour);
		}
		break;
		case textType::shaded:
		{
			theTxtSurface = TTF_RenderText_Shaded(theFont, text, txtColour, txtBkgd);
		}
		break;
		default:
			break;
	}

	theTxtTexture = SDL_CreateTextureFromSurface(theRenderer, theTxtSurface);
	SDL_FreeSurface(theTxtSurface);

	return theTxtTexture;
}