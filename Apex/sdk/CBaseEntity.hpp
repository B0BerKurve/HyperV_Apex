
#include "../sdk/Math/Math.hpp"
#include "../sdk/offsets.h"

struct bone_t
{
	BYTE pad[0xCC];
	float x;
	BYTE pad2[0xC];
	float y;
	BYTE pad3[0xC];
	float z;
};
struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};


enum class SignonState : int32_t
{
	None,
	Challenge,
	Connected,
	StateNew,
	Prespawn,
	GettingData,
	Spawn,
	FirstSnap,
	Full,
	ChangeLevel,
};

typedef struct _EntityCache
{
	uint64_t player_entity;
	int player_id;
} EntityCache;

typedef struct _ItemCache
{
	uint64_t item_entity;
	int item_id;
} ItemCache;



//https://www.unknowncheats.me/forum/3780595-post12510.html
inline Matrix get_matrix()
{
	uint64_t viewRenderer = Memory.Read<uint64_t>(Memory.BaseAddress + OFFSET_RENDER);
	uint64_t viewMatrix = Memory.Read<uint64_t>(viewRenderer + OFFSET_MATRIX);

	return Memory.Read<Matrix>(viewMatrix);
}


bool world_to_screen(Vector2& screen, float x, float y, float z) {
	float* m_vMatrix = get_matrix().matrix;
	float w = m_vMatrix[12] * x + m_vMatrix[13] * y + m_vMatrix[14] * z + m_vMatrix[15];

	if (w < 0.01f)
		return false;

	screen.x = m_vMatrix[0] * x + m_vMatrix[1] * y + m_vMatrix[2] * z + m_vMatrix[3];
	screen.y = m_vMatrix[4] * x + m_vMatrix[5] * y + m_vMatrix[6] * z + m_vMatrix[7];

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	float xx = GetSystemMetrics(SM_CXSCREEN) / 2;
	float xy = GetSystemMetrics(SM_CYSCREEN) / 2;

	xx += 0.5 * screen.x * GetSystemMetrics(SM_CXSCREEN) + 0.5;
	xy -= 0.5 * screen.y * GetSystemMetrics(SM_CYSCREEN) + 0.5;

	screen.x = xx;
	screen.y = xy;

	if (screen.x > GetSystemMetrics(SM_CXSCREEN) || screen.x < 0 || screen.y > GetSystemMetrics(SM_CYSCREEN) || screen.y < 0)
		return false;

	return true;
}

namespace entity
{
	inline bool IsPlayer(uintptr_t ent)
	{
		uint64_t name = Memory.Read<uint64_t>(ent + OFFSET_NAME);
		return (name == 125780153691248);
	}

	inline bool IsKnocked(uintptr_t ent)
	{
		return Memory.Read<int>(ent + OFFSET_BLEED_OUT_STATE) > 0;
	}

	inline int GetTeam(uintptr_t ent)
	{
		return Memory.Read<int>(ent + OFFSET_TEAM);
	}


	inline Vector3 GetViewAngles(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_VIEWANGLES);
	}
	struct visibleTime
	{
		float lastTime[100]; // last visible time in float
		bool lastState[100]; // last visibility state
		uintptr_t lastCheck[100]; // last visibility check time
	} lastVis;

	uintptr_t GetMs()
	{
		static LARGE_INTEGER s_frequency;
		static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
		if (s_use_qpc) {
			LARGE_INTEGER now;
			QueryPerformanceCounter(&now);
			return (1000LL * now.QuadPart) / s_frequency.QuadPart;
		}
		else {
			return GetTickCount64();
		}
	}
	inline bool IsPlayerVisible(uintptr_t ent, int index)
	{
		if (GetMs() >= (lastVis.lastCheck[index] + 10))
		{
			float visTime = Memory.Read<float>(ent + OFFSET_VISIBLE_TIME);
			lastVis.lastState[index] = visTime > lastVis.lastTime[index] || visTime < 0.f && lastVis.lastTime[index] > 0.f;
			lastVis.lastTime[index] = visTime;
			lastVis.lastCheck[index] = GetMs();
		}
		return lastVis.lastState[index];
	}
	

	inline Vector3 GetSwayAngles(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_BREATH_ANGLES);
	}

	inline QAngle GetViewAnglesA(uintptr_t ent)
	{
		return Memory.Read<QAngle>(ent + OFFSET_VIEWANGLES);
	}

	inline Vector3 GetEntityBasePosition(uintptr_t ent)
	{
		Vector3 BasePos = Vector3();

		BasePos = Memory.Read<Vector3>(ent + OFFSET_ORIGIN);

		return BasePos;
	}

	inline Vector3 GetEyePosition(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_CAMERAPOS);
	}

	inline uintptr_t GetEntityBoneArray(uintptr_t ent)
	{
		return Memory.Read<uintptr_t>(ent + OFFSET_BONES);
	}

	inline Vector3 GetEntityBonePosition(uintptr_t ent, uint32_t BoneId, Vector3 BasePosition)
	{
		Vector3 pos = Memory.Read<Vector3>(ent + OFFSET_ORIGIN);
		uintptr_t bones = Memory.Read<uintptr_t>(ent + OFFSET_BONES);
		Vector3 bone = {};
		UINT32 boneloc = (BoneId * 0x30);
		bone_t bo = {};
		bo = Memory.Read<bone_t>(bones + boneloc);

		bone.x = bo.x + BasePosition.x;
		bone.y = bo.y + BasePosition.y;
		bone.z = bo.z + BasePosition.z;
		return bone;
	}


	inline void SetViewAngles(uintptr_t ent, QAngle angles)
	{
		Memory.Write<QAngle>(ent + OFFSET_VIEWANGLES, angles);

	}


	inline QAngle GetViewAngles1(uintptr_t ent)
	{
		return Memory.Read<QAngle>(ent + OFFSET_VIEWANGLES);

	}

	inline float LastVisTime(uintptr_t ent)
	{
		return Memory.Read<float>(ent + OFFSET_VISIBLE_TIME);
	}

	inline Vector3 GetCamPos(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_CAMERAPOS);
	}

	inline QAngle GetSwayAnglesA(uintptr_t ent)
	{
		return Memory.Read<QAngle>(ent + OFFSET_BREATH_ANGLES);
	}

	inline QAngle GetRecoil(uintptr_t ent)
	{
		return Memory.Read<QAngle>(ent + OFFSET_AIMPUNCH);
	}

	inline Vector3 GetRecoilA(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_AIMPUNCH);
	}

	inline Vector3 GetAbsVelocity(uintptr_t ent)
	{
		return Memory.Read<Vector3>(ent + OFFSET_ABS_VELOCITY);
	}

	struct HighlightFunctionBits
	{
		uint8_t FunctionBits[4];
	};

	struct HighlightParameter
	{
		float Parameter[3];
	};


	void EnableGlowItem(uintptr_t item)
	{
		Memory.Write<uint32_t>(item + OFFSET_HIGHLIGHTVISIBILITYTYPE, 2);
		uint32_t contextId = Memory.Read<uint32_t>(item + OFFSET_HIGHLIGHTCURRENTCONTEXTID);
		uint8_t hightState = Memory.Read<uint8_t>(item + contextId + OFFSET_HIGHLIGHTSERVERACTIVESTATES);
		uint64_t HighlightSettings = Memory.Read<uint64_t>(Memory.BaseAddress + OFFSET_HIGHLIGHTSETTINGS);
		HighlightFunctionBits setting{ 137,138,64,64 };
		Memory.Write<HighlightFunctionBits>(HighlightSettings + 40 * hightState + 4, setting);
	}


	bool IsDummy(uintptr_t entity)
	{
		if (entity::GetTeam(entity) == 97)
			return true;
		else
			return false;
	}
};