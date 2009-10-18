//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "../AppState.hpp"

#include "../DotSceneLoader.hpp"

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

enum QueryFlags
{
	OGRE_HEAD_MASK	= 1<<0,
        CUBE_MASK	= 1<<1
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState
{
public:
	GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause(); //{return true;}
	void resume(); //{};

	void moveCamera();
	void getInput();

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
	Ogre::SceneNode*			m_pOgreHeadNode;
	Ogre::Entity*				m_pOgreHeadEntity;
	Ogre::MaterialPtr			m_pOgreHeadMat;
	Ogre::MaterialPtr			m_pOgreHeadMatHigh;

	bool					m_bQuit;

	Ogre::Vector3				m_TranslateVector;
	Ogre::Real				m_MoveSpeed;
	Ogre::Degree				m_RotateSpeed;
	float					m_MoveScale;
	Ogre::Degree				m_RotScale;

	Ogre::RaySceneQuery*		        m_pRSQ;
	Ogre::SceneNode*			m_pCurrentObject;
	Ogre::Entity*				m_pCurrentEntity;
	bool					m_bLMouseDown, m_bRMouseDown;
	bool					m_bChatMode;

	CEGUI::Window*				m_pMainWnd;
	CEGUI::Window*				m_pChatWnd;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

