#ifndef MOVINGENTITY_HPP
#define MOVINGENTITY_HPP

//#include "SceneNode.hpp"
#include "Entity/Entity.hpp"
#include "Entity/SteeringBehaviour.hpp"
//#include "Utility.hpp"
//#include "Level.hpp"
//#include "LevelBlock.hpp"
#include "Entity/EntityStats.hpp"
//#include "MovingTarget.hpp"
//#include "Params.hpp"

//class World;
class Level;
class Params;

class MovingEntity : public Entity
{
//public:
//    enum EntityType
//    {
//        Character,
//        Enemy,
//        All
//    };

protected:
//   Level*                      mLevel;

   const float                 mMass;
//   const float                 mMaxHealth;
   const float                 mWalkMaxSpeed;
   const float                 mRunMaxSpeed;
   const float                 mMaxForce;
   const float                 mMaxTurnRate;

//   const float                 mPanicDistance;
//   const float                 mAgroDistance;
//   const float                 mAttackDistance;

//   float                       mHealth;
   float                       mMaxSpeed;
//   EntityType                  mType;

//   LevelBlock*                 mCurrentBlock;
//
//   sf::Sprite                  mSprite;
   sf::Vector2f                mVelocity;
   sf::Vector2f                mHeading;

//   float                       mRadius;
   float                       mSpeed;

   SteeringBehaviour           mSteering;
//   sf::Vector2f                mTargetPos;
//   MovingEntity*               mCurrentTarget;
//   sf::Text                    mText;


   virtual void                updateCurrent(sf::Time);

   void                        adjustPosition();
   void                        ensureZeroOverlap();

public:
                              MovingEntity(EntityType
                                           , Level*
                                           , const sf::Texture&
                                           , const sf::Font&
                                           , sf::Vector2f
                                           , EntityStats
                                           , const Params&
                                           , float = 1.f);

   virtual                     ~MovingEntity() {};

   // Virtual Functions
//   virtual sf::Vector2f       targetPosition() const{return getWorldPosition();}
//
//   virtual sf::Vector2f       targetVelocity() const {return getVelocity();}
//
//   virtual sf::Vector2f       targetHeading() const {return getHeading();}
//
//   virtual float              targetSpeed() const {return getSpeed(); }
//
//   virtual float              targetRadius() const {return getRadius();}

   // Getters
//   std::vector<MovingEntity*> getNeighbours(float
//                                       , int) const;
   //    std::vector<MovingEntity*>  getNeighbourByType(MovingEntity::EntityType) const;
//
//   std::vector<LevelBlock*>   getBlockTypeInRange(LevelBlock::Type, float) const;
//
//   std::vector<LevelBlock*>   getLevelExit();

//   float                      getPanicDistance() {return mPanicDistance; }

//   float                      getAgroDistance() {return mAgroDistance;}

//   float                      getAttackDistance() {return mAttackDistance;}

   sf::Vector2f               getVelocity() const {return mVelocity;}

   sf::Vector2f               getHeading() const {return mHeading;}

   float                      getSpeed() const {return magVec(mVelocity); }

   float                      getMass() const {return mMass;}

//   float                      getHealthPercentage() const {return (mHealth / mMaxHealth) * 100.f;}

   float                      getMaxSpeed() const {return mMaxSpeed;}

   float                      getMaxWalkSpeed() {return mWalkMaxSpeed;}

   float                      getMaxRunSpeed() {return mRunMaxSpeed; }

   float                      getMaxForce() const {return mMaxForce;}

   float                      getMaxTurnRate() const {return mMaxTurnRate;}

   bool                       checkSteeringBehaviour(SteeringBehaviour::Behaviour type) const {return mSteering.checkBehaviour(type);}

//   float                      getRadius() const {return mRadius;}
//
//   float                      getSpriteWidth() const{return mSprite.getLocalBounds().width;}

//   sf::Vector2f               getTargetPos() const {return mCurrentTarget->getWorldPosition();}

//   MovingEntity*              getCurrentTarget() const {return mCurrentTarget;}
//
//   LevelBlock*                getLevelBlock(sf::Vector2i);
//
//   LevelBlock*                getLevelBlock() {return mCurrentBlock;}
//
//   EntityType                 getEntityType() {return mType;}

   // Setters
   void                       setVelocity(sf::Vector2f newVel){mVelocity = newVel; }

   void                       setCurrentTarget(MovingEntity* target){mCurrentTarget = target; }

   void                       setMaxSpeed(float speed){ mMaxSpeed = speed;}

   void                       setSteeringTypes(std::vector<SteeringBehaviour::Behaviour> newBehaviours) {mSteering.setNewBehaviours(newBehaviours); }

   void                       setSteeringTypes(SteeringBehaviour::Behaviour newBehaviour) {mSteering.setNewBehaviours(newBehaviour);}

//   void                       changeColour(sf::Color colour){mSprite.setColor(colour);}
//
//   void                       setText(std::string msg)
//                              {
//                                 mText.setString(msg);
//                                 mText.setColor(sf::Color(255, 255, 255, 255));
//                              }

//   void                       changeHealth(float hpChange) {mHealth += hpChange;}

};

#endif // MOVINGENTITY_HPP
