#ifndef MOVINGENTITY_HPP
#define MOVINGENTITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "SceneNode.hpp"
#include "SteeringBehaviour.hpp"
#include "Utility.hpp"
#include "LevelBlock.hpp"
#include "EntityStats.hpp"
#include "MovingTarget.hpp"
#include "Params.hpp"

class World;

class MovingEntity : public SceneNode, public MovingTarget
{
public:
    enum EntityType
    {
        Dog,
        Sheep
    };

protected:
    World*                      mWorld;

    const float                 mMass;
    const float                 mWalkMaxSpeed;
    const float                 mRunMaxSpeed;
    const float                 mMaxForce;
    const float                 mMaxTurnRate;

    float                       mMaxSpeed;
    EntityType                  mType;

    LevelBlock*                 mCurrentBlock;

    sf::Sprite                  mSprite;
    sf::Vector2f                mVelocity;
    sf::Vector2f                mHeading;

    float                       mRadius;
    float                       mSpeed;

    SteeringBehaviour           mSteering;
    sf::Vector2f                mTargetPos;
    MovingTarget*               mMovingTarget;
    sf::Text                    mText;


    virtual void                updateCurrent(sf::Time);

    void                        adjustPosition();
    void                        ensureZeroOverlap();

public:
                                MovingEntity(World*
                                             , const sf::Texture&
                                             , const sf::Font&
                                             , sf::Vector2f
                                             , EntityStats
                                             , const Params&
                                             , EntityType
                                             , float = 1.f);

    virtual                     ~MovingEntity(){};

    // Virtual Functions
    virtual sf::Vector2f        targetPosition() const
                                { return getWorldPosition(); }

    virtual sf::Vector2f        targetVelocity() const
                                { return getVelocity(); }

    virtual sf::Vector2f        targetHeading() const
                                { return getHeading(); }

    virtual float               targetSpeed() const
                                { return getSpeed(); }

    virtual float               targetRadius() const
                                { return getRadius(); }

    // Getters
    std::vector<MovingEntity*>  getNeighbours(float = 50.f) const;

    std::vector<LevelBlock*>    getBlockTypeInRange(LevelBlock::Type, float) const;

    std::vector<LevelBlock*>    getLevelExit();

    sf::Vector2f                getVelocity() const
                                { return mVelocity; }

    sf::Vector2f                getHeading() const
                                { return mHeading; }

    float                       getSpeed() const
                                { return magVec(mVelocity); }

    float                       getMass() const
                                { return mMass; }

    float                       getMaxSpeed() const
                                { return mMaxSpeed; }

    float                       getMaxWalkSpeed()
                                { return mWalkMaxSpeed; }

    float                       getMaxRunSpeed()
                                { return mRunMaxSpeed; }

    float                       getMaxForce() const
                                { return mMaxForce; }

    float                       getMaxTurnRate() const
                                { return mMaxTurnRate; }

    bool                        checkSteeringBehaviour(SteeringBehaviour::Behaviour type) const
                                { return mSteering.checkBehaviour(type); }

    float                       getRadius() const
                                { return mRadius; }

    float                       getSpriteWidth() const
                                { return mSprite.getLocalBounds().width; }

    sf::Vector2f                getTargetPos() const
                                { return mTargetPos; }

    const MovingTarget*         getMovingTarget() const
                                { return mMovingTarget; }

    LevelBlock*                 getLevelBlock(sf::Vector2i);

    LevelBlock*                 getLevelBlock()
                                { return mCurrentBlock; }

    EntityType                  getEntityType()
                                { return mType; }

    // Setters
    void                        setVelocity(sf::Vector2f newVel)
                                { mVelocity = newVel; }

    void                        setMovingTarget(MovingTarget* mvTarget)
                                { mMovingTarget = mvTarget; }

    void                        setMaxSpeed(float speed)
                                { mMaxSpeed = speed; }

    void                        setSteeringTypes(std::vector<SteeringBehaviour::Behaviour> newBehaviours)
                                { mSteering.setNewBehaviours(newBehaviours); }

    void                        setSteeringTypes(SteeringBehaviour::Behaviour newBehaviour)
                                { mSteering.setNewBehaviours(newBehaviour); }

    void                        changeColour(sf::Color colour)
                                { mSprite.setColor(colour); }

    void                        setText(std::string msg)
                                {
                                    mText.setString(msg);
                                    mText.setColor(sf::Color(255, 255, 255, 255));
                                }

    void                        setTargetPos(sf::Vector2f pos)
                                { mTargetPos = pos; }
};

#endif // MOVINGENTITY_HPP
