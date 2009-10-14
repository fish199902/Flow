#include "Effects.h"

#include "Piece.h"

Piece::Piece(Ogre::String mesh) {
    meshName = mesh;

    x = 0;
    y = 0;
    z = 0;
    sx = 0;
    sy = 0;
    sz = 0;
    rotation = 0;

    modifiers = 0;
    effects = 0;
};

void Piece::buildPieceInOgre(Ogre::SceneManager &sceneMgr, Ogre::String pieceName) {
    if (!hasModifiers(MOD_DYNAMIC)) {
        // This piece is static
        sceneMgr.getStaticGeometry("level")->addEntity(
            sceneMgr.createEntity(pieceName, meshName),
            Ogre::Vector3(x, y, z),
            Ogre::Quaternion(Ogre::Degree(rotation), Ogre::Vector3::UNIT_Y),
            Ogre::Vector3(sx, sy, sz));
    }
    /// \todo Dynamic level objects not implemented
};
