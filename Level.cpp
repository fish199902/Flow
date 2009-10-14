#include "Level.h"

Level::Level() {

};

void Level::buildLevelInOgre(Ogre::SceneManager &sceneMgr) {
    for (unsigned int pos = 0; pos < pieces.size(); pos++) {
        // Tell each piece to add itself to the level
        /// \todo Potential problem: Entity names may be repeated when changing levels
        pieces[pos].buildPieceInOgre(sceneMgr, Ogre::StringConverter::toString(pos));
    }
};
