#ifndef __TEXTFILEMANAGER_H__
#define __TEXTFILEMANAGER_H__

#warning TODO: Adopt this code

#include <OgreResourceManager.h>
#include "TextFile.h"

class TextFileManager : public Ogre::ResourceManager, public Ogre::Singleton<TextFileManager>
{
protected:

    // must implement this from ResourceManager's interface
    Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
        const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
        const Ogre::NameValuePairList *createParams);

public:

    TextFileManager();
    virtual ~TextFileManager();

    virtual TextFilePtr load(const Ogre::String &name, const Ogre::String &group);

    static TextFileManager &getSingleton();
    static TextFileManager *getSingletonPtr();
};

#endif
