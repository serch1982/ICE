/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the particle systems
	*/
	class ParticleSystem
	{
	public:
		struct ParticleSystemParameters
		{
			String	script;
			int		subtype;
			float	repeatMin;
			float	repeatMax;
		};

	public:
		ParticleSystem(SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, ParticleSystemParameters params, String id);
		~ParticleSystem();
		
		void start(void);
		void startAndStopFade(Ogre::Real stopTime);
		void stop(void);
		void update(const float elapsedSeconds);
	
		void attachTo(SceneNode* node);
		void detachFrom(SceneNode* node);

		// Parameters functions
		ParticleUniverse::ParticleSystem* getParticleSystem(){ return mParticleSystem; }
		ParticleSystemParameters getParameters(){ return mParameters; }
		SceneNode* getSceneNode(){ return mSceneNode; }
		String getId(){ return mId; }

		// Time functions
		float getRepeat(){ return mRepeat; }
		void newRepeat(){ fmodf(rand(), ( mParameters.repeatMax - mParameters.repeatMin )) + mParameters.repeatMin; }
		float getTimer(){ return mTimer; }
		void setTimer(float time){ mTimer = time; }

	private:
		ParticleUniverse::ParticleSystem*	mParticleSystem;
		ParticleSystemParameters			mParameters;
		Ogre::SceneNode*					mSceneNode;
		String								mId;

		float		mRepeat;
		float		mTimer;
	};

	typedef boost::shared_ptr<ParticleSystem> ParticleSystemPtr;
}

#endif // __PARTICLE_SYSTEM_H__