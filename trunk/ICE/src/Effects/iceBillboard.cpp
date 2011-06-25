#include "Effects\iceBillboard.h"
#include "iceGame.h"

iceBillboard::iceBillboard(Ogre::SceneNode* sceneNode, float lifeTime,BillType billtype): _currentTime(0){
	_sceneNode = sceneNode;
	Ogre::BillboardSet* shotgunShotSet = iceGame::getSceneManager()->createBillboardSet();
	switch (billtype){
		case DEAD1:
			shotgunShotSet->setMaterialName("crashpff");
			shotgunShotSet->createBillboard(0,0,0);
			_sceneNode->attachObject(shotgunShotSet);
			_sceneNode->setScale(0.1,0.1,0.1);
			break;
		case DEAD2:
			shotgunShotSet->setMaterialName("crashboom");
			shotgunShotSet->createBillboard(0,0,0);
			_sceneNode->attachObject(shotgunShotSet);
			_sceneNode->setScale(0.1,0.1,0.1);
			break;
	}
	//_sceneNode = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode("bbcrash");
	
	_lifeTime =lifeTime;
	_sceneNode->setVisible(false);
	_sceneNode = sceneNode;
}

iceBillboard::~iceBillboard(){

}
		
void iceBillboard::start(Ogre::Vector3 position){
	_currentTime =0;
	_sceneNode->setPosition(position);
	_sceneNode->setVisible(true);
}

void iceBillboard::update(const float elapsedSeconds){
	_currentTime++;
	if(_currentTime >= _lifeTime){
		_sceneNode->setVisible(false);
	}
}