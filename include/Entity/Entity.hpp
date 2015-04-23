#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "App/Utility.hpp"
#include "World/LevelBlock.hpp"
#include "Node/SpriteNode.hpp"
#include "Entity/Dynamic.hpp"
#include "Entity/EntityStats.hpp"
//#include "Level.hpp"
//#include "EntityStats.hpp"
//#include "MovingTarget.hpp"
//#include "Params.hpp"

class Level;
//class LevelBlock;

class Entity : public Dynamic
{
public:
   enum EntityType
   {
      Adventurer,
      Enemy,
      All
   };

//   const float                 mSightRange;
//   const float                 mAngleOfVision;

protected:

   EntityType                 mEntityType;

   Level*                     mLevel;
   LevelBlock*                mCurrentBlock;

   sf::Text                   mText;
   SteeringBehaviour          mSteering;

   virtual void               updateCurrent(sf::Time dt);
   virtual void               drawCurrent(sf::RenderTarget& target
                                          , sf::RenderStates states) const;

   void                       insertIntoLevel();

public:
//                                 Entity(EntityType type
//                                        , Level* level
//                                        , const sf::Texture& texture
//                                        , sf::Vector2f startPos
//                                        , float scale);

                                 Entity(const sf::Texture& texture
                                        , const sf::Font& font
                                        , sf::Vector2f startPos
                                        , float scale
                                        , const EntityStats& stats
                                        , const Params& params
                                        , EntityType type
                                        , Level* level);
//                                 : Dynamic(texture
//                                          , startPos
//                                          , scale
//                                          , stats
//                                          , params)
//                                 , mEntityType(type)
//                                 , mLevel(level)
//                                 , mCurrentBlock(nullptr)
//                                 , mText("", font)
//                                 , mSteering(this
//                                             , params)
//                                 {
//                                     sf::FloatRect bounds = mSprite.getLocalBounds();
//                                     mRadius = std::max(bounds.width, bounds.height);
//                                 }

   virtual                       ~Entity(){};

   virtual std::vector<Entity*>  getNeighbours(float radius
                                              , int type) const;

   // Getters
   EntityType                    getEntityType() {return mEntityType;}

//   std::vector<Entity*>       getNeighbours(float radius
//                                           , int type) const;
//
   std::vector<LevelBlock*>   getBlockTypeInRange(LevelBlock::Type, float) const;

//   std::vector<LevelBlock*>   getLevelExit();

   float                         getRadius() const {return mRadius;}

//   float                      getSpriteWidth() const{return mSprite.getLocalBounds().width;}

//   Entity*                    getCurrentTarget() const {return mCurrentTarget;}

//   LevelBlock*                   getLevelBlock(sf::Vector2i);

   LevelBlock*                   getCurrentBlock() {return mCurrentBlock;}

   // Setters
   void                          changeColour(sf::Color colour){mSprite.setColor(colour);}

   bool                          checkSteeringBehaviour(SteeringBehaviour::Behaviour type) const {return mSteering.checkBehaviour(type);}

   void                          setSteeringTypes(std::vector<SteeringBehaviour::Behaviour> newBehaviours) {mSteering.setNewBehaviours(newBehaviours); }

   void                          setSteeringTypes(SteeringBehaviour::Behaviour newBehaviour) {mSteering.setNewBehaviours(newBehaviour);}


      void                       setText(std::string msg)
                              {
                                 mText.setString(msg);
                                 mText.setColor(sf::Color(255, 255, 255, 255));
                              }

//   void                       setCurrentTarget(Entity* target) {mCurrentTarget = target;}

//   void                       setText(std::string msg)
//                              {
//                                 mText.setString(msg);
//                                 mText.setColor(sf::Color(255, 255, 255, 255));
//                              }
};

#endif // ENTITY_HPP
