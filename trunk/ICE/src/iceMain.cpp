#include "iceGame.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include <direct.h>
    #define GetCurrentDir _getcwd
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
	//int main(int argc, char **argv)
#else
	#include <unistd.h>
    #define GetCurrentDir getcwd
    int main(int argc, char *argv[])
#endif
    {
		iceGame* myGame;// = new iceGame();
		/*char root_path[1024];
		GetCurrentDir(root_path, 1023);
		myGame->go(root_path);*/ 
		try {
					myGame = new iceGame();
				char root_path[1024];
				GetCurrentDir(root_path, 1023);
				myGame->go(root_path);
			} catch( Ogre::Exception& e ) {
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
					MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
					//Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
	#else
					std::cerr << "An exception has occured: " <<
						e.getFullDescription().c_str() << std::endl;
	#endif
			}

			delete myGame;

        return 0;
    }

#ifdef __cplusplus
}
#endif
