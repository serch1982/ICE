#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H
 
// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainMaterialGeneratorA.h>
#include <vector>


#include "Trajectory\iceStep.h"
#include "Trajectory\iceTrajectory.h"
#include "Trajectory\iceLocomotiveTrajectory.h"
#include "Enemies\iceEnemy.h"
#include "Enemies\iceKamikaze.h"
#include "Enemies\iceMini.h"
#include "Enemies\iceSmart.h"
#include "Entities\iceObject.h"
 
#include "rapidxml.hpp"
 
    // Forward declarations
    namespace Ogre
    {
        class SceneManager;
        class SceneNode;
        class TerrainGroup;
        class TerrainGlobalOptions;
    }
 
    class nodeProperty
    {
    public:
        Ogre::String nodeName;
        Ogre::String propertyNm;
        Ogre::String valueName;
        Ogre::String typeName;
 
        nodeProperty(const Ogre::String &node, const Ogre::String &propertyName, const Ogre::String &value, const Ogre::String &type)
            : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
    };
 
    class DotSceneLoader
    {
    public:
        Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;
 
        DotSceneLoader();
        virtual ~DotSceneLoader();
 
        void parseDotScene(const Ogre::String &SceneName, icePlayer &p_Player, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, const Ogre::String &sPrependNode = "");
        Ogre::String getProperty(const Ogre::String &ndNm, const Ogre::String &prop);
 
        Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }
 
        std::vector<nodeProperty> nodeProperties;
        std::vector<Ogre::String> staticObjects;
        std::vector<Ogre::String> dynamicObjects;

		std::vector<iceStep> getPlayerSteps(void);//deprecated
		std::vector<iceTrajectory*> getTrajectories();
		std::vector<iceEnemy*> getEnemies(void);
		std::vector<iceObject*> getObjects(void);
 
    protected:
        void processScene(rapidxml::xml_node<>* XMLRoot, icePlayer &p_Player);
 
        void processNodes(rapidxml::xml_node<>* XMLNode, icePlayer &p_Player);
        void processExternals(rapidxml::xml_node<>* XMLNode);
        void processEnvironment(rapidxml::xml_node<>* XMLNode);
        void processTerrain(rapidxml::xml_node<>* XMLNode);
        void processTerrainPage(rapidxml::xml_node<>* XMLNode);
		void initTerrainBlendMaps(Ogre::Terrain* terrain);
        void processBlendmaps(rapidxml::xml_node<>* XMLNode);
        void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
        void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::Entity *pEntity);
        void processOctree(rapidxml::xml_node<>* XMLNode);
        void processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
        void processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
 
        void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void processStep(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void processTrajectories();
		void processTrajectoryStep(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void processEnemies(rapidxml::xml_node<>* XMLNode, icePlayer &p_Player, Ogre::SceneNode *pParent = 0);
        void processLookTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processTrackTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
        void processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
 
        void processFog(rapidxml::xml_node<>* XMLNode);
        void processSkyBox(rapidxml::xml_node<>* XMLNode);
        void processSkyDome(rapidxml::xml_node<>* XMLNode);
        void processSkyPlane(rapidxml::xml_node<>* XMLNode);
        void processClipping(rapidxml::xml_node<>* XMLNode);
 
        void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
        void processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
 
        Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
        Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
        bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, bool defaultValue = false);
 
        Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
        Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
        Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);
 
 
        Ogre::SceneManager *mSceneMgr;
        Ogre::SceneNode *mAttachNode;
        Ogre::String m_sGroupName;
        Ogre::String m_sPrependNode;
        Ogre::TerrainGroup* mTerrainGroup;
        Ogre::Vector3 mTerrainPosition;
        Ogre::Vector3 mLightDirection;

		int mEnemyId, auxId;


		//ice
		std::vector<iceStep> mPlayerSteps;
		std::vector<std::vector<iceStep>> mTrajectoriesSteps;
		std::vector<iceTrajectory*> mTrajectories;
		std::vector<iceEnemy*> mEnemies;
		std::vector<iceObject*> mObjects;
    };
 
#endif // DOT_SCENELOADER_H