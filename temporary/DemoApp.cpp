//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::DemoApp()
{
    m_bShutdown = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
    delete OgreFramework::getSingletonPtr();
    delete m_pAppStateManager;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::startDemo()
{
    new OgreFramework();
    OgreFramework::getSingletonPtr()->initOgre("AdvancedOgreFramework", 0, 0);

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

    m_pAppStateManager = new AppStateManager();

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}

//|||||||||||||||||||||||||||||||||||||||||||||||
