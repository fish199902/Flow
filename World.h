#ifndef WORLD_H_
#define WORLD_H_

#include "Ogre.h"

/**
 * Manages the world (anything that isnt the GUI).
 */
class World {
    public:
    /**
     * Load and setup a level
     * \param name Name of the level to be loaded.
     */
    void setLevel(Ogre::String name);

    private:
    Ogre::String currentLevel;
    Ogre::SceneManager *sceneMgr;

    /// Remove the previous level (if necessary), and prepare to load a new level.
    void cleanUp();
};

#endif // WORLD_H_
