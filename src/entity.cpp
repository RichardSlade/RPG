#include <string>

#include "Entity/Entity.hpp"
#include "World/World.hpp"
#include "World/LevelBlock.hpp"

Entity::Entity(Level* level
                           , const sf::Texture& texture
                           , const sf::Font& font
                           , sf::Vector2f startPos
                           , EntityStats stats
                           , const Params& params
                           , Type type
                           , float panicDist
                           , float scale)
//: mLevel(world)
: Killable(stats.health)
, Intelligent(stats)
, MeleeFighter(stats)
, mLevel(level)
, mMass(stats.mass)
, mHealth(stats.health)
, mWalkMaxSpeed(stats.walkMaxSpeed)
, mRunMaxSpeed(stats.runMaxSpeed)
, mMaxForce(stats.maxForce)
, mMaxTurnRate(stats.maxTurnRate)
, mMaxSpeed(mWalkMaxSpeed)
//, mPanicDistance(PanicDista)
, mEntityType(type)
, mCurrentBlock(nullptr)
, mSprite(texture)
, mVelocity(0.f, 0.f)
, mHeading(0.f, 0.f)
, mSteering(this, params)
, mCurrentTarget(nullptr)
, mText(".....", font, 12)
, mHPText("100%", font, 12)
{
   mSprite.scale(scale, scale);
   sf::FloatRect bounds = mSprite.getLocalBounds();
   mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
   mRadius = std::max(bounds.width, bounds.height);

   setPosition(startPos);

   float theta = randomClamped() * (2.f * SteeringBehaviour::mPI);
   rotate(theta * (180 / SteeringBehaviour::mPI));
   mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));

   bounds = mText.getLocalBounds();
   mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
   mText.setPosition(0, -20.f);

   bounds = mHPText.getLocalBounds();
   mHPText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
   mHPText.setPosition(0, 20.f);
   mHPText.setColor(sf::Color(0, 255, 0));

   mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

void Entity::updateCurrent(sf::Time dt)
{
   // Check if entity is dead and needs to be removed
   if(isDead())
      mToRemove = true;

   sf::Color currentTextColor = mText.getColor();
   currentTextColor.a -= 1;

   mText.setColor(currentTextColor);

   //    adjustPosition();

//   sf::FloatRect bounds = mText.getLocalBounds();
//   mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

//   sf::Vector2f textPos = getWorldPosition();
//   textPos.y -= 20.f;
//   mText.setPosition(textPos);

   MeleeFighter::update(dt);

   setHPText(std::string(std::to_string(static_cast<int>(getHealthPercentage())) +  + "%"));

   if(getHealthPercentage() < 50.f)
      mHPText.setColor(sf::Color::Yellow);
   else if(getHealthPercentage() < 25.f)
      mHPText.setColor(sf::Color::Red);
   else
      mHPText.setColor(sf::Color::Green);

   mCurrentBlock->deleteEntity(this);
   mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

void Entity::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
   target.draw(mSprite, states);
   target.draw(mText, states);
   target.draw(mHPText, states);
}

void Entity::updateMovement(sf::Time dt)
{
   sf::Vector2f steering = mSteering.calculate(dt);
   sf::Vector2f acceleration = steering / mMass;

   mVelocity *= 0.99f;
   mVelocity += (acceleration * 2.f) * dt.asSeconds();
//   mVelocity = acceleration * dt.asSeconds();

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

//   mVelocity /= 0.1f;

   truncateVec(mVelocity, mMaxSpeed);
   move(mVelocity);

   Entity::updateCurrent(dt);
}

//void Entity::adjustPosition()
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

void Entity::ensureZeroOverlap()
{
    std::vector<Entity*> neighbours = getNeighbours(25.f, Entity::Type::AllTypes);
    sf::Vector2f pos = getWorldPosition();
    float radius = getRadius();

    for(Entity* e : neighbours)
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

std::vector<Entity*> Entity::getNeighbours(float radius
                                          , unsigned int type) const
{
    return mLevel->getEntitiesInRange(const_cast<Entity*>(this)
                                    , radius
                                    , type);
}

std::vector<LevelBlock*> Entity::getBlockTypeInRange(LevelBlock::Type blockType, float radius) const
{
    return mLevel->getBlockTypeInRange(const_cast<Entity*>(this), radius, blockType);
}

LevelBlock* Entity::getLevelBlock(sf::Vector2i index)
{
    return mLevel->getBlock(index);
}

std::vector<LevelBlock*> Entity::getLevelExit()
{
    return mLevel->getLevelExit();
}
