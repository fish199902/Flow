//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef FLOW_HPP
#define FLOW_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

/// \todo Create necessary game states
#include "states/MainMenu.hpp"
#include "states/GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class Flow
{
public:
	Flow();
	~Flow();

	void startFlow();

private:
	AppStateManager*	m_pAppStateManager;
	bool			m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

