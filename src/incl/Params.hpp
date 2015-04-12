#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <fstream>
#include <string>
#include <stdexcept>
#include <iostream>

#include <SFML/Graphics/Color.hpp>

struct Params
{
public:
    Params()
    {
        std::string fileName("data/params/params.txt");

        std::ifstream in(fileName);

        if(!in.is_open())
            throw std::runtime_error("Error loading: " + fileName);

        in >> *this;

        in.close();
    };

    float WindowX;
    float WindowY;
    float LevelBlockSize;
    float WorldDimMin;
    float WorldDimMax;
    float NumEnemyMin;
    float NumEnemyMax;
    float LevelTimeMin;
    float LevelTimeMax;
    float WaypointRadius;
    float ScrollSpeed;
    float EnemySightRange;
    float EnemyAngleOfVision;
    float EnemyPanicDistance;
    float CharacterPanicDistance;
    float PI;
    float WanderRadius;
    float WanderDistance;
    float WanderJitter;
    float InteractionRadius;
    float FeelerLength;
    float MinArriveDist;
    float WallAvoidanceMultiplier;
    float ArriveMultiplier;
    float EvadeMultiplier;
    float WanderMultiplier;
    float SeperationMultiplier;
    float AlignmentMultiplier;
    float CohesionMultiplier;
    float FlockingMultiplier;
    float MinViewBoxLength;
    float ExitRadius;
    float ExitWidth;
    float SeperationRadius;
    float AlignRadius;
    float CohesionRadius;

    friend void operator>>(std::ifstream &in, Params &params)
    {
        in.ignore(255, ' ');
        in >> params.WindowX;

        in.ignore(255, ' ');
        in >> params.WindowY;

        in.ignore(255, ' ');
        in >> params.LevelBlockSize;

        in.ignore(255, ' ');
        in >> params.WorldDimMin;

        in.ignore(255, ' ');
        in >> params.WorldDimMax;

        in.ignore(255, ' ');
        in >> params.NumEnemyMin;

        in.ignore(255, ' ');
        in >> params.NumEnemyMax;

        in.ignore(255, ' ');
        in >> params.LevelTimeMin;

        in.ignore(255, ' ');
        in >> params.LevelTimeMax;

        in.ignore(255, ' ');
        in >> params.WaypointRadius;

        in.ignore(255, ' ');
        in >> params.ScrollSpeed;

        in.ignore(255, ' ');
        in >> params.EnemySightRange;

        in.ignore(255, ' ');
        in >> params.EnemyAngleOfVision;

        in.ignore(255, ' ');
        in >> params.EnemyPanicDistance;

        in.ignore(255, ' ');
        in >> params.CharacterPanicDistance;

        in.ignore(255, ' ');
        in >> params.PI;

        in.ignore(255, ' ');
        in >> params.WanderRadius;

        in.ignore(255, ' ');
        in >> params.WanderDistance;

        in.ignore(255, ' ');
        in >> params.WanderJitter;

        in.ignore(255, ' ');
        in >> params.InteractionRadius;

        in.ignore(255, ' ');
        in >> params.FeelerLength;

        in.ignore(255, ' ');
        in >> params.MinArriveDist;

        in.ignore(255, ' ');
        in >> params.WallAvoidanceMultiplier;

        in.ignore(255, ' ');
        in >> params.ArriveMultiplier;

        in.ignore(255, ' ');
        in >> params.EvadeMultiplier;

        in.ignore(255, ' ');
        in >> params.WanderMultiplier;

        in.ignore(255, ' ');
        in >> params.SeperationMultiplier;

        in.ignore(255, ' ');
        in >> params.AlignmentMultiplier;

        in.ignore(255, ' ');
        in >> params.CohesionMultiplier;

        in.ignore(255, ' ');
        in >> params.FlockingMultiplier;

        in.ignore(255, ' ');
        in >> params.MinViewBoxLength;

        in.ignore(255, ' ');
        in >> params.ExitRadius;

        in.ignore(255, ' ');
        in >> params.ExitWidth;

        in.ignore(255, ' ');
        in >> params.SeperationRadius;

        in.ignore(255, ' ');
        in >> params.AlignRadius;

        in.ignore(255, ' ');
        in >> params.CohesionRadius;
    }

};

#endif // PARAMS_HPP
