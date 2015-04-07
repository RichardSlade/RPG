#include <SFML/Window/Event.hpp>

#include "incl/GameState.hpp"
#include "incl/Controller.hpp"
#include "incl/Params.hpp"

GameState::GameState(Controller& cntrl
                     , sf::RenderWindow& window
                     , std::string username)
: mWorldDimMax(cntrl.getParams().WorldDimMax)
, mNumSheepMax(cntrl.getParams().NumSheepMax)
, mLevelTimeMin(cntrl.getParams().LevelTimeMin)
, mResetWorldDim(cntrl.getParams().WorldDimMin)
, mResetNumSheep(cntrl.getParams().NumSheepMin)
, mResetLevelTime(cntrl.getParams().LevelTimeMax)
, mController(cntrl)
, mWindow(window)
, mWorldDim(mResetWorldDim)
, mNumSheep(mResetNumSheep)
, mLevelTime(sf::seconds(mResetLevelTime * 60.f))
, mTotalSheepHerded(0)
, mUsername(username)
, mWorld(new World(*this
                   , cntrl
                   , window
                   , username
                   , mWorldDim
                   , mNumSheep
                   , mLevelTime))
, mPausedScreen(mController
                , *this
                , mWindow)
, mLevelCompleteScreen(mController
                     , *this
                     , mWindow)
, mGameCompleteScreen(mController
                   , *this
                   , mWindow)
, mCurrentScreen(GameState::Screen::Game)
, mNewScreen(mCurrentScreen)
, mPaused(false)
{

}

void GameState::restartWorld()
{
    mWorld = std::unique_ptr<World>(new World(*this
                                              , mController
                                              , mWindow
                                              , mUsername
                                              , mWorldDim
                                              , mNumSheep
                                              , mLevelTime));

    mNewScreen = GameState::Screen::Game;
}

void GameState::update(sf::Time dt)
{
    if(mNewScreen != mCurrentScreen)
        mCurrentScreen = mNewScreen;

    switch(mCurrentScreen)
    {
        case GameState::Screen::Game:
        {
            if(!mPausedScreen.isPaused())
                mWorld->update(dt);
            else
                mPausedScreen.update(dt);

            break;
        }
        case GameState::Screen::LevelComplete: mLevelCompleteScreen.update(dt); break;
        case GameState::Screen::GameComplete: mGameCompleteScreen.update(dt); break;
        default: break;
    }
}

void GameState::handleInput()
{
    if(mCurrentScreen == GameState::Screen::Game
       && mWorld)
    {
        if(!mPausedScreen.isPaused())
            mWorld->handleInput();
        else
            mPausedScreen.handleInput();
    }
    else if(mCurrentScreen == GameState::Screen::LevelComplete)
        mLevelCompleteScreen.handleInput();
    else if(mCurrentScreen == GameState::Screen::GameComplete)
        mGameCompleteScreen.handleInput();
}

void GameState::display()
{
    mWindow.clear();

    if(mWorld)
        mWorld->display();

    if(mCurrentScreen == GameState::Screen::Game)
    {
        if(mPausedScreen.isPaused())
            mWindow.draw(mPausedScreen);
    }
    else if(mCurrentScreen == GameState::Screen::LevelComplete)
        mWindow.draw(mLevelCompleteScreen);
    else if(mCurrentScreen == GameState::Screen::GameComplete)
        mWindow.draw(mGameCompleteScreen);

    mWindow.display();
}

void GameState::pause()
{
    mPausedScreen.setup(mWindow.getView());
}

void GameState::levelComplete()
{
    mNewScreen = GameState::Screen::LevelComplete;
    mLevelCompleteScreen.setup(mWindow.getView());
}

void GameState::gameComplete(int sheepFromLastLevel)
{
    mNewScreen = GameState::Screen::GameComplete;

    mGameCompleteScreen.setup(mWindow.getView(), mTotalSheepHerded + sheepFromLastLevel);
}

void GameState::nextLevel()
{
    const float blockSize = mController.getParams().LevelBlockSize;

    mWorldDim += blockSize * 4;
    mNumSheep += 10;
    mLevelTime -= mWorld->getTimeTaken();
    mLevelTime += sf::seconds(mLevelTimeMin * 60.f);

    mTotalSheepHerded += mWorld->getSheepHerded();

    if(mWorldDim > mWorldDimMax)
        mWorldDim = mWorldDimMax;

    restartWorld();
}

void GameState::resetGame()
{
    mWorldDim = mResetWorldDim;
    mNumSheep = mResetNumSheep;
    mLevelTime = sf::seconds(mResetLevelTime * 60.f);

    mTotalSheepHerded = 0;

    restartWorld();
}

void GameState::quitGameState()
{
    mController.changeState();
}
