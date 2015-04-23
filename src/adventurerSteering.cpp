#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "App/Utility.hpp"
#include "App/Params.hpp"
#include "World/Wall.hpp"
#include "World/LevelBlock.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/DynamicEntity.hpp"
#include "Entity/Adventurer.hpp"


//const float SteeringBehaviour::mPI = 3.14159265358979;

SteeringBehaviour<Adventurer>::SteeringBehaviour(Adventurer* host
                                     , const Params& params)
: mWanderRadius(params.WanderRadius)
, mWanderDistance(params.WanderDistance)
, mWanderJitter(params.WanderJitter)
, mMinViewBoxLength(params.MinViewBoxLength)
, mInteractionRadius(params.InteractionRadius)
, mFeelerLength(params.FeelerLength)
, mMinArriveDist(params.MinArriveDist)
, mObstacleAvoidanceMultiplier(2.f)
, mWallAvoidanceMultiplier(params.WallAvoidanceMultiplier)
, mArriveMultiplier(params.ArriveMultiplier)
, mEvadeMultiplier(params.EvadeMultiplier)
, mWanderMultiplier(params.WanderMultiplier)
, mSeperationMultiplier(params.SeperationMultiplier)
, mAlignmentMultiplier(params.AlignmentMultiplier)
, mCohesionMultiplier(params.CohesionMultiplier)
, mFlockingMultiplier(params.FlockingMultiplier)
, mSeperationRadius(params.SeperationRadius)
, mAlignRadius(params.AlignRadius)
, mCohesionRadius(params.CohesionRadius)
, mHost(host)
, mTheta(mHost->getRotation() * (mPI / 180.f))
, mWanderTarget(std::sin(mTheta) * mWanderRadius, -std::cos(mTheta) * mWanderRadius)
{
    for(bool& b : mBehaviourFlags)
        b = false;

    if(mHost->getEntityType() != Entity::EntityType::Adventurer)
        mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;

    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
};

bool SteeringBehaviour<Adventurer>::accumulateForce(sf::Vector2f& runningTotal, sf::Vector2f forceToAdd)
{
    float magSoFar = magVec(runningTotal);

    float magRemaining = mHost->getMaxForce() - magSoFar;

    if(magRemaining <= 0.f) return false;

    double magToAdd = magVec(forceToAdd);

    if(magToAdd < magRemaining)
    {
        runningTotal += forceToAdd;
    }
    else
    {
        runningTotal += normVec(forceToAdd) * magRemaining;
    }

    return true;
}

void SteeringBehaviour<Adventurer>::createFeelers()
{
    mFeelers.clear();

    mFeelers.push_back(sf::Vector2f(std::sin(-45.f * (mPI / 180.f)) * mFeelerLength
                                    , -std::cos(-45.f *(mPI / 180.f)) * mFeelerLength));

    mFeelers.push_back(sf::Vector2f(0.f
                                    , -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(45.f * (mPI / 180.f)) * mFeelerLength
                                    , -std::cos(45.f *(mPI / 180.f)) * mFeelerLength));
}

sf::Vector2f SteeringBehaviour<Adventurer>::rest()
{
    sf::Vector2f steeringForce;
    sf::Vector2f hostVel = mHost->getVelocity();

    steeringForce = -hostVel / 2.f;

    return steeringForce;
}

sf::Vector2f SteeringBehaviour<Adventurer>::arrive(sf::Vector2f targ, Deceleration deceleration)
{
    sf::Vector2f toTarget = targ - mHost->getWorldPosition();

    float dist = magVec(toTarget);

    sf::Vector2f steeringForce;

    if(dist > mMinArriveDist)
    {
        const float DecelerationTweaker = 0.3;

        float speed = dist / (static_cast<float>(deceleration) * DecelerationTweaker);

        speed = std::min(speed, mHost->getMaxSpeed());

        sf::Vector2f desiredVelocity = toTarget * speed / dist;

        steeringForce = (desiredVelocity - mHost->getVelocity());
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour<Adventurer>::seek(sf::Vector2f target)
{
    sf::Vector2f desiredVelocity = normVec(target - mHost->getWorldPosition()) * mHost->getMaxSpeed();

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour<Adventurer>::evade()
{
   //    const DynamicEntity* pursuer = mHost->getCurrentTarget();
   const DynamicEntity* pursuer = nullptr;

//    const Enemy* host = dynamic_cast<const Enemy*>(mHost);

//    if(!mHost)
//        throw std::runtime_error("Error:\n SteeringBehaviour::evade(): Casting Host* to Enemy*");

   if(pursuer)
   {
      sf::Vector2f toPursuer(pursuer->getWorldPosition() - mHost->getWorldPosition());

      float magPursuer = magVec(toPursuer);

      if((magPursuer * magPursuer) > mHost->panicDistance * mHost->panicDistance)
        return sf::Vector2f();

      float lookAheadTime = magPursuer / (mHost->getMaxSpeed() + pursuer->getSpeed());

      return flee(pursuer->getWorldPosition() + pursuer->getVelocity() * lookAheadTime);
   }
   else
   {
      return sf::Vector2f();
   }
}

sf::Vector2f SteeringBehaviour<Adventurer>::flee(sf::Vector2f target)
{
    sf::Vector2f desiredVelocity = normVec(mHost->getWorldPosition() - target) * mHost->getMaxSpeed();

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour<Adventurer>::wander(sf::Time dt)
{
    float jitterTimeSlice = mWanderJitter * dt.asSeconds();

    float jitter1 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;
    float jitter2 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;

    mWanderTarget += sf::Vector2f(jitter1
                                  , jitter2);

    mWanderTarget = normVec(mWanderTarget);
    mWanderTarget *= mWanderRadius;

    sf::Vector2f targetLocal = mWanderTarget + sf::Vector2f(0.f, -mWanderDistance);
    sf::Vector2f targetWorld = mHost->getWorldTransform() * targetLocal;

    sf::Vector2f returns(targetWorld - mHost->getWorldPosition());

    return returns;
}

sf::Vector2f SteeringBehaviour<Adventurer>::followPath()
{
    if(mPath.isActive())
    {
        sf::Vector2f toWaypoint = mHost->getPosition() - mPath.currentWaypoint();

        float mag = magVec(toWaypoint);
        float sqMag = mag * mag;
        float sqDistToWypnt = 10.f * 10.f;

        if(sqMag < sqDistToWypnt)
        {
            mHost->setVelocity(sf::Vector2f());

            if(!mPath.nextWaypoint())
                return rest();
        }

        if(!mPath.isEnd())
        {
            return seek(mPath.currentWaypoint());
        }
        else
        {
            return arrive(mPath.currentWaypoint(), Deceleration::Fast);
        }
    }
    else
    {
        return rest();
    }
}

sf::Vector2f SteeringBehaviour<Adventurer>::obstacleAvoidance()
{
    float boxLength = mMinViewBoxLength
                      + (mHost->getSpeed() / mHost->getMaxSpeed())
                      * mMinViewBoxLength;

    std::vector<LevelBlock*> nearObstacles = mHost->getBlockTypeInRange(LevelBlock::Type::ObstacleBlock
            , mHost->getRadius());

    LevelBlock* closestObstacle = nullptr;

    float distToClosest = MAXFLOAT;

    sf::Transform hostTrans = mHost->getInverseTransform();

    for(LevelBlock* blck : nearObstacles)
    {
        sf::Vector2f blckPos = hostTrans * blck->getMiddle();

        if(blckPos.y <= 0.f)
        {
            float expandedRadius = blck->getRadius() + mHost->getRadius();

            if(std::fabs(blckPos.x) < expandedRadius)
            {
                float sqrtPart = std::sqrt((expandedRadius * expandedRadius) - (blckPos.x * blckPos.x));
                float intersectionPoint = blckPos.y - sqrtPart;

                if(intersectionPoint <= 0.f)
                {
                    intersectionPoint = blckPos.y + sqrtPart;
                }

                if(intersectionPoint < distToClosest)
                {
                    distToClosest = intersectionPoint;
                    closestObstacle = blck;
                }
            }
        }
    }

    sf::Vector2f steeringForce;

    if(closestObstacle)
    {
        sf::Vector2f closestPos = hostTrans * closestObstacle->getMiddle();

        float multiplier = 1.f + (boxLength - closestPos.y) / boxLength;

        steeringForce.x = (closestObstacle->getRadius() - closestPos.x) * multiplier;

        const float brakingWeight = 0.02;

        steeringForce.y = (closestObstacle->getRadius() - closestPos.y) * brakingWeight;
    }

    sf::Vector2f worldForce = mHost->getWorldTransform() * steeringForce;

    return worldForce - mHost->getWorldPosition();
}

sf::Vector2f SteeringBehaviour<Adventurer>::wallAvoidance()
{
    createFeelers();

    float distToThisIntersection = 0.f;
    float distToClosestIntersection = MAXFLOAT;

    sf::Vector2f steeringForce, point, closestPoint, closestNorm;

    std::vector<LevelBlock*> wallBlocks = mHost->getBlockTypeInRange(LevelBlock::Type::WallBlock, mFeelerLength);

    for(size_t flr = 0; flr < Feelers::NumFlr; flr++)
    {
        int closestWall = -1;
        int index = 0;

        for(LevelBlock* block : wallBlocks)
        {
            Wall* wall = dynamic_cast<Wall*>(block->getScenery());

            assert(wall);

            Wall::WallData wallData = wall->getSceneryData();

            if(lineIntersection2D(mHost->getWorldPosition()
                                  , mHost->getWorldTransform() * mFeelers.at(flr)
                                  , wallData.first.first
                                  , wallData.first.second
                                  , distToThisIntersection
                                  , point))
            {

                if(distToThisIntersection < distToClosestIntersection)
                {
                    distToClosestIntersection = distToThisIntersection;
                    closestWall = index;
                    closestPoint = point;
                    closestNorm = wallData.second;
                }
            }

            index ++;
        }

        if(closestWall >= 0)
        {
            sf::Vector2f overShoot = (mHost->getWorldTransform() * mFeelers.at(flr)) - closestPoint;
            steeringForce = closestNorm * magVec(overShoot);
        }
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour<Adventurer>::seperation()
{
    sf::Vector2f steeringForce;

    std::vector<DynamicEntity*> neighbours = mHost->getNeighbours(mSeperationRadius
                                                                 , mHost->getEntityType());

    for(DynamicEntity* e : neighbours)
    {
        sf::Vector2f toNeighbour = mHost->getWorldPosition() - e->getWorldPosition();

        steeringForce += normVec(toNeighbour);
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour<Adventurer>::alignment()
{
    sf::Vector2f averageHeading;
    int neighbourCount = 0;

    std::vector<DynamicEntity*> neighbours = mHost->getNeighbours(mAlignRadius
                                                                 , mHost->getEntityType());

    for(DynamicEntity* e : neighbours)
    {
        if(e != mHost)
        {
            averageHeading += e->getWorldTransform() * e->getHeading();
            neighbourCount ++;
        }
    }

    if(neighbourCount > 0)
    {
        averageHeading /= static_cast<float>(neighbourCount);
        averageHeading -= mHost->getWorldTransform() * mHost->getHeading();
    }

    return averageHeading;
}

sf::Vector2f SteeringBehaviour<Adventurer>::cohesion()
{
    sf::Vector2f steeringForce, centerOfMass;
    int neighbourCount = 0;

    std::vector<DynamicEntity*> neighbours = mHost->getNeighbours(mCohesionRadius
                                                                 , mHost->getEntityType());

    for(DynamicEntity* e : neighbours)
    {
        if(e != mHost
                && e)
        {
            centerOfMass += e->getWorldPosition();
            neighbourCount ++;
        }
    }

    if(neighbourCount > 0)
    {
        centerOfMass /= static_cast<float>(neighbourCount);
        steeringForce = seek(centerOfMass);
    }

    return normVec(steeringForce);
}

sf::Vector2f SteeringBehaviour<Adventurer>::flocking()
{
    sf::Vector2f steeringForce;

    steeringForce += alignment() * mAlignmentMultiplier;
    steeringForce += cohesion() * mCohesionMultiplier;

    return steeringForce;
}

sf::Vector2f SteeringBehaviour<Adventurer>::calculate(sf::Time dt)
{
    sf::Vector2f steeringForce;

    if(mBehaviourFlags.at(WallAvoidance))
    {
        sf::Vector2f force = wallAvoidance() * mWallAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(ObstacleAvoidance))
    {
        sf::Vector2f force = obstacleAvoidance() * mObstacleAvoidanceMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Seperation))
    {
        sf::Vector2f force = seperation() * mSeperationMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Arrive))
    {
        sf::Vector2f force = arrive(mHost->getTargetPos(), Deceleration::Fast) * mArriveMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Evade))
    {
        sf::Vector2f force = evade() * mEvadeMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Flock))
    {
        sf::Vector2f force = flocking() * mFlockingMultiplier;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(FollowPath))
    {
        sf::Vector2f force = followPath() * 1.f;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Wander))
    {
        sf::Vector2f force = wander(dt) * mWanderMultiplier;
//        sf::Vector2f force;

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    if(mBehaviourFlags.at(Rest))
    {
        sf::Vector2f force = rest();

        if(!accumulateForce(steeringForce, force))
            return steeringForce;
    }

    return steeringForce;
}

void SteeringBehaviour<Adventurer>::setNewBehaviours(std::vector<SteeringBehaviour::Behaviour> newTypes)
{
    for(bool& b : mBehaviourFlags)
        b = false;

    for(SteeringBehaviour::Behaviour& bh : newTypes)
    {
        mBehaviourFlags.at(bh) = true;
    }

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
}

void SteeringBehaviour<Adventurer>::setNewBehaviours(SteeringBehaviour::Behaviour newType)
{
    for(bool& b : mBehaviourFlags)
        b = false;

    mBehaviourFlags.at(newType) = true;

    if(mHost->getEntityType() != DynamicEntity::EntityType::Character)
        mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;

    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
}

