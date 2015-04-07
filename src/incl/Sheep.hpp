#ifndef SHEEP_HPP
#define SHEEP_HPP

#include <memory>

#include "MovingEntity.hpp"
#include "StateMachine.hpp"

class Sheep : public MovingEntity
{
public:
    enum States
    {
        LookOut,
        Evade,
        Relax,
        Exit,
        NumSheepStates
    };


    const float                 mSightRange;

    const float                 mAngleOfVision;
    const float                 mPanicDistance;

private:
    sf::Vector2i                mTargetBlockIndex;
    StateMachine<Sheep>         mStateMachine;

    virtual void                updateCurrent(sf::Time);
    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                Sheep(World*
                                      , const sf::Texture&
                                      , const sf::Font&
                                      , sf::Vector2f
                                      , State<Sheep>*
                                      , State<Sheep>*
                                      , EntityStats
                                      , const Params&
                                      , float = 1.f);

    virtual                    ~Sheep(){};

    // Getters
    LevelBlock*                 getTargetBlock()
                                { return getLevelBlock(mTargetBlockIndex); }



    // Setters
    void                        changeState(Sheep::States newState);

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

#endif // SHEEP_HPP
