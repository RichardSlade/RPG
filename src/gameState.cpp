#include <SFML/Window/Event.hpp>

#include "App/GameState.hpp"
#include "App/Controller.hpp"
#include "App/Params.hpp"

GameState::GameState(Controller& cntrl
                     , sf::RenderWindow& window
                     , std::string username)
: mWorldDimMax(cntrl.getParams().WorldDimMax)
, mNumEnemyMax(cntrl.getParams().NumEnemyMax)
, mLevelTimeMin(cntrl.getParams().LevelTimeMin)
, mResetWorldDim(cntrl.getParams().WorldDimMin)
, mResetNumEnemy(cntrl.getParams().NumEnemyMin)
, mResetLevelTime(cntrl.getParams().LevelTimeMax)
, mController(cntrl)
, mWindow(window)
, mWorldDim(mResetWorldDim)
, mNumEnemy(mResetNumEnemy)
, mLevelTime(sf::seconds(mResetLevelTime * 60.f))
, mTotalEnemyHerded(0)
, mUsername(username)
, mWorld(new World(*this
                   , cntrl
                   , window
                   , username
                   , mWorldDim
                   , mNumEnemy
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
                                              , mNumEnemy
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

    mGameCompleteScreen.setup(mWindow.getView(), mTotalEnemyHerded + sheepFromLastLevel);
}

void GameState::nextLevel()
{
    const float blockSize = mController.getParams().LevelBlockSize;

    mWorldDim += blockSize * 4;
    mNumEnemy += 10;
    mLevelTime -= mWorld->getTimeTaken();
    mLevelTime += sf::seconds(mLevelTimeMin * 60.f);

//    mTotalEnemyHerded += mWorld->getEnemyHerded();

    if(mWorldDim > mWorldDimMax)
        mWorldDim = mWorldDimMax;

    restartWorld();
}

void GameState::resetGame()
{
    mWorldDim = mResetWorldDim;
    mNumEnemy = mResetNumEnemy;
    mLevelTime = sf::seconds(mResetLevelTime * 60.f);

    mTotalEnemyHerded = 0;

    restartWorld();
}

void GameState::quitGameState()
{
    mController.changeState();
}
