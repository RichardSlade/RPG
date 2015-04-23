//#include <iostream>
//#include <cmath>
//#include <vector>

//#include "App/Utility.hpp"
//#include "App/Params.hpp"
//#include "World/World.hpp"
//#include "Entity/Agent.hpp"



//void Agent::updateCurrent(sf::Time dt)
//{
////    mStateMachine.update();
//
//    DynamicEntity::updateCurrent(dt);
//
//    mCurrentBlock->deleteEntity(this);
//    mCurrentBlock = mLevel->insertEntityIntoLevel(this);
//
////    MeleeCombatant::update(dt);
//
////    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
////    {
////        mToRemove = true;
////        mMovingTarget = nullptr;
////        mCurrentBlock->deleteEntity(this);
////
//////        mLevel->incAgentHerded();
////    }
//}

//void Agent::drawCurrent(sf::RenderTarget& target
//                        , sf::RenderStates states) const
//{
//    target.draw(mSprite, states);
//    target.draw(mText);
//}

//void Agent::addPursuer(MovingEntity* pursuer)
//{
//  mPursuers.push_back(pursuer);
//}
//
//void Agent::deletePursuer(MovingEntity* pursuer)
//{
//    auto it = find (mPursuers.begin(), mPursuers.end(), pursuer);
//
//    assert(it != mPursuers.end());
//
//    mPursuers.erase(it);
//}
//
//void Agent::clearPursuers()
//{
//  mPursuers.clear();
//}

//void Agent::changeState(Agent::States newState)
//{
//    mStateMachine.changeState(mStates.at(newState).get());
//}
