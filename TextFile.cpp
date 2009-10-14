#include "TextFile.h"
#include "TextFileSerializer.h"

/// \todo Adopt this code

TextFile::TextFile(Ogre::ResourceManager* creator, const Ogre::String &name,
                    Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual,
                    Ogre::ManualResourceLoader *loader) :
Ogre::Resource(creator, name, handle, group, isManual, loader)
{
    /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
    */

    /* For consistency with StringInterface, but we don't add any parameters here
    That's because the Resource implementation of StringInterface is to
    list all the options that need to be set before loading, of which
    we have none as such. Full details can be set through scripts.
    */
    createParamDictionary("TextFile");
}

TextFile::~TextFile()
{
    unload();
}

// farm out to TextFileSerializer
void TextFile::loadImpl()
{
    TextFileSerializer serializer;
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
    serializer.importTextFile(stream, this);
}

void TextFile::unloadImpl()
{
    /* If you were storing a pointer to an object, then you would check the pointer here,
    and if it is not NULL, you would destruct the object and set its pointer to NULL again.
    */

    mString.clear();
}

size_t TextFile::calculateSize() const
{
    return mString.length();
}

void TextFile::setString(const Ogre::String &str)
{
    mString = str;
}

const Ogre::String &TextFile::getString() const
{
    return mString;
}
