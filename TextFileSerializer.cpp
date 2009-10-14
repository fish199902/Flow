#include "TextFileSerializer.h"
#include "TextFile.h"

/// \todo Adopt this code

TextFileSerializer::TextFileSerializer()
{
}

TextFileSerializer::~TextFileSerializer()
{
}

void TextFileSerializer::exportTextFile(const TextFile *pText, const Ogre::String &fileName)
{
    std::ofstream outFile;
    outFile.open(fileName.c_str(), std::ios::out);
    outFile << pText->getString();
    outFile.close();
}

void TextFileSerializer::importTextFile(Ogre::DataStreamPtr &stream, TextFile *pDest)
{
    pDest->setString(stream->getAsString());
}
