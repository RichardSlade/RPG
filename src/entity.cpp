#include <string>

#include "Entity/Entity.hpp"
#include "World/World.hpp"
//#include "World/LevelBlock.hpp"
#include "World/QuadTree.hpp"
//#include "World/Scenery.hpp"

Entity::Entity(QuadTree* quadTree
               , const sf::Texture& texture
               , const sf::Font& font
               , sf::Vector2f startPos
               , EntityStats stats
               , const Params& params
               , Type type
               , b2Body* body
               , float scale)
: PhysicsBody(body,
              b2BodyType::b2_dynamicBody,
              startPos,
              sf::Vector2f(1.f, 1.f))
, Killable(stats.health)
, Intelligent(stats)
, MeleeFighter(stats)
//, mLevel(level)
, mQuadTree(quadTree)
//, mPhysicsBody(body)
, mMass(stats.mass)
, mHealth(stats.health)
, mWalkMaxSpeed(stats.walkMaxSpeed)
, mRunMaxSpeed(stats.runMaxSpeed)
, mMaxForce(stats.maxForce)
, mMaxTurnRate(stats.maxTurnRate)
, mMaxSpeed(mWalkMaxSpeed)
, mEntityType(type)
, mCurrentBlock(nullptr)
, mSprite(texture)
, mVelocity(0.f, 0.f)
, mHeading(0.f, 0.f)
, mSteering(this, params)
, mCurrentTarget(nullptr)
, mText(".....", font, 12)
, mHPText("100%", font, 12)
, mWanderTarget(5.f)
, mOrigin(5.f)
{
//  mWanderTarget.setPosition(sf::Vector2f(500.f, 500.f));

   mSprite.scale(scale, scale);
   sf::FloatRect bounds = mSprite.getLocalBounds();
   mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//   mRadius = std::max(bounds.width, bounds.height);

   sf::Transformable::setPosition(meterToPixel(startPos));

  mWanderTarget.setFillColor(sf::Color::Magenta);
  bounds = mWanderTarget.getLocalBounds();
  mWanderTarget.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

  mOrigin.setFillColor(sf::Color::Green);
  bounds = mOrigin.getLocalBounds();
  mOrigin.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  mOrigin.setPosition(0.f, 0.f);
//   mBodyBounds = mSprite.getLocalBounds();

//   float theta = randomClamped() * (2.f * PI);
//   rotate(theta * (180 / PI));
//   mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));

   bounds = mText.getLocalBounds();
   mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
   mText.setPosition(0, -20.f);

   bounds = mHPText.getLocalBounds();
   mHPText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
   mHPText.setPosition(0, 20.f);
   mHPText.setColor(sf::Color(0, 255, 0));

//   mCurrentBlock = mLevel->insertEntityIntoLevel(this);
  mQuadTree->insert(this);
}

void Entity::updateCurrent(sf::Time dt)
{
   // Check if entity is dead and needs to be removed
   if(isDead())
      mToRemove = true;

   sf::Transformable::setPosition(meterToPixel(getWorldPosition()));

   float currentRotation = mBody->GetAngle();
   mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));

   sf::Transformable::setRotation(radianToDegree(currentRotation));

   sf::Color currentTextColor = mText.getColor();
   currentTextColor.a -= 1;
   mText.setColor(currentTextColor);

   MeleeFighter::update(dt);

   setHPText(std::string(std::to_string(static_cast<int>(getHealthPercentage())) +  + "%"));

   if(getHealthPercentage() < 50.f)
      mHPText.setColor(sf::Color::Yellow);
   else if(getHealthPercentage() < 25.f)
      mHPText.setColor(sf::Color::Red);
   else
      mHPText.setColor(sf::Color::Green);

//   mCurrentBlock->deleteEntity(this);
//   mCurrentBlock = mLevel->insertEntityIntoLevel(this);

//  mQuadTree->insert(this);
}

void Entity::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
  target.draw(mSprite, states);
  target.draw(mText, states);
  target.draw(mHPText, states);
//  target.draw(mWanderTarget, states);
//  target.draw(mOrigin, states);
  target.draw(mWanderTarget);
}

void Entity::updatePhysicsBody(sf::Time dt)
{
   // Velocity
   sf::Vector2f steering = mSteering.calculate(dt);
   sf::Vector2f acceleration = steering / mMass;

//   mVelocity *= 0.99f;
//   mVelocity += (acceleration * 2.f) * dt.asSeconds();
//   mVelocity = acceleration * dt.asSeconds() * 10.f;
   mVelocity += acceleration * dt.asSeconds();
//   mVelocity = acceleration * dt.asSeconds() * 20.f;

   truncateVec(mVelocity, mMaxSpeed);
   mBody->SetLinearVelocity(convertVec(mVelocity));

  // Rotation
//  if(std::fabs(magVec(mVelocity)) > MINFLOAT)
//  {
    int sign = signVec(mHeading, mVelocity);

  //     float angle = std::acos(dotVec(mHeading, normVec(mVelocity)));
    float angle = dotVec(mHeading, normVec(mVelocity));
    angle *= sign;

  //     clampRotation(angle
  //                   , -mMaxTurnRate
  //                   , mMaxTurnRate);

  //       sf::Vector2f toCursor = mVelocity;
  //      angle = std::atan2(toCursor.x, -toCursor.y);

  //     if(angle > MINFLOAT || angle < -MINFLOAT)
  //         rotate(angle * (180.f / PI));

//    mBody->SetAngularVelocity(0.f);

    if(angle > MINFLOAT || angle < -MINFLOAT)
      mBody->SetAngularVelocity(sign);
//      mBody->ApplyAngularImpulse(angle, true);
//  }

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
    std::list<Entity*> neighbours;
    getNeighbours(neighbours,
                  Entity::Type::AllTypes);

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

std::list<Entity*>& Entity::getNeighbours(std::list<Entity*>& returnList,
                                          Entity::Type type) const
{
//    return mLevel->getEntitiesInRange(const_cast<Entity*>(this)
//                                    , radius
//                                    , type);

  return mQuadTree->retrieveEntities(returnList,
                                     this,
                                     type);
}

std::list<Scenery*>& Entity::getObstacles(std::list<Scenery*>& returnList,
                                          Scenery::Type type) const
{
  return mQuadTree->retrieveScenery(returnList,
                                     this,
                                     type);
}

void Entity::addToQuadTree(QuadTree* quadTree)
{
  // Add children
  SceneNode::addToQuadTree(quadTree);

//  std::cout << "adding entity to quad tree" << std::endl;

  quadTree->insert(this);
}

//std::vector<LevelBlock*> Entity::getBlockTypeInRange(LevelBlock::Type blockType, float radius) const
//{
////    return mLevel->getBlockTypeInRange(const_cast<Entity*>(this), radius, blockType);
//}

//LevelBlock* Entity::getLevelBlock(sf::Vector2i index)
//{
////    return mLevel->getBlock(index);
//}

//std::vector<LevelBlock*> Entity::getLevelExit()
//{
//    return mLevel->getLevelExit();
//}

//sf::Transform Entity::getWorldTransform() const
//{
//   b2Transform b2dTrans = mPhysicsBody->GetTransform();
//   b2Vec2 pos = b2dTrans.p;
//
//   sf::Transform sfTrans;
//   sfTrans.rotate(b2dTrans.q.GetAngle()).translate(pos.x, pos.y);
//
//   return sfTrans;
//}
//
//sf::Vector2f Entity::getWorldPosition() const
//{
//    return convertVec(mPhysicsBody->GetPosition());
//}
