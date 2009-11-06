/*
 *  BtSoftBody.h
 *  pixtream-ogre
 *
 *  Created by Luiz Geovani Vier on <lgvier@gmail.com> 4/6/09.
 *  Copyright 2009 pixtream.com.br. All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
 */

#ifndef __BtOgreSoftBody_H__
#define __BtOgreSoftBody_H__

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"
#include "BtOgreExtras.h"
#include "BtOgreGP.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "Uncopyable.h"

#include <vector>
#include <map>
using namespace std;
using namespace Ogre;

namespace BtOgre
{
	
	class StaticMeshToSoftBodyConverter : public BtOgre::StaticMeshToShapeConverter {
		
	protected:
		btSoftBodyWorldInfo* mWorldInfo;
		
	public:
		StaticMeshToSoftBodyConverter(btSoftBodyWorldInfo* worldInfo, Ogre::Renderable *rend, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToSoftBodyConverter(btSoftBodyWorldInfo* worldInfo, Ogre::Entity *entity,   const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		StaticMeshToSoftBodyConverter(btSoftBodyWorldInfo* worldInfo);
		btSoftBody* createSoftBodyFromTrimesh();
		
	};
	
	class BtOgreSoftBody : private Uncopyable {
		
	protected:
		btSoftBodyWorldInfo* mWorldInfo;
		Entity *mEntity;
		int *mDupVertices;
		int *mNewIndexes;
		int mDupVerticesCount;
		btSoftBody *mSoftBody;
		int internalId;
	
		int getBulletIndex(int idx);
		
	public:
		BtOgreSoftBody(btSoftBodyWorldInfo* worldInfo);
		~BtOgreSoftBody();
		
		btSoftBody* create(Entity *entity, const unsigned int vertexCount, const Vector3* vertexBuffer, const unsigned int indexCount, const unsigned int* indexBuffer);
		void updateOgreMesh();
		
		static void updateOgreMeshes(btDynamicsWorld *world);
		
	};
	
	
}

#endif