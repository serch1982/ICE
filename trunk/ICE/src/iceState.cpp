#include "iceState.h"
#include <stdlib.h>

//Initializing pointer
iceState* iceState::pinstance = 0;
iceState* iceState::getInstance(){
	if( pinstance == 0 ){
		pinstance = new iceState;
		atexit(&deleteInstance);
	}
	return pinstance;
}

iceState::iceState(){

}

iceState::~iceState(){
	
}

void iceState::setState( STATE peState ){
	eState = peState;
}

int iceState::getState(){
	return eState;
}
