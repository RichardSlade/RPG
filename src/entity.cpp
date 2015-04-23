#include "World/Level.hpp"
#include "Entity/Entity.hpp"

Entity::Entity(const sf::Texture& texture
       , const sf::Font& font
       , sf::Vector2f startPos
       , float scale
       , const EntityStats& stats
       , const Params& params
       , EntityType type
       , Level* level)
: Dynamic(texture
      , startPos
      , scale
      , stats
      , params)
, mEntityType(type)
, mLevel(level)
, mCurrentBlock(nullptr)
, mText("", font)
, mSteering(this
         , params)
{
   mText.setPosition(-10.f, -40.f);

   insertIntoLevel();
}

void Entity::updateCurrent(sf::Time dt)
{
   Dynamic::move(mSteering.calculate(dt));

   insertIntoLevel();
}

void Entity::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
   SpriteNode::drawCurrent(target
                           , states);

   target.draw(mText);
}

void Entity::insertIntoLevel()
{
   mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

std::vector<Entity*> Entity::getNeighbours(float radius
                                           , int type) const
{
    return mLevel->getEntitiesInRange(const_cast<Entity*>(this)
                                      , radius
                                      , type);
}

std::vector<LevelBlock*> Entity::getBlockTypeInRange(LevelBlock::Type blockType, float radius) const
{
    return mLevel->getBlockTypeInRange(const_cast<Entity*>(this), radius, blockType);
}

//LevelBlock* Entity::getLevelBlock(sf::Vector2i index)
//{
//    return mLevel->getBlock(index);
//}
