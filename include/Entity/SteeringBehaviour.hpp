#ifndef STEERINGBEHAVIOUR_HPP
#define STEERINGBEHAVIOUR_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

//#include "Enum.hpp"
<<<<<<< HEAD:include/Entity/SteeringBehaviour.hpp
#include "App/Params.hpp"
#include "Entity/Path.hpp"

class Params;
//class Dynamic;
=======
#include "Entity/Path.hpp"

class Params;
>>>>>>> working:include/Entity/SteeringBehaviour.hpp
class Entity;
class Target;

class SteeringBehaviour
{
public:
    enum Deceleration
    {
        Slow = 3,
        Normal = 2,
        Fast = 1
    };

    enum Behaviour
    {
        Rest,
        Arrive,
        Evade,
        Wander,
        FollowPath,
        ObstacleAvoidance,
        WallAvoidance,
        Seperation,
        Flock,
        NumBehaviour
    };

    enum Feelers
    {
        Left,
        Front,
        Right,
        NumFlr
    };

    static const float                     mPI;

private:
<<<<<<< HEAD:include/Entity/SteeringBehaviour.hpp
    const float                            mWanderRadius;
    const float                            mWanderDistance;
    const float                            mWanderJitter;
    const float                            mMinViewBoxLength;
    const float                            mInteractionRadius;
    const float                            mFeelerLength;
    const float                            mMinArriveDist;

    const float                            mObstacleAvoidanceMultiplier;
    const float                            mWallAvoidanceMultiplier;
    const float                            mArriveMultiplier;
    const float                            mEvadeMultiplier;
    const float                            mWanderMultiplier;
    const float                            mSeperationMultiplier;
    const float                            mAlignmentMultiplier;
    const float                            mCohesionMultiplier;
    const float                            mFlockingMultiplier;
    const float                            mSeperationRadius;
    const float                            mAlignRadius;
    const float                            mCohesionRadius;

//    DynamicEntity*                          mHost;
    Entity*                                mHost;
    float                                  mTheta;
    Path                                   mPath;
=======
     const float                            mWanderRadius;
     const float                            mWanderDistance;
     const float                            mWanderJitter;
     const float                            mMinViewBoxLength;
     const float                            mInteractionRadius;
     const float                            mFeelerLength;
     const float                            mMinArriveDist;

     const float                            mObstacleAvoidanceMultiplier;
     const float                            mWallAvoidanceMultiplier;
     const float                            mArriveMultiplier;
     const float                            mEvadeMultiplier;
     const float                            mWanderMultiplier;
     const float                            mSeperationMultiplier;
     const float                            mAlignmentMultiplier;
     const float                            mCohesionMultiplier;
     const float                            mFlockingMultiplier;
     const float                            mSeperationRadius;
     const float                            mAlignRadius;
     const float                            mCohesionRadius;

    Entity*                           mHost;
    float                                   mTheta;
    Path                                    mPath;
>>>>>>> working:include/Entity/SteeringBehaviour.hpp

    std::array<bool,
        Behaviour::NumBehaviour>    mBehaviourFlags;

    sf::Vector2f                            mWanderTarget;
    std::vector<sf::Vector2f>               mFeelers;
    sf::Vector2f                            mViewBoxEnd;

    bool                                    accumulateForce(sf::Vector2f&, sf::Vector2f);
    void                                    createFeelers();

    sf::Vector2f                            rest();

    sf::Vector2f                            arrive(sf::Vector2f
                                                   , Deceleration);

    sf::Vector2f                            seek(sf::Vector2f);

    sf::Vector2f                            evade();
    sf::Vector2f                            flee(sf::Vector2f);

    sf::Vector2f                            wander(sf::Time);
    sf::Vector2f                            followPath();

    sf::Vector2f                            obstacleAvoidance();
    sf::Vector2f                            wallAvoidance();

    sf::Vector2f                            seperation();
    sf::Vector2f                            alignment();
    sf::Vector2f                            cohesion();
    sf::Vector2f                            flocking();

public:
<<<<<<< HEAD:include/Entity/SteeringBehaviour.hpp
                                             SteeringBehaviour(Entity* host
                                                               , const Params& params);
=======
                                            SteeringBehaviour(Entity*
                                                              , const Params&);
>>>>>>> working:include/Entity/SteeringBehaviour.hpp

    sf::Vector2f                            calculate(sf::Time);

    void                                    addToPath(sf::Vector2f pos) {mPath.addToPath(pos);}

    void                                    startNewPath(sf::Vector2f pos) {mPath.startNewPath(pos);}

    // Getters
    bool                                    checkBehaviour(SteeringBehaviour::Behaviour type) const {return mBehaviourFlags.at(type);}

    std::vector<sf::CircleShape>            getPathToDraw() const{return mPath.getPathToDraw();}

    // Setters
    void                                    setNewBehaviours(std::vector<SteeringBehaviour::Behaviour>);
    void                                    setNewBehaviours(SteeringBehaviour::Behaviour);
};

#endif // STEERINGBEHAVIOUR_HPP
