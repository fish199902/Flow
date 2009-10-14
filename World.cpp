#include "World.h"

void World::setLevel(Ogre::String name) {
    cleanUp();

    /// \todo Load the level data

    /// \todo Parse the level data into a level object

    //level->buildLevelInOgre(*sceneMgr);
};

void World::cleanUp() {
    /// \todo Setup the final part of ogre
}
