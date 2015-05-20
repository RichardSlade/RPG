#ifndef PHYSICSBODY_HPP
#define PHYSICSBODY_HPP

#include <Box2D/Box2D.h>

//#include "SceneNode/SceneNode.hpp"
#include <SFML/Graphics/Transformable.hpp>

class PhysicsBody
{
protected:
   b2Body*                 mBody;
   b2BodyType              mBodyType;

   sf::FloatRect           mBodyBounds;

public:
                           PhysicsBody(b2Body* body,
                                       b2BodyType type)
                           : mBody(body)
                           , mBodyType(type)
                           {};

   // Getters
//   const b2Body*           getBody() const {return mBody;}
   sf::FloatRect           getBodyBounds() {return mBodyBounds;}

   sf::Transform           getWorldTransform() const
                           {
                              b2Transform b2dTrans = mBody->GetTransform();
                              b2Vec2 pos = b2dTrans.p;

                              sf::Transform sfTrans;
                              sfTrans.rotate(b2dTrans.q.GetAngle()).translate(pos.x, pos.y);

                              return sfTrans;
                           }

   sf::Vector2f            getWorldPosition() const {return convertVec(mBody->GetPosition());}
   float                   getAngle() const {return mBody->GetAngle(); }

   // Setters
//   void                    moveBody(sf::Vector2f vel) {mBody->SetLinearVelocity(convertVec(vec));}
//   void                    rotateBody(float angle) {mBody->SetAngularVelocity(angle);}

//   void                    setTransform(sf::Vector2f pos, float angle) { mBody->SetTransform(convertVec(pos), angle); }

};

#endif // PHYSICSBODY_HPP
