
//|||||||||||||||||||||||||||||||||||||||||||||||

#include "Flow.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

Flow::Flow()
{
	m_bShutdown = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

Flow::~Flow()
{
	delete OgreFramework::getSingletonPtr();
	delete m_pAppStateManager;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void Flow::startFlow()
{
	new OgreFramework();
	OgreFramework::getSingletonPtr()->initOgre("Flow", 0, 0);

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Flow initialized!");

	m_pAppStateManager = new AppStateManager();

    /// \todo Create necessary game states

	MainMenu::create(m_pAppStateManager, "MainMenu");
	GameState::create(m_pAppStateManager, "GameState");

	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}

//|||||||||||||||||||||||||||||||||||||||||||||||
