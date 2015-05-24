#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP

#include <cmath>

#include <Box2D/Box2D.h>

//#include "SceneNode/SceneNode.hpp"
#include <SFML/Graphics/Transformable.hpp>

#include "App/Utility.hpp"

class PhysicsBody
{
protected:
   b2Body*                 mBody;
   b2BodyType              mBodyType;

   sf::FloatRect           mBodyBounds;
   float                    mRadius;

public:
                           PhysicsBody(b2Body* body,
                                       b2BodyType type,
                                       sf::Vector2f pos,
                                       sf::Vector2f size)
                           : mBody(body)
                           , mBodyType(type)
                           , mBodyBounds(pos.x - (size.x / 2.f),
                                     pos.y - (size.y / 2.f),
                                     size.x, size.y)
                           , mRadius(std::sqrt((size.x * size.x) + (size.y * size.y)))
                           {};

   // Getters
//   const b2Body*           getBody() const {return mBody;}
  const b2Fixture*         getFixtures() const {return mBody->GetFixtureList(); }
   sf::FloatRect           getBodyBounds() const {return mBodyBounds;}

   sf::Transform           getWorldTransform() const
                           {
                              b2Transform b2dTrans = mBody->GetTransform();
                              b2Vec2 pos = b2dTrans.p;

                              sf::Transform sfTrans;
                              sfTrans.rotate(b2dTrans.q.GetAngle()).translate(pos.x, pos.y);

                              return sfTrans;
                           }

   sf::Vector2f            getWorldPosition() const {return convertVec(mBody->GetPosition());}
   float                   getBodyRotation() const {return mBody->GetAngle(); }

  float                    getBodyRadius() {return mRadius;}

   // Setters
//   void                    moveBody(sf::Vector2f vel) {mBody->SetLinearVelocity(convertVec(vec));}
//   void                    rotateBody(float angle) {mBody->SetAngularVelocity(angle);}

//   void                    setTransform(sf::Vector2f pos, float angle) { mBody->SetTransform(convertVec(pos), angle); }

};

#endif // PHYSICSBODY_HPP
