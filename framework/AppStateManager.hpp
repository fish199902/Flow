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

#ifndef APP_STATE_MANAGER_HPP
#define APP_STATE_MANAGER_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class AppStateManager : public AppStateListener
{
    public:
        typedef struct
        {
            Ogre::String name;
            AppState* state;
        } state_info;

        AppStateManager();
        ~AppStateManager();

        void manageAppState(Ogre::String stateName, AppState* state);

        AppState* findByName(Ogre::String stateName);

        void start(AppState* state);
        void changeAppState(AppState* state);
        bool pushAppState(AppState* state);
        void popAppState(void);
        void shutdown(void);

    protected:
        void init(AppState *state);

        std::vector<AppState*>      m_ActiveStateStack;
        std::vector<state_info>     m_States;
        bool                m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

