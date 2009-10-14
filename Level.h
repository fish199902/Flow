#ifndef LEVEL_H_
#define LEVEL_H_

#include "Piece.h"

#include "Ogre.h"

#include <vector>

class Level {
    public:
    Level();

    /**
     * Create the level in an Ogre SceneManager
     * \param sceneMgr The SceneManager to build the level in.
     */
    void buildLevelInOgre(Ogre::SceneManager &sceneMgr);

    /// Check which modifiers are enabled. Refer to Effects.h for constants.
    inline bool hasModifiers(int mods) {return (modifiers&mods)!=0;};
    /// Enable modifiers. Refer to Effects.h for constants.
    inline void addModifiers(int mods) {modifiers |= mods;};

    private:
    int modifiers;
    std::vector<Piece> pieces;
};

#endif // LEVEL_H_
