/*
*   Controller.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <iostream>
#include <string>
#include <cassert>

#include <SFML/Graphics/RenderTexture.hpp>

#include "App/Controller.hpp"
#include "App/MenuState.hpp"
#include "App/GameState.hpp"
#include "World/LevelBlock.hpp"

// Const FPS for frame cap
const sf::Time Controller::mFPS = sf::seconds(1.f / 60.f);

Controller::Controller()
: mParams()
, mWindowX(mParams.WindowX)
, mWindowY(mParams.WindowY)
, mWindow(sf::VideoMode(mWindowX, mWindowY), "AI Steering behaviours")// sf::Style::Fullscreen)
//, mCountDown(sf::Time::Zero)
//, mTimeSinceLastUpdate(sf::Time::Zero)
, mResetViewCenter(mWindow.getView().getCenter())
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mAppStateType(AppState::StateType::Menu)
, mChangeState(false)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    loadMedia();

    mStatisticsText.setFont(getFont(Controller::Fonts::Sansation));
    mStatisticsText.setCharacterSize(16);
    mStatisticsText.setString("0.000");

    // Initialise menu state
//    mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this,
//                                                                mWindow));

    mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this,
                                                                mWindow
                                                                , "Debug"));
};

/*
*   Load Textures and fonts from files
*   store in appropriate containers in Controller
*/
void Controller::loadMedia()
{
    for(unsigned int i = 0; i < Textures::NumTextures; i++)
        mTextures.push_back(Controller::upTexture());

    std::vector<std::string> fileNames;
    fileNames.push_back("media/textures/adventurer.png");
    fileNames.push_back("media/textures/enemy.png");
    fileNames.push_back("media/textures/grass.png");
    fileNames.push_back("media/textures/wall.png");
    fileNames.push_back("media/textures/corner.png");
    fileNames.push_back("media/textures/exit.png");
//    fileNames.push_back("media/textures/checkerMeter.png");
    fileNames.push_back("media/textures/brickLarge.png");
    fileNames.push_back("media/textures/stoneLarge.png");

   assert(mTextures.size() >= fileNames.size());

   std::vector<Controller::upTexture>::iterator texIter = mTextures.begin();
   std::vector<std::string>::iterator fileNameIter;

   unsigned int index = 0;

    for(fileNameIter = fileNames.begin();
         fileNameIter != fileNames.end();
         fileNameIter++)
    {
      sf::Texture t;

      if(!t.loadFromFile(*fileNameIter))
         throw std::runtime_error("Failed to load file: " + *fileNameIter);

//      Controller::upTexture upTex(new sf::Texture(t));

      if(index == Controller::Textures::Brick
        || index == Controller::Textures::Stone)
         t.setRepeated(true);

      (*texIter).reset(new sf::Texture(t));
      texIter++;

      index ++;

//      mTextures.at(index) =
    }

//   texIter ++;
//   sf::Texture tex = mTe;
//   tex.setRepeated(true);
//   mTextures.at(Controller::Textures::GameBackground).reset(new sf::Texture(tex));

   // Fonts
   for(unsigned int i = 0; i < Fonts::NumFonts; i++)
      mFonts.push_back(Controller::upFont());

   std::vector<Controller::upFont>::iterator fontIter = mFonts.begin();

    fileNames.clear();

    fileNames.push_back("media/fonts/Sansation.ttf");
//    fileNames.push_back("media/fonts/AlphaSmoke.TTF");
//    fileNames.push_back("media/fonts/KingthingsSheepishly.ttf");

     for(fileNameIter = fileNames.begin();
         fileNameIter != fileNames.end();
         fileNameIter++)
    {
        sf::Font f;

        if(!f.loadFromFile(*fileNameIter))
            throw std::runtime_error("Failed to load file: " + (*fileNameIter));

        (*fontIter).reset(new sf::Font(f));
        fontIter++;
    }
}

/*
*   Create tiled background from sf::Textures
*/
const sf::Texture Controller::createBackgroundTexture()
{
   const sf::Texture& Tex = getTexture(Textures::Brick);
   const sf::Vector2u TexSize = Tex.getSize();

   const unsigned int Multiplier = 10;
   const unsigned int BackgroundTexSizeX = TexSize.x * Multiplier;
   const unsigned int BackgroundTexSizeY = TexSize.y * Multiplier;

   sf::RenderTexture backgroundTex;
   backgroundTex.create(BackgroundTexSizeX
                              , BackgroundTexSizeY);
   backgroundTex.clear();
//   backgroundTex.setRepeated(true);

   sf::Vector2f spritePos;

//   const sf::Texture& texture = getTexture(Textures::Grass);

//   sf::Sprite sprite(texture, sf::IntRect(sf::Vector2i(0, 0),
//                                          sf::Vector2i(texture.getSize())));
//
//   mBackgroundTexture.draw(sprite);

   sf::Sprite sprite(Tex);

   for(unsigned int row = 0; row < Multiplier; row ++)
   {
      for(unsigned int col = 0; col < Multiplier; col++)
      {
         sprite.setPosition(spritePos);
         sprite.setColor(sf::Color(150, 125, 200));
         backgroundTex.draw(sprite);

         spritePos.x += TexSize.x;
      }
      spritePos.x = 0.f;
      spritePos.y += TexSize.y;
   }

//   backgroundTex.getTexture().setRepeated(true);
   return backgroundTex.getTexture();
}

/*
*   Change App state from menu -> game or vice versa
*/
void Controller::changeAppState()
{
    resetView();

    switch(mAppStateType)
    {
        case AppState::StateType::Menu:
        {
            mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this
                                                                        , mWindow
                                                                        , mUserName));
            mAppStateType = AppState::StateType::Game;
            break;

        }
        case AppState::StateType::Game:
        {
            mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this
                                                                        , mWindow));
            mAppStateType = AppState::StateType::Menu;
            break;
        }
        default: break;
    }

    mChangeState = false;
}

void Controller::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

/*
*   Application main loop
*/
void Controller::run()
{
   sf::Time timeSinceLastUpdate = sf::Time::Zero;

   while(mWindow.isOpen())
   {
   //        mCountDown -= mClock.restart();
      sf::Time dt = mClock.restart();
      timeSinceLastUpdate += dt;

      while(timeSinceLastUpdate > mFPS)
      {
         timeSinceLastUpdate -= mFPS;

         mCurrentAppState->handleInput();
         mCurrentAppState->lockedUpdate(mFPS);
//         mCountDown = mFPS;
      }

      updateStatistics(dt);

      mCurrentAppState->unlockedUpdate();

      mWindow.clear();
      mCurrentAppState->display();

      sf::View view = mWindow.getView();
      mStatisticsText.setPosition(view.getCenter().x - (view.getSize().x / 2),
                                 view.getCenter().y - (view.getSize().y / 2));
      mWindow.draw(mStatisticsText);

      mWindow.display();

      if(mChangeState)
         changeAppState();
   }
}

/*
*   Ensure view is always at centre when changing states
*/
void Controller::resetView()
{
    sf::View view = mWindow.getView();
    view.setCenter(mResetViewCenter);
    mWindow.setView(view);
}

