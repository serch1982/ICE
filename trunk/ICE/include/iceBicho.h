#ifndef __ICE_BICHO_H__
#define __ICE_BICHO_H__

class iceBicho
{
	public:
		iceBicho();
		~iceBicho();
		
	protected:
		bool initialize();
		void finalize();
		void update();
};

#endif