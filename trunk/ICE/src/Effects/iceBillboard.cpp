#include "Effects\iceBillboard.h"
#include "iceGame.h"

iceBillboard::iceBillboard(Ogre::SceneNode* sceneNode, float lifeTime,BillType billtype): _currentTime(0){
	_sceneNode = sceneNode;
	Ogre::BillboardSet* billboard = iceGame::getSceneManager()->createBillboardSet();
	switch (billtype){
		case DEAD1:
			billboard->setMaterialName("ice/crashpff");
			billboard->createBillboard(0,0,0);
			_sceneNode->attachObject(billboard);
			_sceneNode->setScale(0.2,0.2,0.2);
			break;
		case DEAD2:
			billboard->setMaterialName("ice/crashboom");
			billboard->createBillboard(0,0,0);
			_sceneNode->attachObject(billboard);
			_sceneNode->setScale(0.2,0.2,0.2);
			break;
	}
	
	_lifeTime =lifeTime;
	_sceneNode->setVisible(false);
}

iceBillboard::~iceBillboard(){

}
		
void iceBillboard::start(Ogre::Vector3 position){
	_currentTime = 0;
	_sceneNode->setPosition(position);
	_sceneNode->setVisible(true);
}

void iceBillboard::update(const float elapsedSeconds){
	_currentTime++;
	if(_currentTime  >= _lifeTime ){
		_sceneNode->setVisible(false);
		_currentTime = 0;
	}
}