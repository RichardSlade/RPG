#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

<<<<<<< HEAD:include/App/GameState.hpp
#include "App/PausedScreen.hpp"
#include "App/LevelCompleteScreen.hpp"
#include "App/GameCompleteScreen.hpp"
#include "App/AppState.hpp"
=======
#include "App/AppState.hpp"
#include "App/LevelCompleteScreen.hpp"
#include "App/GameCompleteScreen.hpp"
#include "App/PausedScreen.hpp"
>>>>>>> working:include/App/GameState.hpp
#include "World/World.hpp"

class Controller;
class Params;

class GameState : public AppState
{
private:
    enum Screen
    {
        Game,
        Paused,
        LevelComplete,
        GameComplete
    };

    const int                       mWorldDimMax;
    const int                       mNumEnemyMax;
    const float                     mLevelTimeMin;

    const int                       mResetWorldDim;
    const int                       mResetNumEnemy;
    const float                     mResetLevelTime;

    Controller&                     mController;
    sf::RenderWindow&               mWindow;

    float                           mWorldDim;
    float                           mNumEnemy;
    sf::Time                        mLevelTime;
    float                           mTotalEnemyHerded;

    std::string                     mUsername;

    std::unique_ptr<World>          mWorld;

    PausedScreen                    mPausedScreen;
    LevelCompleteScreen             mLevelCompleteScreen;
    GameCompleteScreen              mGameCompleteScreen;

    Screen                          mCurrentScreen;
    Screen                          mNewScreen;
    bool                            mPaused;

    void                            restartWorld();

public:
    GameState(Controller&
              , sf::RenderWindow&
              , std::string);

    virtual                         ~GameState() {};

    virtual void                    update(sf::Time);
    virtual void                    handleInput();
    virtual void                    display();

    void                            pause();
    void                            levelComplete();
    void                            gameComplete(int);

    void                            nextLevel();
    void                            resetGame();

    void                            quitGameState();

    // Getters
    bool                            getPaused()
    {
        return mPaused;
    }
};

#endif // GAMESTATE_HPP
