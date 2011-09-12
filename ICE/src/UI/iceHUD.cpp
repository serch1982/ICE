#include <UI/iceHUD.h>
#include "iceGame.h"

iceHUD::iceHUD()
{
	iceGame::getGameLog()->logMessage("iceMenu::iceHUD()");
}

iceHUD::~iceHUD()
{
	iceGame::getGameLog()->logMessage("iceMenu::~iceHUD()");
}

void iceHUD::init(iceStateManager* pStateManager)
{
	iceGame::getGameLog()->logMessage("iceHUD::init()");

	mWeaponNames.push_back("MINIGUN");
	mWeaponNames.push_back("SHOTGUN");
	mWeaponNames.push_back("MISSILE");

	mWeaponCharSizes.push_back(22);
	mWeaponCharSizes.push_back(21);
	mWeaponCharSizes.push_back(25);

	mStateManager = pStateManager;

	mHUDOverlay = Ogre::OverlayManager::getSingleton().getByName("HUD/hud_overlay");

	mAchievementsTextArea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_text"));
	mWeaponNameTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_name_text"));
	mWeaponLevelTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_level_text"));

	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_1_picture_panel")));
	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_2_picture_panel")));
	mWeaponPicturePanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/weapon_3_picture_panel")));

	mPlayerNameTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/player_name_text"));
	mLevelTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/player_level_text"));

	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_1_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_2_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_3_panel")));
	mFacesPanels.push_back(static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/face_4_panel")));

	mAchievementPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_panel"));
	mAchievementTextarea = static_cast<Ogre::TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/achievements_text"));

	mLifeBarPanel = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().getOverlayElement("HUD/life_bar_panel"));
	mMaxLifeBarWidth = mLifeBarPanel->getWidth();
	mLifeBarLeftMargin = mLifeBarPanel->getLeft();

	for(unsigned int i=0;i<mWeaponPicturePanels.size();i++)
	{
		mWeaponPicturePanels[i]->hide();
	}
	mCurrentWeapon = 0;

	for(unsigned int i=0;i<mFacesPanels.size();i++)
	{
		mFacesPanels[i]->hide();
	}
	mCurrentFace = 1;
	_showFace(1);
	mShowingAchievement = false;
	mAchievementPanel->hide();
	mUpdateOnce = true;
}

void iceHUD::show()
{
	mHUDOverlay->show();
}

void iceHUD::hide()
{
	mHUDOverlay->hide();	
}

void iceHUD::update(Ogre::Real pTimeSinceLastFrame)
{
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

	if(mUpdateOnce)
	{
		setWeapon(1,1);
		mPlayerNameTextarea->setCaption("SCOTT");
		setLevel(1);
		//mAchievementTextarea->setCaption("Ninja");
		mUpdateOnce = false;
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