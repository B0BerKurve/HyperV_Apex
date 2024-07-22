#include <string>
#include <filesystem>
#include "sdk/../CBaseEntity.hpp"
#include "../sdk/Menu.h"
#include "../col.h"




vector<EntityCache> CachedPlayerList;

enum class LootType : int
{
	Ammo,
	Attachement,
	Deathbox,
	Equipment,
	Heal,
	Grenade,
	Weapon,
	Scope,
	Trash
};

std::string ReadStr(uint64_t src, int buffer)
{
	std::string result;
	char ret = { 0 };

	for (int i = 0; i < buffer; i++) {
		ret = Memory.Read<uint64_t>(src + i);
		result += ret;
	}
	return result;
}

void LoadPlayers()
{
	vector<EntityCache> TempList;

	uint64_t EntityList = Memory.BaseAddress + OFFSET_ENTITYLIST;
	/*std::cout << "EntityList: " << EntityList << "\n";*/

	if (EntityList == 0)
		return;


	uint64_t BaseEntity = Memory.Read<uint64_t>(EntityList);

	if (BaseEntity == 0)
		return;

	for (int i = 0; i < 80; i++)
	{
		uint64_t LocalEntity = Memory.Read<uint64_t>(Memory.BaseAddress + OFFSET_LOCAL_ENT);
		/*std::cout << "LocalEntity: " << LocalEntity << "\n";*/
		if (!LocalEntity)
			continue;

		uint64_t CurrentEntity = Memory.Read<uint64_t>(EntityList + ((uint64_t)i << 5));

		if (!CurrentEntity)
			continue;

		if (!entity::IsPlayer(CurrentEntity)or entity::IsDummy(CurrentEntity))
			continue;

		int health = Memory.Read<int>(CurrentEntity + OFFSET_HEALTH);

		if (health < 1 || entity::GetTeam(CurrentEntity) == entity::GetTeam(LocalEntity))
			continue;

		EntityCache EntityCache{ };

		EntityCache.player_entity = CurrentEntity;
		EntityCache.player_id = i;

		TempList.push_back(EntityCache);
	}

	CachedPlayerList = TempList;
	TempList.clear();
}



namespace ColorS
{
	inline  auto EliteWeapons = SColor(238, 115, 119, 255);
	inline  auto EnergyWeapons = SColor(124, 222, 68, 255);
	inline  auto HeavyWeapons = SColor(152, 236, 172, 255);
	inline  auto LightWeapons = SColor(247, 199, 87, 255);
	inline auto SniperWeapons = SColor(123, 154, 255, 255);
	inline  auto BowWeapons = SColor(247, 241, 59, 255);
	inline  auto ExtraWeapons = SColor(240, 27, 77, 255);
	inline  auto GrenadeWeapons = SColor(186, 186, 186, 255);

	inline  auto Level1 = SColor(239, 239, 239, 255);
	inline  auto Level2 = SColor(86, 164, 242, 255);
	inline  auto Level3 = SColor(229, 127, 255, 255);
	inline  auto Level4 = SColor(250, 240, 95, 255);
	inline  auto Level5 = SColor(247, 96, 101, 255);

	inline  auto Trash = SColor(223, 223, 223, 255);
	inline  auto WhitePastel = SColor(255, 255, 255, 255);

	inline  auto GreenAcid = SColor(143, 255, 77, 255);
	inline  auto KnockedColor = SColor(250, 182, 103, 255);;
}

vector<ItemCache> CachedItemList;




std::string GetLootName(int LootID, SColor* Color, LootType* LootType, int* LootLevel)
{
	switch (LootID)
	{
	case 1: *Color = ColorS::ExtraWeapons; *LootType = LootType::Weapon; return _xor_ ("Kraber");
	case 2: *Color = ColorS::EliteWeapons; *LootType = LootType::Weapon; return _xor_ ("Mastiff");
	case 7: *Color = ColorS::EnergyWeapons; *LootType = LootType::Weapon; return _xor_ ("L-Star");
	case 12: *Color = ColorS::EnergyWeapons; *LootType = LootType::Weapon; return _xor_ ("Havoc");
	case 17: *Color = ColorS::EnergyWeapons; *LootType = LootType::Weapon; return _xor_ ("Devotion LMG");
	case 22: *Color = ColorS::EnergyWeapons; *LootType = LootType::Weapon; return _xor_ ("Triple Take");
	case 27: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Flatline");
	case 32: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Hemlock");
	case 37: *Color = ColorS::ExtraWeapons; *LootType = LootType::Weapon; return _xor_ ("G7-Scout");
	case 42: *Color = ColorS::LightWeapons; *LootType = LootType::Weapon; return _xor_ ("Alternator");
	case 47: *Color = ColorS::LightWeapons; *LootType = LootType::Weapon; return _xor_ ("R-99");
	case 52: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Prowler");
	case 57: *Color = ColorS::ExtraWeapons; *LootType = LootType::Weapon; return _xor_ ("Volt");
	case 58: *Color = ColorS::SniperWeapons; *LootType = LootType::Weapon; return _xor_ ("Longbow");
	case 63: *Color = ColorS::SniperWeapons; *LootType = LootType::Weapon; return _xor_ ("Charge Rifle");
	case 68: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("M600");
	case 73: *Color = ColorS::LightWeapons; *LootType = LootType::Weapon; return _xor_ ("R-301");
	case 78: *Color = ColorS::EliteWeapons; *LootType = LootType::Weapon; return _xor_ ("EVA 8");
	case 83: *Color = ColorS::EliteWeapons; *LootType = LootType::Weapon; return _xor_ ("Peacekeeper");
	case 88: *Color = ColorS::EliteWeapons; *LootType = LootType::Weapon; return _xor_ ("Mozambique");
	case 93: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Wingman");
	case 98: *Color = ColorS::LightWeapons; *LootType = LootType::Weapon; return _xor_ ("P-2020");
	case 103: *Color = ColorS::LightWeapons; *LootType = LootType::Weapon; return _xor_ ("RE-45");
	case 108: *Color = ColorS::SniperWeapons; *LootType = LootType::Weapon; return _xor_ ("Sentinel");
	case 113: *Color = ColorS::BowWeapons; *LootType = LootType::Weapon; return _xor_ ("Bow");
	case 118: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Repeater");

	case 124: *Color = ColorS::LightWeapons; *LootType = LootType::Ammo; return _xor_ ("Ammo");
	case 125: *Color = ColorS::EnergyWeapons; *LootType = LootType::Ammo; return _xor_ ("Ammo");
	case 126: *Color = ColorS::EliteWeapons; *LootType = LootType::Ammo; return _xor_ ("Ammo");
	case 127: *Color = ColorS::HeavyWeapons; *LootType = LootType::Ammo; return _xor_ ("Ammo");
	case 128: *Color = ColorS::SniperWeapons; *LootType = LootType::Ammo; return _xor_ ("Ammo");

	case 129: *Color = ColorS::BowWeapons; *LootType = LootType::Ammo; return _xor_ ("Arrows");

	case 130: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("Rampage");
	case 135: *Color = ColorS::HeavyWeapons; *LootType = LootType::Weapon; return _xor_ ("CAR SMG");

	case 164: *Color = ColorS::Level2; *LootType = LootType::Heal; *LootLevel = 2; return _xor_ ("Accelerant");
	case 165: *Color = ColorS::Level3; *LootType = LootType::Heal; *LootLevel = 3; return _xor_ ("Phoenix");
	case 166: *Color = ColorS::Level2; *LootType = LootType::Heal; *LootLevel = 2; return _xor_ ("Medkit");
	case 167: *Color = ColorS::Level1; *LootType = LootType::Heal; *LootLevel = 1; return _xor_ ("Syringe");
	case 168: *Color = ColorS::Level2; *LootType = LootType::Heal; *LootLevel = 2; return _xor_ ("Battery");
	case 169: *Color = ColorS::Level1; *LootType = LootType::Heal; *LootLevel = 1; return _xor_ ("Cell");

	case 170: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("Helmet");
	case 171: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("Helmet");
	case 172: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Helmet");
	case 173: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("Helmet");

	case 174: *Color = ColorS::Level1; *LootType = LootType::Equipment; *LootLevel = 1; return _xor_ ("Armor");
	case 175: *Color = ColorS::Level2; *LootType = LootType::Equipment; *LootLevel = 2; return _xor_ ("Armor");
	case 176: *Color = ColorS::Level3; *LootType = LootType::Equipment; *LootLevel = 3; return _xor_ ("Armor");
	case 177: *Color = ColorS::Level4; *LootType = LootType::Equipment; *LootLevel = 4; return _xor_ ("Armor");

	case 179: *Color = ColorS::Level1; *LootType = LootType::Equipment; *LootLevel = 1; return _xor_ ("Armor");
	case 180: *Color = ColorS::Level2; *LootType = LootType::Equipment; *LootLevel = 2; return _xor_ ("Armor");
	case 181: *Color = ColorS::Level3; *LootType = LootType::Equipment; *LootLevel = 3; return _xor_ ("Armor");
	case 182: *Color = ColorS::Level5; *LootType = LootType::Equipment; *LootLevel = 4; return _xor_ ("Armor");

	case 184: *Color = ColorS::Level1; *LootType = LootType::Equipment; *LootLevel = 1; return _xor_ ("Shield");
	case 185: *Color = ColorS::Level2; *LootType = LootType::Equipment; *LootLevel = 2; return _xor_ ("Shield");
	case 186: *Color = ColorS::Level3; *LootType = LootType::Equipment; *LootLevel = 3; return _xor_ ("Shield");
	case 187: *Color = ColorS::Level4; *LootType = LootType::Equipment; *LootLevel = 4; return _xor_ ("Shield");

	case 188: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("Backpack");
	case 189: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("Backpack");
	case 190: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Backpack");
	case 191: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("Backpack");

	case 192: *Color = ColorS::GrenadeWeapons; *LootType = LootType::Grenade; return _xor_ ("Molotov");
	case 193: *Color = ColorS::GrenadeWeapons; *LootType = LootType::Grenade; return _xor_ ("Grenade");
	case 194: *Color = ColorS::GrenadeWeapons; *LootType = LootType::Grenade; return _xor_ ("ArcStar");

	case 195: *Color = ColorS::Level1; *LootType = LootType::Scope; *LootLevel = 1; return _xor_ ("1x HCOG");
	case 196: *Color = ColorS::Level2; *LootType = LootType::Scope; *LootLevel = 2; return _xor_ ("2x HCOG");

	case 197: *Color = ColorS::Level1; *LootType = LootType::Scope; *LootLevel = 1; return _xor_ ("1x Holo");
	case 198: *Color = ColorS::Level2; *LootType = LootType::Scope; *LootLevel = 2; return _xor_ ("2x Holo");

	case 199: *Color = ColorS::Level4; *LootType = LootType::Scope; *LootLevel = 4; return _xor_ ("1x Digital");

	case 200: *Color = ColorS::Level3; *LootType = LootType::Scope; *LootLevel = 3; return _xor_ ("3x HCOG");
	case 201: *Color = ColorS::Level3; *LootType = LootType::Scope; *LootLevel = 3; return _xor_ ("4x Holo");
	case 202: *Color = ColorS::Level2; *LootType = LootType::Scope; *LootLevel = 2; return _xor_ ("6x Sniper");
	case 203: *Color = ColorS::Level3; *LootType = LootType::Scope; *LootLevel = 3; return _xor_ ("8x Sniper");
	case 204: *Color = ColorS::Level4; *LootType = LootType::Scope; *LootLevel = 4; return _xor_ ("10x Sniper");

	case 205: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("Barrel");
	case 206: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("Barrel");
	case 207: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Barrel");

	case 209: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("LightMag");
	case 210: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("LightMag");
	case 211: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("LightMag");
	case 212: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("LightMag");

	case 213: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("HeavyMag");
	case 214: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("HeavyMag");
	case 215: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("HeavyMag");
	case 216: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("HeavyMag");

	case 217: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("EnergyMag");
	case 218: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("EnergyMag");
	case 219: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("EnergyMag");
	case 220: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("EnergyMag");

	case 221: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("SniperMag");
	case 222: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("SniperMag");
	case 223: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("SniperMag");
	case 224: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("SniperMag");

	case 225: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("Bolt");
	case 226: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("Bolt");
	case 227: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Bolt");

	case 228: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("Stock");
	case 229: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("Stock");
	case 230: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Stock");

	case 231: *Color = ColorS::Level1; *LootType = LootType::Attachement; *LootLevel = 1; return _xor_ ("SnStock");
	case 232: *Color = ColorS::Level2; *LootType = LootType::Attachement; *LootLevel = 2; return _xor_ ("SnStock");
	case 233: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("SnStock");

	case 234: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("Turbocharger");

	case 238: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("HammerPoint");

	case 245: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Tempo");
	case 247: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Shatter");
	case 248: *Color = ColorS::Level3; *LootType = LootType::Attachement; *LootLevel = 3; return _xor_ ("Kinetic");
	case 249: *Color = ColorS::Level4; *LootType = LootType::Attachement; *LootLevel = 4; return _xor_ ("Boost");

	case 1488: *Color = ColorS::Level4; *LootType = LootType::Deathbox; return _xor_ ("Deathbox");

	default:
		*Color = ColorS::Trash;
		*LootType = LootType::Trash;
		*LootLevel = 1;

		return "";
	}
}


void LoadLoot()
{
	while (true)
	{
		vector<ItemCache> TempList;

		uint64_t EntityList = Memory.BaseAddress + OFFSET_ENTITYLIST;

		if (EntityList == 0)
			return;

		for (size_t i = 0; i < 10000; i++)
		{
			uint64_t LocalEntity = Memory.Read<uint64_t>(Memory.BaseAddress + OFFSET_LOCAL_ENT);
			if (!LocalEntity)
				continue;

			uint64_t CurrentEntity = Memory.Read<uint64_t>(EntityList + ((uint64_t)i << 5));
			if (!CurrentEntity)
				continue;

			uint64_t iSignifierName = Memory.Read<uint64_t>(CurrentEntity + OFFSET_SIG_NAME);
			if (iSignifierName == NULL || iSignifierName == -1)
				continue;

			std::string objectName = "";
			objectName = ReadStr(iSignifierName, 128);

			if (objectName == "")
				continue;

			if (objectName != ("prop_survival"))
			{
				int ItemId = Memory.Read<int>(CurrentEntity + OFFSET_ITEM_ID);

				if (ItemId <= 0 && ItemId > 220)
					continue;

				if (settings.LootGlow)
				{
					entity::EnableGlowItem(CurrentEntity);
				}
			}
		}

		//CachedItemList = TempList;
		//TempList.clear();

		// Sleep for some time (adjust the sleep duration as needed)
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
}




VOID RenderItem(uintptr_t item, int itemId, uintptr_t localEntity)
{
	
	Vector3 FootPos3D = entity::GetEntityBasePosition(item);

	if (FootPos3D.IsNull())
		return;

	Vector3 HeadPos3D = FootPos3D;
	HeadPos3D.y += .5;
	Vector2 FootPos2D = Vector2();

	FootPos2D; world_to_screen(FootPos2D, HeadPos3D.x, HeadPos3D.y, HeadPos3D.z);
	if (FootPos2D.x == 0 || FootPos2D.y == 0); //continue;
	{
		/*if (FootPos2D.Zero())
			return;*/
		{
			SColor Color = SColor(0, 0, 0, 0);
			LootType Type;
			int Level;
			
			auto LootName = GetLootName(itemId, &Color, &Type, &Level);

			if (LootName.empty())
				return;

			if (Type == LootType::Trash)
				return;

			if (Type == LootType::Deathbox && !settings.LootDeadbox)
				return;

			auto DistFrom = Math::Distance(entity::GetEntityBasePosition(localEntity), FootPos3D);

			if (DistFrom == -1)
				return;

			

			if (settings.LootESP)
			{
				auto LootInfo = LootName;

				int middle[3] = { FootPos2D.y, FootPos2D.y, FootPos2D.y };

				int fontsize = 11;
				float offset = (LootInfo.size() * fontsize) / 5;
				//DrawString(12, FootPos2D.x - offset, middle[1], &Color, false, false, LootInfo.c_str());
				draw_text_outline(FootPos2D.x - offset, middle[1], &Color, LootInfo.c_str());
				middle[1] += fontsize;
			}
		}
	}  
}



inline float RandomFloat(float min, float max)
{
	if (max < min)
		return 0.0f;

	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;

	return (random * range) + min;
}

VOID FacePlayer(uint64_t LocalEntity, uint64_t PlayerEntity, QAngle FaceAngles)
{
	if (!FaceAngles.IsValid())
		return;

	if (entity::IsKnocked(LocalEntity) || (entity::IsKnocked(PlayerEntity)))
		return;

	QAngle LocalViewAngles = QAngle();
	LocalViewAngles = entity::GetViewAngles1(LocalEntity); //ok

	if (!LocalViewAngles.IsValid())
		return;

	QAngle AimAngles = FaceAngles;
	Math::NormalizeAngles(AimAngles);

	QAngle Delta = AimAngles - LocalViewAngles;
	Math::NormalizeAngles(Delta);

	if (!Delta.IsValid())
		return;

	auto RandomSmooth = 0;

	if (settings.AimBone == 4)
		RandomSmooth = (int)RandomFloat(4.0f, 11.0f);

	QAngle PlayerViewAngles = LocalViewAngles + (settings.AimBotSmooth > 0 ? settings.AimBone == 4 ? Delta / RandomSmooth : Delta / settings.AimBotSmooth : Delta);

	if (!PlayerViewAngles.IsValid())
		return;
	entity::SetViewAngles(LocalEntity,PlayerViewAngles);
}

static __forceinline auto GetAimBone() -> int
{
	auto Bone = 7;

	if (settings.AimBone == 0)
		Bone = 3;
	else if (settings.AimBone == 1)
		Bone = 7;
	else if (settings.AimBone == 2)
		Bone = 5;
	else if (settings.AimBone == 3)
		Bone = 4;

	return Bone;
}
float m_powf(float x, float y)
{
	return _mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(x), _mm_set_ss(y)));
}

float m_sqrtf(float x)
{
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}
double GetCrossDistance(double x1, double y1, double x2, double y2)
{
	return m_sqrtf(m_powf((float)(x1 - x2), (float)(2)) + m_powf((float)(y1 - y2), (float)2));
}


QAngle GetFaceAngles(uint64_t LocalEntity, uint64_t PlayerEntity)
{
	QAngle FaceAngles = QAngle();

	Vector3 LocalCamera = entity::GetCamPos(LocalEntity);

	if (!LocalCamera.IsValid())
		return FaceAngles;

	QAngle LocalViewAngles = QAngle();
	LocalViewAngles =  entity::GetViewAngles1(LocalEntity); //OK

	if (!LocalViewAngles.IsValid())
		return FaceAngles;

	Vector3 PlayerPosition = entity::GetEntityBasePosition(PlayerEntity);

	if (!PlayerPosition.IsValid())
		return FaceAngles;

	int CurrentAimBone = GetAimBone();

	auto RandomBoneId = 0;

	if (CurrentAimBone == 4)
		RandomBoneId = (int)RandomFloat(4.0f, 8.0f);

	Vector3 HeadPos3D = entity::GetEntityBonePosition(PlayerEntity, CurrentAimBone == 4 ? RandomBoneId : CurrentAimBone, PlayerPosition);

	if (!HeadPos3D.IsValid())
		return FaceAngles;

	if (CurrentAimBone == 4)
	{
		auto RandomX = RandomFloat(-5.0f, 7.0f);
		auto RandomY = RandomFloat(-5.0f, 7.0f);
		auto RandomZ = RandomFloat(-5.0f, 7.0f);

		HeadPos3D.x += RandomX;
		HeadPos3D.y += RandomY;
		HeadPos3D.z += RandomZ;
	}

	FaceAngles = Math::CalcAngle(LocalCamera, HeadPos3D);

	QAngle BreathAngles = entity::GetSwayAnglesA(LocalEntity);

	FaceAngles -= BreathAngles - LocalViewAngles;

	return FaceAngles;
}


//uint64_t GetClosestPlayerToCrossHair(Vector2 playerPos, float& maxDistance, uint64_t entity)
//{
//	if (entity)
//	{
//
//		//float crossDistance = GetCrossDistance(playerPos.x, playerPos.y, (ScreenCenterX), (ScreenCenterY));
//
//		//if (crossDistance < maxDistance)
//		{
//			float Radius = (settings.fov * ScreenCenterX /*/ cameraFov*/) / 2;
//
//			if (playerPos.x <= ((ScreenCenterX)+Radius) &&
//				playerPos.x >= ((ScreenCenterX)-Radius) &&
//				playerPos.y <= ((ScreenCenterY)+Radius) &&
//				playerPos.y >= ((ScreenCenterY)-Radius))
//			{
//				//maxDistance = crossDistance;
//
//				return entity;
//			}
//
//			return 0;
//		}
//	}
//
//	return 0;
//}

uint64_t GetClosestPlayerToCrossHair(Vector2 playerPos, float& maxDistance, uint64_t entity)
{
	float Dist = GetCrossDistance(playerPos.x, playerPos.y, (ScreenCenterX), (ScreenCenterY));

	if (entity)

	if (Dist < maxDistance)
	{
		float Radius = settings.aimbotfov;

		if (playerPos.x <= ((ScreenCenterX) + Radius) &&
			playerPos.x >= ((ScreenCenterX) - Radius) &&
			playerPos.y <= ((ScreenCenterY) + Radius) &&
			playerPos.y >= ((ScreenCenterY) - Radius))
		{
			maxDistance = Dist;
			return entity;
		}
		return 0;
	}
	return 0;
}


uint64_t GetPlayerByFOV(uintptr_t LocalEntity, uintptr_t PlayerEntity, float& max)
{
	if (!LocalEntity || !PlayerEntity)
		return 0;

	/*if (!IsPlayerAlive(LocalEntity) || !IsPlayerAlive(PlayerEntity))
		return 0;

	if (!IsPlayerVisible(LocalEntity, PlayerEntity))
		return 0;*/

	/*if (!entity::IsPlayerVisible(PlayerEntity, i));*/
		//return 0; 

	Vector3 LocalCamera = entity::GetCamPos(LocalEntity);

	if (!LocalCamera.IsValid())
		return 0;

	Vector3 PlayerPosition = entity::GetEntityBasePosition(PlayerEntity);

	if (!PlayerPosition.IsValid())
		return 0;

	Vector3 HeadPos3D = entity::GetEntityBonePosition(PlayerEntity, 7, PlayerPosition);

	if (!HeadPos3D.IsValid())
		return 0;


	Vector2  HeadPos2D; world_to_screen(HeadPos2D, HeadPos3D.x, HeadPos3D.y, HeadPos3D.z); {
		if (HeadPos2D.Zero())
			return 0;

		auto ClosestPlayer = GetClosestPlayerToCrossHair(HeadPos2D, max, PlayerEntity);

		if (ClosestPlayer)
			return ClosestPlayer;
	}

	return 0;
}


int ImDbgFormatString(char* buf, size_t buf_size, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int w = vsnprintf(buf, buf_size, fmt, args);
	va_end(args);
	if (buf == NULL)
		return w;
	if (w == -1 || w >= (int)buf_size)
		w = (int)buf_size - 1;
	buf[w] = 0;
	return w;
}




VOID DrawArmorBar(Vector2 HeadPos, Vector2 FootPos, WORD Armor, WORD MaxArmor)
{
	if (Armor <= 0)
		return;

	auto BarHeight = FootPos.y - HeadPos.y;
	auto BarWidth = (BarHeight / 2) * 1.25f;

	int BarX = (int)HeadPos.x - (BarWidth / 2);
	int BarY = (int)HeadPos.y;

	auto Percentage = Armor * (BarHeight / MaxArmor);
	auto Deduction = BarHeight - Percentage;


	Color allcol;

	auto ArmorColor = &allcol.greens; // &allcol.red

	if (MaxArmor > 100)
		ArmorColor = &allcol.white;
	else if (MaxArmor == 100)
		ArmorColor = &allcol.purple;
	else if (MaxArmor == 75)
		ArmorColor = &allcol.purple; //240, 27, 77, 255
	else if (MaxArmor == 50)
		ArmorColor = &allcol.red_;

	//DrawCornerBox(BarX - 6, BarY, 3, (int)BarHeight, 1.5, &allcol.black);
	DrawFilledRect(BarX - 10, BarY + Deduction, 3, Percentage, ArmorColor);
}

VOID RenderEntity(uintptr_t entity, uintptr_t localent)
{
	/*if (!MemoryUtils::IsAddressValid(pMaterialSurface) || !pMaterialSurface)
		return;*/

	{
		  Vector3 pos_base = entity::GetEntityBasePosition(entity);
		  if (pos_base.x == 0 || pos_base.y == 0);

		  Vector3 pos_head = entity::GetEntityBonePosition(entity, 8, pos_base);
		  if (pos_head.x == 0 || pos_head.y == 0);

		  Vector3 pos_body = entity::GetEntityBonePosition(entity, 5, pos_base);
		  if (pos_body.x == 0 || pos_body.y == 0);

		  Vector2 screen_base; world_to_screen(screen_base, pos_base.x, pos_base.y, pos_base.z);
		  if (screen_base.x == 0 || screen_base.y == 0);

		  Vector2 screen_head; world_to_screen(screen_head, pos_head.x, pos_head.y, pos_head.z);
		  if (screen_head.x == 0 || screen_head.y == 0);

		  Vector2 screen_body; world_to_screen(screen_body, pos_body.x, pos_body.y, pos_body.z);

		  if (screen_body.x == 0 || screen_body.y == 0);
	
			auto height = abs(screen_head.y - screen_base.y);
			float width = height * 0.65f;
			//auto width = abs(screen_head.y - screen_base.y) / 0.65f;

			int health = Memory.Read<int>(entity + OFFSET_HEALTH);
			int shield = Memory.Read<int>(entity + OFFSET_SHIELD);
			int max_shield = Memory.Read<int>(entity + OFFSET_MAX_SHIELD);
			int m_aromortype = Memory.Read<int>(entity + OFFSET_Armortype);// m_armortype

			if (health <= 0)
				return;

			

			//if (pos_base.IsNull()) 
			{
				//return;

			auto DistFrom = Math::Distance(entity::GetEntityBasePosition(localent), pos_base);

			if (DistFrom == -1 || DistFrom > settings.DistanceESPFL)
				return;

			//if (HealthBar)
				//DrawHealthBar(pMaterialSurface, HeadPos2D, FootPos2D, health);

			
			if (settings.ArmorBox)
			{
				DrawArmorBar(screen_head, screen_base, shield, max_shield);
			}

			auto BoxColor = SColor(255, 255, 255, 255);
		    auto TextColor = SColor(255, 255, 255, 255);

			

			
			if (settings.SeerHealth)
			{
				SeerHealth(screen_head.x, screen_head.y - 11, shield, max_shield, m_aromortype, health);

			}


			if (settings.visuals_box)
				if (settings.box_mode == 1)
				{
					DrawCornerBox((screen_head.x - width / 2), screen_head.y, width + 1, height + 1, 1.5, &allcol.black);
					DrawCornerBox((screen_head.x - width / 2), screen_head.y, width, height, 1.5, &allcol.red);
				}

			if (settings.box_mode == 2)
			{
				DrawRect((screen_head.x - width / 2), screen_head.y, width, height, &allcol.red, 1.5);
			}

			if (settings.box_mode == 3)
			{
				DrawFilledRect((screen_head.x - width / 2), screen_head.y, width, height, &allcol.black_);
				DrawRect((screen_head.x - width / 2) + 1, screen_head.y, width, height, &allcol.black_, 1.5);
				DrawRect((screen_head.x - width / 2) - 1, screen_head.y, width, height, &allcol.black_, 1.5);
				DrawRect((screen_head.x - width / 2), screen_head.y + 1, width, height, &allcol.black_, 1.5);
				DrawRect((screen_head.x - width / 2), screen_head.y - 1, width, height, &allcol.black_, 1.5);
				DrawRect((screen_head.x - width / 2), screen_head.y, width, height, &allcol.red, 1.5);
			}

			std::string left_delim =  ("[");
			std::string right_delim =  ("]");

			if (settings.DistanceESP /*&& ActiveWeaponESP*/)
			{
				//std::string weaponName = GetActiveWeaponName(entity);

				char value_buf[64];
				ImDbgFormatString(value_buf, IM_ARRAYSIZE(value_buf),("%f"), DistFrom);

				std::string formatted = std::string(value_buf);
				std::string distanceFormat = formatted.substr(0, formatted.size() - 7);

				std::string distanceText = left_delim + distanceFormat + right_delim;
				//std::string displayText = !weaponName.empty() ? left_delim + weaponName + right_delim + distanceText : distanceText;

				DrawString(12, screen_base.x, (screen_base.y + 7), &TextColor, true, true, distanceText.c_str());
				//SourceDrawText(pMaterialSurface, HeadPos2D.x, FootPos2D.y + 7, displayText.c_str(), 12, TextColor, false);
			}
			//else if (ActiveWeaponESP && !DistanceESP)
			//{
			//	std::string weaponName = GetActiveWeaponName(entity);

			//	if (!weaponName.empty())
			//	{
			//		std::string displayText = left_delim + weaponName + right_delim;

			//		SourceDrawText(pMaterialSurface, HeadPos2D.x, FootPos2D.y + 7, displayText.c_str(), 12, TextColor, false);
			//	}
			//}
			//else if (DistanceESP && !ActiveWeaponESP)
			//{
			//	char value_buf[64];
			//	//ImDbgFormatString(value_buf, IM_ARRAYSIZE(value_buf), ("%f"), DistFrom);

			//	std::string formatted = std::string(value_buf);
			//	std::string distanceFormat = formatted.substr(0, formatted.size() - 7);

			//	std::string distanceText = left_delim + distanceFormat + right_delim;

			//	//SourceDrawText(pMaterialSurface, HeadPos2D.x, FootPos2D.y + 7, distanceText.c_str(), 12, TextColor, false);
			//}

			//if (Nicknames)
			//{
			//	auto PlayerNameCh = GetPlayerName(entity);

			//	std::wstring PlayerNameUTF = ToUnicode(std::string(PlayerNameCh));
			//	std::string PlayerNameAnsi = ToANSI(PlayerNameUTF.c_str());

			//	SourceDrawText(pMaterialSurface, HeadPos2D.x, HeadPos2D.y - 18, PlayerNameAnsi.c_str(), 12, TextColor, false);
			//}
			}
	}
}



void OnPaint()
{

	/*if (!IsInGame())
		return;*/

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(Width, Height));
	ImGui::Begin(skCrypt("##BOX"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

	RenderMenu();
	

	uint64_t LocalPlayer = Memory.Read<uint64_t>(Memory.BaseAddress + OFFSET_LOCAL_ENT);

	if (!LocalPlayer)
		return;

	//if (!IsPlayerAlive(LocalPlayer))
		//return;

	//crossX = (float)screenWidth / 2;
	//crossY = (float)screenHeight / 2;

	float maxFov = 9999.0f;


	//std::thread(LoadLoot).detach();
	//LoadLoot();

	auto LootList = CachedItemList;

	if (!LootList.empty())
	{
		for (unsigned long i = 0; i < LootList.size(); ++i)
		{
			ItemCache Item = LootList[i];

			if (Item.item_entity != 0);
				RenderItem(Item.item_entity, Item.item_id, LocalPlayer);
		}
	}

	//cameraFov = GetPlayerFOV(LocalPlayer);

	//float fovRadius = (AimBotFOV * crossX / cameraFov) / 2;
	if (settings.misc_showfov)
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), settings.aimbotfov, IM_COL32_WHITE, 10000, 1);

	LoadPlayers();
	/*NoRecoil();*/
	auto PlayerList = CachedPlayerList;

	if (!PlayerList.empty())
	{
		for (unsigned long i = 0; i < PlayerList.size(); ++i)
		{
			EntityCache Entity = PlayerList[i];


			if (Entity.player_entity != 0)
			{
				if (Entity.player_entity != LocalPlayer && entity::GetTeam(Entity.player_entity) != entity::GetTeam(LocalPlayer)/*&& IsPlayerAlive(Entity.player_entity)*/)
				{
					 RenderEntity(Entity.player_entity, LocalPlayer);
					if (settings.aimbot) {
						if (entity::IsPlayerVisible(Entity.player_entity, i))
						if (GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_LBUTTON))
						{
							auto TargetPlayer = GetPlayerByFOV(LocalPlayer, Entity.player_entity, maxFov);

							if (!TargetPlayer)
								continue;

							auto FaceAngles = GetFaceAngles(LocalPlayer, TargetPlayer);

							if (!FaceAngles.IsValid())
								continue;


							FacePlayer(LocalPlayer, TargetPlayer, FaceAngles);
						}
					}
				}
			}
		}
	}
}