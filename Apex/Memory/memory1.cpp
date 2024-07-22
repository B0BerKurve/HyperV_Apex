#include "memory1.h"

bool CMemory::Initialize()
{


	while (!GetProcessID())
	{

		WRAP_IF_DEBUG(
			printf("[?] Searching for target process \n");
		)

			Sleep(1000);

	}

	GamePID = GetProcessID();

	if (!GamePID)
	{
		WRAP_IF_DEBUG(
			std::cout << "Failed to get Apex Legends PID.\n";
		)

			ERROR_MESSAGE("Failed to initialize. (Code: 0x3)");

		return false; // failed to get the process id

	}

#ifdef _RELEASE
	Sleep(5000);
#endif

	while (!FindWindowA("Respawn001", NULL))
		Sleep(1000);

	Window = FindWindowA("Respawn001", NULL);

	if (!Window)
	{

		WRAP_IF_DEBUG(
			std::cout << "Failed to get Apex Legends window.\n";
		)

			ERROR_MESSAGE("Failed to initialize. (Code: 0x4)");

		return false;

	}

	if (!_HyperV->GetExtendProcCr3(GamePID))
	{
		std::cout << "HYPERVISOR NOT LOADED! [CR3]\n";

		ERROR_MESSAGE("Failed to initialize. (Code: 0x5)");

		return false;

	}

	BaseAddress = _HyperV->GetProccessBase();

	if (!BaseAddress)
	{
		WRAP_IF_DEBUG(
			std::cout << "Failed to get Apex Legends base address..\n";
		)

			ERROR_MESSAGE("Failed to initialize. (Code: 0x6)");

		return false;

	}

	WRAP_IF_DEBUG(
		std::cout << "Memory initialized.\n";
	)

		return true;

}

int GetProcessThreadNumByID(DWORD dwPID)
{
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(pe32);
	BOOL bRet = ::Process32First(hProcessSnap, &pe32);;
	while (bRet)
	{
		if (pe32.th32ProcessID == dwPID)
		{
			::CloseHandle(hProcessSnap);
			return pe32.cntThreads;
		}
		bRet = ::Process32Next(hProcessSnap, &pe32);
	}
	return 0;
}

DWORD CMemory::GetProcessID()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD result = NULL;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

	pe32.dwSize = sizeof(PROCESSENTRY32); // <----- IMPORTANT

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
	/*	if (debug)
			printf("Failed to gather information on system processes! \n");*/
		return(NULL);
	}

	do
	{
		if (0 == strcmp("r5apex.exe", pe32.szExeFile))
		{
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return result;
}


//DWORD CMemory::GetProcessID()
//{
//	DWORD dwRet = 0;
//	DWORD dwThreadCountMax = 0;
//	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	PROCESSENTRY32 pe32;
//	pe32.dwSize = sizeof(PROCESSENTRY32);
//	Process32First(hSnapshot, &pe32);
//	do
//	{
//		if (wcscmp(pe32.szExeFile, L"r5apex.exe") == 0)
//
//		{
//			DWORD dwTmpThreadCount = GetProcessThreadNumByID(pe32.th32ProcessID);
//
//			if (dwTmpThreadCount > dwThreadCountMax)
//			{
//				dwThreadCountMax = dwTmpThreadCount;
//				dwRet = pe32.th32ProcessID;
//			}
//		}
//	} while (Process32Next(hSnapshot, &pe32));
//	CloseHandle(hSnapshot);
//	return dwRet;
//}