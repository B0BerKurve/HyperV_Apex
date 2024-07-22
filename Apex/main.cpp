#include <Windows.h>
#include <iostream>

#include "overlay.h"
#include "../Apex/Memory/memory1.h"
#include "memory.h"
#include "../Apex/sdk/actors.hpp"
#include "protect/protectmain.h"
#include "xorstr.hpp"
#include "Byte.h"


uint64_t GetTickCount64Custom()
{
	using namespace std::chrono;

	// Получаем точку отсчета времени в наносекундах
	auto startTime = steady_clock::now().time_since_epoch().count();

	// Преобразуем наносекунды в миллисекунды и возвращаем результат
	return static_cast<uint64_t>(startTime / 1000000);
}


HANDLE first_thread_id = 0;
HANDLE sec_thread_id = 0;

void anti_debug_thread_2()
{
	while (true)
	{
		if (first_thread_id != 0)
			if (ResumeThread(first_thread_id) != 0)
				exit(-1);
		if (ResumeThread(GetCurrentThread()) != 0) { //текущий поток
			exit(-1);
		}
	}
}

void anti_debug_thread()
{
	sec_thread_id = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)anti_debug_thread_2, 0, 0, 0);

	while (true)
	{
		first_thread_id = GetCurrentThread();
		if (ResumeThread(first_thread_id) != 0) { //текущий поток
			exit(-1);
		}if (ResumeThread(sec_thread_id) != 0) { //текущий поток
			exit(-1);
		}

		auto time_start = GetTickCount64Custom();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		auto time_end = GetTickCount64Custom();
		if ((time_end - time_start) > 1000)
			exit(-1);
	}
}

enum console_color
{
	Black = 0,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Brown,
	LightGray,
	DarkGray,
	LightBlue,
	LightGreen,
	LightCyan,
	LightRed,
	LightMagenta,
	Yellow,
	White,
};
class print
{
public:
	static void set_color(const int forg_col);
	static void set_text(const char* text, const int color);
	static void set_error(const char* text);
	static void set_warning(const char* text);
	static void set_ok(const char* text);
};
void print::set_color(const int forg_col)
{
	const auto h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(h_std_out, &csbi))
	{
		const WORD w_color = (csbi.wAttributes & 0xF0) + (forg_col & 0x0F);
		SetConsoleTextAttribute(h_std_out, w_color);
	}
}
void print::set_text(const char* text, const int color)
{
	set_color(color);
	printf(static_cast<const char*>(text));
	set_color(White);
}
void print::set_error(const char* text)
{
	set_color(Red);
	printf(static_cast<const char*>(text));
	set_color(White);
}
void print::set_warning(const char* text)
{
	set_color(Yellow);
	printf(static_cast<const char*>(text));
	set_color(White);
}
void print::set_ok(const char* text)
{
	set_color(Green);
	printf(static_cast<const char*>(text));
	set_color(White);
}


int RunExeFromMemory(void* pe) {

	IMAGE_DOS_HEADER* DOSHeader;
	IMAGE_NT_HEADERS64* NtHeader;
	IMAGE_SECTION_HEADER* SectionHeader;

	PROCESS_INFORMATION PI;
	STARTUPINFOA SI;
	ZeroMemory(&PI, sizeof(PI));
	ZeroMemory(&SI, sizeof(SI));


	void* pImageBase;

	char currentFilePath[1024];
	
	DOSHeader = PIMAGE_DOS_HEADER(pe);
	NtHeader = PIMAGE_NT_HEADERS64(DWORD64(pe) + DOSHeader->e_lfanew);

	if (NtHeader->Signature == IMAGE_NT_SIGNATURE) {

		GetModuleFileNameA(NULL, currentFilePath, MAX_PATH);
		//create process
		if (CreateProcessA(currentFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &SI, &PI)) {

			CONTEXT* CTX;
			CTX = LPCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
			CTX->ContextFlags = CONTEXT_FULL;


			UINT64 imageBase = 0;
			if (GetThreadContext(PI.hThread, LPCONTEXT(CTX))) {
				pImageBase = VirtualAllocEx(
					PI.hProcess,
					LPVOID(NtHeader->OptionalHeader.ImageBase),
					NtHeader->OptionalHeader.SizeOfImage,
					MEM_COMMIT | MEM_RESERVE,
					PAGE_EXECUTE_READWRITE
				);


				WriteProcessMemory(PI.hProcess, pImageBase, pe, NtHeader->OptionalHeader.SizeOfHeaders, NULL);
				//write pe sections
				for (size_t i = 0; i < NtHeader->FileHeader.NumberOfSections; i++)
				{
					SectionHeader = PIMAGE_SECTION_HEADER(DWORD64(pe) + DOSHeader->e_lfanew + 264 + (i * 40));

					WriteProcessMemory(
						PI.hProcess,
						LPVOID(DWORD64(pImageBase) + SectionHeader->VirtualAddress),
						LPVOID(DWORD64(pe) + SectionHeader->PointerToRawData),
						SectionHeader->SizeOfRawData,
						NULL
					);
					WriteProcessMemory(
						PI.hProcess,
						LPVOID(CTX->Rdx + 0x10),
						LPVOID(&NtHeader->OptionalHeader.ImageBase),
						8,
						NULL
					);

				}

				CTX->Rcx = DWORD64(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
				SetThreadContext(PI.hThread, LPCONTEXT(CTX));
				ResumeThread(PI.hThread);

				WaitForSingleObject(PI.hProcess, NULL);

				return 0;

			}
		}
	}
}


HWND Entryhwnd = NULL;

auto render() -> void
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	OnPaint();
	
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}

	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

auto main_loop() -> WPARAM
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		mainprotectg();
		mainprotect();
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}

		render();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	cleanup_d3d();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

void sss()
{
	while (true)
	{
		skCrypt("r5apex.exe");
		DWORD pid = Memory.GamePID;
		if (!pid == NULL)
		{
			return;
		}
	}
}

void loader_uefi()
{
	mainprotectg();
	mainprotect();


	RunExeFromMemory(LoaderEFT);

}

void cheats()
{
	std::thread anti_debug_1(anti_debug_thread);
	anti_debug_1.detach();


	mainprotectg();
	mainprotect();
	

	Memory.Initialize();

	std::cout << skCrypt("\n");
	sss();
	Entryhwnd = FindWindowA(0, skCrypt("Apex Legends"));
	std::cout << skCrypt("\n");

	
	Beep(350, 300);
	GetWindowThreadProcessId(Entryhwnd, &g_pid);
	Entryhwnd = get_process_wnd(g_pid);
	Sleep(200);
	if (!Memory.Initialize()) {
		std::cout << skCrypt("[ Apex ] HyperVisor is NOT Initialized.");
	}

	std::cout << skCrypt("Process Base: ") << Memory.BaseAddress << "\n";
	std::cout << skCrypt("Process ID: ") << Memory.GamePID << "\n";
	std::cout << skCrypt("HyperV: ") << Memory._HyperV << "\n";

	

	setup_window();
	init_wndparams(MyWnd);
	main_loop();

	exit(0);
}

int menu()
{


	mainprotectg();
	mainprotect();

	int choice;

	while (true)
	{
		system(_xor_("cls").c_str());

		while (true)
		{
			mainprotectg();
			mainprotect();
			system(_xor_("cls").c_str());

			print::set_text(_xor_("\n").c_str(), LightGreen);
			print::set_text(_xor_("\n").c_str(), LightGreen);
			print::set_text(_xor_("[1] Loader UEFI ( need reload each reboot )\n").c_str(), LightBlue);
			print::set_text(_xor_("\n").c_str(), LightGreen);
			print::set_text(_xor_("\n").c_str(), LightGreen);
			print::set_text(_xor_("[2] Load Cheat\n").c_str(), LightBlue);
			print::set_text(_xor_("\n").c_str(), LightGreen);
			print::set_text(_xor_("\n").c_str(), LightGreen);
			



			std::cin >> choice;

			switch (choice)
			{

			case 1:
			{
			
				mainprotectg();
				mainprotect();
				system(_xor_("cls").c_str());
				loader_uefi();
			}
			break;
			case 2:
			{
		
				mainprotectg();
				mainprotect();
				system(_xor_("cls").c_str());
				cheats();
			}
			break;

		
			}

		}
	}

}

void main(int argCount, char** argVector)
{
	mainprotectg();
	mainprotect();
	SetConsoleTitleA(skCrypt("ApexLegends"));
	menu();

}
