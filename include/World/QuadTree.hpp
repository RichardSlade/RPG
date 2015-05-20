#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <memory>

#include "World/Scenery.hpp"
#include "Entity/Entity.hpp"

class QuadTree
{
public:
   enum ObjectType
   {
      SceneryObjects,
      EntityObjects,
      AllObjects
   };

private:
   typedef std::unique_ptr<QuadTree> upQuadTree;

   const int                  mMaxObjects = 10;
   const int                  mMaxLevels = 5;

   int                        mLevel;
   std::list<Scenery*>        mScenery;
   std::list<Entity*>         mEntities;

   sf::FloatRect              mBounds;
   std::array<upQuadTree, 4>  mChildren;

   void                       split();
   void                       checkForsplit();
   void                       distributeObjects();
   int                        getIndex(sf::FloatRect pRect);


public:
                              QuadTree(int level,
                                       sf::FloatRect bounds);

   void                       insert(Scenery* scenery);
   void                       insert(Entity* entity);

   std::list&                 retrieve(QuadTree::ObjectType objType,
                                       std::list& returnObjects,
                                       PhysicsBody* body)

   void                       clear();
};

#endif // QUADTREE_HPP
