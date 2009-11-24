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

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "../btogre\BtOgreSoftBody.hpp"

#include "../framework/AppState.hpp"

#include "../framework/DotSceneLoader.hpp"

#include <btBulletDynamicsCommon.h>

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

#define IMPULSE_VALUE 10
#define GRAVITY_ACCELERATION 0, -9.81, 0

//|||||||||||||||||||||||||||||||||||||||||||||||

enum QueryFlags
{
    OGRE_HEAD_MASK  = 1 << 0,
    CUBE_MASK   = 1 << 1
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState
{
    public:
        GameState();

        DECLARE_APPSTATE_CLASS(GameState)

        void enter();
        void createScene();
        void createPlayer();
        void exit();
        bool pause(); //{return true;}
        void resume(); //{};

        void setLevel(Ogre::String levelName);

        void moveCamera();
        void getInput();
        void movePlayer();

        bool keyPressed(const OIS::KeyEvent &keyEventRef);
        bool keyReleased(const OIS::KeyEvent &keyEventRef);

        bool mouseMoved(const OIS::MouseEvent &arg);
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        void onLeftPressed(const OIS::MouseEvent &evt);
        bool onExitButtonGame(const CEGUI::EventArgs &args);

        void update(double timeSinceLastFrame);

        void setBufferedMode();
        void setUnbufferedMode();

    private:
        bool                        m_bQuit;

        Ogre::String                m_LevelName;

        Ogre::Vector3               m_ImpulseVector;   // Player motion relative to camera
        Ogre::Vector3               m_TranslateVector; // Camera motion relative to istelf
        Ogre::Real                  m_MoveSpeed;
        Ogre::Degree                m_RotateSpeed;
        float                       m_MoveScale;       // Per frame motion of camera
        float                       m_ImpulseScale;
        Ogre::Degree                m_RotScale;

        Ogre::RaySceneQuery*        m_pRSQ;
        Ogre::SceneNode*            m_pCurrentObject;
        Ogre::Entity*               m_pCurrentEntity;
        bool                        m_bLMouseDown, m_bRMouseDown;

        CEGUI::Window*              m_pMainWnd;
        //CEGUI::Window*                m_pChatWnd;

        btSoftRigidDynamicsWorld*                   physicsWorld;
        btSequentialImpulseConstraintSolver*        solver;
        btCollisionDispatcher*                      dispatcher;
        btSoftBodyRigidBodyCollisionConfiguration*  collisionConfiguration;
        btAxisSweep3*                               broadphase;
        btSoftBodyWorldInfo                         m_SoftBodyWorldInfo;

        BtOgre::DebugDrawer*                        physicsDebug;

        btSoftBody*                                 m_pPlayerPhysics;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

