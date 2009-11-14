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

#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "../framework/AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class MenuState : public AppState
{
    public:
        DECLARE_APPSTATE_CLASS(MenuState)

        void enter();
        void createScene();
        void exit();
        bool pause();
        void resume();

        bool keyPressed(const OIS::KeyEvent &keyEventRef);
        bool keyReleased(const OIS::KeyEvent &keyEventRef);

        bool mouseMoved(const OIS::MouseEvent &evt);
        bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

        bool onExitButton(const CEGUI::EventArgs &args);
        bool onEnterButton(const CEGUI::EventArgs &args);

        void update(double timeSinceLastFrame);

    private:
        bool        m_bQuit;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||

