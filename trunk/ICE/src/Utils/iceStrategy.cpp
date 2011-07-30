#include "Utils\iceStrategy.h"
#include "Entities\icePlayer.h"
#include "Utils\iceSdkTray.h"


iceStrategy::~iceStrategy(void){

}

iceStrategy::AxisToMove iceStrategy::getPlayerAxis(void){
	Ogre::Vector3 camdir = icePlayer::getSingletonPtr()->getNode()->_getDerivedOrientation().zAxis();
	iceSdkTray::getInstance()->updateScreenInfo( 15, Ogre::StringConverter::toString(camdir.x) + " - " + Ogre::StringConverter::toString(camdir.y ) + " - " + Ogre::StringConverter::toString(camdir.z ));
	if((camdir.z > 0) && (camdir.z > camdir.x))
		return aZ;
	else 
		if((camdir.z < 0) && (camdir.z < camdir.x))
			return aZ;
		else 
			return aX;
}


iceStrategyForward::iceStrategyForward(Ogre::Real velocity) {
	_velocity = velocity;
}
iceStrategyForward::~iceStrategyForward() {

}

Ogre::Vector3 iceStrategyForward::move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame){
	return ((icePlayer::getSingletonPtr()->getPosition() - enemyPos) / _velocity) * timeSinceLastFrame; 
}


iceStrategySin::iceStrategySin(Ogre::Real radius,Ogre::Real  velocity, bool dir){
	_radius = radius;
	_counter = 0;
	_velocity = velocity;
	_xPos =0;
	_dir = dir;
}

iceStrategySin::~iceStrategySin(){

}


Ogre::Vector3 iceStrategySin::move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame){
	float grados,y, x;
	grados=  _radius * ( Ogre::Math::PI / 180);
	y = grados *  Ogre::Math::Sin(_counter * timeSinceLastFrame);
	_counter++;

	if (_dir){
		x = _velocity * timeSinceLastFrame;
		_xPos++;
		if ( _xPos > _radius * 3) _dir = false;
	}else {
		x = -_velocity * timeSinceLastFrame;
		_xPos--;
		if ( _xPos < -_radius * 3) _dir = true;
	}
	if (getPlayerAxis() == iceStrategy::aZ)
		return Ogre::Vector3(x , y , (_velocity / 10)* timeSinceLastFrame ); 
	else 
		return Ogre::Vector3((_velocity / 10)* timeSinceLastFrame ,y,x); 
}


iceStrategyCircle::iceStrategyCircle(Ogre::Real velocity, Ogre::Real radius, bool dir) {
	_radius = radius;
	_velocity = velocity;
	_dir = dir;
	if(dir) {
		_counter = 0;
	}
	else {
		_counter = 360;
	}
}

iceStrategyCircle::~iceStrategyCircle(){

}



Ogre::Vector3 iceStrategyCircle::move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame){
	float grados,y, x;
	grados=  _radius * ( Ogre::Math::PI / 180);
	y= (grados * Ogre::Math::Sin(_counter)) ;
	x= (grados * Ogre::Math::Cos(_counter)) ;

	if (_dir){
		_counter += _velocity * timeSinceLastFrame;
		if(_counter > 360) _counter = 0;
	}
	else {
		_counter -= _velocity * timeSinceLastFrame;
		if(_counter < 0) _counter = 360;
	}
	
	if (getPlayerAxis() == iceStrategy::aZ)
		return Ogre::Vector3(x , y , (_velocity / 10)* timeSinceLastFrame ); 
	else 
		return Ogre::Vector3( _velocity, y, x) ; 
}




