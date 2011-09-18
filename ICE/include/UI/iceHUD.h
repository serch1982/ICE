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
		void showWeaponUpgrade(unsigned int pWeapon);


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
		std::vector<Ogre::PanelOverlayElement*> mWeaponNotificationPanels;

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
};

#endif