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

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}

//|||||||||||||||||||||||||||||||||||||||||||||||
