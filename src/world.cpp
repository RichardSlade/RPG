#include <iostream>
#include <algorithm>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "App/GameState.hpp"
#include "App/Controller.hpp"
#include "App/HUD.hpp"
#include "App/Utility.hpp"
#include "World/World.hpp"
#include "World/LevelBlock.hpp"
//#include "Node/SpriteNode.hpp"
//#include "Node/Scenery.hpp"
//#include "Node/SpriteNode.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/Adventurer.hpp"
//#include "Entity/Goblin.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "Entity/State/EnemyStates.hpp"

//template<typename Target>
//struct CharacterToTarget
//{
//public:
//  Target*       operator()( Adventurer* value )
//                { return dynamic_cast<Target*>(value); }
//};

const sf::Time World::mComboTime = sf::seconds(1.f);

World::World(GameState& gameState
             , const Controller& controller
             , sf::RenderWindow& window
             , std::string username
             , int worldDim
             , int numEnemy
             , sf::Time levelTime)
: mWorldX(worldDim)
, mWorldY(worldDim)
, mExitRadius(controller.getParams().ExitRadius)
, mLevelBlockSize(controller.getParams().LevelBlockSize)
, mNumEnemy(numEnemy)
, mWaypointRadius(controller.getParams().WaypointRadius)
, mScrollSpeed(controller.getParams().ScrollSpeed)
, mGameState(gameState)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
, mWorldBounds(sf::Vector2i(0, 0), sf::Vector2i(mWorldX, mWorldY))
, mFocusPoint(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f)
, mTimeLeft(levelTime)
, mTimeTaken(sf::Time::Zero)
, mHUD(this
			 , controller.getFont(Controller::Fonts::Sansation)
			 , getViewBounds()
			 , username)
, mExitPos(mWorldBounds.width / 2.f, 0.f)
, mCurrentAdventurer(nullptr)
, mCurrentAdventurerIndex(0)
{
	mLevel = std::unique_ptr<Level>(new Level(mLevelBlockSize
																	, controller.getParams().ExitWidth
																	, mWorldBounds));

	initialiseStatesAndStats();
	buildScene(controller);
}

void World::initialiseStatesAndStats()
{
//    std::vector<MovingEntity*> chars = dynamic_cast<std::vector<MovingEntity*>&>(mCharacters);
//    std::vector<MovingEntity*> chars = mCharacters;

//  std::vector<MovingEntity*> chars;

//  std::transform(mCharacters.begin()
//               , mCharacters.end()
//               , chars.begin()
//               , CharacterToTarget<MovingEntity>());

   // Initialise enemy states
   mEnemyStates.push_back(std::unique_ptr<EnemyStates::LookOut>(new EnemyStates::LookOut));
   mEnemyStates.push_back(std::unique_ptr<EnemyStates::Relax>(new EnemyStates::Relax));
   mEnemyStates.push_back(std::unique_ptr<EnemyStates::Evade>(new EnemyStates::Evade));
   mEnemyStates.push_back(std::unique_ptr<EnemyStates::Attack>(new EnemyStates::Attack));
   //    mEnemyStates.push_back(std::unique_ptr<Exit>(new Exit));

   mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::LookOut>(new AdventurerStates::LookOut));
   mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::Relax>(new AdventurerStates::Relax));
   mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::Attack>(new AdventurerStates::Attack));

//    mCharacterStates.push_back(std::unique_ptr<Exit>(new Exit));

    mEntityStats.push_back(EntityStats("data/entityStats/goblinStats.dat"));
    mEntityStats.push_back(EntityStats("data/entityStats/characterStats.dat"));
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

    mLevel->generateLevel(mSceneLayers, controller);

    generateAgents(controller);
}

void World::generateAgents(const Controller& controller)
{
    // Initialise characters and add to scene graph
    for(int i = 0; i < 2; i++)
    {
        float inc = i * 40.f;

        std::unique_ptr<Adventurer> characterNode(new Adventurer(Adventurer::Type::Barbarian
                                                                , controller.getTexture(Controller::Textures::Adventurer)
                                                                , controller.getFont(Controller::Fonts::Sansation)
                                                                , sf::Vector2f((mWorldBounds.width / 2.f) + inc, (mWorldBounds.height / 2.f) + inc)
                                                                , rangedClamped(0.75f, 1.25f)
                                                                , mEntityStats.at(StatsType::AdventurerStats)
                                                                , controller.getParams()
                                                                , mLevel.get()
                                                                , mAdventurerStates.at(Adventurer::StateType::LookOut).get()
                                                                , mAdventurerStates.at(Adventurer::StateType::Relax).get()
                                                                , mAdventurerStates
                                                                , Adventurer::StateType::Relax));

        // Save pointer to character for enemy initialisation
        Adventurer* characterPtr = characterNode.get();
        mAdventurers.push_back(characterPtr);

        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(characterNode));
    }

    mCurrentAdventurer = mAdventurers.at(mCurrentAdventurerIndex);

    // Initialise enemy and add to scene graph
//    for(int i = 0 ; i < mNumEnemy; i++)
    for(int i = 0 ; i < 10; i++)
    {
        // Find square for enemy to start in
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

        int maxX = mLevel->getLevelX();

        index = sf::Vector2i(rangedClamped(1, maxX - 2)
                             , rangedClamped(1, maxX - 2));

        levelBlock = mLevel->getBlock(index);

        pos = levelBlock->getMiddle();

        std::unique_ptr<Enemy> enemyNode(new Enemy(Enemy::Type::Goblin
                                                  , controller.getTexture(Controller::Textures::Enemy)
                                                  , controller.getFont(Controller::Fonts::Sansation)
                                                  , pos
                                                  , rangedClamped(0.75f, 1.25f)
                                                  , mEntityStats.at(StatsType::EnemyStats)
                                                  , controller.getParams()
                                                  , mLevel.get()
                                                  , mEnemyStates.at(Enemy::StateType::LookOut).get()
                                                  , mEnemyStates.at(Enemy::StateType::Relax).get()
                                                  , mEnemyStates
                                                  , Enemy::StateType::Relax));

//        enemyNode->setMovingTarget(mCharacters.at(0));
        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(enemyNode));
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

//    if(mEnemyHerded == mNumEnemy)
//        mGameState.levelComplete();
//    else if(mTimeLeft < sf::Time::Zero)
//        mGameState.gameComplete(mEnemyHerded);
}

void World::handleInput()
{
    sf::Event event;

    bool characterChange = false;

    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Escape)
                mWindow.close();
            else if(event.key.code == sf::Keyboard::Tab)
            {
               changeAdventurer();
               characterChange = true;
            }
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
                  mCurrentAdventurer->addToPath(mousePosF);
                }
                else
                {
                  mCurrentAdventurer->startNewPath(mousePosF);
                }
            }
        }
    }

    handleRealTimeInput();

    if(characterChange)
      mFocusPoint = mCurrentAdventurer->getWorldPosition();
}

void World::changeAdventurer()
{
   Adventurer* curChar = nullptr;

   do
   {
      mCurrentAdventurerIndex ++;

      if(mCurrentAdventurerIndex > mAdventurers.size() - 1)
         mCurrentAdventurerIndex = 0;

      curChar = mAdventurers.at(mCurrentAdventurerIndex);

   } while(!curChar);

   mCurrentAdventurer = curChar;
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

//std::vector<LevelBlock*> World::getBlockTypeInRange(const MovingEntity* entity
//        , LevelBlock::Type blockType
//        , float radius = 0.f) const
//{
//    return mLevel->getBlockTypeInRange(entity
//                                       , radius
//                                       , blockType);
//}
//
//LevelBlock* World::insertEntityIntoLevel(MovingEntity* entity) const
//{
//    return mLevel->insertEntityIntoLevel(entity);
//}

//std::vector<MovingEntity*> World::getEntitiesInRange(const MovingEntity* entity
//        , float neighbourhoodRadius) const
//{
//    return mLevel->getEntitiesInRange(entity
//                                      , neighbourhoodRadius);
//}

