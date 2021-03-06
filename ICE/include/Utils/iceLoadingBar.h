#pragma once

#include "OgreResourceGroupManager.h"
#include "OgreException.h"
#include "OgreOverlay.h"
#include "OgreOverlayManager.h"
#include "OgreRenderWindow.h"

/** Defines an example loading progress bar which you can use during 
	startup, level changes etc to display loading progress. 
@remarks
	Basically you just need to create an instance of this class, call start()
	before loading and finish() afterwards. You may also need to stop areas of
	your scene rendering in between since this method will call 
	RenderWindow::update() to update the display of the bar - we advise using
	SceneManager's 'special case render queues' for this, see
	SceneManager::addSpecialCaseRenderQueue for details.
@note 
	This progress bar relies on you having the OgreCore.zip package already 
	added to a resource group called 'Bootstrap' - this provides the basic 
	resources required for the progress bar and will be loaded automatically.
*/
class iceLoadingBar : public Ogre::ResourceGroupListener
{
protected:
	Ogre::RenderWindow* mWindow;
	Ogre::Overlay* mLoadOverlay;
	Ogre::Real mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	Ogre::Real mProgressBarMaxSize;
	Ogre::Real mProgressBarScriptSize;
	Ogre::Real mProgressBarInc;
	Ogre::Overlay* mBackGround;
	Ogre::OverlayElement* mLoadingBarElement;
	Ogre::OverlayElement* mLoadingDescriptionElement;
	Ogre::OverlayElement* mLoadingCommentElement;
	Ogre::String mCaption;

public:
	iceLoadingBar() {}
	virtual ~iceLoadingBar(){}

	/** Show the loading bar and start listening.
	@param window The window to update 
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken
		up by initialisation (ie script parsing etc). Defaults to 0.7 since
		script parsing can often take the majority of the time.
	*/
	virtual void start(Ogre::RenderWindow* window, 
		unsigned short numGroupsInit = 1, 
		unsigned short numGroupsLoad = 1, 
		Ogre::Real initProportion = 0.70f,
		Ogre::String background_overlay = "Loading/Scott",
		Ogre::String caption = "Please Wait, The ICE GAME is Loading...")
	{
		mWindow = window;
		mNumGroupsInit = numGroupsInit;
		mNumGroupsLoad = numGroupsLoad;
		mInitProportion = initProportion;
		mCaption = caption;
		// We need to pre-initialise the 'Bootstrap' group so we can use
		// the basic contents in the loading screen
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Bootstrap");

		Ogre::OverlayManager& omgr = Ogre::OverlayManager::getSingleton();
		mBackGround = (Ogre::Overlay*)omgr.getByName(background_overlay);
		mLoadOverlay = (Ogre::Overlay*)omgr.getByName("Core/LoadOverlay");
		if (!mLoadOverlay)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
				"Cannot find loading overlay", "iceLoadingBar::start");
		}
		mBackGround->show();
		mLoadOverlay->show();

		// Save links to the bar and to the loading text, for updates as we go
		mLoadingBarElement = omgr.getOverlayElement("Core/LoadPanel/Bar/Progress");
		mLoadingCommentElement = omgr.getOverlayElement("Core/LoadPanel/Comment");
		mLoadingDescriptionElement = omgr.getOverlayElement("Core/LoadPanel/Description");
		mLoadingDescriptionElement->setHeight(Ogre::Real(1.0));
		Ogre::OverlayElement* barContainer = omgr.getOverlayElement("Core/LoadPanel/Bar");
		mProgressBarMaxSize = barContainer->getWidth();
		mLoadingBarElement->setWidth(0);

		// self is listener
		Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
	}

	/** Hide the loading bar and stop listening. 
	*/
	virtual void finish(void)
	{
		// hide loading screen
		mLoadOverlay->hide();
		mBackGround->hide();

		// Unregister listener
		Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);

	}


	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
	{
		assert(mNumGroupsInit > 0 && "You stated you were not going to init "
			"any groups, but you did! Divide by zero would follow...");
		// Lets assume script loading is 70%
		mProgressBarInc = mProgressBarMaxSize * mInitProportion / (Ogre::Real)scriptCount;
		mProgressBarInc /= mNumGroupsInit;
		mLoadingDescriptionElement->setCaption(mCaption);
		
		mWindow->update();
	}
	void scriptParseStarted(const Ogre::String& scriptName, bool &skipThisScript)
	{
		mLoadingCommentElement->setCaption(scriptName);
		mWindow->update();
	}
	void scriptParseEnded(const Ogre::String& scriptName, bool skipped)
	{
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	}
	void resourceGroupScriptingEnded(const Ogre::String& groupName)
	{
	}
	void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
	{
		assert(mNumGroupsLoad > 0 && "You stated you were not going to load "
			"any groups, but you did! Divide by zero would follow...");
		mProgressBarInc = mProgressBarMaxSize * (1-mInitProportion) / 
			(Ogre::Real)resourceCount;
		mProgressBarInc /= mNumGroupsLoad;
		mLoadingDescriptionElement->setCaption("Loading resources...");
		mWindow->update();
	}
	void resourceLoadStarted(const Ogre::ResourcePtr& resource)
	{
		mLoadingCommentElement->setCaption(resource->getName());
		mWindow->update();
	}
	void resourceLoadEnded(void)
	{
	}
	void worldGeometryStageStarted(const Ogre::String& description)
	{
		mLoadingCommentElement->setCaption(description);
		mWindow->update();
	}
	void worldGeometryStageEnded(void)
	{
		mLoadingBarElement->setWidth(
			mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	}
	void resourceGroupLoadEnded(const Ogre::String& groupName)
	{
	}

	Ogre::DataStreamPtr resourceLoading(const Ogre::String &name, const Ogre::String &group, Ogre::Resource *resource)
	{
		return Ogre::DataStreamPtr();
	}

	bool resourceCollision(Ogre::ResourcePtr &resource, Ogre::ResourceManager *resourceManager)
	{
		return false;
	}

};

