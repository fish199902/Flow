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
 */

#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <vector>
#include <sstream>

#include "../btogre/BtOgreSoftBody.hpp"

#include "../btogre/BtOgreExtras.hpp"
#include "../btogre/BtOgreGP.hpp"
#include "../btogre/BtOgrePG.hpp"

// Forward declarations
class TiXmlElement;

namespace Ogre
{
    // Forward declarations
    class SceneManager;
    class SceneNode;

    class nodeProperty
    {
        public:
            String nodeName;
            String propertyNm;
            String valueName;
            String typeName;

            nodeProperty(const String &node, const String &propertyName, const String &value, const String &type)
                    : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
    };

    class DotSceneLoader
    {
        public:
            DotSceneLoader() : mSceneMgr(0) {}
            virtual ~DotSceneLoader() {}

            void parseDotScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, btSoftRigidDynamicsWorld *physWorld, SceneNode *pAttachNode = NULL, const String &sPrependNode = "");
            String getProperty(const String &ndNm, const String &prop);

            std::vector<nodeProperty>   nodeProperties;
            std::vector<String>         staticObjects;
            std::vector<String>         dynamicObjects;

        protected:
            void processScene(TiXmlElement *XMLRoot);

            void processNodes(TiXmlElement *XMLNode);
            void processExternals(TiXmlElement *XMLNode);
            void processEnvironment(TiXmlElement *XMLNode);
            void processTerrain(TiXmlElement *XMLNode);
            void processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent = 0);
            void processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity);
            void processOctree(TiXmlElement *XMLNode);
            void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
            void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);

            void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
            void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
            void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
            void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
            void processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent);
            void processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent);
            void processPlane(TiXmlElement *XMLNode, SceneNode *pParent);
            void processPhysics(TiXmlElement *XMLNode, Entity *pEntity, SceneNode *pNode);

            void processFog(TiXmlElement *XMLNode);
            void processSkyBox(TiXmlElement *XMLNode);
            void processSkyDome(TiXmlElement *XMLNode);
            void processSkyPlane(TiXmlElement *XMLNode);
            void processClipping(TiXmlElement *XMLNode);

            void processLightRange(TiXmlElement *XMLNode, Light *pLight);
            void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);

            String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
            Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
            bool getAttribBool(TiXmlElement *XMLNode, const String &parameter, bool defaultValue = false);

            Vector3 parseVector3(TiXmlElement *XMLNode);
            Quaternion parseQuaternion(TiXmlElement *XMLNode);
            ColourValue parseColour(TiXmlElement *XMLNode);


            SceneManager *mSceneMgr;
            SceneNode *mAttachNode;
            String m_sGroupName;
            String m_sPrependNode;

            btSoftRigidDynamicsWorld     *physicsWorld;
    };
}

#endif // DOT_SCENELOADER_H
