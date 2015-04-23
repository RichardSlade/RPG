#ifndef DYNAMIC_HPP
#define DYNAMIC_HPP

#include "App/Params.hpp"
#include "Node/SpriteNode.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/EntityStats.hpp"

class Dynamic : public SpriteNode
{
protected:
   const float                 mMass;
   const float                 mWalkMaxSpeed;
   const float                 mRunMaxSpeed;
   const float                 mMaxForce;
   const float                 mMaxTurnRate;

   float                       mMaxSpeed;
   sf::Vector2f                  mVelocity;
   sf::Vector2f                  mHeading;

//   float                       mRadius;
   float                            mSpeed;
      float                      mRadius;


//   SteeringBehaviour             mSteering;
//   sf::Vector2f                mTargetPos;
//   DynamicEntity*               mCurrentTarget;
//   sf::Text                    mText;


   virtual void                updateCurrent(sf::Time dt){};

//   void                       update(sf::Time dt);

//   virtual LevelBlock*        insertIntoLevel();// {mCurrentBlock = mLevel->insertEntityIntoLevel(this);}

   void                       ensureZeroOverlap();


public:
                              Dynamic(const sf::Texture& texture
                                           , sf::Vector2f startPos
                                           , float scale
                                           , EntityStats stats
                                           , const Params& params);

   virtual                     ~Dynamic() {};

   void                       move(sf::Vector2f newDir);

   // Getters
//   virtual std::vector<Dynamic*>    getNeighbours(float radius, int type) const;

//   std::vector<LevelBlock*>      getBlockTypeInRange(LevelBlock::Type, float) const;

   sf::Vector2f                           getVelocity() const {return mVelocity;}

   sf::Vector2f                           getHeading() const {return mHeading;}

   float                                  getSpeed() const {return magVec(mVelocity); }

   float                                  getRadius() { return mRadius; }

   float                                  getMass() const {return mMass;}

   float                                  getMaxSpeed() const {return mMaxSpeed;}

   float                                  getMaxWalkSpeed() {return mWalkMaxSpeed;}

   float                                  getMaxRunSpeed() {return mRunMaxSpeed; }

   float                                  getMaxForce() const {return mMaxForce;}

   float                                  getMaxTurnRate() const {return mMaxTurnRate;}


   // Setters
   void                                   setVelocity(sf::Vector2f newVel){mVelocity = newVel; }

   void                                   setMaxSpeed(float speed){ mMaxSpeed = speed;}


};

#endif // DYNAMIC_HPP
