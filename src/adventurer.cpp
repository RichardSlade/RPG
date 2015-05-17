/*
*   Adventurer.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <cstdlib>

#include "Entity/Adventurer.hpp"
#include "World/World.hpp"

Adventurer::Adventurer(const sf::RenderWindow& window
                     , Level* level
                     , const sf::Texture& texture
                     , const sf::Font& font
                     , sf::Vector2f startPos
                     , EntityStats stats
                     , const Params& params
                     , State<Adventurer>* globalState
                     , State<Adventurer>* initState
                     , StateContainer& states
                     , unsigned int currentState
                     , b2Body* body
                     , float scale)
: Entity(level
         , texture
         , font
         , startPos
         , stats
         , params
         , Entity::Type::Adventurer
         , body
         , scale)
, mWindow(window)
, mStates(states)
, mStateMachine(this, globalState, initState, currentState)
, mPhysicsBody(body)
, mIsSelected(false)
, mMove(false)
{
    setSteeringTypes(SteeringBehaviour::Behaviour::FollowPath);
    mText.setString("....");
}

/*
*   Main update for sheep dog
*/
void Adventurer::updateCurrent(sf::Time dt)
{
   if(!mIsSelected)
   {
      mStateMachine.update();
      Entity::updatePhysicsBody(dt);
   }
   else
   {

      sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
      sf::Vector2f convertedMousePos = pixelToMeter(mWindow.mapPixelToCoords(mousePos));

      sf::Vector2f toCursor = convertedMousePos - getWorldPosition();

      float angle = std::atan2(toCursor.x, -toCursor.y);

      sf::Transformable::setRotation(radianToDegree(angle));
      float currentRotation = mPhysicsBody->GetAngle();
      mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));

      sf::Transformable::move(mVelocity);

      sf::Vector2f p = pixelToMeter(sf::Transformable::getPosition());
      b2Vec2 pos(p.x, p.y);

      mPhysicsBody->SetTransform(pos, currentRotation);

//      mPhysicsBody->SetRota

//      mPhysicsBody->SetLinearVelocity(b2Vec2(mVelocity.x, mVelocity.y));
//
//      setPosition(meterToPixel(mPhysicsBody->GetPosition()));

//      move(mVelocity);

//      std::vector<LevelBlock*> wallsInRange = getBlockTypeInRange(LevelBlock::Type::WallBlock
//                                                                  , mRadius);
//      float expandedRadius = mRadius;

//      for(unsigned int wall = 0; wall < wallsInRange.size(); wall++)
//      {
//         sf::Vector2f toWall = getWorldPosition() - wallsInRange.at(wall)->getCenter();
//         float mag = magVec(toWall);
//
//         if(mag < expandedRadius)
//         {
//            move(-mVelocity);
//            break;
//         }
//      }

//      mVelocity = sf::Vector2f();
   }

   Entity::updateCurrent(dt);

//   ensureZeroOverlap();
}

/*
*   Draw function used by SFML to render to sf::RenderTarget
*/
void Adventurer::drawCurrent(sf::RenderTarget& target
                                    , sf::RenderStates states) const
{
   Entity::drawCurrent(target, states);

   std::vector<sf::CircleShape> wypnts = mSteering.getPathToDraw();

   for(sf::CircleShape circle : wypnts)
     target.draw(circle);
}

//void Adventurer::rotateToCursor()
//{
//   sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
//   sf::Vector2f convertedMousePos = pixelToMeter(mWindow.mapPixelToCoords(mousePos));
//
//   sf::Vector2f toCursor = convertedMousePos - getWorldPosition();
//
//   float angle = std::atan2(toCursor.x, -toCursor.y);
//
//   mPhysicsBody->SetTransform(mPhysicsBody->GetPosition(), angle);
//}
