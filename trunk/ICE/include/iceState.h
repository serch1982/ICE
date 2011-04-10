#ifndef __ICE_STATE_H__
#define __ICE_STATE_H__

class iceState
{
	public:
	static iceState* getInstance();
	//Deleting the instance
	static void deleteInstance(){
		if( pinstance != 0 ) 
			delete pinstance;
	}
		iceState();
		~iceState();

	//Game States
	enum STATE{
		MENU,
		PLAY,
		CONTINUE,
		PAUSE,
		LOAD_LEVEL,
		NEXT_LEVEL,
		GAME_OVER,
		GOD,
		EXIT
	};

	void setState( STATE peState );
	int getState();

	private:
		STATE eState;

		//Singleton
		static iceState* pinstance;
};


#endif