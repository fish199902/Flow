#ifndef FLOW_H_
#define FLOW_H_

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "Ogre.h"

/**
 * \class PhysicsListener
 * Ogre FrameListener responsible for updating the physics simulation
 * before each frame.
 */
class PhysicsListener : public Ogre::FrameListener {
    public:
    PhysicsListener(btDiscreteDynamicsWorld* physicsWorld);
    bool frameStarted(const Ogre::FrameEvent& evt);

    private:
    btDiscreteDynamicsWorld *world;
};

/**
 * \class Flow
 * Controls initialization and game state
 */
class Flow {
    public:
    /// Clean up Bullet, Ogre and anything else we have created
    ~Flow();

    /**
     * Initialize Ogre3D. This includes only startup initialization,
     * and will not be used to change levels.
     * \return Success
     */
    bool initializeOgre();

    /**
     * Initialize Bullet Physics. This includes only startup
     * initialization, and will not be used to change levels.
     * \return Success
     */
    bool initializeBullet();

    /**
     * Set game mode.
     * \param twoPlayer Setup two player mode
     * \param battleMode If true, both players will be in the same scene, instead of separated in a side-by-side race.
     * \return Success
     */
    bool setupFlow(bool twoPlayer=false, bool battleMode=false);

    /**
     * Run the game.
     * \return Return code
     */
    int runFlow();

    private:
    Ogre::Root *root;
    Ogre::RenderWindow *window;
    Ogre::SceneManager *sceneMgr, *sceneMgr2;

    btDiscreteDynamicsWorld* dynamicsWorld;
    btAxisSweep3* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
};

#endif // FLOW_H_
