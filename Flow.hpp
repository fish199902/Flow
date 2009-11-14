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

#ifndef FLOW_HPP
#define FLOW_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "framework/AdvancedOgreFramework.hpp"
#include "framework/AppStateManager.hpp"

/// \todo Create necessary game states
#include "states/MenuState.hpp"
#include "states/GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class Flow
{
    public:
        Flow();
        ~Flow();

        void startFlow();

    private:
        AppStateManager*    m_pAppStateManager;
        bool            m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

