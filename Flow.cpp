#include "config.h"
#include "Flow.h"

#include "OgreRoot.h"

#include <iostream>

using namespace std;

PhysicsListener::PhysicsListener(btDiscreteDynamicsWorld* physicsWorld) {
    world = physicsWorld;
};

bool PhysicsListener::frameStarted(const Ogre::FrameEvent& evt) {
    /// \todo Test for best value for maxSubSteps
    /// \todo Check that evt.timeSinceLastFrame returns seconds
    world->stepSimulation(evt.timeSinceLastFrame, 6);
    return true;
};

Flow::~Flow() {
    // Clean up bullet
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
};

bool Flow::initializeOgre() {
    // Create root object
    root = new Ogre::Root();

    // Setup resources
    root->loadPlugin("Plugin_OctreeSceneManager");

    /// \todo This whole renderer selection part is 'brittle'. Rewrite it.
    root->loadPlugin("RenderSystem_GL");
    #ifdef _WINDOWS
    root->loadPlugin("RenderSystem_Direct3D9");
    #endif

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    /*
    // Set RenderSystem
    Ogre::RenderSystemList *list = root->getAvailableRenderers();
    Ogre::RenderSystemList::iterator iter = list->begin();
    Ogre::RenderSystem *cur = 0;
    while (iter != list->end()) {
        cur = *(iter++);
        #ifdef _WINDOWS
        if (cur->getName().find("Direct3D")) {
        #else
        if (cur->getName().find("OpenGL")) {
        #endif
            cout << "***Setting render system plugin " << cur->getName() << endl;
            root->setRenderSystem(cur);
            break;
        }
    }

    if (cur == 0) {
        // Couldn't load RenderSystem
        return false;
    }*/

    if (!root->showConfigDialog())
        return false;

    // Initialize the Root object
    //root->initialize(true, Ogre::String("test"), Ogre::StringUtil::BLANK);
    //root->initialise(false);

    // Create a render window
    root->initialise(false);
    window = root->createRenderWindow(
        TITLE,   // Set window title
        640, 480, false, 0);       // Set size to 1024x768 windowed

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // All further initalization will be done later as it depends on game type.
    return true;
};

bool Flow::initializeBullet() {
    int maxProxies = 1024;

    // World is 10km in each direction
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);

    // Setup broadphase
    broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

    // Collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // Physics solver
    /// \todo Consider using a threaded solver here.
    solver = new btSequentialImpulseConstraintSolver;

    // World
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    // Add physics frame listener
    root->addFrameListener(new PhysicsListener(dynamicsWorld));

    // Any further physics configuration will be done when loading a level
    return true;
};


bool Flow::setupFlow(bool twoPlayer, bool battleMode) {
    // Create the main SceneManager
    cout << "Creating scene managers" << endl;
    sceneMgr = root->createSceneManager(Ogre::ST_GENERIC, "scene1");
    Ogre::Camera *camera = sceneMgr->createCamera("camera1");
    Ogre::Viewport *viewport = window->addViewport(camera); // Add the main viewport, we will set the size later as it depends on whether or not splitscreen will be used

    if (twoPlayer) {
        return false; /// \todo Implement two player mode.

        // Set the first viewport to fill only the left half of the screen
        viewport->setDimensions(0.0f, 0.0f, 0.5f, 1.0f);

        Ogre::Camera* camera2;
        Ogre::Viewport* viewport2;

        if (!battleMode) {
            // Create the second sceneManager, viewport, and camera
            sceneMgr2 = root->createSceneManager(Ogre::ST_GENERIC, "scene2");
            camera2 = sceneMgr2->createCamera("camera1"); // Create Camera 1 in SceneManager 2
            viewport2 = window->addViewport(camera2, 1, 0.5f, 0.0f, 1.0f, 1.0f);
        }
        else {
            // Create only the second viewport and camera
            camera2 = sceneMgr->createCamera("camera2"); // Create Camera 2 in SceneManager 1
            viewport2 = window->addViewport(camera2, 1, 0.5f, 0.0f, 1.0f, 1.0f);
        }
        camera2->setAspectRatio((float)viewport2->getActualWidth() / (float) viewport2->getActualHeight());
        camera2->setFarClipDistance(1000.0f);
        camera2->setNearClipDistance(5.0f);

        camera->setAspectRatio((float)viewport->getActualWidth() / (float) viewport->getActualHeight());
        camera->setFarClipDistance(1000.0f);
        camera->setNearClipDistance(5.0f);
    }
    else {
        viewport->setDimensions(0.0f, 0.0f, 1.0f, 1.0f);
        camera->setAspectRatio((float)viewport->getActualWidth() / (float) viewport->getActualHeight());
        camera->setFarClipDistance(1000.0f);
        camera->setNearClipDistance(5.0f);
    }

    return true;
};


int Flow::runFlow() {
    if (!initializeOgre())
        return 100;
    else if (!initializeBullet())
        return 101;
    else if (!setupFlow(false))
        return 102;

    sceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );
    Ogre::Entity *ent1 = sceneMgr->createEntity( "Robot", "robot.mesh" );
    Ogre::SceneNode *node1 = sceneMgr->getRootSceneNode()->createChildSceneNode( "RobotNode" );
    node1->attachObject( ent1 );

    for (int i = 0; i < 600; i++) {
        root->renderOneFrame();
    }

    return 0;
}
