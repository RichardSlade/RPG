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
#include "Entity/Adventurer.hpp"
#include "Entity/Goblin.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "Entity/State/GoblinStates.hpp"

//template<typename Target>
//struct CharacterToTarget
//{
//public:
//  Target*       operator()( Character* value )
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
, mCurrentCharacter(nullptr)
, mCurrentCharacterIndex(0)
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
   mGoblinStates.push_back(std::unique_ptr<GoblinStates::LookOut>(new GoblinStates::LookOut));
   mGoblinStates.push_back(std::unique_ptr<GoblinStates::Relax>(new GoblinStates::Relax));
   mGoblinStates.push_back(std::unique_ptr<GoblinStates::Evade>(new GoblinStates::Evade));
   mGoblinStates.push_back(std::unique_ptr<GoblinStates::Attack>(new GoblinStates::Attack));
   //    mEnemyStates.push_back(std::unique_ptr<Exit>(new Exit));

   mCharacterStates.push_back(std::unique_ptr<CharacterStates::LookOut>(new CharacterStates::LookOut));
   mCharacterStates.push_back(std::unique_ptr<CharacterStates::Relax>(new CharacterStates::Relax));
   mCharacterStates.push_back(std::unique_ptr<CharacterStates::Attack>(new CharacterStates::Attack));

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

        std::unique_ptr<Character> characterNode(new Character(mLevel.get()
                , controller.getTexture(Controller::Textures::Character)
                , controller.getFont(Controller::Fonts::Sansation)
                , sf::Vector2f((mWorldBounds.width / 2.f) + inc, (mWorldBounds.height / 2.f) + inc)
                , mEntityStats.at(StatsType::CharacterStats)
                , controller.getParams()
                , mCharacterStates.at(Character::StateType::LookOut).get()
                , mCharacterStates.at(Character::StateType::Relax).get()
                , mCharacterStates
                , Character::StateType::Relax
                , rangedClamped(0.75f, 1.25f)));

        // Save pointer to character for enemy initialisation
        Character* characterPtr = characterNode.get();
        mCharacters.push_back(characterPtr);

        mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(characterNode));
    }

    mCurrentCharacter = mCharacters.at(mCurrentCharacterIndex);

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

        std::unique_ptr<Goblin> enemyNode(new Goblin(mLevel.get()
                                         , controller.getTexture(Controller::Textures::Enemy)
                                         , controller.getFont(Controller::Fonts::Sansation)
                                         , pos
                                         , mEntityStats.at(StatsType::EnemyStats)
                                         , controller.getParams()
                                         , mGoblinStates.at(GoblinStates::StateType::LookOut).get()
                                         , mGoblinStates.at(GoblinStates::StateType::Relax).get()
                                         , mGoblinStates
                                         , Enemy::StateType::Relax
                                         , rangedClamped(0.75f, 1.25f)));

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
               changeCharacter();
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
                  mCurrentCharacter->addToPath(mousePosF);
                }
                else
                {
                  mCurrentCharacter->startNewPath(mousePosF);
                }
            }
        }
    }

    handleRealTimeInput();

    if(characterChange)
      mFocusPoint = mCurrentCharacter->getWorldPosition();
}

void World::changeCharacter()
{
   Character* curChar = nullptr;

   do
   {
      mCurrentCharacterIndex ++;

      if(mCurrentCharacterIndex > mCharacters.size() - 1)
         mCurrentCharacterIndex = 0;

      curChar = mCharacters.at(mCurrentCharacterIndex);

   } while(!curChar);

   mCurrentCharacter = curChar;
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
    return mLevel->getBlockTypeInRange(entity
                                       , radius
                                       , blockType);
}

LevelBlock* World::insertEntityIntoLevel(MovingEntity* entity) const
{
    return mLevel->insertEntityIntoLevel(entity);
}

//std::vector<MovingEntity*> World::getEntitiesInRange(const MovingEntity* entity
//        , float neighbourhoodRadius) const
//{
//    return mLevel->getEntitiesInRange(entity
//                                      , neighbourhoodRadius);
//}

