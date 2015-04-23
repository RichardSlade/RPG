#ifndef BARBARIAN_HPP
#define BARBARIAN_HPP

#include "Adventurer.hpp"
#include "MeleeFighter.hpp"

class Barbarian : public Adventurer, public MeleeFighter
{
public:
//   typedef std::vector<std::unique_ptr<State<Barbarian>>> StateContainer;
//
//   enum StateType
//   {
//      LookOut,
//      Relax,
//      Attack,
//      NumStates
//   };

private:
//   StateContainer&           mStates;
//   StateMachine<Adventurer>  mStateMachine;

   virtual void               updateCurrent(sf::Time dt)
                              {
                                 Adventurer::updateCurrent(dt);
                                 MeleeFighter::update(dt);
                              }

public:
                              Barbarian(Level* level
                                       , const sf::Texture& texture
                                       , const sf::Font& font
                                       , sf::Vector2f startPos
                                       , float scale
                                       , EntityStats stats
                                       , const Params& params
                                       , State<Adventurer>* globalState
                                       , State<Adventurer>* initState
                                       , StateContainer& states
                                       , Adventurer::StateType currentStateType)
                              : Adventurer(Adventurer::AdventurerType::Barbarian
                                          , level
                                          , texture
                                          , font
                                          , startPos
                                          , scale
                                          , stats
                                          , params
                                          , globalState
                                          , initState
                                          , states
                                          , currentStateType)
                              , MeleeFighter(sf::seconds(stats.attackDelay)
                                              , stats.meleeDamage
                                              , stats.attackDist)
                              {};


   virtual                    ~Barbarian(){};

//   int                        getCurrentState() {return mStateMachine.getCurrentStateType(); }
//   void                       changeState(Adventurer::StateType newState){ mStateMachine.changeState(mStates.at(newState).get(), newState); }
};

#endif // BARBARIAN_HPP
