#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include <cstdint> // uintptr_t abuse
#include <cmath>

#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include <algorithm>

#ifdef _DEBUG
#define WRAP_IF_DEBUG(x) { x };
#else
#define WRAP_IF_DEBUG(x)
#endif
#define ERROR_MESSAGE(x) MessageBoxA(NULL, x, NULL, NULL)

#include "../HyperV/HyperV.h"
#include "../Memory/stdafx.h"
class CMemory
{

private:

	DWORD GetProcessID();

public:

	uintptr_t BaseAddress;
	HWND Window;
	DWORD GamePID;
	HyperV* _HyperV = new HyperV();

	template<typename type>
	inline type Read(uintptr_t address)
	{
		return _HyperV->ReadValue64<type>(address);
	}

	template<typename type>
	inline type Read2(uintptr_t PhysAddr)
	{
		return _HyperV->ReadGayPhys64<type>(PhysAddr);
	}

	template<typename type>
	inline bool Write(uintptr_t address, type value)
	{
		return _HyperV->WriteValue64<type>(address, value);
	}

	template<typename type>
	inline type ReadChain(uintptr_t base, std::vector<uintptr_t> chain)
	{
		return _HyperV->ReadChain<type>(base, chain);
	}

	bool Initialize();

}; inline CMemory Memory;