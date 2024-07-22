#pragma once

#pragma once



#define OFFSET_ENTITYLIST			0x1d71858  //cl_entitylist 
#define OFFSET_LOCAL_ENT			0x211fac8  //(.?AVC_GameMovement@@ + 0x8) //ok

#define OFFSET_TEAM					0x037c //m_iTeamNum
#define OFFSET_HEALTH			    0x036c //m_iHealth
#define OFFSET_SHIELD				0x01a0 //m_shieldHealth
#define OFFSET_MAX_SHIELD 		    0x01a4	//m_shieldHealthMax
#define OFFSET_NAME				    0x04b9	 //m_iName	
#define OFFSET_Armortype 			0x4634	//m_armortype


#define OFFSET_SIG_NAME				0x04b0 //m_iSignifierName

#define OFFSET_ITEM_ID	           0x1588 //m_customScriptInt

#define OFFSET_ORIGIN				0x017c //m_vecAbsOrigin
#define OFFSET_BONES				0x0dd0 +  0x48  //m_nForceBone
#define OFFSET_AIMPUNCH				0x2468  ///m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle 
#define OFFSET_CAMERAPOS			0x1f00 ///CPlayer!camera_origin
#define OFFSET_VIEWANGLES			0x2564- 0x14//m_ammoPoolCapacity //


#define OFFSET_ABS_VELOCITY       0x0170 ///m_vecAbsVelocity
#define OFFSET_CURRENT_WEAPON		0x1964 //m_latestPrimaryWeapon
#define OFFSET_BULLET_SPEED    0x19e0 + 0x04dc		  // WeaponSettingsMeta.base + WeaponSettings.projectile_launch_speed
#define OFFSET_BULLET_SCALE  0x19e0 + 0x04e4 // WeaponSettingsMeta.base + WeaponSettings.projectile_gravity_scale


#define OFFSET_RENDER			0x7389d40
#define OFFSET_MATRIX			0x11a350


#define OFFSET_VISIBLE_TIME         0x19c0

#define OFFSET_LIFE_STATE			0x06c8//m_lifeState, >0 = dead
#define OFFSET_BLEED_OUT_STATE		0x2710 //m_bleedoutState, >0 = knocked

#define OFFSET_BREATH_ANGLES		OFFSET_VIEWANGLES - 0x10







#define OFFSET_HIGHLIGHTSETTINGS 0xB5C5090
#define OFFSET_HIGHLIGHTSERVERACTIVESTATES  0x298
#define OFFSET_HIGHLIGHTCURRENTCONTEXTID  0x294
#define OFFSET_HIGHLIGHTVISIBILITYTYPE  0x278


//#define OFFSET_ENTITYLIST			0x1e213f8 //cl_entitylist 
//#define OFFSET_LOCAL_ENT			0x21d1738 /*+ 0x8*/  //(.?AVC_GameMovement@@ + 0x8) // LocalPlayer might be moved to AVC_GameMovement
//
//#define OFFSET_TEAM					0x037c //m_iTeamNum
//#define OFFSET_HEALTH				0x036c //m_iHealth
//#define OFFSET_SHIELD				0x01a0 //m_shieldHealth
//#define OFFSET_MAX_SHIELD 			0x01a4	//m_shieldHealthMax
//#define OFFSET_NAME					0x04b9	 //m_iName	
//#define OFFSET_Armortype 			0x45c4	//m_armortype
//
//
//#define OFFSET_SIG_NAME				0x04b0 //m_iSignifierName
//
//#define OFFSET_ITEM_ID	           0x1578 //m_customScriptInt
//
//#define OFFSET_ORIGIN				0x017c //m_vecAbsOrigin
//#define OFFSET_BONES				0x0dd0 +  0x48  //m_nForceBone
//#define OFFSET_AIMPUNCH				0x23f8  ///m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle 
//#define OFFSET_CAMERAPOS			0x1e90 ///CPlayer!camera_origin
//#define OFFSET_VIEWANGLES			0x24f4 - 0x14//m_ammoPoolCapacity //
//
//#define OFFSET_ABS_VELOCITY        0x0170 ///m_vecAbsVelocity
//
//#define OFFSET_MATRIX 0x11a350 //ViewMatrix
//#define OFFSET_RENDER 0x741f0a8 //ViewRender displays ESp, heath dist names etc
//
//
//#define OFFSET_VISIBLE_TIME      0x19B0
//
//#define OFFSET_LIFE_STATE			0x06c8 //m_lifeState, >0 = dead
//#define OFFSET_BLEED_OUT_STATE		0x26a0 //m_bleedoutState, >0 = knocked
//#define OFFSET_BREATH_ANGLES		OFFSET_VIEWANGLES - 0x10
//
//
//#define OFFSET_HIGHLIGHTSETTINGS 0xB5F8620
//#define OFFSET_HIGHLIGHTSERVERACTIVESTATES  0x298
//#define OFFSET_HIGHLIGHTCURRENTCONTEXTID  0x294
//#define OFFSET_HIGHLIGHTVISIBILITYTYPE  0x278
//
