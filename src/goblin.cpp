#include "Entity/Goblin.hpp"

Goblin::Goblin(Level* level
            , const sf::Texture& texture
            , const sf::Font& font
            , sf::Vector2f startPos
            , float scale
            , EntityStats stats
            , const Params& params
            , State<Goblin>* globalState
            , State<Goblin>* initState
            , StateContainer& states
            , Goblin::StateType currentStateType)
: Enemy(level
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
{}

void Goblin::updateCurrent(sf::Time dt)
{
   Enemy::updateCurrent(dt);
   MeleeFighter::update(dt);
}
