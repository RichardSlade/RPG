/*
*   Adventurer.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include "World/World.hpp"
#include "Entity/Adventurer.hpp"

Adventurer::Adventurer(Adventurer::Type adventurerType
                     , const sf::Texture& texture
                     , const sf::Font& font
                     , sf::Vector2f startPos
                     , float scale
                     , EntityStats stats
                     , const Params& params
                     , Level* level
                     , State<Adventurer>* globalState
                     , State<Adventurer>* initState
                     , StateContainer& states
                     , Adventurer::StateType currentStateType)
: Entity(texture
         , font
         , startPos
         , scale
         , stats
         , params
         , Entity::EntityType::Adventurer
         , level)
, Intelligent(stats)
, Killable(stats.health)
, MeleeFighter(sf::seconds(stats.attackDelay)
                , stats.meleeDamage
                , stats.attackDist)
, mAdventurerType(adventurerType)
, mStates(states)
, mStateMachine(this
               , globalState
               , initState
               , currentStateType)
{
   setSteeringTypes(SteeringBehaviour::Behaviour::FollowPath);
//   mText.setPosition(-10.f, -40.f);
}

/*
*   Main update for sheep dog
*/
void Adventurer::updateCurrent(sf::Time dt)
{
   mStateMachine.update();
   Entity::updateCurrent(dt);
}

/*
*   Draw function used by SFML to render to sf::RenderTarget
*/
void Adventurer::drawCurrent(sf::RenderTarget& target
                            , sf::RenderStates states) const
{
   Entity::drawCurrent(target, states);

    std::vector<sf::CircleShape> wypnts = mSteering.getPathToDraw();

    for(sf::CircleShape circle : wypnts)
        target.draw(circle);
}
