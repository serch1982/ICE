#ifndef __ICE_LEVEL_H__
#define __ICE_LEVEL_H__

class iceLevel
{
	public:
		iceLevel();
		~iceLevel();

	protected:
		bool initialize();
		void finalize();
		void update();
};

#endif