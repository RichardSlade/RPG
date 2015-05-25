#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <memory>
#include <list>

#include <SFML/Graphics/Rect.hpp>

//#include "World/Scenery.hpp"
//#include "Entity/Entity.hpp"

class Scenery;
class Entity;
class PhysicsBody;

class QuadTree
{
public:
  typedef std::unique_ptr<QuadTree> upQuadTree;

  enum ObjectType
  {
    SceneryObjects,
    EntityObjects,
    AllObjects
  };

private:
  const int                             mMaxObjects = 10;
  const int                             mMaxLevels = 5;

  int                                   mLevel;
  std::list<Scenery*>                   mScenery;
  std::list<Entity*>                    mEntities;
  bool                                  mHasSplit;

  sf::FloatRect                         mBounds;
  std::array<QuadTree::upQuadTree, 4>   mChildren;
//  std::array<int, 4>   mChildren;
//  std::vector<QuadTree::upQuadTree>     mChildren;

  void                                  split();
  void                                  checkForSplit();
  void                                  distributeObjects();
  int                                   getIndex(const PhysicsBody* body) const;


public:
                                        QuadTree(){};

                                        QuadTree(int level,
                                               sf::FloatRect bounds);

  void                                  insert(Scenery* scenery);
  void                                  insert(Entity* entity);

  std::list<Scenery*>&                  retrieveScenery(std::list<Scenery*>& returnObjects,
                                                        const PhysicsBody* body,
                                                        unsigned int type) const;

  std::list<Entity*>&                   retrieveEntities(std::list<Entity*>& returnObjects,
                                                         const PhysicsBody* body,
                                                         unsigned int type) const;

  void                                  clear();
};

#endif // QUADTREE_HPP
