#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "World/Level.hpp"
#include "Node/SceneNode.hpp"
//#include "Entity/Goblin.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/Adventurer.hpp"
#include "Entity/State/State.hpp"
#include "Entity/EntityStats.hpp"
#include "App/HUD.hpp"

class Controller;
class LevelBlock;
class Wall;
class Waypoint;
//class MovingEntity;
class GameState;
//class Dog;
class SpriteNode;

class World : private sf::NonCopyable
{
public:
    const float                                 mWorldX;
    const float                                 mWorldY;
    const float                                 mExitRadius;

    const int                                   mLevelBlockSize;
    const int                                   mNumEnemy;

    enum StatsType
    {
        EnemyStats,
        AdventurerStats,
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

   //    int                                         mEnemyHerded;
   sf::Time                                    mTimeLeft;
   sf::Time                                    mTimeTaken;

   std::unique_ptr<Level>                      mLevel;
   HUD                                         mHUD;

   std::vector<EntityStats>                    mEntityStats;

   //    std::vector<std::unique_ptr<State<Dog>>>    mDogStates;
   //    std::vector<std::unique_ptr<State<Enemy>>>  mEnemyStates;
   Adventurer::StateContainer                    mAdventurerStates;
   Enemy::StateContainer                       mEnemyStates;

   SpriteNode*                                  mBackground;
   sf::Vector2f                                 mExitPos;
   std::vector<Adventurer*>                      mAdventurers;
   Adventurer*                                   mCurrentAdventurer;
   unsigned int                                mCurrentAdventurerIndex;


   void                                        initialiseStatesAndStats();
   void                                        buildScene(const Controller&);
   void                                        generateAgents(const Controller&);
   void                                        handleRealTimeInput();
   void                                        adjustView();
      void                                        changeAdventurer();

public:
                                             World(GameState&
                                                    , const Controller&
                                                    , sf::RenderWindow&
                                                    , std::string
                                                    , int worldDim
                                                    , int numEnemy
                                                    , sf::Time levelTime);

    void                                        update(sf::Time);
    void                                        handleInput();

    void                                        display();

    // Getters
    const sf::FloatRect                         getViewBounds() const;

//    std::vector<LevelBlock*>                    getBlockTypeInRange(const MovingEntity*, LevelBlock::Type, float) const;

//    State<Dog>*                                 getDogState(Dog::States newStateType) const
//                                                { return mDogStates.at(newStateType).get(); }

//    State<Enemy>*                               getEnemyState(Enemy::States newStateType) {return mEnemyStates.at(newStateType).get();}

    const sf::IntRect                           getWorldBounds() const {return mWorldBounds;}

    LevelBlock*                                 getLevelBlock(sf::Vector2i index) const {return mLevel->getBlock(index);}

    int                                         getEnemyNum() const {return mNumEnemy;}

//    int                                         getEnemyHerded() const
//                                                { return mEnemyHerded; }

    sf::Time                                    getTimeLeft() const {return mTimeLeft;}

    sf::Time                                    getTimeTaken() const {return mTimeTaken;}


    std::vector<LevelBlock*>                    getLevelExit() const {return mLevel->getLevelExit();}

    // Setters
//    LevelBlock*                                 insertEntityIntoLevel(MovingEntity*) const;

//    std::vector<MovingEntity*>                  getEntitiesInRange(const MovingEntity*
//            , float) const;

//    void                                        incEnemyHerded()
//                                                { mEnemyHerded ++; }
};

#endif // WORLD_HPP
