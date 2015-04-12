#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <memory>

#include "MovingEntity.hpp"
#include "StateMachine.hpp"

class Enemy : public MovingEntity
{
public:
    typedef std::vector<std::unique_ptr<State<Enemy>>> StateContainer;

    enum States
    {
        LookOut,
        Evade,
        Relax,
//        Exit,
        NumSheepStates
    };


    const float                 mSightRange;

    const float                 mAngleOfVision;
//    const float                 mPanicDistance;

private:
    sf::Vector2i                mTargetBlockIndex;

//    std::vector<State<Enemy>>&  mStates;
    StateContainer&             mStates;
    StateMachine<Enemy>         mStateMachine;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Enemy(Level*
                                      , const sf::Texture&
                                      , const sf::Font&
                                      , sf::Vector2f
                                      , EntityStats
                                      , const Params&
                                      , State<Enemy>*
                                      , State<Enemy>*
                                      , StateContainer&
                                      , float = 1.f);

    virtual                    ~Enemy(){};

    void                        changeState(Enemy::States newState)
                                { mStateMachine.changeState(mStates.at(newState).get()); }

    // Getters
    LevelBlock*                 getTargetBlock()
                                { return getLevelBlock(mTargetBlockIndex); }



    // Setters
    void                        setVelocity(sf::Vector2f vel)
                                { mVelocity = vel; }

    void                        setText(std::string msg)
                                {
                                    mText.setString(msg);
                                    mText.setColor(sf::Color(255, 255, 255, 255));
                                }

    void                        setTargetBlockIndex(sf::Vector2i index)
                                { mTargetBlockIndex = index; }

    void                        returnToPreviousState()
                                { mStateMachine.returnToPreviousState(); }
};

#endif // ENEMY_HPP
