#ifndef TEXTFILE_H_
#define TEXTFILE_H_

/// \todo Adopt this code

#include <OgreResourceManager.h>

class TextFile : public Ogre::Resource
{
    Ogre::String mString;

protected:

    // must implement these from the Ogre::Resource interface
    void loadImpl();
    void unloadImpl();
    size_t calculateSize() const;

public:

    TextFile(Ogre::ResourceManager *creator, const Ogre::String &name,
        Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
        Ogre::ManualResourceLoader *loader = 0);

    virtual ~TextFile();

    void setString(const Ogre::String &str);
    const Ogre::String &getString() const;
};

class TextFilePtr : public Ogre::SharedPtr<TextFile>
{
public:
    TextFilePtr() : Ogre::SharedPtr<TextFile>() {}
    explicit TextFilePtr(TextFile *rep) : Ogre::SharedPtr<TextFile>(rep) {}
    TextFilePtr(const TextFilePtr &r) : Ogre::SharedPtr<TextFile>(r) {}
    TextFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<TextFile>()
    {
        // lock & copy other mutex pointer
        OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<TextFile*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
    }

    /// Operator used to convert a ResourcePtr to a TextFilePtr
    TextFilePtr& operator=(const Ogre::ResourcePtr& r)
    {
        if (pRep == static_cast<TextFile*>(r.getPointer()))
            return *this;
        release();
        // lock & copy other mutex pointer
        OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<TextFile*>(r.getPointer());
        pUseCount = r.useCountPointer();
        if (pUseCount)
        {
            ++(*pUseCount);
        }
        return *this;
    }
};

#endif // TEXTFILE_H_
