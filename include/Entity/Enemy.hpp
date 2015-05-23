#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <memory>

#include "Entity/Entity.hpp"
#include "Entity/State/StateMachine.hpp"

class Enemy : public Entity
{
public:
   typedef std::vector<std::unique_ptr<State<Enemy>>> StateContainer;

   enum States
   {
      LookOut,
      Evade,
      Relax,
      Attack,
      //        Exit,
      NumStates
   };

   const float                 mSightRange;
   const float                 mAngleOfVision;
//    const float                 mPanicDistance;

private:
//    sf::Vector2i                mTargetBlockIndex;

//    std::vector<State<Enemy>>&  mStates;
    StateContainer&             mStates;
    StateMachine<Enemy>         mStateMachine;

//    Entity*                      mCurrentTarget;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Enemy(QuadTree* quadTree
                                      , const sf::Texture&
                                      , const sf::Font&
                                      , sf::Vector2f
                                      , EntityStats
                                      , const Params&
                                      , State<Enemy>*
                                      , State<Enemy>*
                                      , StateContainer&
                                      , unsigned int currentState
                                      , b2Body* body
                                      , float = 1.f);

    virtual                    ~Enemy(){};

    void                        changeState(Enemy::States newState)
                                { mStateMachine.changeState(mStates.at(newState).get(), newState); }

    // Getters
//    LevelBlock*                 getTargetBlock()
//                                { return getLevelBlock(mTargetBlockIndex); }

//   Entity*                getCurrentTarget() { return mCurrentTarget; }

    // Setters
    void                        setVelocity(sf::Vector2f vel)
                                { mVelocity = vel; }

    void                        setText(std::string msg)
                                {
                                    mText.setString(msg);
                                    mText.setColor(sf::Color(255, 255, 255, 255));
                                }

      unsigned int                     getCurrentStateType() {return mStateMachine.getCurrentStateType(); }
//
//    void                        setTargetBlockIndex(sf::Vector2i index)
//                                { mTargetBlockIndex = index; }

//    void                        returnToPreviousState()
//                                { mStateMachine.returnToPreviousState(); }
};

#endif // ENEMY_HPP
