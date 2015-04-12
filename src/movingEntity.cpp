#include "incl/MovingEntity.hpp"
#include "incl/World.hpp"
#include "incl/LevelBlock.hpp"


MovingEntity::MovingEntity(//World* world
                           Level* level
                           , const sf::Texture& texture
                           , const sf::Font& font
                           , sf::Vector2f startPos
                           , EntityStats stats
                           , const Params& params
                           , EntityType type
                           , float panicDist
                           , float scale)
//: mLevel(world)
: mLevel(level)
, mMass(stats.mass)
, mHealth(stats.health)
, mWalkMaxSpeed(stats.walkMaxSpeed)
, mRunMaxSpeed(stats.runMaxSpeed)
, mMaxForce(stats.maxForce)
, mMaxTurnRate(stats.maxTurnRate)
, mMaxSpeed(mWalkMaxSpeed)
, mPanicDistance(panicDist)
, mType(type)
, mCurrentBlock(nullptr)
, mSprite(texture)
, mVelocity(0.f, 0.f)
, mHeading(0.f, 0.f)
, mSteering(this, params)
, mMovingTarget(nullptr)
, mText("", font, 12)
{
    mSprite.scale(scale, scale);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mRadius = std::max(bounds.width, bounds.height);

    setPosition(startPos);

    float theta = randomClamped() * (2.f * SteeringBehaviour::mPI);
    rotate(theta * (180 / SteeringBehaviour::mPI));
    mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));
}

void MovingEntity::updateCurrent(sf::Time dt)
{
    sf::Color currentTextColor = mText.getColor();
    currentTextColor.a -= 1;

    mText.setColor(currentTextColor);

    sf::Vector2f steering = mSteering.calculate(dt);
    sf::Vector2f acceleration = steering / mMass;

    mVelocity += acceleration * dt.asSeconds();

    if(std::fabs(magVec(mVelocity)) > MINFLOAT)
    {
        int sign = signVec(mHeading, mVelocity);

        float angle = std::acos(dotVec(mHeading, normVec(mVelocity)));
        angle *= sign;

        clampRotation(angle
                      , -mMaxTurnRate
                      , mMaxTurnRate);

        if(angle > MINFLOAT || angle < -MINFLOAT)
            rotate(angle * (180.f / SteeringBehaviour::mPI));
    }

    float currentRotation = getRotation() * (SteeringBehaviour::mPI / 180.f);
    mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));

    truncateVec(mVelocity, mMaxSpeed);
    move(mVelocity);

//    adjustPosition();

    sf::FloatRect bounds = mText.getLocalBounds();
    mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sf::Vector2f textPos = getWorldPosition();
    textPos.y -= 20.f;
    mText.setPosition(textPos);
}

//void MovingEntity::adjustPosition()
//{
//    sf::IntRect worldBounds = mLevel->getWorldBounds();
//
//    sf::Vector2f pos = getWorldPosition();
//
//    pos.x = std::min(pos.x, static_cast<float>(worldBounds.width));
//    pos.x = std::max(pos.x, 0.f);
//    pos.y = std::min(pos.y, static_cast<float>(worldBounds.height));
//    pos.y = std::max(pos.y, 0.f);
//
//    setPosition(pos);
//}

void MovingEntity::ensureZeroOverlap()
{
    std::vector<MovingEntity*> neighbours = getNeighbours(25.f);
    sf::Vector2f pos = getWorldPosition();
    float radius = getRadius();

    for(MovingEntity* e : neighbours)
    {
        if(e != this)
        {
            sf::Vector2f awayFromEntity = pos - e->getWorldPosition();
            float expandedRadius = radius + e->getRadius();
            float mag = magVec(awayFromEntity);

            if(mag < expandedRadius)
            {
                move(normVec(awayFromEntity));
            }
        }
    }
}

std::vector<MovingEntity*> MovingEntity::getNeighbours(float radius) const
{
    return mLevel->getEntitiesInRange(const_cast<MovingEntity*>(this)
                                   , radius);
}

std::vector<LevelBlock*> MovingEntity::getBlockTypeInRange(LevelBlock::Type blockType, float radius) const
{
    return mLevel->getBlockTypeInRange(const_cast<MovingEntity*>(this), radius, blockType);
}

LevelBlock* MovingEntity::getLevelBlock(sf::Vector2i index)
{
    return mLevel->getBlock(index);
}

std::vector<LevelBlock*> MovingEntity::getLevelExit()
{
    return mLevel->getLevelExit();
}
