#include "TextFileManager.h"

/// \todo Adopt this code

template<> TextFileManager *Ogre::Singleton<TextFileManager>::ms_Singleton = 0;

TextFileManager *TextFileManager::getSingletonPtr()
{
    return ms_Singleton;
}

TextFileManager &TextFileManager::getSingleton()
{
    assert(ms_Singleton);
    return(*ms_Singleton);
}

TextFileManager::TextFileManager()
{
    mResourceType = "TextFile";

    // low, because it will likely reference other resources
    mLoadOrder = 30.0f;

    // this is how we register the ResourceManager with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

TextFileManager::~TextFileManager()
{
    // and this is how we unregister it
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

TextFilePtr TextFileManager::load(const Ogre::String &name, const Ogre::String &group)
{
    TextFilePtr textf = getByName(name);

    if (textf.isNull())
        textf = create(name, group);

    textf->load();
    return textf;
}

Ogre::Resource *TextFileManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                            const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                            const Ogre::NameValuePairList *createParams)
{
    return new TextFile(this, name, handle, group, isManual, loader);
}
