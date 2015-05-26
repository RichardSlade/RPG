#include <cassert>

#include "World/Scenery.hpp"
#include "World/QuadTree.hpp"
#include "Entity/Entity.hpp"

QuadTree::QuadTree()
//: mHasSplit(false)
{

};

QuadTree::QuadTree(int level,
                   sf::FloatRect mBounds)
: mLevel(level)
, mBounds(mBounds)
//, mHasSplit(false)
{
  for(int i = 0; i < 4; i++)
    mChildren.at(i) = nullptr;
};

/*
 * Splits the node into 4 subnodes
 */
void QuadTree::split(){
  int subWidth = static_cast<int>(mBounds.width / 2);
  int subHeight = static_cast<int>(mBounds.height / 2);
  int x = static_cast<int>(mBounds.left);
  int y = static_cast<int>(mBounds.top);

  mChildren.at(0) = QuadTree::upQuadTree(new QuadTree(mLevel + 1,
                                                     sf::FloatRect(x + subWidth, y, subWidth, subHeight)));
  mChildren.at(1) = QuadTree::upQuadTree(new QuadTree(mLevel + 1,
                                                     sf::FloatRect(x, y, subWidth, subHeight)));
  mChildren.at(2) = QuadTree::upQuadTree(new QuadTree(mLevel + 1,
                                                     sf::FloatRect(x, y + subHeight, subWidth, subHeight)));
  mChildren.at(3) = QuadTree::upQuadTree(new QuadTree(mLevel + 1,
                                                     sf::FloatRect(x + subWidth, y + subWidth, subWidth, subHeight)));


//  std::cout << "Splitting" << std::endl;

//  mHasSplit = true;
}

void QuadTree::checkForSplit(){
   int totalObjects = mScenery.size() + mEntities.size();

//   std::cout << "checkForSplit(): " << totalObjects << std::endl;

   if(totalObjects > mMaxObjects && mLevel < mMaxLevels)
   {
//      if(!mHasSplit)
      if(!mChildren.at(0))
         split();
      else
         distributeObjects();
   }
}

void QuadTree::distributeObjects(){
   std::list<Scenery*>::iterator sceneryIter;

   for(sceneryIter = mScenery.begin();
       sceneryIter != mScenery.end();
       sceneryIter++)
   {
      int index = getIndex(*sceneryIter);

      if(index != -1)
         mChildren.at(index)->insert(*sceneryIter);
   }

   std::list<Entity*>::iterator entityIter;

   for(entityIter = mEntities.begin();
       entityIter != mEntities.end();
       entityIter++)
   {
      int index = getIndex(*entityIter);

      if(index != -1)
         mChildren.at(index)->insert(*entityIter);
   }
}

/*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
int QuadTree::getIndex(const PhysicsBody* body) const{
   int index = -1;
   double verticalMidpoint = mBounds.left + (mBounds.width / 2);
   double horizontalMidpoint = mBounds.top + (mBounds.height / 2);

   assert(body);

   sf::FloatRect rect = body->getBodyBounds();

   // Object can completely fit within the top quadrants
   bool topQuadrant = (rect.top < horizontalMidpoint && rect.top + rect.height < horizontalMidpoint);
   // Object can completely fit within the bottom quadrants
   bool bottomQuadrant = (rect.top > horizontalMidpoint);

   // Object can completely fit within the left quadrants
   if (rect.left < verticalMidpoint && rect.left + rect.width < verticalMidpoint) {
      if (topQuadrant) {
        index = 1;
      }
      else if (bottomQuadrant) {
        index = 2;
      }
    }
    // Object can completely fit within the right quadrants
    else if (rect.left > verticalMidpoint) {
     if (topQuadrant) {
       index = 0;
     }
     else if (bottomQuadrant) {
       index = 3;
     }
   }

//  std::cout << "Index: " << index << std::endl;

   return index;
 }

/*
 * Insert entity in quad tree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding mChildren.
 */
void QuadTree::insert(Scenery* scenery){
//  std::cout << "insert Scenery: " << mHasSplit << std::endl;

  if(mChildren.at(0)){

//          std::cout << "In scenery if" << std::endl;

    int index = getIndex(scenery);

    if(index != -1)
      mChildren.at(index)->insert(scenery);
   }
   else{
//          std::cout << "In scenery else" << std::endl;

      mScenery.push_back(scenery);
      checkForSplit();
   }
}

/*
 * Insert entity in quad tree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding mChildren.
 */
void QuadTree::insert(Entity* entity){
//  std::cout << "insert entity: " << mHasSplit << std::endl;

  if(mChildren.at(0))
  {
//        std::cout << "In entity if" << std::endl;

      int index = getIndex(entity);

      if(index != -1)
        mChildren.at(index)->insert(entity);
  }
  else
  {
//    std::cout << "In entity else" << std::endl;
    mEntities.push_back(entity);
    checkForSplit();
  }
}

/*
 * Return all scenery that could collide with the given object
 */
std::list<Scenery*>& QuadTree::retrieveScenery(std::list<Scenery*>& returnObjects,
                                               const PhysicsBody* body,
                                               unsigned int type) const{
   int index = getIndex(body);

   if(index != -1 && mChildren.at(0))
   {
      mChildren.at(index)->retrieveScenery(returnObjects,
                                           body,
                                           type);
   }

  std::list<Scenery*> sceneryOfType;
//  std::list<Entity*>::iterator iter;
  for(Scenery* sc : mScenery)
  {
    if(sc->getSceneryType() == type)
      sceneryOfType.push_back(sc);
  }

  returnObjects.insert(returnObjects.end(),
                       sceneryOfType.begin(),
                       sceneryOfType.end());

   return returnObjects;
 }

/*
 * Return all entities that could collide with the given object
 */
std::list<Entity*>& QuadTree::retrieveEntities(std::list<Entity*>& returnObjects,
                                              const PhysicsBody* body,
                                              unsigned int type) const{
   int index = getIndex(body);

   if(index != -1 && mChildren.at(0))
   {
      mChildren.at(index)->retrieveEntities(returnObjects, body, type);
   }

  std::list<Entity*> entitiesOfType;
//  std::list<Entity*>::iterator iter;
  for(Entity* e : mEntities)
  {
    if(e->getEntityType() == type)
      entitiesOfType.push_back(e);
  }

  returnObjects.insert(returnObjects.end(),
                       entitiesOfType.begin(),
                       entitiesOfType.end());

   return returnObjects;
 }

void QuadTree::clear(){
  mScenery.clear();
  mEntities.clear();
//  mChildren.clear();

  std::array<QuadTree::upQuadTree, 4>::iterator iter;

  for(iter = mChildren.begin();
    iter != mChildren.end();
    iter ++)
  {
    (*iter).release();
    (*iter) = nullptr;
  }

//  std::cout << "Clearing quad tree" << std::endl;

//  mHasSplit = false;
}

