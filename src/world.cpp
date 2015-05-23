#include <iostream>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "App/Utility.hpp"
#include "App/Controller.hpp"
#include "App/GameState.hpp"
#include "App/HUD.hpp"
#include "World/World.hpp"
#include "World/Scenery.hpp"
#include "World/LevelBlock.hpp"
#include "SceneNode/SpriteNode.hpp"
#include "Entity/Adventurer.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/State/EnemyStates.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "SceneNode/SpriteNode.hpp"

//const sf::Time World::mComboTime = sf::seconds(1.f);

World::World(GameState& gameState
             , const Controller& controller
             , sf::RenderWindow& window
             , std::string username
             , int worldDim
             , int numEnemy
             , sf::Time levelTime)
: mViewSize(640, 480)
, mWorldBounds(sf::Vector2f(0, 0),
               sf::Vector2f(worldDim, worldDim))
//, mPhysicsWorldDim(worldDim)
//, mPhysicsWorldX(worldDim)
//, mPhysicsWorldY(worldDim)
//, mPixelPerMeter(40.f)
//, mPhysicsWorldScale(1.f / PixelPerMeter)
, mVelocityIter(6)
, mPositionIter(2)
, mTimeStep(1.f / 60.f)
//, mLevelBlockSize(controller.getParams().LevelBlockSize)
, mWaypointRadius(controller.getParams().WaypointRadius)
, mScrollSpeed(controller.getParams().ScrollSpeed)
, mGameState(gameState)
, mWindow(window)
, mWorldView(mWindow.getDefaultView())
//, mWorldRect(sf::Vector2f(mWorldBounds.width, mWorldBounds.height))
, mFocusPoint(0.f, 0.f)
, mQuadTree(new QuadTree(0,
                         mWorldBounds))
, mPhysicsEngine(b2Vec2(0.f, 0.f))
, mHUD(this
       , controller.getFont(Controller::Fonts::Sansation)
       , getViewBounds()
       , username)
, mCurrentAdventurer(nullptr)
, mCurrentAdventurerIndex(0)
{
//    mLevel = std::unique_ptr<Level>(new Level(controller.getParams().LevelBlockSize
//                                            , mWorldBounds));

//    mLevel = std::unique_ptr<Level>(new Level(controller.getParams().LevelBlockSize
//                                            , mWorldBounds));

    initialiseStatesAndStats();
    buildScene(controller);
}

b2Body* World::generatePhysicsBody(sf::Vector2f pos,
                         sf::Vector2f size,
                         b2BodyType type = b2_staticBody)
{
   b2BodyDef bodyDef;
   bodyDef.type = type;
   bodyDef.linearDamping = 4.f;
   bodyDef.angularDamping = 4.f;
   bodyDef.position.Set(pos.x, pos.y);

   b2Body* body = mPhysicsEngine.CreateBody(&bodyDef);
//   b2Body* body = nullptr;

   b2PolygonShape box;
   box.SetAsBox(size.x / 2.f, size.y / 2.f);

//   PhysicsBody::PhyBody phyBody = nullptr;

   if(type == b2_staticBody)
   {
      body->CreateFixture(&box, 0.f);
   }
   else
   {
      b2FixtureDef fixtureDef;
      fixtureDef.shape = &box;
      fixtureDef.density = 1.f;
      fixtureDef.friction = 0.3f;

      body->CreateFixture(&fixtureDef);
   }

   return body;
}

b2Body* World::generatePhysicsBody(sf::Vector2f pos,
                      float radius,
                      b2BodyType type = b2_staticBody)
{
   b2BodyDef bodyDef;
   bodyDef.type = type;
   bodyDef.linearDamping = 1.f;
   bodyDef.angularDamping = 1.f;
   bodyDef.position.Set(pos.x, pos.y);

   b2Body* body = mPhysicsEngine.CreateBody(&bodyDef);
//   b2Body* body = nullptr;

   b2CircleShape circle;
   circle.m_p.Set(0.f, 0.f);
   circle.m_radius = radius;

//   PhysicsBody::PhyBody phyBody = nullptr;

   if(type == b2_staticBody)
   {
      body->CreateFixture(&circle, 0.f);
   }
   else
   {
      b2FixtureDef fixtureDef;
      fixtureDef.shape = &circle;
      fixtureDef.density = 1.f;
      fixtureDef.friction = 0.3f;

      body->CreateFixture(&fixtureDef);
   }

   return body;
}

void World::initialiseStatesAndStats()
{
    // Initialise enemy states
    mEnemyStates.push_back(std::unique_ptr<EnemyStates::LookOut>(new EnemyStates::LookOut));
    mEnemyStates.push_back(std::unique_ptr<EnemyStates::Evade>(new EnemyStates::Evade));
    mEnemyStates.push_back(std::unique_ptr<EnemyStates::Relax>(new EnemyStates::Relax));
    mEnemyStates.push_back(std::unique_ptr<EnemyStates::Attack>(new EnemyStates::Attack));
//    mEnemyStates.push_back(std::unique_ptr<Exit>(new Exit));

    mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::LookOut>(new AdventurerStates::LookOut));
//    mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::Evade>(new AdventurerStates::Evade));
    mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::Relax>(new AdventurerStates::Relax));
    mAdventurerStates.push_back(std::unique_ptr<AdventurerStates::Attack>(new AdventurerStates::Attack));
//    mAdventurerStates.push_back(std::unique_ptr<Exit>(new Exit));

    mEntityStats.push_back(EntityStats("data/entityStats/enemyStats.dat"));
    mEntityStats.push_back(EntityStats("data/entityStats/adventurerStats.dat"));
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

//    sf::Vector2f bckgrndSpritePos(mWorldBounds.width / 2.f
//                                  , mWorldBounds.height / 2.f);

   sf::IntRect backgroundSpriteBounds = sf::IntRect(sf::Vector2i(0, 0),
                                                    sf::Vector2i(meterToPixel(mWorldBounds.width),
                                                                 meterToPixel(mWorldBounds.height)));

    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(controller.getTexture(Controller::Textures::Ground),
                                                                backgroundSpriteBounds));

//    mBackground = backgroundSprite.get();
    mSceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(backgroundSprite));

//    mLevel->generateLevel(mSceneLayers, controller);

    generateAgents(controller);
}

void World::generateAgents(const Controller& controller)
{
    // Initialise characters and add to scene graph
   for(int i = 0; i < 1; i++)
   {
      float inc = i * 40.f;

//      sf::Vector2f pos((mWorldBounds.width / 2.f) + inc, (mWorldBounds.height / 2.f) + inc);
//      sf::Vector2f pos(mWorldBounds.width / 2.f, mWorldBounds.height / 2.f);
//      sf::Vector2f pos(10, 10);
      sf::Vector2f pos;
//      float size = controller.getTexture(Controller::Textures::Adventurer).getSize().x;

      b2Body* body = generatePhysicsBody(pos,
                                         1.f,
                                         b2BodyType::b2_dynamicBody);

      std::unique_ptr<Adventurer> adventurerNode(new Adventurer(mWindow
                                                               , mQuadTree.get()
                                                               , controller.getTexture(Controller::Textures::Adventurer)
                                                               , controller.getFont(Controller::Fonts::Sansation)
                                                               , pos
                                                               , mEntityStats.at(World::Stats::AdventurerStats)
                                                               , controller.getParams()
                                                               , mAdventurerStates.at(Adventurer::States::LookOut).get()
                                                               , mAdventurerStates.at(Adventurer::States::Relax).get()
                                                               , mAdventurerStates
                                                               , Adventurer::States::Relax
                                                               , body));

   // Save pointer to character for enemy initialisation
//      Adventurer* adventurerPtr = adventurerNode.get();
      mAdventurers.push_back(adventurerNode.get());

      mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(adventurerNode));
   }

    mCurrentAdventurer = mAdventurers.at(0);
    mCurrentAdventurer->setIsSelected(true);

    // Initialise enemy and add to scene graph
    for(int i = 0 ; i < 0; i++)
    {
      // Find square for enemy to start in
      LevelBlock* levelBlock;
      sf::Vector2i index;
      sf::Vector2f pos;

//      int maxX = mLevel->getLevelX();

//      index = sf::Vector2i(rangedClamped(1, maxX - 2)
//                           , rangedClamped(1, maxX - 2));

//      levelBlock = mLevel->getBlock(index);

//      pos = levelBlock->getCenter();

//      float size = controller.getTexture(Controller::Textures::Enemy).getSize().x;

      b2Body* body = generatePhysicsBody(pos,
                                         1.f,
                                         b2BodyType::b2_dynamicBody);

      std::unique_ptr<Enemy> enemyNode(new Enemy(mQuadTree.get()
                                              , controller.getTexture(Controller::Textures::Enemy)
                                              , controller.getFont(Controller::Fonts::Sansation)
                                              , pos
                                              , mEntityStats.at(World::Stats::EnemyStats)
                                              , controller.getParams()
                                              , mEnemyStates.at(Enemy::States::LookOut).get()
                                              , mEnemyStates.at(Enemy::States::Relax).get()
                                              , mEnemyStates
                                              , Enemy::States::Relax
                                              , body));

//        enemyNode->setMovingTarget(mAdventurers.at(0));
//      mSceneLayers.at(SceneNode::Layers::Foreground)->addChild(std::move(enemyNode));
   }
}

void World::handleRealTimeInput()
{
//   sf::Vector2f adventurerMovement = mCurrentAdventurer->getVelocity() / 2.f;
   sf::Vector2f movement;// = mCurrentAdventurer->getVelocity() / 2.f;
   float speed = 4.f;

   if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)
   || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
   {
      movement.x -= speed;
   }

   if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)
   || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
   {
      movement.x += speed;
   }

   if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)
   || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
   {
      movement.y -= speed;
   }

   if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)
   || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
   {
      movement.y += speed;
   }

//   if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
//   {
//      mFocusPoint += movement;
//   }
//   else
//   {
      if(mCurrentAdventurer)
      {
         mCurrentAdventurer->setVelocity(movement);
//         mCurrentAdventurer->setVelocity(sf::Vector2f(1.f, 2.f));
         adaptPlayerVelocity();
//         mFocusPoint =
   //      mFocusPoint = meterToPixel(mCurrentAdventurer->getWorldPosition());
      }
//   }
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

void World::rotateViewToAdventurer()
{
//   if(std::fabs(magVec(mVelocity)) > MINFLOAT)
//   {
//     int sign = signVec(mHeading, mVelocity);
//
//     float angle = std::acos(dotVec(mHeading, normVec(mVelocity)));
//     angle *= sign;
//
//     clampRotation(angle
//                   , -mMaxTurnRate
//                   , mMaxTurnRate);
//
//     if(angle > MINFLOAT || angle < -MINFLOAT)
//         rotate(angle * (180.f / SteeringBehaviour::mPI));
//   }
//
//   float currentRotation = getRotation() * (SteeringBehaviour::mPI / 180.f);
//   mHeading = sf::Vector2f(std::sin(currentRotation), -std::cos(currentRotation));
}

void World::cycleAdventurer()
{
   mCurrentAdventurerIndex ++;

   if(mCurrentAdventurerIndex > mAdventurers.size() - 1)
      mCurrentAdventurerIndex = 0;

   mCurrentAdventurer->setIsSelected(false);
   mCurrentAdventurer->resetVelocity();

   mCurrentAdventurer = mAdventurers.at(mCurrentAdventurerIndex);
   mCurrentAdventurer->setIsSelected(true);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mCurrentAdventurer->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mCurrentAdventurer->setVelocity(velocity / std::sqrt(2.f));
}

void World::update(sf::Time dt)
{
  mQuadTree->clear();

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

void World::updatePhysicsEngine()
{
   mPhysicsEngine.Step(mTimeStep, mVelocityIter, mPositionIter);
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
                mWindow.close();
            else if(event.key.code == sf::Keyboard::Tab)
            {
               cycleAdventurer();
               mFocusPoint = meterToPixel(mCurrentAdventurer->getWorldPosition());
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
                mousePosF.x = std::min(mousePosF.x, mWorldBounds.width);
                mousePosF.x = std::max(mousePosF.x, 0.f);

                mousePosF.y = std::min(mousePosF.y, mWorldBounds.height);
                mousePosF.y = std::max(mousePosF.y, 0.f);

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                {
                    if(mCurrentAdventurer)
                        mCurrentAdventurer->addToPath(mousePosF);
                }
                else
                {
                    if(mCurrentAdventurer)
                        mCurrentAdventurer->startNewPath(mousePosF);
                }
            }
        }
    }

    handleRealTimeInput();
}

void World::display()
{
//    adjustView();

   if(mCurrentAdventurer)
      mFocusPoint = meterToPixel(mCurrentAdventurer->getWorldPosition());
//      mFocusPoint = mCurrentAdventurer->Transformable::getPosition();

   mWorldView.setCenter(mFocusPoint);
   mWindow.setView(mWorldView);

//   mWindow.draw(mWorldRect);
   mWindow.draw(mSceneGraph);

   mHUD.setHUDPosition(getViewBounds());
   mWindow.draw(mHUD);
}

const sf::FloatRect World::getViewBounds() const
{
   return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}
