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

#ifndef _BtOgrePG_H_
#define _BtOgrePG_H_

#include "btBulletDynamicsCommon.h"
#include "BtOgreExtras.hpp"
#include "Ogre.h"

namespace BtOgre {

typedef std::map<unsigned char, Vector3Array*> BoneIndex;
typedef std::pair<unsigned short, Vector3Array*> BoneKeyIndex;

class VertexIndexToShape
{
public:
	VertexIndexToShape(const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
	~VertexIndexToShape();

	Ogre::Real getRadius();
	Ogre::Vector3 getSize();


	btSphereShape* createSphere();
	btBoxShape* createBox();
	btBvhTriangleMeshShape* createTrimesh();
	btCylinderShape* createCylinder();
	btConvexHullShape* createConvex();

	const Ogre::Vector3* getVertices();
	unsigned int getVertexCount();
	const unsigned int* getIndices();
	unsigned int getIndexCount();

protected:

	void addStaticVertexData(const Ogre::VertexData *vertex_data);

	void addAnimatedVertexData(const Ogre::VertexData *vertex_data,
		const Ogre::VertexData *blended_data,
		const Ogre::Mesh::IndexMap *indexMap);

	void addIndexData(Ogre::IndexData *data, const unsigned int offset = 0);


protected:
	Ogre::Vector3*	    mVertexBuffer;
	unsigned int*       mIndexBuffer;
	unsigned int        mVertexCount;
	unsigned int        mIndexCount;

	Ogre::Matrix4		mTransform;

	Ogre::Real		    mBoundRadius;
	Ogre::Vector3		mBounds;

	BoneIndex           *mBoneIndex;
};

//For static (non-animated) meshes.
class StaticMeshToShapeConverter : public VertexIndexToShape
{
public:

	StaticMeshToShapeConverter(Ogre::Renderable *rend, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
	StaticMeshToShapeConverter(Ogre::Entity *entity,   const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
	StaticMeshToShapeConverter();

	~StaticMeshToShapeConverter();

	void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);

	void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);


protected:

	Ogre::Entity*		mEntity;
	Ogre::SceneNode*	mNode;
};

//For animated meshes.
class AnimatedMeshToShapeConverter : public VertexIndexToShape
{
public:

	AnimatedMeshToShapeConverter(Ogre::Entity *entity, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
	AnimatedMeshToShapeConverter();
	~AnimatedMeshToShapeConverter();

	void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
	void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform);

	btBoxShape* createAlignedBox(unsigned char bone,
		const Ogre::Vector3 &bonePosition,
		const Ogre::Quaternion &boneOrientation);

	btBoxShape* createOrientedBox(unsigned char bone,
		const Ogre::Vector3 &bonePosition,
		const Ogre::Quaternion &boneOrientation);

protected:

	bool getBoneVertices(unsigned char bone,
		unsigned int &vertex_count,
		Ogre::Vector3* &vertices,
		const Ogre::Vector3 &bonePosition);

	bool getOrientedBox(unsigned char bone,
		const Ogre::Vector3 &bonePosition,
		const Ogre::Quaternion &boneOrientation,
		Ogre::Vector3 &extents,
		Ogre::Vector3 *axis,
		Ogre::Vector3 &center);


	Ogre::Entity*		mEntity;
	Ogre::SceneNode*	mNode;

	Ogre::Vector3       *mTransformedVerticesTemp;
	size_t               mTransformedVerticesTempSize;
};

}

#endif
