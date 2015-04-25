/*
*   Controller.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <iostream>

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
, mCountDown(mFPS)
, mResetViewCenter(mWindow.getView().getCenter())
, mAppStateType(AppState::StateType::Menu)
, mChangeState(false)
{
    mWindow.setPosition(sf::Vector2i(0, 0));
    loadMedia();

    // Initialise menu state
//    mCurrentAppState = std::unique_ptr<MenuState>(new MenuState(*this,
//                                                                mWindow));

    mCurrentAppState = std::unique_ptr<GameState>(new GameState(*this,
                       mWindow
                       , "Debug"));
};

/*
*   Load Textures and fonts from file
*   store in appropriate containers in Controller
*/
void Controller::loadMedia()
{
    for(unsigned int i = 0; i < Textures::NumTextures; i++)
        mTextures.push_back(sf::Texture());

    std::vector<std::string> fileNames;
    fileNames.push_back("media/textures/character.png");
    fileNames.push_back("media/textures/enemy.png");
    fileNames.push_back("media/textures/grass.png");
    fileNames.push_back("media/textures/wall.png");
    fileNames.push_back("media/textures/corner.png");
    fileNames.push_back("media/textures/exit.png");

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
    const float WorldX = mParams.WorldDimMax;
    const float WorldY = mParams.WorldDimMax;
    const int BlockSize = mParams.LevelBlockSize;

    const float TextureX = WorldX * 2.f;
    const float TextureY = WorldY * 2.f;

    mBackgroundTexture.create(TextureX
                              , TextureY);

    mBackgroundTexture.clear();

    for(float row = 0; row < TextureY * 4; row += BlockSize)
    {
        for(float col = 0; col < TextureX; col += BlockSize)
        {
            LevelBlock levelBlock(mTextures.at(Textures::Grass)
                                  , sf::Vector2f(col, row));

            mBackgroundTexture.draw(levelBlock.getBackground());
        }
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
    default:
        break;
    }

    mChangeState = false;
}

/*
*   Application main loop
*/
void Controller::run()
{
    while(mWindow.isOpen())
    {
        mCountDown -= mClock.restart();

        if(mCountDown < sf::Time::Zero)
        {
            mCountDown = mFPS;
            mCurrentAppState->update(mFPS);
        }

        mCurrentAppState->handleInput();
        mCurrentAppState->display();

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

