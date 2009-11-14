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

#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
    public:
        OgreFramework();
        ~OgreFramework();

        void initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
        void updateOgre(double timeSinceLastFrame);
        void updateStats();

        bool keyPressed(const OIS::KeyEvent &keyEventRef);
        bool keyReleased(const OIS::KeyEvent &keyEventRef);

        bool mouseMoved(const OIS::MouseEvent &evt);
        bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

        Ogre::Root*         m_pRoot;
        Ogre::RenderWindow*     m_pRenderWnd;
        Ogre::Viewport*         m_pViewport;
        Ogre::Log*          m_pLog;
        Ogre::Timer*            m_pTimer;

        OIS::InputManager*      m_pInputMgr;
        OIS::Keyboard*          m_pKeyboard;
        OIS::Mouse*         m_pMouse;

        CEGUI::OgreCEGUIRenderer*   m_pGUIRenderer;
        CEGUI::System*          m_pGUISystem;

    private:
        OgreFramework(const OgreFramework&);
        OgreFramework& operator= (const OgreFramework&);

        Ogre::Overlay*          m_pDebugOverlay;
        Ogre::Overlay*          m_pInfoOverlay;
        int             m_iNumScreenShots;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

