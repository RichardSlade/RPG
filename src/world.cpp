#include <iostream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "incl/World.hpp"
#include "incl/Controller.hpp"
#include "incl/SpriteNode.hpp"
#include "incl/Utility.hpp"
#include "incl/LevelBlock.hpp"
#include "incl/Dog.hpp"
#include "incl/Sheep.hpp"
#include "incl/Scenery.hpp"
#include "incl/SheepStates.hpp"
#include "incl/HUD.hpp"
#include "incl/GameState.hpp"
#include "incl/SpriteNode.hpp"

const sf::Time World::mComboTime = sf::seconds(1.f);

World::World(GameState& gameState
             , const Controller& controller
             , sf::RenderWindow& window
             , std::string username
             , int worldDim
             , int numSheep
             , sf::Time levelTime)
: mWorldX(worldDim)
, mWorldY(worldDim)
, mExitRadius(controller.getParams().ExitRadius)
, mLevelBlockSize(controller.getParams().LevelBlockSize)
, mNumSheep(numSheep)
, mWaypointRadius(controller.getParams().WaypointRadius)
, mScrollSpeed(controller.getParams().ScrollSpeed)
, mGameState(gameState)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
, mWorldBounds(sf::Vector2i(0, 0), sf::Vector2i(mWorldX, mWorldY))
, mFocusPoint(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
, mSheepHerded(0)
, mTimeLeft(levelTime)
, mTimeTaken(sf::Time::Zero)
, mLevel(mLevelBlockSize
         , controller.getParams().ExitWidth
         , mWorldBounds)
, mHUD(this
       , controller.getFont(Controller::Fonts::Sansation)
       , getViewBounds()
       , username)
, mExitPos(mWorldBounds.width / 2.f, 0.f)
, mDog(nullptr)
{
    initialiseStatesAndStats();
    buildScene(controller);
}

void World::initialiseStatesAndStats()
{
    // Initialise sheep states
    mSheepStates.push_back(std::unique_ptr<LookOutForDog>(new LookOutForDog));
    mSheepStates.push_back(std::unique_ptr<EvadeDog>(new EvadeDog));
    mSheepStates.push_back(std::unique_ptr<Relax>(new Relax));
    mSheepStates.push_back(std::unique_ptr<Exit>(new Exit));

    mEntityStats.push_back(EntityStats("data/entityStats/sheepStats.dat"));
    mEntityStats.push_back(EntityStats("data/entityStats/dogStats.dat"));
}

void World::buildScene(const Controller& controller)
{
    // Initialize the different scene graph layers
	for(std::size_t i = 0; i < SceneNode::Layers::Num; i++)
	{
		SceneNode::upScNode layer(new SceneNode());
		mSceneLayers.at(i) = layer.get();

		mSceneGraph.addChild(std::move(layer));
	}

    sf::Vector2f bckgrndSpritePos(mWorldBounds.width / 2.f
                                  , mWorldBounds.height / 2.f);

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(controller.getTexture(Controller::Textures::GameBackground)
                                                                 , bckgrndSpritePos
                                                                 , true));

    mBackground = backgroundSprite.get();
    mSceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(backgroundSprite));

    mLevel.generateLevel(mSceneLayers, controller);

    generateAgents(controller);
}

void World::generateAgents(const Controller& controller)
{
    // Initialise dog and add to scene graph
//    std::unique_ptr<Dog> dogNode(new Dog(const_cast<World*>(this)
//                                         , controller.getTexture(Controller::Textures::Dog)
//                                         , controller.getFont(Controller::Fonts::Sansation)
//                                         , sf::Vector2f(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
//                                         , mEntityStats.at(StatsType::DogStats)
//                                         , controller.getParams()));

    // Save pointer to dog for sheep initialisation
//    Dog* dogPtr = dogNode.get();
//    mDog = dogPtr;

//    mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(dogNode));

    // Initialise sheep and add to scene graph
//    for(int i = 0 ; i < mNumSheep; i++)
    for(int i = 0 ; i < 0; i++)
    {
        // Find square for sheep to start in
        LevelBlock* levelBlock;
        sf::Vector2i index;
        sf::Vector2f pos;

//        do
//        {
////            sf::Vector2f pos(rangedClamped(mLevelBlockSize, mWorldBounds.width - mLevelBlockSize)
////                             , rangedClamped(mLevelBlockSize, mWorldBounds.width - mLevelBlockSize));
//
//            index = sf::Vector2i(rangedClamped(1, 10)
//                                 , rangedClamped(1, 10));
//
//            levelBlock = mLevel.getBlock(index);
//
//        } while(levelBlock->getType() == LevelBlock::Type::ObstacleBlock);

        int maxX = mLevel.getLevelX();

        index = sf::Vector2i(rangedClamped(1, maxX - 2)
                             , rangedClamped(1, maxX - 2));

        levelBlock = mLevel.getBlock(index);

        pos = levelBlock->getMiddle();

//        std::unique_ptr<Sheep> sheepNode(new Sheep(const_cast<World*>(this)
//                                                    , controller.getTexture(Controller::Textures::Sheep)
//                                                    , controller.getFont(Controller::Fonts::Sansation)
//                                                    , pos
//                                                    , mSheepStates.at(Sheep::States::LookOut).get()
//                                                    , mSheepStates.at(Sheep::States::Relax).get()
//                                                    , mEntityStats.at(StatsType::SheepStats)
//                                                    , controller.getParams()
//                                                    , rangedClamped(0.75f, 1.25f)));

//        sheepNode->setMovingTarget(dogPtr);
//        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(sheepNode));
    }
}

void World::handleRealTimeInput()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)
       || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        mFocusPoint.x -= mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)
       || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mFocusPoint.x += mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)
       || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        mFocusPoint.y -= mScrollSpeed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)
       || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        mFocusPoint.y += mScrollSpeed;
    }
}

void World::adjustView()
{
    sf::View view = mWindow.getView();
    view.setCenter(mFocusPoint);

    sf::FloatRect bckgrndBounds = mBackground->getGlobalBounds();

    sf::Vector2f center = view.getCenter();
    sf::Vector2f dim = view.getSize();
    dim /= 2.f;

    if(center.x - dim.x < bckgrndBounds.left)
        mFocusPoint.x += mScrollSpeed;
    else if(center.x + dim.x > bckgrndBounds.left + bckgrndBounds.width)
        mFocusPoint.x -= mScrollSpeed;

    if(center.y - dim.y < bckgrndBounds.top)
        mFocusPoint.y += mScrollSpeed;
    else if(center.y + dim.y > bckgrndBounds.top + bckgrndBounds.height)
        mFocusPoint.y -= mScrollSpeed;
}

void World::update(sf::Time dt)
{
    mSceneGraph.removeDeletedNodes();
    mSceneGraph.update(dt);

    mTimeLeft -= dt;
    mTimeTaken += dt;

    mHUD.update();

    if(mSheepHerded == mNumSheep)
        mGameState.levelComplete();
    else if(mTimeLeft < sf::Time::Zero)
        mGameState.gameComplete(mSheepHerded);
}

void World::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Escape)
                mGameState.pause();
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos;
            sf::FloatRect vBounds = getViewBounds();

            mousePos.x = sf::Mouse::getPosition(mWindow).x + vBounds.left;
            mousePos.y = sf::Mouse::getPosition(mWindow).y + vBounds.top;

            sf::Vector2f mousePosF(mousePos.x, mousePos.y);

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                mousePosF.x = std::min(mousePosF.x, static_cast<float>(mWorldBounds.width - (mLevelBlockSize + mWaypointRadius)));
                mousePosF.x = std::max(mousePosF.x, static_cast<float>(mLevelBlockSize + mWaypointRadius));

                mousePosF.y = std::min(mousePosF.y, static_cast<float>(mWorldBounds.height - (mLevelBlockSize + mWaypointRadius)));
                mousePosF.y = std::max(mousePosF.y, static_cast<float>(mLevelBlockSize + mWaypointRadius));

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    if(mDog)
                        mDog->addToPath(mousePosF);
                }
                else
                {
                    if(mDog)
                        mDog->startNewPath(mousePosF);
                }
            }
        }
    }

    handleRealTimeInput();
}

void World::display()
{
    adjustView();

    mWorldView.setCenter(mFocusPoint);
    mWindow.setView(mWorldView);

    mWindow.draw(mSceneGraph);

    mHUD.setHUDPosition(getViewBounds());
    mWindow.draw(mHUD);
}

const sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

std::vector<LevelBlock*> World::getBlockTypeInRange(const MovingEntity* entity
                                                    , LevelBlock::Type blockType
                                                    , float radius = 0.f) const
{
    return mLevel.getBlockTypeInRange(entity
                                           , radius
                                           , blockType);
}

LevelBlock* World::insertEntityIntoLevel(MovingEntity* entity) const
{
    return mLevel.insertEntityIntoLevel(entity);
}

std::vector<MovingEntity*> World::getEntitiesInRange(const MovingEntity* entity
                                                     , float neighbourhoodRadius) const
{
    return mLevel.getEntitiesInRange(entity
                                     , neighbourhoodRadius);
}

