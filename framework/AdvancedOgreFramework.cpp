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

#include "AdvancedOgreFramework.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_iNumScreenShots   = 0;

    m_pRoot         = 0;
    m_pRenderWnd        = 0;
    m_pViewport     = 0;
    m_pLog          = 0;
    m_pTimer        = 0;

    m_pInputMgr     = 0;
    m_pKeyboard     = 0;
    m_pMouse        = 0;

    m_pDebugOverlay     = 0;
    m_pInfoOverlay      = 0;
}

OgreFramework::~OgreFramework()
{
    if (m_pInputMgr)
    {
        delete m_pKeyboard;
        delete m_pMouse;
        OIS::InputManager::destroyInputSystem(m_pInputMgr);
    }

    delete m_pRoot;
}

/**
 * Setup ogre and CEGUI
 */
void OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    m_pRoot->showConfigDialog();

    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.8, 0.8, 0.8, 1.0));

    m_pViewport->setCamera(0);

    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width  = m_pRenderWnd->getWidth();

    if (pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if (pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    m_pGUIRenderer = new CEGUI::OgreCEGUIRenderer(m_pRenderWnd, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, 0);
    m_pGUISystem = new CEGUI::System(m_pGUIRenderer);

    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
    CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"FlowMenu.layout");
    CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"FlowGame.layout");

    m_pDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
    m_pRenderWnd->setActive(true);
}

/**
 * Handle global keypresses, including activating/deactivating the debug overlay
 * and taking screenshots.
 */
bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if (m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        std::ostringstream ss;
        ss << "screenshot_" << ++m_iNumScreenShots << ".png";
        m_pRenderWnd->writeContentsToFile(ss.str());
        return true;
    }

    // Show debug overlay if shift-ctrl-/ is pressed
    if (m_pKeyboard->isKeyDown(OIS::KC_RCONTROL) &&
            m_pKeyboard->isKeyDown(OIS::KC_F12))
    {
        if (m_pDebugOverlay)
        {
            if (!m_pDebugOverlay->isVisible())
                m_pDebugOverlay->show();
            else
                m_pDebugOverlay->hide();
        }
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
    updateStats();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateStats()
{
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";
    static String batches = "Batch Count: ";

    OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
    OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
    OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
    OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

    const RenderTarget::FrameStats& stats = m_pRenderWnd->getStatistics();
    guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
    guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
    guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                        + " " + StringConverter::toString(stats.bestFrameTime) + " ms");
    guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                         + " " + StringConverter::toString(stats.worstFrameTime) + " ms");

    OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
    guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

    OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
    guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

    OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
    guiDbg->setCaption("");
}
