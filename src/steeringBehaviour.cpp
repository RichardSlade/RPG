#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "App/Utility.hpp"
#include "App/Params.hpp"
#include "World/Scenery.hpp"
//#include "World/WallBlock.hpp"
//#include "World/LevelBlock.hpp"
#include "Entity/Entity.hpp"
#include "Entity/SteeringBehaviour.hpp"
//#include "Entity/Entity.hpp"

//const float SteeringBehaviour::PI = 3.14159265358979;

SteeringBehaviour::SteeringBehaviour(Entity* host
                                     , const Params& params)
: mWanderRadius(params.WanderRadius)
, mWanderDistance(params.WanderDistance)
, mWanderJitter(params.WanderJitter)
, mMinViewBoxLength(params.MinViewBoxLength)
, mInteractionRadius(params.InteractionRadius)
, mFeelerLength(params.FeelerLength)
//, mFeelerLength(1.f)
, mMinArriveDist(params.MinArriveDist)
, mWallAvoidanceMultiplier(params.WallAvoidanceMultiplier)
, mObstacleAvoidanceMultiplier(params.ObstacleAvoidanceMultiplier)
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
//, mTheta(mHost->getRotation() * (PI / 180.f))
, mWanderTarget(std::sin(mTheta) * mWanderRadius, -std::cos(mTheta) * mWanderRadius){
   for(bool& b : mBehaviourFlags)
     b = false;

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;

//   if(mHost->getEntityType() != Entity::Type::Adventurer)
//      mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
};

bool SteeringBehaviour::accumulateForce(sf::Vector2f& runningTotal, sf::Vector2f forceToAdd){
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

void SteeringBehaviour::createFeelers(){
    mFeelers.clear();

    mFeelers.push_back(sf::Vector2f(std::sin(degreeToRadian(-90.f)) * mFeelerLength
                                      , std::cos(degreeToRadian(-90.f)) * -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(degreeToRadian(-45.f)) * mFeelerLength
                                      , std::cos(degreeToRadian(-45.f)) * -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(0.f
                                      , -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(degreeToRadian(45.f)) * mFeelerLength
                                      , std::cos(degreeToRadian(45.f)) * -mFeelerLength));

    mFeelers.push_back(sf::Vector2f(std::sin(degreeToRadian(90.f)) * mFeelerLength
                                      , std::cos(degreeToRadian(90.f)) * -mFeelerLength));
}

sf::Vector2f SteeringBehaviour::rest(){
   sf::Vector2f steeringForce, hostVel = mHost->getVelocity();

   steeringForce = -hostVel;

   return steeringForce;
}

sf::Vector2f SteeringBehaviour::face(){
   sf::Vector2f steeringForce;//, hostheading = mHost->getHeading();

   steeringForce = mHost->getCurrentTarget()->getWorldPosition() - mHost->getWorldPosition();

//   steeringForce = -hostVel;

   return steeringForce;
}

sf::Vector2f SteeringBehaviour::arrive(sf::Vector2f targ, Deceleration deceleration){
   sf::Vector2f toTarget = targ - mHost->getWorldPosition();

   float dist = magVec(toTarget);

   sf::Vector2f steeringForce;

   if(dist > mMinArriveDist)
   {
      const float DecelerationTweaker = 0.3;

      float speed = dist / (static_cast<float>(deceleration) * DecelerationTweaker);

      speed = std::min(speed, mHost->getMaxSpeed());

      sf::Vector2f desiredVelocity = toTarget;// * speed / dist;

//      steeringForce = (desiredVelocity - mHost->getVelocity());
      steeringForce = toTarget;
   }

   return steeringForce;
}

sf::Vector2f SteeringBehaviour::seek(sf::Vector2f target){
    sf::Vector2f desiredVelocity = normVec(target - mHost->getWorldPosition()) * mHost->getMaxSpeed();

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour::evade(){
    const Entity* pursuer = mHost->getCurrentTarget();

//    const Entity* mHost = dynamic_cast<const Entity*>(mHost);

    if(!mHost)
        throw std::runtime_error("Error:\n SteeringBehaviour::evade(): Casting Host* to Entity*");

    sf::Vector2f toPursuer(pursuer->getWorldPosition() - mHost->getWorldPosition());

    float magPursuer = magVec(toPursuer);

    if((magPursuer * magPursuer) > mHost->PanicDistance * mHost->PanicDistance)
        return sf::Vector2f();

    float lookAheadTime = magPursuer / (mHost->getMaxSpeed() + pursuer->getSpeed());

    return flee(pursuer->getWorldPosition() + pursuer->getVelocity() * lookAheadTime);
}

sf::Vector2f SteeringBehaviour::flee(sf::Vector2f target){
    sf::Vector2f desiredVelocity = normVec(mHost->getWorldPosition() - target) * mHost->getMaxSpeed();

    return (desiredVelocity - mHost->getVelocity());
}

sf::Vector2f SteeringBehaviour::wander(sf::Time dt){
  float jitterTimeSlice = mWanderJitter * dt.asSeconds();

  float jitter1 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;
  float jitter2 = rangedClamped(-1.f, 1.f) * jitterTimeSlice;

  mWanderTarget += sf::Vector2f(jitter1
                                , jitter2);

//   mWanderTarget += sf::Vector2f(0.f
//                                  , 0.f);

  mWanderTarget = normVec(mWanderTarget);
  mWanderTarget *= mWanderRadius;
//  mWanderTarget.y *= -1.f;


//  sf::Vector2f targetLocal = mWanderTarget;// + sf::Vector2f(0.f, -mWanderDistance);
  sf::Vector2f targetLocal = mWanderTarget + sf::Vector2f(0.f, -mWanderDistance);
//  sf::Vector2f targetLocal(0.f, 0.f);// + sf::Vector2f(0.f, -mWanderDistance);
//  sf::Vector2f targetLocal = sf::Vector2f(0.f, -mWanderDistance);
//  sf::Vector2f targetLocal = sf::Vector2f(1.f, -1.f);
//  sf::Vector2f targetWorld = mHost->getWorldTransform() * targetLocal;
  sf::Vector2f targetWorld = mHost->getWorldTransform().transformPoint(targetLocal);

//    sf::Vector2f returnVec(targetWorld - mHost->getWorldPosition());

//  mHost->setWanderTargetPosition(sf::Vector2f(0.f, -mWanderDistance));
//  mHost->setWanderTargetPosition(meterToPixel(targetWorld));
//  mHost->setWanderTargetPosition(meterToPixel(targetWorld));
  mHost->setWanderTargetPosition(meterToPixel(targetWorld));

  return sf::Vector2f(targetWorld - mHost->getWorldPosition());
//  return sf::Vector2f();
}

sf::Vector2f SteeringBehaviour::followPath(){
    if(mPath.isActive())
    {
        sf::Vector2f toWaypoint = mHost->getPosition() - mPath.currentWaypoint();

        float mag = magVec(toWaypoint);
        float sqMag = mag * mag;
        float sqDistToWypnt = 20.f * 20.f;

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

sf::Vector2f SteeringBehaviour::obstacleAvoidance(){
  float boxLength = mMinViewBoxLength
                + (mHost->getSpeed() / mHost->getMaxSpeed())
                * mMinViewBoxLength;

  std::list<Scenery*> obstacles;
  mHost->getObstacles(obstacles,
                    Scenery::Type::Obstacle);

//std::cout << obstacles.size() << std::endl;

  Scenery* closestObstacle = nullptr;

  float distToClosest = MAXFLOAT;

  //    sf::Transform hostTrans = mHost->getInverseTransform();
  sf::Transform hostTrans = mHost->getWorldTransform().getInverse();

  std::list<Scenery*>::iterator iter;

  for(iter = obstacles.begin();
    iter != obstacles.end();
    iter++)
  {
    sf::Vector2f obsPos = hostTrans * (*iter)->getWorldPosition();

    if(obsPos.y <= 0.f)
    {
      float expandedRadius = (*iter)->getBodyRadius() + mHost->getBodyRadius();

      if(std::fabs(obsPos.x) < expandedRadius)
      {
        float sqrtPart = std::sqrt((expandedRadius * expandedRadius) - (obsPos.x * obsPos.x));
        float intersectionPoint = obsPos.y - sqrtPart;

        if(intersectionPoint <= 0.f)
          intersectionPoint = obsPos.y + sqrtPart;


        if(intersectionPoint < distToClosest)
        {
          distToClosest = intersectionPoint;
          closestObstacle = *iter;
        }
      }
    }
  }

  sf::Vector2f steeringForce;

  if(closestObstacle)
  {
    sf::Vector2f closestPos = hostTrans * closestObstacle->getWorldPosition();

    float multiplier = 1.f + (boxLength - closestPos.y) / boxLength;

    steeringForce.x = (closestObstacle->getBodyRadius() - closestPos.x) * multiplier;

    const float brakingWeight = 0.02;

    steeringForce.y = (closestObstacle->getBodyRadius() - closestPos.y) * brakingWeight;

//    std::cout << "steering: " << steeringForce.x << ", " << steeringForce.y << std::endl;
  }

  sf::Vector2f worldForce = mHost->getWorldTransform().transformPoint(steeringForce);

  return worldForce - mHost->getWorldPosition();
}

sf::Vector2f SteeringBehaviour::wallAvoidance(){
    createFeelers();
    sf::Vector2f steeringForce, point, closestPoint, closestNorm;

    std::list<Scenery*> walls;
    mHost->getObstacles(walls, Scenery::Type::Wall);

    sf::Vector2f closestFeeler, closestIntersection;
    float closestFraction = 1.f;
    bool intersectionOccured = false;

    for(size_t flr = 0; flr < SteeringBehaviour::Feelers::NumFlr; flr++)
    {
        std::list<Scenery*>::iterator itrScenery;

        for(itrScenery = walls.begin();
            itrScenery != walls.end();
            itrScenery++)
        {
            const b2Fixture* fixture = (*itrScenery)->getFixtures();

            sf::Vector2f p1(mHost->getWorldPosition());
            sf::Vector2f p2(mHost->getWorldTransform() * mFeelers.at(flr));

            b2RayCastInput input;
            input.p1 = convertVec(p1);
            input.p2 = convertVec(p2);
            input.maxFraction = 1.f;

            b2RayCastOutput output;

            if(!fixture->RayCast(&output, input, 0))
              continue;
            if(output.fraction < closestFraction)
            {
              closestFraction = output.fraction;
              closestFeeler = p2;
              closestIntersection = p1 + closestFraction * (p2 - p1);
              closestNorm = convertVec(output.normal);
              intersectionOccured = true;
            }
        }

        if(intersectionOccured)
        {
//          std::cout << "Intersection occured " << closestNorm.x << "," << closestNorm.y << std::endl;

            sf::Vector2f overShoot = closestFeeler - closestIntersection;
            steeringForce = closestNorm * magVec(overShoot);
        }
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::seperation(){
    sf::Vector2f steeringForce;

    std::list<Entity*> neighbours;
     mHost->getNeighbours(neighbours
                          , Entity::Type::AllTypes);

    for(Entity* e : neighbours)
    {
        sf::Vector2f toNeighbour = mHost->getWorldPosition() - e->getWorldPosition();
        steeringForce += normVec(toNeighbour);
    }

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::alignment(){
    sf::Vector2f averageHeading;
    int neighbourCount = 0;

    std::list<Entity*> neighbours;
    mHost->getNeighbours(neighbours
                         , mHost->getEntityType());

    for(Entity* e : neighbours)
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

sf::Vector2f SteeringBehaviour::cohesion(){
    sf::Vector2f steeringForce, centerOfMass;
    int neighbourCount = 0;

    std::list<Entity*> neighbours;
    mHost->getNeighbours(neighbours
                         , mHost->getEntityType());

    for(Entity* e : neighbours)
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

sf::Vector2f SteeringBehaviour::flocking(){
    sf::Vector2f steeringForce;

    steeringForce += alignment() * mAlignmentMultiplier;
    steeringForce += cohesion() * mCohesionMultiplier;

    return steeringForce;
}

sf::Vector2f SteeringBehaviour::calculate(sf::Time dt){
  sf::Vector2f steeringForce;

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Rest))
  {
    sf::Vector2f force = rest();

    if(!accumulateForce(steeringForce, force))
        return steeringForce;

//    std::cout << "Resting" << std::endl;
//    return -mHost->getBodyVelocity();
//    return -mHost->getBodyVelocity();
//    return sf::Vector2f();
//    return -steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::WallAvoidance))
  {
      sf::Vector2f force = wallAvoidance() * mWallAvoidanceMultiplier;

      if(!accumulateForce(steeringForce, force))
          return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::ObstacleAvoidance))
  {
    sf::Vector2f force = obstacleAvoidance() * mObstacleAvoidanceMultiplier;

    if(!accumulateForce(steeringForce, force))
      return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Seperation))
  {
    sf::Vector2f force = seperation() * mSeperationMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Arrive))
  {
    sf::Vector2f force = arrive(mHost->getCurrentTarget()->getWorldPosition(), Deceleration::Fast) * mArriveMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

 if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Seek))
  {
    sf::Vector2f force = seek(mHost->getCurrentTarget()->getWorldPosition()) * mArriveMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Evade))
  {
    sf::Vector2f force = evade() * mEvadeMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Flock))
  {
    sf::Vector2f force = flocking() * mFlockingMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::FollowPath))
  {
    sf::Vector2f force = followPath() * 1.f;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Wander))
  {
    sf::Vector2f force = wander(dt) * mWanderMultiplier;

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  if(mBehaviourFlags.at(SteeringBehaviour::Behaviour::Face))
  {
    sf::Vector2f force = face();

    if(!accumulateForce(steeringForce, force))
        return steeringForce;
  }

  return steeringForce;
}

void SteeringBehaviour::setNewBehaviours(std::vector<SteeringBehaviour::Behaviour> newTypes){
    for(bool& b : mBehaviourFlags)
        b = false;

    for(SteeringBehaviour::Behaviour& bh : newTypes)
        mBehaviourFlags.at(bh) = true;

    mBehaviourFlags.at(SteeringBehaviour::ObstacleAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
    mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
}

//void SteeringBehaviour::setNewBehaviours(SteeringBehaviour::Behaviour newType){
//    for(bool& b : mBehaviourFlags)
//        b = false;
//
//    mBehaviourFlags.at(newType) = true;
//
//   if(mHost->getEntityType() != Entity::Type::Adventurer)
//      mBehaviourFlags.at(SteeringBehaviour::Seperation) = true;
//
//   mBehaviourFlags.at(SteeringBehaviour::WallAvoidance) = true;
//}

