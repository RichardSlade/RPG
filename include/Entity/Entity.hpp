#ifndef MOVINGENTITY_HPP
#define MOVINGENTITY_HPP

#include <list>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Box2D/Box2D.h>

#include "App/Params.hpp"
#include "App/Utility.hpp"
//#include "World/Level.hpp"
#include "World/Scenery.hpp"
#include "SceneNode/SceneNode.hpp"
#include "Physics/PhysicsBody.hpp"
#include "Entity/SteeringBehaviour.hpp"
//#include "LevelBlock.hpp"
#include "Entity/EntityStats.hpp"
#include "Entity/Attribute/Killable.hpp"
#include "Entity/Attribute/Intelligent.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"
//#include "MovingTarget.hpp"

class QuadTree;
//class Scenery;

class Entity : public SceneNode, public PhysicsBody, public Killable, public Intelligent, public MeleeFighter
{
public:
    enum Type
    {
        Adventurer,
        Enemy,
        AllTypes
    };

//    const float                 mPanicDistance;

protected:
//    Level*                      mLevel;
    QuadTree*                   mQuadTree;


//    b2Body*                     mPhysicsBody;

  const float                 mMass;
  const float                 mHealth;
  const float                 mWalkMaxSpeed;
  const float                 mRunMaxSpeed;
  const float                 mMaxForce;
  const float                 mMaxTurnRate;

  float                       mMaxSpeed;
  Type                        mEntityType;

  LevelBlock*                 mCurrentBlock;

  sf::Sprite                  mSprite;
  sf::Vector2f                mVelocity;
  sf::Vector2f                mHeading;

  float                       mRadius;
  float                       mSpeed;

  SteeringBehaviour           mSteering;
  //    sf::Vector2f                mTargetPos;
  Entity*                     mCurrentTarget;
  sf::Text                    mText;
  sf::Text                    mHPText;
  sf::CircleShape               mWanderTarget;
  sf::CircleShape               mOrigin;

  virtual void                updateCurrent(sf::Time);
  virtual void                drawCurrent(sf::RenderTarget& target
                                         , sf::RenderStates states) const;

  void                         updatePhysicsBody(sf::Time);

  void                        adjustPosition();
  void                        ensureZeroOverlap();

public:
                                Entity(QuadTree*
//                                      Level*
                                       , const sf::Texture&
                                       , const sf::Font&
                                       , sf::Vector2f
                                       , EntityStats
                                       , const Params&
                                       , Type type
                                       , b2Body* body
                                       , float = 1.f);

    virtual                     ~Entity(){};

    virtual void                addToQuadTree(QuadTree* quadTree);

    // Getters
    std::list<Entity*>&          getNeighbours(std::list<Entity*>& returnList,
                                              Entity::Type type) const;

    std::list<Scenery*>&         getObstacles(std::list<Scenery*>& returnList,
                                              Scenery::Type type) const;

//    std::vector<LevelBlock*>    getBlockTypeInRange(LevelBlock::Type, float) const;

//    std::vector<LevelBlock*>    getLevelExit();

   sf::Transform			           getWorldTransform() const {return PhysicsBody::getWorldTransform();};
   sf::Vector2f			              getWorldPosition() const {return PhysicsBody::getWorldPosition();};

   sf::Vector2f                  getVelocity() const { return mVelocity; }

   sf::Vector2f                  getHeading() const { return mHeading; }

   float                         getSpeed() const { return magVec(mVelocity); }

   float                         getMass() const { return mMass; }

   float                         getMaxSpeed() const { return mMaxSpeed; }

   float                         getMaxWalkSpeed() { return mWalkMaxSpeed; }

   float                         getMaxRunSpeed() { return mRunMaxSpeed; }

   float                         getMaxForce() const { return mMaxForce; }

   float                         getMaxTurnRate() const { return mMaxTurnRate; }

   bool                          checkSteeringBehaviour(SteeringBehaviour::Behaviour type) const { return mSteering.checkBehaviour(type); }

   float                         getRadius() const { return mRadius; }

   float                         getSpriteWidth() const { return mSprite.getLocalBounds().width; }

   //    sf::Vector2f                getTargetPos() const
   //                                { return mTargetPos; }

   const Entity*                 getCurrentTarget() const { return mCurrentTarget; }

//   LevelBlock*                   getLevelBlock(sf::Vector2i);

//   LevelBlock*                   getLevelBlock() { return mCurrentBlock; }

   Entity::Type                  getEntityType(){ return mEntityType; }

   // Setters
   void                          setVelocity(sf::Vector2f newVel){ mVelocity = newVel; }

   void                          resetVelocity()
                              { mVelocity = sf::Vector2f(); }

   void                       changeVelocity(sf::Vector2f newVec)
                              { mVelocity += newVec; }

   void                       setCurrentTarget(Entity* target)
                              { mCurrentTarget = target; }

   void                       setMaxSpeed(float speed)
                              { mMaxSpeed = speed; }

   void                       setSteeringTypes(std::vector<SteeringBehaviour::Behaviour> newBehaviours)
                              { mSteering.setNewBehaviours(newBehaviours); }

//   void                       setSteeringTypes(SteeringBehaviour::Behaviour newBehaviour)
//                              { mSteering.setNewBehaviours(newBehaviour); }

   void                       changeColour(sf::Color colour)
                              { mSprite.setColor(colour); }

   void                       setText(std::string msg)
                              {
                                 mText.setString(msg);
                                 mText.setColor(sf::Color(255, 255, 255, 255));
                              }

   void                       setHPText(std::string msg)
                              {
                                 mHPText.setString(msg);
                                 mHPText.setColor(sf::Color(255, 255, 255, 255));
                              }

  void                          setWanderTargetPosition(sf::Vector2f pos) {mWanderTarget.setPosition(pos);}


   //    void                        setTargetPos(sf::Vector2f pos)
   //                                { mTargetPos = pos; }
};

#endif // MOVINGENTITY_HPP
