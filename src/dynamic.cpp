#include "App/Params.hpp"
#include "World/Level.hpp"
#include "World/LevelBlock.hpp"
#include "Entity/Entity.hpp"

Dynamic::Dynamic(const sf::Texture& texture
                , sf::Vector2f startPos
                , float scale
                , EntityStats stats
                , const Params& params)
: SpriteNode(texture
            , startPos
            , scale)
, mMass(stats.mass)
, mWalkMaxSpeed(stats.walkMaxSpeed)
, mRunMaxSpeed(stats.runMaxSpeed)
, mMaxForce(stats.maxForce)
, mMaxTurnRate(stats.maxTurnRate)
, mMaxSpeed(mWalkMaxSpeed)
, mVelocity(0.f, 0.f)
, mHeading(0.f, 0.f)
//, mSteering(this, params)
, mSpeed(0.f)
{
    float theta = randomClamped() * (2.f * SteeringBehaviour::mPI);
    rotate(theta * (180 / SteeringBehaviour::mPI));
    mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));

   sf::FloatRect bounds = mSprite.getLocalBounds();
    mRadius = std::max(bounds.width, bounds.height);
}

void Dynamic::move(sf::Vector2f newDir)
{
//   sf::Vector2f steering = mSteering.calculate(dt);
   sf::Vector2f acceleration = newDir / mMass;

   mVelocity += acceleration;// * dt.asSeconds();

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

//   mCurrentBlock->deleteEntity(this);
//   mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}
