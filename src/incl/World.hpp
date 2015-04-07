#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "Level.hpp"
#include "SceneNode.hpp"
#include "Sheep.hpp"
#include "Dog.hpp"
#include "State.hpp"
#include "EntityStats.hpp"
#include "HUD.hpp"

class Controller;
class LevelBlock;
class Wall;
class Waypoint;
class MovingEntity;
class GameState;
class Dog;
class SpriteNode;

class World : private sf::NonCopyable
{
public:
    const float                                 mWorldX;
    const float                                 mWorldY;
    const float                                 mExitRadius;

    const int                                   mLevelBlockSize;
    const int                                   mNumSheep;

    enum StatsType
    {
        SheepStats,
        DogStats,
        StatsTypeNum
    };

private:
    static const sf::Time                       mComboTime;

    const float                                 mWaypointRadius;
    const float                                 mScrollSpeed;

    GameState&                                  mGameState;
    sf::RenderWindow&                           mWindow;
    sf::View                                    mWorldView;
    sf::IntRect                                 mWorldBounds;
    sf::Vector2f                                mFocusPoint;

    SceneNode                                   mSceneGraph;
    std::array<SceneNode*
                , SceneNode::Layers::Num>       mSceneLayers;

    int                                         mSheepHerded;
    sf::Time                                    mTimeLeft;
    sf::Time                                    mTimeTaken;

    Level                                       mLevel;
    HUD                                         mHUD;

    std::vector<EntityStats>                    mEntityStats;
    std::vector<std::unique_ptr<State<Dog>>>    mDogStates;
    std::vector<std::unique_ptr<State<Sheep>>>  mSheepStates;

    SpriteNode*                                 mBackground;
    sf::Vector2f                                mExitPos;
    Dog*                                        mDog;

    void                                        initialiseStatesAndStats();
    void                                        buildScene(const Controller&);
    void                                        generateAgents(const Controller&);
    void                                        handleRealTimeInput();
    void                                        adjustView();

public:
                                                World(GameState&
                                                      , const Controller&
                                                      , sf::RenderWindow&
                                                      , std::string
                                                      , int worldDim
                                                      , int numSheep
                                                      , sf::Time levelTime);

    void                                        update(sf::Time);
    void                                        handleInput();
    void                                        display();

    // Getters
    const sf::FloatRect                         getViewBounds() const;

    std::vector<LevelBlock*>                    getBlockTypeInRange(const MovingEntity*
                                                                    , LevelBlock::Type, float) const;

    State<Dog>*                                 getDogState(Dog::States newStateType) const
                                                { return mDogStates.at(newStateType).get(); }

    State<Sheep>*                               getSheepState(Sheep::States newStateType) const
                                                { return mSheepStates.at(newStateType).get(); }

    const sf::IntRect                           getWorldBounds() const
                                                { return mWorldBounds; }

    LevelBlock*                                 getLevelBlock(sf::Vector2i index) const
                                                { return mLevel.getBlock(index); }

    int                                         getSheepNum() const
                                                { return mNumSheep; }

    int                                         getSheepHerded() const
                                                { return mSheepHerded; }

    sf::Time                                    getTimeLeft() const
                                                { return mTimeLeft; }

    sf::Time                                    getTimeTaken() const
                                                { return mTimeTaken; }


    std::vector<LevelBlock*>                    getLevelExit() const
                                                { return mLevel.getLevelExit(); }

    // Setters
    LevelBlock*                                 insertEntityIntoLevel(MovingEntity*) const;

    std::vector<MovingEntity*>                  getEntitiesInRange(const MovingEntity*
                                                                   , float) const;

    void                                        incSheepHerded()
                                                { mSheepHerded ++; }
};

#endif // WORLD_HPP
