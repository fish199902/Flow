#ifndef TEXTFILESERIALIZER_H_
#define TEXTFILESERIALIZER_H_

/// \todo Adopt this code

#include <OgreSerializer.h>

class TextFile; // forward declaration

class TextFileSerializer : public Ogre::Serializer
{
public:
    TextFileSerializer();
    virtual ~TextFileSerializer();

    void exportTextFile(const TextFile *pText, const Ogre::String &fileName);
    void importTextFile(Ogre::DataStreamPtr &stream, TextFile *pDest);
};

#endif // TEXTFILESERIALIZER_H_
