/* Copyright 2009 Austin Brown
 *
 * This file is part of Flow.
 *
 * Flow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Flow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Flow.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 * Author: Nikhilesh (nikki)
 *
 * The BtOgre license is based on the Bullet (zlib) license, which is as follows:-
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 */

#ifndef _BtOgreGP_H_
#define _BtOgreGP_H_

#include "btBulletDynamicsCommon.h"
#include "OgreSceneNode.h"
#include "BtOgreExtras.hpp"

namespace BtOgre {

//A MotionState is Bullet's way of informing you about updates to an object.
//Pass this MotionState to a btRigidBody to have your SceneNode updated automaticaly.
class RigidBodyState : public btMotionState
{
    public:
	RigidBodyState(const btTransform &transform, Ogre::SceneNode *node)
	    : mNode(node),
	    mTransform(transform)
	{
	}

	RigidBodyState(Ogre::SceneNode *node)
	    : mNode(node),
	      mTransform(((node != NULL) ? BtOgre::Convert::toBullet(node->getOrientation()) : btQuaternion(0,0,0,1)),
		         ((node != NULL) ? BtOgre::Convert::toBullet(node->getPosition())    : btVector3(0,0,0)))
	{

	}

	virtual ~RigidBodyState()
	{
	}

	void setNode(Ogre::SceneNode *node)
	{
	    mNode = node;
	}

	virtual void getWorldTransform(btTransform &retVal) const
	{
	    retVal = mTransform;
	}

	void setKinematicPos(btTransform &transform)
	{
	    mTransform = transform;
	}

	virtual void setWorldTransform(const btTransform &transform)
	{
	    if (mNode == NULL)
		return;

	    btQuaternion rot = transform.getRotation();
	    mNode->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	    btVector3 pos = transform.getOrigin();
	    mNode->setPosition(pos.x(), pos.y(), pos.z());
	}

    protected:
	Ogre::SceneNode *mNode;
	btTransform mTransform;
};

//Softbody-Ogre connection goes here!

}

#endif
