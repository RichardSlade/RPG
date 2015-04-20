/*
*   Character.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include "World/World.hpp"
#include "Entity/Adventurer.hpp"

Character::Character(Level* level
                     , const sf::Texture& texture
                     , const sf::Font& font
                     , sf::Vector2f startPos
                     , EntityStats stats
                     , const Params& params
                     , State<Character>* globalState
                     , State<Character>* initState
                     , StateContainer& states
                     , Character::StateType currentStateType
                     , float scale)
: MovingEntity(level
               , texture
               , font
               , startPos
               , stats
               , params
               , MovingEntity::EntityType::Character
               , scale)
,MeleeCombatant(dynamic_cast<MovingEntity*>(this)
               , sf::seconds(stats.attackDelay))
, mStates(states)
, mStateMachine(this
               , globalState
               , initState
               , currentStateType)
{
    setSteeringTypes(SteeringBehaviour::Behaviour::FollowPath);
    mText.setString("");
}

/*
*   Main update for sheep dog
*/
void Character::updateCurrent(sf::Time dt)
{
//    mStateMachine.update();

//    MovingEntity::updateCurrent(dt);
    sf::Color currentTextColor = mText.getColor();

    currentTextColor.a -= 1;

    mText.setColor(currentTextColor);

    sf::Vector2f steering = mSteering.calculate(dt);

    mVelocity += steering;

    if(std::fabs(magVec(mVelocity)) > MINFLOAT)
    {
        int sign = signVec(mHeading, mVelocity);

        float angle = std::acos(dotVec(mHeading, normVec(mVelocity)));
        angle *= sign;

        clampRotation(angle
                      , -mMaxTurnRate
                      , mMaxTurnRate);

        if(angle > MINFLOAT || angle < -MINFLOAT)
            rotate(angle * (180.f / SteeringBehaviour::mPI));
    }

    float currentRotation = getRotation() * (SteeringBehaviour::mPI / 180.f);
    mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));

    move(mVelocity);

//    adjustPosition();

    sf::FloatRect bounds = mText.getLocalBounds();
    mText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sf::Vector2f textPos = getWorldPosition();
    textPos.y -= 20.f;
    mText.setPosition(textPos);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mLevel->insertEntityIntoLevel(this);

    MeleeCombatant::update(dt);
}

/*
*   Draw function used by SFML to render to sf::RenderTarget
*/
void Character::drawCurrent(sf::RenderTarget& target
                            , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);

    std::vector<sf::CircleShape> wypnts = mSteering.getPathToDraw();

    for(sf::CircleShape circle : wypnts)
        target.draw(circle);
}
