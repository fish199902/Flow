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

#include "AppStateManager.hpp"

#include <OgreWindowEventUtilities.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

AppStateManager::AppStateManager()
{
    m_bShutdown = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

AppStateManager::~AppStateManager()
{
    while (!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    while (!m_States.empty())
    {
        m_States.pop_back();
    }
}

void AppStateManager::manageAppState(Ogre::String stateName, AppState* state)
{
    try
    {
        state_info new_state_info;
        new_state_info.name = stateName;
        new_state_info.state = state;
        m_States.push_back(new_state_info);
    }
    catch (std::exception& e)
    {
        delete state;
        throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Error while trying to manage a new AppState\n" + Ogre::String(e.what()), "AppStateManager.cpp (39)");
    }
}

AppState* AppStateManager::findByName(Ogre::String stateName)
{
    std::vector<state_info>::iterator itr;

    for (itr = m_States.begin(); itr != m_States.end(); itr++)
    {
        if (itr->name == stateName)
            return itr->state;
    }

    return 0;
}

void AppStateManager::start(AppState* state)
{
    changeAppState(state);

    double timeSinceLastFrame = 1;
    double startTime = 0;

    while (!m_bShutdown)
    {
        if (OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        Ogre::WindowEventUtilities::messagePump();
#endif

        if (OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
        {
            startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

            OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
            OgreFramework::getSingletonPtr()->m_pMouse->capture();

            m_ActiveStateStack.back()->update(timeSinceLastFrame);

            OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
            OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

            timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
        }
        else
        {
#ifdef _WINDOWS
            Sleep(1000);
#else
            sleep(1000);
#endif
        }
    }

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

void AppStateManager::changeAppState(AppState* state)
{
    if (!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    m_ActiveStateStack.push_back(state);
    init(state);
    m_ActiveStateStack.back()->enter();
}

bool AppStateManager::pushAppState(AppState* state)
{
    if (!m_ActiveStateStack.empty())
    {
        if (!m_ActiveStateStack.back()->pause())
            return false;
    }

    m_ActiveStateStack.push_back(state);
    init(state);
    m_ActiveStateStack.back()->enter();

    return true;
}

void AppStateManager::popAppState(void)
{
    if (!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    if (!m_ActiveStateStack.empty())
    {
        init(m_ActiveStateStack.back());
        m_ActiveStateStack.back()->resume();
    }
    else
        shutdown();
}

void AppStateManager::shutdown()
{
    m_bShutdown = true;
}

void AppStateManager::init(AppState* state)
{
    OgreFramework::getSingletonPtr()->m_pKeyboard->setEventCallback(state);
    OgreFramework::getSingletonPtr()->m_pMouse->setEventCallback(state);

    OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
}
