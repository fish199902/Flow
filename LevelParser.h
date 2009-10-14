#ifndef LEVELPARSER_H_
#define LEVELPARSER_H_

#include "Level.h"
#include "Piece.h"

#include "Ogre.h"

namespace LevelParser {
    /**
     * Parse level data from data
     * \param data String containing XML data.
     */
    Level parseLevel(Ogre::String data);

    /**
     * Convert a level object back to XML.
     * \param level Level object to serialize.
     */
    Ogre::String serializeLevel(Level level);
}

#endif // LEVELPARSER_H_
