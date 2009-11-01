//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
	m_MoveSpeed 		= 0.1;
	m_RotateSpeed		= 0.3;

	m_pCurrentObject	= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	m_pRSQ = m_pSceneMgr->createRayQuery(Ray());
	m_pRSQ->setQueryMask(OGRE_HEAD_MASK);

	m_pCamera = m_pSceneMgr->createCamera("GameCamera");
	m_pCamera->setPosition(Vector3(5, 60, 60));
	m_pCamera->lookAt(Vector3(5, 20, 0));
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
	CEGUI::System::getSingleton().injectMouseMove(state.X.abs-mousePos.d_x,state.Y.abs-mousePos.d_y);

	m_pMainWnd = CEGUI::WindowManager::getSingleton().getWindow("AOF_GUI_GAME");
	//m_pChatWnd = CEGUI::WindowManager::getSingleton().getWindow("ChatWnd");

	OgreFramework::getSingletonPtr()->m_pGUISystem->setGUISheet(m_pMainWnd);

	CEGUI::PushButton* pExitButton = (CEGUI::PushButton*)m_pMainWnd->getChild("ExitButton_Game");
	pExitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameState::onExitButtonGame, this));

	m_bLMouseDown = m_bRMouseDown = false;
	m_bQuit = false;
	m_bChatMode = false;

	setUnbufferedMode();

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
	if(m_pSceneMgr)
		OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

/// Set the name of the level to be loaded when the game state is activated
void GameState::setLevel(Ogre::String levelName)
{
    /// \todo Any setup this might require
    m_LevelName = levelName;
}

void GameState::createScene()
{
	//m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	m_pSceneMgr->createLight("Light")->setPosition(75,75,75);

	Ogre::DotSceneLoader* pDotSceneLoader = new Ogre::DotSceneLoader();
	pDotSceneLoader->parseDotScene(m_LevelName + ".xml", "General", m_pSceneMgr, m_pSceneMgr->getRootSceneNode());
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		m_bQuit = true;
		return true;
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
	OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseWheelChange(evt.state.Z.rel);
	OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseMove(evt.state.X.rel, evt.state.Y.rel);

	return true;
}

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseButtonDown(CEGUI::LeftButton);

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		OgreFramework::getSingletonPtr()->m_pGUISystem->injectMouseButtonUp(CEGUI::LeftButton);

	return true;
}

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{

}

bool GameState::onExitButtonGame(const CEGUI::EventArgs &args)
{
	m_bQuit = true;
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::moveCamera()
{
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_pCamera->moveRelative(m_TranslateVector);
	m_pCamera->moveRelative(m_TranslateVector / 10);
}

void GameState::getInput()
{
	if(m_bChatMode == false)
	{
		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
		{
			m_TranslateVector.x = -m_MoveScale;
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
		{
			m_TranslateVector.x = m_MoveScale;
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
		{
			m_TranslateVector.z = -m_MoveScale;
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
		{
			m_TranslateVector.z = m_MoveScale;
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LEFT))
		{
			m_pCamera->yaw(m_RotScale);
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
		{
			m_pCamera->yaw(-m_RotScale);
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP))
		{
			m_pCamera->pitch(m_RotScale);
		}

		if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN))
		{
			m_pCamera->pitch(-m_RotScale);
		}
	}
}

void GameState::update(double timeSinceLastFrame)
{
	if(m_bQuit == true)
	{
		this->popAppState();
		return;
	}

	m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * timeSinceLastFrame;

	m_TranslateVector = Vector3::ZERO;

	getInput();
	moveCamera();
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
	pControlsPanel->setText("[Tab] - To switch between input modes\n\nAll keys to write in the chat box.\n\nPress [Enter] or [Return] to send message.\n\n[Print] - Take screenshot\n\n[Esc] - Quit to main menu");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::setUnbufferedMode()
{
	CEGUI::Editbox* pModeCaption = (CEGUI::Editbox*)m_pMainWnd->getChild("ModeCaption");
	pModeCaption->setText("Unuffered Input Mode");

	CEGUI::MultiLineEditbox* pControlsPanel = (CEGUI::MultiLineEditbox*)m_pMainWnd->getChild("ControlsPanel");
	pControlsPanel->setText("[Tab] - To switch between input modes\n\n[W] - Forward\n[S] - Backwards\n[A] - Left\n[D] - Right\n\nPress [Shift] to move faster\n\n[O] - Toggle Overlays\n[Print] - Take screenshot\n\n[Esc] - Quit to main menu");
}

