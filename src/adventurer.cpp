/*
*   Adventurer.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include "Entity/Adventurer.hpp"
#include "World/World.hpp"

Adventurer::Adventurer(Level* level
         , const sf::Texture& texture
         , const sf::Font& font
         , sf::Vector2f startPos
         , EntityStats stats
         , const Params& params
         , State<Adventurer>* globalState
         , State<Adventurer>* initState
         , StateContainer& states
         , unsigned int currentState
         , float scale)
: Entity(level
               , texture
               , font
               , startPos
               , stats
               , params
               , Entity::Type::Adventurer
               , params.CharacterPanicDistance
               , scale)
, mStates(states)
, mStateMachine(this, globalState, initState, currentState)
{
    setSteeringTypes(SteeringBehaviour::Behaviour::FollowPath);
    mText.setString("Woof!");
}

/*
*   Main update for sheep dog
*/
void Adventurer::updateCurrent(sf::Time dt)
{
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
}

/*
*   Draw function used by SFML to render to sf::RenderTarget
*/
void Adventurer::drawCurrent(sf::RenderTarget& target
                                    , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);

    std::vector<sf::CircleShape> wypnts = mSteering.getPathToDraw();

    for(sf::CircleShape circle : wypnts)
        target.draw(circle);
}
