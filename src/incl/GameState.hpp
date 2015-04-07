#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "AppState.hpp"
#include "World.hpp"
#include "PausedScreen.hpp"
#include "LevelCompleteScreen.hpp"
#include "GameCompleteScreen.hpp"

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
    const int                       mNumSheepMax;
    const float                     mLevelTimeMin;

    const int                       mResetWorldDim;
    const int                       mResetNumSheep;
    const float                     mResetLevelTime;

    Controller&                     mController;
    sf::RenderWindow&               mWindow;

    float                           mWorldDim;
    float                           mNumSheep;
    sf::Time                        mLevelTime;
    float                           mTotalSheepHerded;

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

    virtual                         ~GameState(){};

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
                                    { return mPaused; }
};

#endif // GAMESTATE_HPP
