#ifndef __ICE_PLAYER_H__
#define __ICE_PLAYER_H__

class icePlayer
{
	public:
		icePlayer();
		~icePlayer();

	protected:
		bool initialize();
		void finalize();
		void update();
};

#endif