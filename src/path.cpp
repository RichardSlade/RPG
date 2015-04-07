#include "incl/Path.hpp"

void Path::resetPath()
{
    mWypnts.clear();
    mWypntIndex = 0;
    mPathActive = false;
    mEndOfPath = false;
}

bool Path::nextWaypoint()
{
    mWypntIndex ++;

    if(mWypntIndex == mWypnts.size() - 2)
    {
        mEndOfPath = true;
        return true;
    }
    else if(mWypntIndex > mWypnts.size() - 1)
    {
        resetPath();
        return false;
    }
    else
    {
        mEndOfPath = false;
        return true;
    }
}

void Path::addToPath(sf::Vector2f pos)
{
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color::Red);

    sf::FloatRect bounds = circle.getLocalBounds();
    circle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    circle.setPosition(pos);

    mWypnts.push_back(circle);
}

void Path::startNewPath(sf::Vector2f pos)
{
    resetPath();
    addToPath(pos);

    mPathActive = true;
    mWypntIndex = 0;
}

std::vector<sf::CircleShape> Path::getPathToDraw() const
{
    std::vector<sf::CircleShape> toDraw;

    auto it = mWypnts.begin();
    it += mWypntIndex;

    toDraw.insert(toDraw.begin()
                  , it
                  , mWypnts.end());

    return toDraw;
}

