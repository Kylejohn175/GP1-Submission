/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	///* Let the computer pick a random number */
	//random_device rd;    // non-deterministic engine 
	//mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	//uniform_int_distribution<> AsteroidDis{ 1, 5 };
	//uniform_int_distribution<> AsteroidTextDis{ 1, 1 };

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;

	// Store the textures
	textureName = { "spaceship1", "spaceship2", "laser", "explosion", "gameBackground", "menuBackground"};
	texturesToUse = { "Images\\Sprites\\heroShip.png", "Images\\Sprites\\enemyShip.png", "Images\\Sprites\\laser.png", "Images\\Sprites\\explosion.png", "Images\\Bkg\\gameBackground.png", "Images\\Bkg\\menuBackground.png" };
	for (int tCount = 0; tCount < (int)textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge", "8bit", "8bitI" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/8bit.ttf", "Fonts/8bit.ttf" };
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		if (fonts == 3)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 14);
		}
		else
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
		}
	}

	// Create text Textures
	gameTextNames = { "enemies", "score", "Title", "highscore", "Instructions", "gameover"};
	gameTextList = { "Enemies", "Score ", "SPACE DEFENDERS", "HIGHSCORE", "Left and Right Arrows to move! Space to shoot! Dont let the enemies reach Earth!", "GAME OVER"};
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 4)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("8bitI")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("8bit")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
		//theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("8bit")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
	}

	strScore = gameTextList[1];
	strScore += to_string(theScore).c_str();
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("8bit")->createTextTexture(theRenderer, gameTextList[0], textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("theScore", theFontMgr->getFont("8bit")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("8bitI")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "gametheme", "laser", "explosion", "menutheme", "gameover"};
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx, soundType::music,soundType::sfx };
	soundsToUse = { "Audio/gameTheme.mp3", "Audio/laser.wav", "Audio/explosion.wav", "Audio/menuTheme.mp3", "Audio/gameover.wav" };
	for (int sounds = 0; sounds < (int)soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("gametheme")->play(-1);

	//Load game buttons
	btnNameList = { "Start", "Quit", "Highscore", "Back" };
	btnTexturesToUse = { "Images\\Sprites\\start.png", "Images\\Sprites\\quit.png", "Images\\Sprites\\highscore.png", "Images\\Sprites\\back.png" };
	btnPos = { { 450, 375 },{ 455, 425 },{ 467, 475 }, { 475, 350 } };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;
	
	heroShip.setSpritePos({ 475 , 600 });
	heroShip.setTexture(theTextureMgr->getTexture("spaceship1"));
	heroShip.setSpriteDimensions(theTextureMgr->getTexture("spaceship1")->getTWidth(), theTextureMgr->getTexture("spaceship1")->getTHeight());
	heroShip.setRocketVelocity(100);
	heroShip.setSpriteTranslation({ 50,50 });

	for (int astro = 0; astro < 9; astro++)
	{
		theenemies.push_back(new cAsteroid);
		theenemies[astro]->setSpritePos({ (50 * (astro + 1) + (50 * astro)), -75 });
		theenemies[astro]->setSpriteTranslation({ 0, -50 });
		int randAsteroid = AsteroidTextDis(gen);
		theenemies[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theenemies[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theenemies[astro]->setAsteroidVelocity(200);
		theenemies[astro]->setSpriteRotAngle(180);
		theenemies[astro]->setActive(true);
	}
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);

	switch (theGameState)
	{
	case gameState::menu:
	{		

		spriteBkgd.setTexture(theTextureMgr->getTexture("menuBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//spriteBkgd.setSpriteRotAngle();

		//Render the Title
			tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 250, 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render the instructions
		tempTextTexture = theTextureMgr->getTexture("Instructions");
		pos = { 10, 700, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render Button
			theButtonMgr->getBtn("Start")->setSpritePos({ 450, 375 });
		theButtonMgr->getBtn("Start")->render(theRenderer, &theButtonMgr->getBtn("Start")->getSpriteDimensions(), &theButtonMgr->getBtn("Start")->getSpritePos(), theButtonMgr->getBtn("Start")->getSpriteScale());
		theButtonMgr->getBtn("Quit")->setSpritePos({ 455, 425 });
		theButtonMgr->getBtn("Quit")->render(theRenderer, &theButtonMgr->getBtn("Quit")->getSpriteDimensions(), &theButtonMgr->getBtn("Quit")->getSpritePos(), theButtonMgr->getBtn("Quit")->getSpriteScale());
		//theButtonMgr->getBtn("Highscore")->setSpritePos({ 467, 475 });
		//theButtonMgr->getBtn("Highscore")->render(theRenderer, &theButtonMgr->getBtn("Highscore")->getSpriteDimensions(), &theButtonMgr->getBtn("Highscore")->getSpritePos(), theButtonMgr->getBtn("Highscore")->getSpriteScale());
	}
	break;

	case gameState::playing:

	{
		spriteBkgd.setSpritePos({ 0, 0 });
		spriteBkgd.setTexture(theTextureMgr->getTexture("gameBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("gameBackground")->getTWidth(), theTextureMgr->getTexture("gameBackground")->getTHeight());

		/*heroShip.setSpritePos({ 475 , 600 });
		heroShip.setTexture(theTextureMgr->getTexture("spaceship1"));
		heroShip.setSpriteDimensions(theTextureMgr->getTexture("spaceship1")->getTWidth(), theTextureMgr->getTexture("spaceship1")->getTHeight());
		heroShip.setRocketVelocity(100);
		heroShip.setSpriteTranslation({ 50,50 });*/

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//Render each asteroid in the vector array
			for (int draw = 0; draw < (int)theenemies.size(); draw++)
			{
				theenemies[draw]->render(theRenderer, &theenemies[draw]->getSpriteDimensions(), &theenemies[draw]->getSpritePos(), theenemies[draw]->getSpriteRotAngle(), &theenemies[draw]->getSpriteCentre(), theenemies[draw]->getSpriteScale());
			}
		//Render each bullet in the vector array
			for (int draw = 0; draw < (int)thelasers.size(); draw++)
			{
				thelasers[draw]->render(theRenderer, &thelasers[draw]->getSpriteDimensions(), &thelasers[draw]->getSpritePos(), thelasers[draw]->getSpriteRotAngle(), &thelasers[draw]->getSpriteCentre(), thelasers[draw]->getSpriteScale());
			}
		//Render each explosion in the vector array
			for (int draw = 0; draw < (int)theExplosions.size(); draw++)
			{
				theExplosions[draw]->render(theRenderer, &theExplosions[draw]->getSourceRect(), &theExplosions[draw]->getSpritePos(), theExplosions[draw]->getSpriteScale());
			}

		//Render the Title
			cTexture* tempTextTexture = theTextureMgr->getTexture("theScore");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render updated score value
		if (updateScore)
		{
			theTextureMgr->deleteTexture("theScore");
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("8bit")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
			updateScore = false;
		}

		//render the hero Ship
		heroShip.render(theRenderer, &heroShip.getSpriteDimensions(), &heroShip.getSpritePos(), heroShip.getSpriteRotAngle(), &heroShip.getSpriteCentre(), heroShip.getSpriteScale());
		SDL_RenderPresent(theRenderer);
	}

	break;

	case gameState::end:
	{

	}
	break;

	case gameState::gameover:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("menuBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Render the Header
		tempTextTexture = theTextureMgr->getTexture("gameover");
		pos = { 350, 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render the Score
		cTexture* tempTextTexture = theTextureMgr->getTexture("theScore");
		SDL_Rect pos = { 350, 300, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Render the Button
		theButtonMgr->getBtn("Back")->setSpritePos({ 100, 600 });
		theButtonMgr->getBtn("Back")->render(theRenderer, &theButtonMgr->getBtn("Back")->getSpriteDimensions(), &theButtonMgr->getBtn("Back")->getSpritePos(), theButtonMgr->getBtn("Back")->getSpriteScale());
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
	
}

void cGame::update(double deltaTime)
{
	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("Start")->update(theGameState, gameState::playing, theAreaClicked);
		gameOver = false;
	
		theGameState = theButtonMgr->getBtn("Quit")->update(theGameState, gameState::end, theAreaClicked);

		theGameState = theButtonMgr->getBtn("Highscore")->update(theGameState, gameState::gameover, theAreaClicked);
	}

	// Check Button clicked and change state
	//if (theGameState == gameState::menu || theGameState == gameState::end)
	//{
		//theGameState = theButtonMgr->getBtn("Quit")->update(theGameState, gameState::end, theAreaClicked);
	//}

	if (theGameState == gameState::gameover)
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("menuBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("menuBackground")->getTWidth(), theTextureMgr->getTexture("menuBackground")->getTHeight());
		theGameState = theButtonMgr->getBtn("Back")->update(theGameState, gameState::menu, theAreaClicked);
	}

	if (theGameState == gameState::playing)
	{
		if (gameOver)
		{
			theGameState = gameState::gameover;
		}
	}

	if (theGameState == gameState::end)
	{
		loop = false;
	}

	IPoint heroPos = { (int)heroShip.getSpritePos().x, (int)heroShip.getSpritePos().y };
	if (heroPos.X >= 910 || heroPos.X <= 0)
	{
		heroShip.setSpritePos({ heroShip.getSpritePos().x, heroShip.getSpritePos().y });
		heroShip.setRocketMove(heroShip.getRocketMove()*(-1));
	}
	

	// Update the visibility and position of each asteriod
	vector<cAsteroid*>::iterator asteroidIterator = theenemies.begin();
	while (asteroidIterator != theenemies.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theenemies.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			if ((*asteroidIterator)->getSpritePos().y >= 632)
			{
				gameOver = true;
			}

			++asteroidIterator;
		}
	}
	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = thelasers.begin();
	while (bulletIterartor != thelasers.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = thelasers.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}
	// Update the visibility and position of each explosion
	vector<cSprite*>::iterator expIterartor = theExplosions.begin();
	while (expIterartor != theExplosions.end())
	{
		if ((*expIterartor)->isActive() == false)
		{
			expIterartor = theExplosions.erase(expIterartor);
		}
		else
		{
			(*expIterartor)->animate(deltaTime);
			++expIterartor;
		}
	}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = thelasers.begin(); bulletIterartor != thelasers.end(); ++bulletIterartor)
	{
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theenemies.begin(); asteroidIterator != theenemies.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				theExplosions.push_back(new cSprite);
				int index = theExplosions.size()-1;
				theExplosions[index]->setSpriteTranslation({ 0, 0 });
				theExplosions[index]->setActive(true);
				theExplosions[index]->setNoFrames(16);
				theExplosions[index]->setTexture(theTextureMgr->getTexture("explosion"));
				theExplosions[index]->setSpriteDimensions(theTextureMgr->getTexture("explosion")->getTWidth()/ theExplosions[index]->getNoFrames(), theTextureMgr->getTexture("explosion")->getTHeight());
				theExplosions[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x + (int)((*asteroidIterator)->getSpritePos().w/2), (*asteroidIterator)->getSpritePos().y + (int)((*asteroidIterator)->getSpritePos().h / 2) });


				theSoundMgr->getSnd("explosion")->play(0);
				
				theScore += 100;
				strScore = gameTextList[1];
				strScore += to_string(theScore).c_str();
				updateScore = true;
			}
		}
	}


	// Update the hero and enemy position
	heroShip.update(deltaTime);
	if (frames % 360 == 0)
	{
		int currentSize = theenemies.size();
		int end = currentSize + 9;
		int count = 0;
		for (int astro = currentSize; astro < end; astro++)
		{
			theenemies.push_back(new cAsteroid);
			theenemies[astro]->setSpritePos({ (50 * (count + 1) + (50 * count)), -75 });
			theenemies[astro]->setSpriteTranslation({ 0, -50 });
			int randAsteroid = AsteroidTextDis(gen);
			theenemies[astro]->setTexture(theTextureMgr->getTexture("spaceship2"));
			theenemies[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
			theenemies[astro]->setAsteroidVelocity(200);
			theenemies[astro]->setSpriteRotAngle(180);
			theenemies[astro]->setActive(true);
			count++;
		}
	}

	frames++;
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					
				}
				break;

				case SDLK_UP:
				{
					
				}
				break;
				case SDLK_RIGHT:
				{
					heroShip.setRocketMove(-1);
					cout << "Right" << endl;
				}
				break;

				case SDLK_LEFT:
				{
					heroShip.setRocketMove(1);
					cout << "Left" << endl;
				}
				break;
				case SDLK_SPACE:
				{
					thelasers.push_back(new cBullet);
					int numBullets = thelasers.size() - 1;
					thelasers[numBullets]->setSpritePos({ heroShip.getBoundingRect().x + heroShip.getSpriteCentre().x, heroShip.getBoundingRect().y + heroShip.getSpriteCentre().y });
					thelasers[numBullets]->setSpriteTranslation({ 50, 50 });
					thelasers[numBullets]->setTexture(theTextureMgr->getTexture("laser"));
					thelasers[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("laser")->getTWidth(), theTextureMgr->getTexture("laser")->getTHeight());
					thelasers[numBullets]->setBulletVelocity(5);
					thelasers[numBullets]->setSpriteRotAngle(heroShip.getSpriteRotAngle() -90.0f );
					thelasers[numBullets]->setActive(true);
					cout << "Laser added to Vector at position - x: " << heroShip.getBoundingRect().x << " y: " << heroShip.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("laser")->play(0);
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

