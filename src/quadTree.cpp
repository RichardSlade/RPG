#include <cassert>

#include "World/QuadTree.hpp"

QuadTree::QuadTree(int level,
         sf::FloatRect mBounds)
: mLevel(level)
, mBounds(mBounds)
{};

/*
 * Splits the node into 4 subnodes
 */
void QuadTree::split()
{
   int subWidth = static_cast<int>(mBounds.width() / 2);
   int subHeight = static_cast<int>(mBounds.height() / 2);
   int x = static_cast<int>mBounds.left();
   int y = static_cast<int>mBounds.top();

   nodes[0] = upQuadTree(new Quadtree(mLevel+1, sf::FloatRect(x + subWidth, y, subWidth, subHeight)));
   nodes[1] = upQuadTree(new Quadtree(mLevel+1, sf::FloatRect(x, y, subWidth, subHeight)));
   nodes[2] = upQuadTree(new Quadtree(mLevel+1, sf::FloatRect(x, y + subHeight, subWidth, subHeight)));
   nodes[3] = upQuadTree(new Quadtree(mLevel+1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight)));
}

void QuadTree::checkForsplit()
{
   int totalObjects = mScenery.size() + mEntities.size();

   if(totalObjects > mMaxObjects && mLevel < mMaxLevels)
   {
      if(mChildren.at(0))
         split();
      else
         distributeObjects();
   }
}

void QuadTree::distributeObjects()
{
   std::vector<Scenery*>::iterator sceneryIter;

   for(sceneryIter = mScenery.begin();
       sceneryIter != mScenery.end();
       sceneryIter++)
   {
      int index = getIndex(*sceneryIter);

      if(index != -1)
         mChildren.at(index).insert(*sceneryIter);
   }

   std::vector<Entity*>::iterator entityIter;

   for(entityIter = mEntities.begin();
       entityIter != mEntities.end();
       entityIter++)
   {
      int index = getIndex(*entityIter);

      if(index != -1)
         mChildren.at(index).insert(*entityIter);
   }
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
int QuadTree::getIndex(PhysicsBody* body)
{
   int index = -1;
   double verticalMidpoint = mBounds.left() + (mBounds.width() / 2);
   double horizontalMidpoint = mBounds.top() + (mBounds.height() / 2);

   assert(body);

   sf::FloatRect rect = body->getBodyBounds();

   // Object can completely fit within the top quadrants
   boolean topQuadrant = (rect.top() < horizontalMidpoint && rect.top() + rect.height() < horizontalMidpoint);
   // Object can completely fit within the bottom quadrants
   boolean bottomQuadrant = (rect.top() > horizontalMidpoint);

   // Object can completely fit within the left quadrants
   if (rect.left() < verticalMidpoint && rect.left() + rect.width() < verticalMidpoint) {
      if (topQuadrant) {
        index = 1;
      }
      else if (bottomQuadrant) {
        index = 2;
      }
    }
    // Object can completely fit within the right quadrants
    else if (rect.left() > verticalMidpoint) {
     if (topQuadrant) {
       index = 0;
     }
     else if (bottomQuadrant) {
       index = 3;
     }
   }

   return index;
 }

/*
 * Insert entity in quad tree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
void QuadTree::insert(Scenery* entity)
{
   if(mChildren.at(0))
   {
      int index = getIndex(entity);

      if(index != -1)
      {
         nodes.at(index).insert(entity);
         return;
      }
   }

   mScenery.push_back(entity);
   checkForsplit();
}

/*
 * Insert entity in quad tree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
void QuadTree::insert(Entity* entity)
{
   if(mChildren.at(0))
   {
      int index = getIndex(entity);

      if(index != -1)
      {
         nodes.at(index).insert(entity);
         return;
      }
   }

   mScenery.push_back(entity);
   checkForSplit();
}

/*
 * Return all objects that could collide with the given object
 */
std::list& QuadTree::retrieve(QuadTree::ObjectType objType,
                   std::list& returnObjects,
                   PhysicsBody* body)
{
   int index = getIndex(body);

   if(index != -1 && mChildren.at(0))
   {
      nodes.at(index).retrieve(returnObjects, body);
   }

   switch(type)
   {
      case QuadTree::ObjectType::SceneryObjects: returnObjects.insert(returnObjects.end(),
                                                                      mScenery.begin(),
                                                                      mScenery.end()); break;

      case QuadTree::ObjectType::EntityObjects: returnObjects.insert(returnObjects.end(),
                                                                     mEntities.begin(),
                                                                     mEntities.end());
                                                                     break;
      case default:
      {
        returnObjects.insert(returnObjects.end(), mScenery.begin(), mScenery.end());
        returnObjects.insert(returnObjects.end(), mEntities.begin(), mEntities.end());
        break;
      }
   }

   return returnObjects;
 }

void QuadTree::clear()
{
   mScenery.clear();
   mEntities.clear();
   mChildren.clear();
}
