#ifndef _ICE_HUD_H_
#define _ICE_HUD_H_

#include <OGRE/Ogre.h>
#include "States\iceStateManager.h"

#define FACE_2_TIME 1
#define ACHIEVEMENT_TIME 2

class iceHUD
{
    public:
        iceHUD();
		~iceHUD();

		void init(iceStateManager* pStateManager);

		void show();
		void hide();

		void update(Ogre::Real pTimeSinceLastFrame);

		void setLife(unsigned int pLife, unsigned int pMaxLife);
		void setWeapon(unsigned int pWeapon, unsigned int pLevel);
		void setLevel(unsigned int pLevel);
		void setFace(unsigned int pFace);
		void showAchievement(Ogre::String pAchievement);

		void hideAllNotifications(void);
		void showHealAvailable(void);
		void showSprintAvailable(void);
		void showBrakeAvailable();
		void showWeaponUpgrade(unsigned int pWeapon);

		void hideHealAvailable();
		void hideSprintAvailable();
		void hideBrakeAvailable();

		void showFrame();
		void hideFrame();

		void showGameover();
		void hideGameover();

		void setCheating(bool pCheating);

	protected:
		std::vector<std::string> mWeaponNames;
		std::vector<Ogre::Real> mWeaponCharSizes;
		unsigned int mCurrentWeapon;

		icePlayer* mPlayer;

		iceStateManager* mStateManager;
		Ogre::Overlay* mHUDOverlay;

		Ogre::TextAreaOverlayElement* mAchievementsTextArea;
		Ogre::TextAreaOverlayElement* mWeaponNameTextarea;
		Ogre::TextAreaOverlayElement* mWeaponLevelTextarea;
		std::vector<Ogre::PanelOverlayElement*> mWeaponPicturePanels;

		Ogre::TextAreaOverlayElement* mPlayerNameTextarea;
		Ogre::TextAreaOverlayElement* mLevelWordTextarea;
		Ogre::TextAreaOverlayElement* mLevelTextarea;

		std::vector<Ogre::PanelOverlayElement*> mFacesPanels;
		Ogre::PanelOverlayElement* mLifeBarPanel;
		Ogre::Real mMaxLifeBarWidth;
		Ogre::Real mLifeBarLeftMargin;

		Ogre::PanelOverlayElement* mAchievementPanel;
		Ogre::TextAreaOverlayElement* mAchievementTextarea;

		Ogre::PanelOverlayElement* mNotificationsPanel;
		Ogre::PanelOverlayElement* mHealNotificationPanel;
		Ogre::PanelOverlayElement* mSprintNotificationPanel;
		Ogre::PanelOverlayElement* mBrakeNotificationPanel;
		std::vector<Ogre::PanelOverlayElement*> mWeaponNotificationPanels;

		Ogre::PanelOverlayElement* mDisplayHealPanel;
		Ogre::PanelOverlayElement* mDisplaySprintPanel;
		Ogre::PanelOverlayElement* mDisplayBrakePanel;

		Ogre::PanelOverlayElement* mTopFramePanel;
		Ogre::PanelOverlayElement* mBottomFramePanel;

		Ogre::PanelOverlayElement* mGameoverPanel;

		Ogre::TextAreaOverlayElement* mCheatingTextarea;

		unsigned int mCurrentFace;
		bool mShowingFace2;
		Ogre::Real mFace2Time;

		bool mShowingAchievement;
		Ogre::Real mShowAchievementTime;

		bool mShowinwg;

		void _showFace(unsigned int pFace);
		void _hideFace(unsigned int pFace);

		Ogre::Real mShowNotificationTime;
		bool mShowingNotification;

		Ogre::Real mShowingFrameTime;
		Ogre::Real mHidingFrameTime;
		Ogre::Real mFrameHeight;

		bool mCheating;
};

#endif