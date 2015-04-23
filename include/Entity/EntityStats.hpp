#ifndef ENTITYSTATS_HPP
#define ENTITYSTATS_HPP

/*
*   EntityStats.hpp
*
*   Class holding parameters of different entity types in game
*   one for each type
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <stdexcept>
#include <fstream>
#include <string>

struct EntityStats
{
public:
   float       mass;
   float       walkMaxSpeed;
   float       runMaxSpeed;
   float       maxForce;
   float       maxTurnRate;
   float       health;
   float       meleeDamage;
   float       panicDist;
   float       agroDist;
   float       attackDist;
   float       attackDelay;

    EntityStats(std::string fileName)
    {
        std::ifstream in(fileName);

        if(!in.is_open())
            throw std::runtime_error("ERROR: Could not open file" + fileName);

        in >> *this;
        in.close();
    }

    // Extractor
    friend void operator>>(std::ifstream &in, EntityStats &stats)
    {
        in.ignore(255, ' ');
        in >> stats.mass;

        in.ignore(255, ' ');
        in >> stats.health;

        in.ignore(255, ' ');
        in >> stats.meleeDamage;

        in.ignore(255, ' ');
        in >> stats.walkMaxSpeed;

        in.ignore(255, ' ');
        in >> stats.runMaxSpeed;

        in.ignore(255, ' ');
        in >> stats.maxForce;

        in.ignore(255, ' ');
        in >> stats.maxTurnRate;

        in.ignore(255, ' ');
        in >> stats.panicDist;

        in.ignore(255, ' ');
        in >> stats.agroDist;

        in.ignore(255, ' ');
        in >> stats.attackDist;

        in.ignore(255, ' ');
        in >> stats.attackDelay;
    }

};

#endif // ENTITYSTATS_HPP
