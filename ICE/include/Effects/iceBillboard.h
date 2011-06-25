#ifndef ICEBILLBOARD_H__
#define ICEBILLBOARD_H__

#include <Ogre.h>

class iceBillboard
{
public:
	enum BillType{
		DEAD1 = 1,
		DEAD2 = 2
	};

	iceBillboard(Ogre::SceneNode* sceneNode, float lifeTime,BillType billtype);
	~iceBillboard();
		
	void start(Ogre::Vector3);
	void update(const float elapsedSeconds);

private:
	Ogre::SceneNode* _sceneNode;

	float _lifeTime, _currentTime;
};


#endif