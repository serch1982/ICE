#include <UI/iceHUD.h>
#include "iceGame.h"

#define SHOW_NOTIFICATION_TIME 2
#define SHOW_FRAME_TIME 1

iceHUD::iceHUD()
{
	iceGame::getGameLog()->logMessage("iceMenu::iceHUD()");
	mShowinwg = false;
}

iceHUD::~iceHUD()
{
	iceGame::getGameLog()->logMessage("iceMenu::~iceHUD()");
}

void iceHUD::init(iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceHUD::init()");

	mPlayer = icePlayer::getSingletonPtr();

	mWeaponNames.push_back("MINIGUN");
	mWeaponNames.push_back("SHOTGUN");
	mWeaponNames.push_back("MISSILE");

	mWeaponCharSizes.push_back(22);
	mWeaponCharSizes.push_back(21);
	mWeaponCharSizes.push_back(25);

	mStateManager = pStateManager;

	mHUDOverlay = Ogre::OverlayManager::getSingleton().getByName("HUD/hud_overlay");

	mStatsPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/stats_panel"));
	mWeaponsPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapons_panel"));

	mAchievementsTextArea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_text"));
	mWeaponNameTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_name_text"));
	mWeaponLevelTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_level_text"));

	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_1_picture_panel")));
	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_2_picture_panel")));
	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_3_picture_panel")));

	mPlayerNameTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/player_name_text"));
	mLevelWordTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/level_word_text"));
	mLevelTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/player_level_text"));

	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_1_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_2_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_3_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_4_panel")));

	mNotificationsPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_container_panel"));
	mHealNotificationPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_heal_panel"));
	mSprintNotificationPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_sprint_panel"));
	mBrakeNotificationPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_brake_panel"));
	mWeaponNotificationPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_weapon_1_panel")));
	mWeaponNotificationPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_weapon_2_panel")));
	mWeaponNotificationPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/notification_weapon_3_panel")));

	mDisplayHealPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/heal_availability_container"));
	mDisplaySprintPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/sprint_availability_container"));
	mDisplayBrakePanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/brake_availability_container"));

	mAchievementPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_panel"));
	mAchievementTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_text"));

	mLifeBarPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/life_bar_panel"));
	mMaxLifeBarWidth = mLifeBarPanel->getWidth();
	mLifeBarLeftMargin = mLifeBarPanel->getLeft();

	mTopFramePanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/frame_top"));
	mBottomFramePanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/frame_bottom"));

	mGameoverPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/gameover_panel"));

	mCheatingTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/cheating_text"));

	mFrameHeight = mTopFramePanel->getHeight();
	mTopFramePanel->hide();
	mBottomFramePanel->hide();
	mShowingFrameTime = 0;
	mHidingFrameTime = 0;

	for(unsigned int i=0;i<mWeaponPicturePanels.size();i++)
	{
		mWeaponPicturePanels[i]->hide();
	}
	mCurrentWeapon = 0;

	for(unsigned int i=0;i<mFacesPanels.size();i++)
	{
		mFacesPanels[i]->hide();
	}
	
	hideAllNotifications();
	//mDisplayHealPanel->hide();
	//mDisplaySprintPanel->hide();
	//mDisplayBrakePanel->hide();

	mCurrentFace = 1;
	_showFace(1);
	mShowingAchievement = false;
	mAchievementPanel->hide();
	mShowinwg = false;

	mGameoverPanel->hide();
	mCheating = false;
}

void iceHUD::show()
{
	mHUDOverlay->show();
	mShowinwg = true;
}

void iceHUD::hide()
{
	mShowinwg = false;
	mHUDOverlay->hide();	
}

void iceHUD::update(Ogre::Real pTimeSinceLastFrame)
{
	if(!mShowinwg)
		return;
	if(!mPlayer)
		mPlayer = icePlayer::getSingletonPtr();

	if(mShowingFace2)
	{
		mFace2Time -= pTimeSinceLastFrame;

		if(mFace2Time <= 0)
		{
			mShowingFace2 = false;
			_hideFace(2);
			_showFace(mCurrentFace);
		}
	}

	if(mShowingAchievement)
	{
		mShowAchievementTime -= pTimeSinceLastFrame;

		if(mShowAchievementTime <= 0)
		{
			mShowingAchievement = false;
			mAchievementPanel->hide();
		}
	}

	//if(mUpdateOnce)
	//{
		//setWeapon(1,1);
		mPlayerNameTextarea->setCaption("SCOTT");
		mLevelWordTextarea->setCaption("level");
		setWeapon(mPlayer->getCurrentWeapon()+1,mPlayer->getWeaponLevel(mPlayer->getCurrentWeapon()));
		setLevel(mPlayer->getLevel());
		//setLevel(1);
		////mAchievementTextarea->setCaption("Ninja");
		//mUpdateOnce = false;
	//}

	if(mShowingNotification)
	{
		mShowNotificationTime -= pTimeSinceLastFrame;

		if(mShowNotificationTime < 0)
		{
			hideAllNotifications();
		}
	}

	if(mShowingFrameTime > 0)
	{
		mShowingFrameTime -= pTimeSinceLastFrame;
		if(mShowingFrameTime <= 0)
		{
			mTopFramePanel->setTop(0);
			mBottomFramePanel->setTop(-mFrameHeight);
		}
		else
		{
			Ogre::Real offset = (SHOW_FRAME_TIME - mShowingFrameTime) * mFrameHeight / SHOW_FRAME_TIME;
			mTopFramePanel->setTop(-mFrameHeight + offset);
			mBottomFramePanel->setTop(-offset);
		}
	}

	if(mHidingFrameTime > 0)
	{
		mHidingFrameTime -= pTimeSinceLastFrame;
		if(mHidingFrameTime <= 0)
		{
			mTopFramePanel->setTop(-mFrameHeight);
			mBottomFramePanel->setTop(0);
			mTopFramePanel->hide();
			mBottomFramePanel->hide();
		}
		else
		{
			Ogre::Real offset = (SHOW_FRAME_TIME - mHidingFrameTime) * mFrameHeight / SHOW_FRAME_TIME;
			mTopFramePanel->setTop(-offset);
			mBottomFramePanel->setTop(-mFrameHeight + offset);
		}
	}
	
	if(mCheating)
	{
		mCheatingTextarea->setCaption("Cheating mode ON");
	}
	else
	{
		mCheatingTextarea->setCaption("");
	}
}

void iceHUD::setLife(unsigned int pLife, unsigned int pMaxLife)
{
	Ogre::Real life = (Ogre::Real) pLife / (Ogre::Real) pMaxLife;
	Ogre::Real width = mMaxLifeBarWidth * life;
	Ogre::Real left = mLifeBarLeftMargin + (mMaxLifeBarWidth - width);
	Ogre::Real topLeftU = 1.0f - life;

	mLifeBarPanel->setWidth(width);
	mLifeBarPanel->setLeft(left);
	mLifeBarPanel->setUV(topLeftU,0,1,1);

	if(life > 0.66f)
	{
		setFace(1);
	}
	else if (life > 0.33f)
	{
		setFace(3);
	}
	else
	{
		setFace(4);
	}
}

void iceHUD::setWeapon(unsigned int pWeapon, unsigned int pLevel)
{
	char level[16];
	sprintf(level,"LEVEL %d",pLevel);
	mWeaponNameTextarea->setCharHeight(mWeaponCharSizes[pWeapon-1]);
	mWeaponNameTextarea->setCaption(mWeaponNames[pWeapon-1]);
	mWeaponLevelTextarea->setCaption(level);
	mWeaponPicturePanels[mCurrentWeapon]->hide();
	mCurrentWeapon = pWeapon-1;
	mWeaponPicturePanels[mCurrentWeapon]->show();
}

void iceHUD::setLevel(unsigned int pLevel)
{
	char level[4];
	sprintf(level,"%02d",pLevel);
	mLevelTextarea->setCaption(level);
}

void iceHUD::setFace(unsigned int pFace)
{
	if(!mShowingFace2) 
	{
		_hideFace(mCurrentFace);
		_showFace(pFace);
	}

	if(pFace == 2)
	{
		mShowingFace2 = true;
		mFace2Time = FACE_2_TIME;
	}
	else if(mCurrentFace != pFace)
	{
		mCurrentFace = pFace;
	}
}

void iceHUD::showAchievement(Ogre::String pAchievement)
{
	mAchievementsTextArea->setCaption(pAchievement);
	mAchievementPanel->show();
	mShowingAchievement = true;
	mShowAchievementTime = ACHIEVEMENT_TIME;
}

void iceHUD::_showFace(unsigned int pFace)
{
	mFacesPanels[pFace-1]->show();
}

void iceHUD::_hideFace(unsigned int pFace)
{
	mFacesPanels[pFace-1]->hide();
}

void iceHUD::hideAllNotifications(void)
{
	mHealNotificationPanel->hide();
	mSprintNotificationPanel->hide();
	mBrakeNotificationPanel->hide();
	for(unsigned int i=0;i<mWeaponNotificationPanels.size();i++)
	{
		mWeaponNotificationPanels[i]->hide();
	}
	mNotificationsPanel->hide();
	mShowNotificationTime = 0;
	mShowingNotification = false;
}

void iceHUD::showHealAvailable(void)
{
	hideAllNotifications();
	mNotificationsPanel->show();
	mHealNotificationPanel->show();
	mShowNotificationTime = SHOW_NOTIFICATION_TIME;
	mShowingNotification = true;
	mDisplayHealPanel->show();
}

void iceHUD::showSprintAvailable(void)
{
	hideAllNotifications();
	mNotificationsPanel->show();
	mSprintNotificationPanel->show();
	mShowNotificationTime = SHOW_NOTIFICATION_TIME;
	mShowingNotification = true;
	mDisplaySprintPanel->show();
}

void iceHUD::showBrakeAvailable(void)
{
	hideAllNotifications();
	mNotificationsPanel->show();
	mBrakeNotificationPanel->show();
	mShowNotificationTime = SHOW_NOTIFICATION_TIME;
	mShowingNotification = true;
	mDisplayBrakePanel->show();
}

void iceHUD::showWeaponUpgrade(unsigned int pWeapon)
{
	hideAllNotifications();
	mNotificationsPanel->show();
	mWeaponNotificationPanels[pWeapon]->show();
	mShowNotificationTime = SHOW_NOTIFICATION_TIME;
	mShowingNotification = true;
}

void iceHUD::hideHealAvailable()
{
	mDisplayHealPanel->hide();
}

void iceHUD::hideSprintAvailable()
{
	mDisplaySprintPanel->hide();
}

void iceHUD::hideBrakeAvailable()
{
	mDisplayBrakePanel->hide();
}

void iceHUD::showFrame()
{
	mShowingFrameTime = SHOW_FRAME_TIME;
	mTopFramePanel->show();
	mBottomFramePanel->show();
	mTopFramePanel->setTop(-mFrameHeight);
	mBottomFramePanel->setTop(0);
	mStatsPanel->hide();
	mWeaponsPanel->hide();
}

void iceHUD::hideFrame()
{
	mHidingFrameTime = SHOW_FRAME_TIME;
	mTopFramePanel->show();
	mBottomFramePanel->show();
	mTopFramePanel->setTop(0);
	mBottomFramePanel->setTop(-mFrameHeight);
	mStatsPanel->show();
	mWeaponsPanel->show();
}

void iceHUD::showGameover()
{
	mGameoverPanel->show();
}

void iceHUD::hideGameover()
{
	mGameoverPanel->hide();
}

void iceHUD::setCheating(bool pCheating)
{
	mCheating = pCheating;
}