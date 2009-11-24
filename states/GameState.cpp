/* Copyright 2009 Austin Brown
 *
 * This file is part of Flow.
 *
 * Flow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Flow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Flow.  If not, see <http://www.gnu.org/licenses/>.
 */

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_MoveSpeed         = 0.1;
    m_RotateSpeed       = 0.1;

    m_pCurrentObject    = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

    /// \todo Make sure this is not necessary for anything, and remove it *cleanly*.
    m_pRSQ = m_pSceneMgr->createRayQuery(Ray());
    m_pRSQ->setQueryMask(OGRE_HEAD_MASK);

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(3, 20, 40));
    m_pCamera->lookAt(Vector3(3, 5, 0));
    m_pCamera->setNearClipDistance(5);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
                              Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    OgreFramework::getSingletonPtr()->m_pKeyboard->setEventCallback(this);
    OgreFramework::getSingletonPtr()->m_pMouse->setEventCallback(this);

    OgreFramework::getSingletonPtr()->m_pGUIRenderer->setTargetSceneManager(m_pSceneMgr);

    OgreFramework::getSingletonPtr()->m_pGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseArrow");
    const OIS::MouseState state = OgreFramework::getSingletonPtr()->m_pMouse->getMouseState();
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
    CEGUI::System::getSingleton().injectMouseMove(state.X.abs - mousePos.d_x, state.Y.abs - mousePos.d_y);

    m_pMainWnd = CEGUI::WindowManager::getSingleton().getWindow("AOF_GUI_GAME");
    //m_pChatWnd = CEGUI::WindowManager::getSingleton().getWindow("ChatWnd");

    OgreFramework::getSingletonPtr()->m_pGUISystem->setGUISheet(m_pMainWnd);

    CEGUI::PushButton* pExitButton = (CEGUI::PushButton*)m_pMainWnd->getChild("ExitButton_Game");
    pExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState::onExitButtonGame, this));

    m_bLMouseDown = m_bRMouseDown = false;
    m_bQuit = false;

    setUnbufferedMode();

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Initializing Bullet physics...");

    // Maximum number of rigid bodies
    int maxProxies = 512;

    // Size of the world, 10km in every direction
    /// \todo Tune this to match ogre and save memory. Check the other options vere as well
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);

    // Sweep and prune broadphase
    /// \todo use the collision dispatcher to register a callback that filters overlapping broadphase proxies so that the collisions are not processed by the rest of the system.
    broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
    m_SoftBodyWorldInfo.m_broadphase = broadphase;

    // Collision configuration is used to fine tune the algorithms used for the full collision detection
    collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    m_SoftBodyWorldInfo.m_dispatcher = dispatcher;

    // Solver determines how object interact
    solver = new btSequentialImpulseConstraintSolver;

    physicsWorld = new btSoftRigidDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    // Set Y axis to 'up', this should match ogre.
    physicsWorld->setGravity(btVector3(GRAVITY_ACCELERATION));
    m_SoftBodyWorldInfo.m_gravity.setValue(GRAVITY_ACCELERATION);

    // btOgre Stuff
    physicsDebug = new BtOgre::DebugDrawer(m_pSceneMgr->getRootSceneNode(), physicsWorld);
    physicsWorld->setDebugDrawer(physicsDebug);
    physicsDebug->setDebugMode(OgreFramework::getSingletonPtr()->getDebugState());

    createScene();
}

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");

    OgreFramework::getSingletonPtr()->m_pGUISystem->setGUISheet(0);
    OgreFramework::getSingletonPtr()->m_pGUIRenderer->setTargetSceneManager(0);

    return true;
}

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    OgreFramework::getSingletonPtr()->m_pGUIRenderer->setTargetSceneManager(m_pSceneMgr);

    OgreFramework::getSingletonPtr()->m_pGUISystem->setGUISheet(CEGUI::WindowManager::getSingleton().getWindow("AOF_GUI_GAME"));

    m_bQuit = false;
}

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");

    OgreFramework::getSingletonPtr()->m_pGUISystem->setGUISheet(0);
    OgreFramework::getSingletonPtr()->m_pGUIRenderer->setTargetSceneManager(0);

    m_pSceneMgr->destroyCamera(m_pCamera);
    m_pSceneMgr->destroyQuery(m_pRSQ);

    if (m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    // Cleanup physics simulation
    delete physicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;

}

/// Set the name of the level to be loaded when the game state is activated
void GameState::setLevel(Ogre::String levelName)
{
    /// \todo Any setup this might require
    m_LevelName = levelName;
}

/// Contruct the level in Ogre
void GameState::createScene()
{
    /// \todo Make this include setting up physics
    //m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    m_pSceneMgr->setSkyBox(true, "Examples/EveningSkyBox");

    m_pSceneMgr->createLight("Light")->setPosition(75, 75, 75);

    Ogre::DotSceneLoader* pDotSceneLoader = new Ogre::DotSceneLoader();
    pDotSceneLoader->parseDotScene(m_LevelName + ".xml", "General", m_pSceneMgr, physicsWorld, m_pSceneMgr->getRootSceneNode());

    createPlayer();
}

void GameState::createPlayer()
{
    LogManager::getSingleton().logMessage("Creating player");

    Ogre::SceneNode *playerNode = m_pSceneMgr->getSceneNode("player");

    MeshManager::getSingletonPtr()->load("player.mesh", "default");
    Ogre::Entity *playerEntity = m_pSceneMgr->createEntity("player", "player.mesh");

    BtOgre::StaticMeshToSoftBodyConverter converter(&m_SoftBodyWorldInfo, playerEntity);

    m_pPlayerPhysics = converter.createSoftBodyFromTrimesh();
    physicsWorld->addSoftBody(m_pPlayerPhysics);

    //body->setActivationState(DISABLE_DEACTIVATION);
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        // Exit from GameState to MenuState on next update()
        m_bQuit = true;
        return true;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RCONTROL) &&
            OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F12))
    {
        // Toggle debug physics
        physicsDebug->setDebugMode(!physicsDebug->getDebugMode());
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
    // Pass motion to CEGUI
    OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseWheelChange(evt.state.Z.rel);
    OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);

    return true;
}

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    // Pass clicks to CEGUI
    if (id == OIS::MB_Left)
        OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseButtonDown(CEGUI::LeftButton);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    // Pass releases to CEGUI
    if (id == OIS::MB_Left)
        OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseButtonUp(CEGUI::LeftButton);

    return true;
}

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{

}

bool GameState::onExitButtonGame(const CEGUI::EventArgs &args)
{
    // Exit from GameState to MenuState on next update()
    m_bQuit = true;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

/**
 * Move the camera (if necessary). Called each frame, after m_TranslateVector has beed updated.
 */
void GameState::moveCamera()
{
    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);

    m_pCamera->moveRelative(m_TranslateVector / 10);
}

/**
 * Gets keypresses. Called while updating each frame
 */
void GameState::getInput()
{
    // Camera motion

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
    {
        m_TranslateVector.x = -m_MoveScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
    {
        m_TranslateVector.x = m_MoveScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
    {
        m_TranslateVector.z = -m_MoveScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
    {
        m_TranslateVector.z = m_MoveScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
    {
        m_TranslateVector.y = -m_MoveScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E))
    {
        m_TranslateVector.y = m_MoveScale;
    }


    // Camera rotation

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPAD1))
    {
        m_pCamera->yaw(m_RotScale);
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPAD3))
    {
        m_pCamera->yaw(-m_RotScale);
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPAD5))
    {
        m_pCamera->pitch(m_RotScale);
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPAD2))
    {
        m_pCamera->pitch(-m_RotScale);
    }


    // Player motion

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    {
        m_ImpulseVector.x = -m_ImpulseScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
        m_ImpulseVector.x = m_ImpulseScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP))
    {
        m_ImpulseVector.z = -m_ImpulseScale;
    }

    if (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN))
    {
        m_ImpulseVector.z = m_ImpulseScale;
    }
}

/**
 * Move the player, called each frame from update
 */
void GameState::movePlayer()
{
    /// \todo Make the player motion relative to the camera.
    // Old code (before softbody:
    //m_ImpulseVector = m_ImpulseVector * m_pCamera->getWorldPosition();

    // Convert m_ImpulseVector to a btVector3 and pass it to bullet
    //m_pPlayerPhysics->applyCentralImpulse(btVector3(m_ImpulseVector.x, m_ImpulseVector.y, m_ImpulseVector.z));

    // Updated for softbody
    /// \todo Make this more realistic
    m_pPlayerPhysics->setVelocity(btVector3(m_ImpulseVector.x, m_ImpulseVector.y, m_ImpulseVector.z));
}

/**
 * Called each frame to update the scene.
 */
void GameState::update(double timeSinceLastFrame)
{
    if (m_bQuit == true)
    {
        this->popAppState();
        return;
    }

    // Keep all motion in real time
    m_MoveScale    = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale     = m_RotateSpeed * timeSinceLastFrame;
    /// \bug Total impulse will be greater if moving in both directions. Use less (sqrt10?) per side.
    m_ImpulseScale = IMPULSE_VALUE * timeSinceLastFrame;

    // Clear the last update()'s motion
    m_TranslateVector = Vector3::ZERO;
    m_ImpulseVector   = Vector3::ZERO;

    // Update Bullet
    physicsWorld->stepSimulation(timeSinceLastFrame, 2, 0.5/timeSinceLastFrame);
    physicsWorld->debugDrawWorld();

    getInput();
    moveCamera();
    movePlayer();

    physicsDebug->step();
}

void GameState::setBufferedMode()
{
    CEGUI::Editbox* pModeCaption = (CEGUI::Editbox*)m_pMainWnd->getChild("ModeCaption");
    pModeCaption->setText("Buffered Input Mode");

    //CEGUI::Editbox* pChatInputBox = (CEGUI::Editbox*)m_pChatWnd->getChild("ChatInputBox");
    //pChatInputBox->setText("");
    //pChatInputBox->activate();
    //pChatInputBox->captureInput();

    CEGUI::MultiLineEditbox* pControlsPanel = (CEGUI::MultiLineEditbox*)m_pMainWnd->getChild("ControlsPanel");
    pControlsPanel->setText("[Tab] - To switch between input modes\n"
                            "\n"
                            "All keys to write in the chat box.\n"
                            "\n"
                            "Press [Enter] or [Return] to send message.\n"
                            "\n"
                            "[Print] - Take screenshot\n"
                            "\n"
                            "[Esc] - Quit to main menu");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::setUnbufferedMode()
{
    CEGUI::Editbox* pModeCaption = (CEGUI::Editbox*)m_pMainWnd->getChild("ModeCaption");
    pModeCaption->setText("Welcome to Flow");

    CEGUI::MultiLineEditbox* pControlsPanel = (CEGUI::MultiLineEditbox*)m_pMainWnd->getChild("ControlsPanel");
    pControlsPanel->setText("Player Motion:\n"
                            "Up / Down - Z Motion\n"
                            "Left / Right - X Motion\n"
                            "\n"
/*                            "Look Controls:\n"
 *                            "[NUM1] - Left\n"
 *                            "[NUM2] - Down\n"
 *                            "[NUM3] - Right\n"
 *                            "[NUM5] - Up\n"
 *                            "\n"
 */
                            "Move Camera:\n"
                            "[W] - In\n"
                            "[S] - Out\n"
                            "[A] - Left\n"
                            "[D] - Right\n"
                            "[E] - Up\n"
                            "[C] - Down\n"
                            "[Shift] Move Faster\n"
                            "\n"
                            "[R-CTRL]+[F12] - Toggle Debug\n"
                            "[Print] - Take screenshot\n"
                            "\n"
                            "[Esc] - Quit to main menu");
}

