#include <algorithm>
#include <cassert>
#include <iostream>

#include "SceneNode/SceneNode.hpp"
#include "World/QuadTree.hpp"

SceneNode::SceneNode()
: mParent(nullptr)
, mChildren()
, mToRemove(false)
{

}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::addToQuadTree(QuadTree* quadTree)
{
  for(upScNode& node : mChildren)
    node->addToQuadTree(quadTree);
}

void SceneNode::updateChildren(sf::Time dt)
{
  for(upScNode& node : mChildren)
    node->update(dt);
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
  for(const upScNode& node : mChildren)
    node->draw(target, states);
}

void SceneNode::update(sf::Time dt)
{
//  addToQuadTree();
  updateCurrent(dt);
  updateChildren(dt);
}

void SceneNode::addChild(upScNode node)
{
    node->mParent = this;
    mChildren.push_back(std::move(node));
}

SceneNode::upScNode SceneNode::deleteChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                                [&] (SceneNode::upScNode& p) -> bool { return p.get() == &node; } );

    assert(found != mChildren.end());

    SceneNode::upScNode result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);

    return result;
}

void SceneNode::removeDeletedNodes()
{
    // Remove all children which request so
	auto wreckfieldBegin = std::remove_if(mChildren.begin()
                                       , mChildren.end()
                                       , std::mem_fn(&SceneNode::getToRemove));

	mChildren.erase(wreckfieldBegin, mChildren.end());

	// Call function recursively for all remaining children
	std::for_each(mChildren.begin()
               , mChildren.end()
               , std::mem_fn(&SceneNode::removeDeletedNodes));
}

//sf::Transform SceneNode::getWorldTransform() const
//{
//
//    sf::Transform trans = sf::Transform::Identity;
//
//    for(const SceneNode* node = this; node != nullptr; node = node->mParent)
//        trans = node->getTransform() * trans;
//
//    return trans;
//}
//
//sf::Vector2f SceneNode::getWorldPosition() const
//{
//    return getWorldTransform() * sf::Vector2f();
//}
