#ifndef ICESTRATEGY_H_
#define ICESTRATEGY_H_

#include <Ogre.h>
//abstract class to implement and improve the moviments of the enemies
class iceStrategy
{
public:
	//enum to change axis to move
	enum AxisToMove{
		aX, aZ
	};
	/**
    *  descontructor
    */
	virtual ~iceStrategy() = 0;
	/**
	*  to implement the movement
    *  @return Ogre::Vector3
    */
	virtual Ogre::Vector3 move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame) = 0;
	/**
	*  change the direction
    */
	virtual void reverse(void) = 0;
protected:
	//vars
	Ogre::Real _radius, _velocity, _velocityZ;
	Ogre::Real _counter, _numRand, _counterAux;
	bool _dir;
	//function to get the player axis
	AxisToMove getPlayerAxis(void);
};
typedef boost::shared_ptr<iceStrategy> iceStrategyPtr;



/**
*  rectilinear movement toward the to player
*/
class iceStrategyForward : public iceStrategy
{
public:
	/**
    *  contructor
	*  @velocity = velocity to the movement
    */
	iceStrategyForward(Ogre::Real velocityZ);
	/**
    *  descontructor
    */
	~iceStrategyForward();
	/**
	*  get the next movement
    *  @return Ogre::Vector3
    */
	Ogre::Vector3 move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame);
	/**
	*  change the direction
    */
	void reverse(void);
};

/**
*  sine movement
*/
class iceStrategySin : public iceStrategy
{
public:
	/**
	*  to implement the movement
	*  @radius = radiuos, @velocity = velocity, @dir = forward or inverse
    *  @return Ogre::Vector3
    */
	iceStrategySin(Ogre::Real radius,Ogre::Real  velocity, Ogre::Real velocityZ, bool dir);
	/**
    *  descontructor
    */
	~iceStrategySin();
	/**
	*  get the next movement
    *  @return Ogre::Vector3
    */
	Ogre::Vector3 move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame);
	/**
	*  change the direction
    */
	void reverse(void);
};


/**
*  circular movement 
*/
class iceStrategyCircle: public iceStrategy
{
public:
	/**
	*  to implement the movement
	*  @radius = radiuos, @velocity = velocity, @dir = forward or inverse
    *  @return Ogre::Vector3
    */
	iceStrategyCircle(Ogre::Real velocity,Ogre::Real velocityZ,  Ogre::Real radius, bool dir);
	/**
    *  descontructor
    */
	~iceStrategyCircle();
	/**
	*  get the next movement
    *  @return Ogre::Vector3
    */
	Ogre::Vector3 move(Ogre::Vector3 enemyPos, Ogre::Real timeSinceLastFrame);
	/**
	*  change the direction
    */
	void reverse(void);
};
#endif