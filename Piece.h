#ifndef PIECE_H_
#define PIECE_H_

#include "Ogre.h"

class Piece {
    public:
    Piece(Ogre::String mesh);

    /**
     * Create the piece in an Ogre SceneManager.
     * \param sceneMgr The SceneManager to build the piece in.
     */
    void buildPieceInOgre(Ogre::SceneManager &sceneMgr, Ogre::String pieceName);

    /// Check which modifiers are enabled. Refer to Effects.h for constants.
    inline bool hasModifiers(int mods) {return (modifiers&mods)!=0;};
    /// Check which effects are enabled. Refer to Effects.h for constants.
    inline bool hasEffects(int fx) {return (effects&fx)!=0;};

    /// Enable modifiers. Refer to Effects.h for constants.
    inline void addModifiers(int mods) {modifiers |= mods;};
    /// Enable effects. Refer to Effects.h for constants.
    inline void addEffects(int fx) {effects |= fx;};

    private:
    Ogre::String meshName;
    int modifiers, effects;
    float x, y, z, sx, sy, sz;
    Ogre::Degree rotation;
};

#endif // PIECE_H_
