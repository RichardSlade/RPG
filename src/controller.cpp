/*
*   Controller.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <iostream>
#include <string>

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
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mResetViewCenter(mWindow.getView().getCenter())
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
        mTextures.push_back(sf::Texture());

    std::vector<std::string> fileNames;
    fileNames.push_back("media/textures/adventurer.png");
    fileNames.push_back("media/textures/enemy.png");
    fileNames.push_back("media/textures/grass.png");
    fileNames.push_back("media/textures/wall.png");
    fileNames.push_back("media/textures/corner.png");
    fileNames.push_back("media/textures/exit.png");
    fileNames.push_back("media/textures/checkerMeter.png");

    int index = 0;

    for(std::string s : fileNames)
    {
        sf::Texture t;

        if(!t.loadFromFile(s))
            throw std::runtime_error("Failed to load file: " + s);

        mTextures.at(index) = t;

        index ++;
    }

    mTextures.at(Textures::GameBackground) = createBackgroundTexture();

    fileNames.clear();

    fileNames.push_back("media/fonts/Sansation.ttf");
//    fileNames.push_back("media/fonts/AlphaSmoke.TTF");
//    fileNames.push_back("media/fonts/KingthingsSheepishly.ttf");

     for(std::string s : fileNames)
    {
        sf::Font f;

        if(!f.loadFromFile(s))
            throw std::runtime_error("Failed to load file: " + s);

        mFonts.push_back(f);
    }
}

/*
*   Create tiled background from sf::Textures
*/
const sf::Texture& Controller::createBackgroundTexture()
{
    mBackgroundTexture.create(mParams.WorldDimMax
                              , mParams.WorldDimMax);
   mBackgroundTexture.clear();

   sf::Vector2f spritePos;
   const sf::Texture& texture = getTexture(Textures::Grass);
   sf::Sprite sprite(texture);

   for(int row = 0; row < 1000; row ++)
   {
      for(int col = 0; col < 1000; col++)
      {
         sprite.setPosition(spritePos);
         sprite.setColor(sf::Color(150, 125, 200));
         mBackgroundTexture.draw(sprite);

         spritePos.x += texture.getSize().x;
      }
      spritePos.x = 0.f;
      spritePos.y += texture.getSize().y;
   }

    return mBackgroundTexture.getTexture();
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

