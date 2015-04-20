#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Node/SceneNode.hpp"
#include "Entity/EntityStats.hpp"
#include "App/Utility.hpp"
//#include "Level.hpp"
//#include "LevelBlock.hpp"
//#include "EntityStats.hpp"
//#include "MovingTarget.hpp"
//#include "Params.hpp"

class Level;

class Entity : public SceneNode
{
public:
   enum EntityType
   {
      Character,
      Enemy,
      All
   };

protected:

   EntityType                 mType;

   Level*                     mLevel;
   LevelBlock*                mCurrentBlock;

   sf::Sprite                 mSprite;
   sf::Text                   mText;

   Entity*                    mCurrentTarget;
   float                      mRadius

public:
                              Entity(EntityType type
                                     , Level* level
                                     , const sf::Texture& texture
                                     , const sf::Font& font
                                     , float scale);

   virtual                    ~Entity(){};

   virtual void               updateCurrent(sf::Time) = 0;

   // Getters
   std::vector<MovingEntity*> getNeighbours(float
                                           , int) const;

   std::vector<LevelBlock*>   getBlockTypeInRange(LevelBlock::Type, float) const;

   std::vector<LevelBlock*>   getLevelExit();

   float                      getRadius() const {return mRadius;}

   float                      getSpriteWidth() const{return mSprite.getLocalBounds().width;}

   MovingEntity*              getCurrentTarget() const {return mCurrentTarget;}

   LevelBlock*                getLevelBlock(sf::Vector2i);

   LevelBlock*                getLevelBlock() {return mCurrentBlock;}

   EntityType                 getEntityType() {return mType;}

   // Setters
   void                       changeColour(sf::Color colour){mSprite.setColor(colour);}

   void                       setCurrentTarget(Entity* target) {mCurrentTarget = target;}

   void                       setText(std::string msg)
                              {
                                 mText.setString(msg);
                                 mText.setColor(sf::Color(255, 255, 255, 255));
                              }
};

#endif // ENTITY_HPP
