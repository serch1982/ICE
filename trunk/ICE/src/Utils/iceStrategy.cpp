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
/*---------------------------------------------------------------------------------------------------------------------------*/

iceStrategyForward::iceStrategyForward(Ogre::Real velocityZ) {
	_velocityZ = velocityZ;
}
iceStrategyForward::~iceStrategyForward() {

}

Ogre::Vector3 iceStrategyForward::move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame){
	return ((icePlayer::getSingletonPtr()->getPosition() - enemyPos) / _velocityZ) * timeSinceLastFrame; 
}

void iceStrategyForward::reverse(void){

}
/*---------------------------------------------------------------------------------------------------------------------------*/

iceStrategySin::iceStrategySin(Ogre::Real radius,Ogre::Real  velocity,Ogre::Real  velocityZ, bool dir){
	_radius = radius;
	_counter = 0;
	_velocity = velocity;
	_velocityZ = velocityZ;
	_counterAux =1;
	_dir = dir;
	_numRand = Ogre::Math::RangeRandom(45.0,90.0);
}

iceStrategySin::~iceStrategySin(){

}


Ogre::Vector3 iceStrategySin::move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame){
	float grados,y;
	grados=  _radius * ( Ogre::Math::PI / 180);
	y = grados *  Ogre::Math::Sin(_counter);
	Ogre::Vector3 translation = Ogre::Vector3::ZERO;

	_counterAux++;
	if(_counterAux > _numRand )  {
			_dir = !_dir;
			_counterAux = 1;
	}

	if (getPlayerAxis() == iceStrategy::aX){
		if (_dir){
			translation += Ogre::Vector3::UNIT_X ;
			translation += Ogre::Vector3::UNIT_Z * _velocity;
			translation += Ogre::Vector3::UNIT_Y * y;
			_counter += _velocity * timeSinceLastFrame;
			if(_counter > 360) _counter = 0;
		}
		else {
			translation += Ogre::Vector3::NEGATIVE_UNIT_X;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Z * _velocity;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Y * y;
			_counter -= _velocity * timeSinceLastFrame;
			if(_counter < 0) _counter = 360;
		}
		translation.normalise();
		translation.x *= _velocityZ;
	}
	else {
		if (_dir){
			translation += Ogre::Vector3::UNIT_Z;
			translation += Ogre::Vector3::UNIT_X * _velocity;
			translation += Ogre::Vector3::UNIT_Y * y;
			_counter += _velocity * timeSinceLastFrame;
			if(_counter > 360) _counter = 0;
		}
		else {
			translation += Ogre::Vector3::NEGATIVE_UNIT_Z;
			translation += Ogre::Vector3::NEGATIVE_UNIT_X * _velocity;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Y * y;
			_counter -= _velocity * timeSinceLastFrame;
			if(_counter < 0) _counter = 360;
		}
		translation.normalise();
		translation.z *= _velocityZ;
	} 
	
	return translation ;
}

void iceStrategySin::reverse(void){
	_dir = !_dir;
	_numRand = Ogre::Math::RangeRandom(_numRand,90.0);
}

/*---------------------------------------------------------------------------------------------------------------------------*/
iceStrategyCircle::iceStrategyCircle(Ogre::Real velocity, Ogre::Real velocityZ, Ogre::Real radius, bool dir) {
	_radius = radius;
	_velocity = velocity;
	_velocityZ = velocityZ;
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
	Ogre::Vector3 translation = Ogre::Vector3::ZERO;

	if (getPlayerAxis() == iceStrategy::aX){
		if (_dir){
			translation += Ogre::Vector3::UNIT_X;
			translation += Ogre::Vector3::UNIT_Z * x;
			translation += Ogre::Vector3::UNIT_Y * y;
			_counter += _velocity * timeSinceLastFrame;
			if(_counter > 360) _counter = 0;
		}
		else {
			translation += Ogre::Vector3::NEGATIVE_UNIT_X ;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Z * x;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Y * y;
			_counter -= _velocity * timeSinceLastFrame;
			if(_counter < 0) _counter = 360;
		}
		translation.normalise();
		translation.x *= _velocityZ;
	}
	else {
		if (_dir){
			translation += Ogre::Vector3::UNIT_Z;
			translation += Ogre::Vector3::UNIT_X * x;
			translation += Ogre::Vector3::UNIT_Y * y;
			_counter += _velocity * timeSinceLastFrame;
			if(_counter > 360) _counter = 0;
		}
		else {
			translation += Ogre::Vector3::NEGATIVE_UNIT_Z;
			translation += Ogre::Vector3::NEGATIVE_UNIT_X * x;
			translation += Ogre::Vector3::NEGATIVE_UNIT_Y * y;
			_counter -= _velocity * timeSinceLastFrame;
			if(_counter < 0) _counter = 360;
		}
		translation.normalise();
		translation.z *= _velocityZ;
	} 
	return translation ;

}

void iceStrategyCircle::reverse(void){
	_dir = !_dir;
}
/*---------------------------------------------------------------------------------------------------------------------------*/


