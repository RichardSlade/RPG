#ifndef PATH_HPP
#define PATH_HPP

#include <SFML/Graphics/CircleShape.hpp>

class Path
{
private:
    std::vector<sf::CircleShape>        mWypnts;
    unsigned int                        mWypntIndex;

    bool                                mPathActive;
    bool                                mEndOfPath;

    void                                resetPath();

public:
                                        Path()
                                        : mWypntIndex(0)
                                        , mPathActive(false)
                                        , mEndOfPath(false)
                                        {};

    bool                                nextWaypoint();
    void                                addToPath(sf::Vector2f);
    void                                startNewPath(sf::Vector2f);

    // Getters
    sf::Vector2f                        currentWaypoint()
                                        { return mWypnts.at(mWypntIndex).getPosition();}

    bool                                isActive()
                                        { return mPathActive; }

    bool                                isEnd()
                                        { return mEndOfPath; }

    std::vector<sf::CircleShape>        getPathToDraw() const;
};

#endif // PATH_HPP
