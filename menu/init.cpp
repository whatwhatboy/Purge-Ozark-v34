#include "init.h"
#include "global/vars.h"
#include "util/dirs.h"
#include "util/log.h"
#include "util/threads.h"
#include "util/va.h"
#include "util/wmi.h"
#include "util/util.h"
#include "util/fiber.h"
#include "util/fiber_pool.h"
#include "util/hooking/hooking.h"
#include "menu/hooks/hooks.h"
#include "menu/base/util/input.h"
#include "menu/base/submenu_handler.h"
#include "menu/base/util/textures.h"
#include <Psapi.h>
#include "../SimpleIni.h"
#include <random>
#include <fstream>
#include <filesystem>
#include "../auth/api/api.hh"

#include <shlobj_core.h>
#include <Windows.h>
char buff[512];
//===========================================================================
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;
//===========================================================================
typedef struct _PEB_LDR_DATA
{
	ULONG           Length;
	BOOLEAN         Initialized;
	PVOID           SsHandle;
	LIST_ENTRY      InLoadOrderModuleList;
	LIST_ENTRY      InMemoryOrderModuleList;
	LIST_ENTRY      InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;
//===========================================================================
typedef struct _LDR_MODULE
{
	LIST_ENTRY      InLoadOrderModuleList;
	LIST_ENTRY      InMemoryOrderModuleList;
	LIST_ENTRY      InInitializationOrderModuleList;
	PVOID           BaseAddress;
	PVOID           EntryPoint;
	ULONG           SizeOfImage;
	UNICODE_STRING  FullDllName;
	UNICODE_STRING  BaseDllName;
	ULONG           Flags;
	SHORT           LoadCount;
	SHORT           TlsIndex;
	LIST_ENTRY      HashTableEntry;
	ULONG           TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;
//===========================================================================

void UnlinkModule(void* Module)
{
	DWORD dwPEB = 0, dwOffset = 0;
	PLIST_ENTRY pUserModuleHead = NULL, pUserModule = NULL;
	PPEB_LDR_DATA pLdrData;
	PLDR_MODULE pLdrModule = NULL;
	void* ModuleBase = NULL;
	int i = 0, n = 0;
#ifndef _WIN64
	_asm
	{
		pushad
		mov eax, fs: [48]
		mov dwPEB, eax
		popad
	}

	pLdrData = (PPEB_LDR_DATA)(PDWORD)(*(PDWORD)(dwPEB + 12));
#else
	BYTE* _teb = (BYTE*)__readgsqword(0x30);
	pLdrData = (PPEB_LDR_DATA)(PULONGLONG)(*(PULONGLONG)((*(PULONGLONG)(_teb + 0x60)) + 0x18));
#endif  

	for (; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			pUserModuleHead = (PLIST_ENTRY)(&(pLdrData->InLoadOrderModuleList));
			pUserModule = (PLIST_ENTRY)(&(pLdrData->InLoadOrderModuleList));
			dwOffset = 0;
			break;

		case 1:
			pUserModuleHead = (PLIST_ENTRY)(&(pLdrData->InMemoryOrderModuleList));
			pUserModule = (PLIST_ENTRY)(&(pLdrData->InMemoryOrderModuleList));
#ifndef _WIN64
			dwOffset = 8;
#else
			dwOffset = 16;
#endif
			break;
		case 2:
			pUserModuleHead = pUserModule = (PLIST_ENTRY)(&(pLdrData->InInitializationOrderModuleList));
#ifndef _WIN64
			dwOffset = 16;
#else
			dwOffset = 32;
#endif
			break;
		}
		while (pUserModule->Flink != pUserModuleHead)
		{
			pUserModule = pUserModule->Flink;
#ifndef _WIN64
			ModuleBase = *(void**)(((DWORD)(pUserModule)) + (24 - dwOffset));
#else
			ModuleBase = *(void**)(((LONGLONG)(pUserModule)) + (48 - dwOffset));
#endif          
			if (ModuleBase == Module)
			{
#ifndef _WIN64
				if (!pLdrModule)
					pLdrModule = (PLDR_MODULE)(((DWORD)(pUserModule)) - dwOffset);
#else
				if (!pLdrModule)
					pLdrModule = (PLDR_MODULE)(((LONGLONG)(pUserModule)) - dwOffset);
#endif              
				pUserModule->Blink->Flink = pUserModule->Flink;
				pUserModule->Flink->Blink = pUserModule->Blink;
			}
		}
	}
	// Unlink from LdrpHashTable
	if (pLdrModule)
	{
		pLdrModule->HashTableEntry.Blink->Flink = pLdrModule->HashTableEntry.Flink;
		pLdrModule->HashTableEntry.Flink->Blink = pLdrModule->HashTableEntry.Blink;
	}
}
//===========================================================================
void RemovePeHeader(HANDLE GetModuleBase)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)GetModuleBase;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		return;

	if (pNTHeader->FileHeader.SizeOfOptionalHeader)
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect((void*)GetModuleBase, Size, PAGE_EXECUTE_READWRITE, &Protect);
		RtlZeroMemory((void*)GetModuleBase, Size);
		VirtualProtect((void*)GetModuleBase, Size, Protect, &Protect);
	}
}
//===========================================================================



void Thread()
{
	while (true)
	{
		MessageBoxA(NULL, buff, "10s一次弹窗证明我还在运行！", MB_OK);
		Sleep(10000);
	}
}
CSimpleIniA auth;
std::string username;
std::string password;

bool Auth_Check()
{

	BOOL returnVal = TRUE;
	wchar_t path[MAX_PATH];
	char target[MAX_PATH];

	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
	if (result != S_OK)
	{
		LOG_ERROR("[AUTH] - ERROR [ Credentials Not Found ] ");
		returnVal = FALSE;
	}
	else
		wcstombs(target, path, MAX_PATH);
	std::string folder = target;
	strcat(target, "\\Purge\\Purge.auth");




	auth.SetUnicode();
	auth.LoadFile(target);

	if (auth.IsEmpty())
	{
		auth.SetValue("authentication", "key", "key here");
	}

	username = auth.GetValue("authentication", "key", FALSE);

	auth.SaveFile(target);


	if (api::auth(username.c_str())) {
		returnVal = true;
	}
	else {
		returnVal = false;
		exit(0);
	}

}

namespace menu::init {
	LONG WINAPI exception_filter(struct _EXCEPTION_POINTERS* ExceptionInfoPtr) {
#undef exception_code
#undef exception_info

		DWORD exception_code = ExceptionInfoPtr->ExceptionRecord->ExceptionCode;
		ULONG_PTR exception_info = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[0];
		ULONG_PTR exception_info1 = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[1];
		ULONG_PTR exception_info2 = ExceptionInfoPtr->ExceptionRecord->ExceptionInformation[2];

		uint64_t exception_address = (uint64_t)ExceptionInfoPtr->ExceptionRecord->ExceptionAddress;

		LOG_CUSTOM_ERROR("Crash", "Game crashed with code 0x%X", exception_code);
		LOG_CUSTOM_ERROR("Crash", "Crashed @ 0x%llx (game=%llx cheat=%llx)", exception_address, global::vars::g_game_address.first, global::vars::g_module_handle);
		// LOG_DEV("Game Script: %s", *(const char**)(global::vars::g_game_address.first + 0x2D59018));

		switch (exception_code) {
			case EXCEPTION_ACCESS_VIOLATION:
				LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_ACCESS_VIOLATION");
				if (exception_info == 0) {
					LOG_CUSTOM_ERROR("Crash", "Attempted to read from: 0x%llx", exception_info1);
				} else if (exception_info == 1) {
					LOG_CUSTOM_ERROR("Crash", "Attempted to write to: 0x%llx", exception_info1);
				} else if (exception_info == 8) {
					LOG_CUSTOM_ERROR("Crash", "Data Execution Prevention (DEP) at: 0x%llx", exception_info1);
				} else {
					LOG_CUSTOM_ERROR("Crash", "Unknown access violation at: 0x%llx", exception_info1);
				}
				break;

			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
			case EXCEPTION_BREAKPOINT: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_BREAKPOINT"); break;
			case EXCEPTION_DATATYPE_MISALIGNMENT: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_DATATYPE_MISALIGNMENT"); break;
			case EXCEPTION_FLT_DENORMAL_OPERAND: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_DENORMAL_OPERAND"); break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_FLT_INEXACT_RESULT: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_INEXACT_RESULT"); break;
			case EXCEPTION_FLT_INVALID_OPERATION: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_INVALID_OPERATION"); break;
			case EXCEPTION_FLT_OVERFLOW: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_OVERFLOW"); break;
			case EXCEPTION_FLT_STACK_CHECK: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_STACK_CHECK"); break;
			case EXCEPTION_FLT_UNDERFLOW: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_FLT_UNDERFLOW"); break;
			case EXCEPTION_ILLEGAL_INSTRUCTION: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_ILLEGAL_INSTRUCTION"); break;
			case EXCEPTION_IN_PAGE_ERROR:
				LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_IN_PAGE_ERROR");
				if (exception_info == 0) {
					LOG_CUSTOM_ERROR("Crash", "Attempted to read from: 0x%llx", exception_info1);
				} else if (exception_info == 1) {
					LOG_CUSTOM_ERROR("Crash", "Attempted to write to: 0x%llx", exception_info1);
				} else if (exception_info == 8) {
					LOG_CUSTOM_ERROR("Crash", "Data Execution Prevention (DEP) at: 0x%llx", exception_info1);
				} else {
					LOG_CUSTOM_ERROR("Crash", "Unknown access violation at: 0x%llx", exception_info1);
				}

				LOG_CUSTOM_ERROR("Crash", "NTSTATUS: 0x%llx", exception_info2);
				break;

			case EXCEPTION_INT_DIVIDE_BY_ZERO: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_INT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_INT_OVERFLOW: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_INT_OVERFLOW"); break;
			case EXCEPTION_INVALID_DISPOSITION: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_INVALID_DISPOSITION"); break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
			case EXCEPTION_PRIV_INSTRUCTION: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_PRIV_INSTRUCTION"); break;
			case EXCEPTION_SINGLE_STEP: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_SINGLE_STEP"); break;
			case EXCEPTION_STACK_OVERFLOW: LOG_CUSTOM_ERROR("Crash", "Cause: EXCEPTION_STACK_OVERFLOW"); break;
			case DBG_CONTROL_C: LOG_CUSTOM_ERROR("Crash", "Cause: DBG_CONTROL_C"); break;
			default: LOG_CUSTOM_ERROR("Crash", "Cause: 0x%08x", exception_code);
		}

		LOG_CUSTOM_ERROR("Crash", "Dumping ASM registers:");
		LOG_CUSTOM_ERROR("Crash", "RAX: 0x%llx || RSI: 0x%llx", ExceptionInfoPtr->ContextRecord->Rax, ExceptionInfoPtr->ContextRecord->Rsi);
		LOG_CUSTOM_ERROR("Crash", "RBX: 0x%llx || RDI: 0x%llx", ExceptionInfoPtr->ContextRecord->Rbx, ExceptionInfoPtr->ContextRecord->Rdi);
		LOG_CUSTOM_ERROR("Crash", "RCX: 0x%llx || RBP: 0x%llx", ExceptionInfoPtr->ContextRecord->Rcx, ExceptionInfoPtr->ContextRecord->Rbp);
		LOG_CUSTOM_ERROR("Crash", "RDX: 0x%llx || RSP: 0x%llx", ExceptionInfoPtr->ContextRecord->Rdx, ExceptionInfoPtr->ContextRecord->Rsp);
		LOG_CUSTOM_ERROR("Crash", "R8: 0x%llx || R9: 0x%llx", ExceptionInfoPtr->ContextRecord->R8, ExceptionInfoPtr->ContextRecord->R9);
		LOG_CUSTOM_ERROR("Crash", "R10: 0x%llx || R11: 0x%llx", ExceptionInfoPtr->ContextRecord->R10, ExceptionInfoPtr->ContextRecord->R11);
		LOG_CUSTOM_ERROR("Crash", "R12: 0x%llx || R13: 0x%llx", ExceptionInfoPtr->ContextRecord->R12, ExceptionInfoPtr->ContextRecord->R13);
		LOG_CUSTOM_ERROR("Crash", "R14: 0x%llx || R15: 0x%llx", ExceptionInfoPtr->ContextRecord->R14, ExceptionInfoPtr->ContextRecord->R15);
		LOG_CUSTOM_ERROR("Crash", util::get_stack_trace().c_str());

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void unload() {
		if (!global::vars::g_unloading) {
			global::vars::g_unloading = true;
			
			util::threads::add_job([](void*) {

				for (patches& patch : global::vars::g_patches) {
					memory::write_vector(patch.m_address, patch.m_bytes);
				}

				for (auto& hack_patch : global::vars::g_hack_patches) {
					memory::write_vector(hack_patch.second.m_address, hack_patch.second.m_bytes);
				}

				hooking::cleanup();
				util::threads::cleanup();
				util::fiber::cleanup();
				menu::input::cleanup();
				menu::submenu::handler::cleanup();
				util::va::cleanup();
				util::log::cleanup();

				free((void*)global::vars::g_texture_bypass);
				free((void*)global::vars::g_marker_bypass);
				free((void*)global::ui::m_line_2d);

				PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
				FreeConsole();

#ifdef DEV_MODE
				Sleep(2000);
				FreeLibraryAndExitThread(global::vars::g_module_handle, 0);
#endif
			});
		}
	}

	void entry(void* handle) {
		SetUnhandledExceptionFilter(exception_filter);
		srand(GetTickCount());

		MODULEINFO module_info;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &module_info, sizeof(module_info));

		global::vars::g_steam = (uint64_t)GetModuleHandleA(XOR("steam_api64.dll").c_str()) > 0;
		global::vars::g_game_address = { (uint64_t)module_info.lpBaseOfDll, module_info.SizeOfImage };



		if (!util::dirs::load()) {
			unload();
			return;
		}

		util::log::load();
		const char* buildString = XOR("This build Of Purge was built at " __DATE__ ", " __TIME__ ".").c_str();
		LOG(buildString);

		if (!(global::vars::g_window = FindWindowA(XOR("grcWindow").c_str(), NULL))) {
			int timeout = 0;
			while (!global::vars::g_window && !global::vars::g_unloading) {
				if (timeout >= 20) {
					LOG_ERROR(XOR("Failed to find window!").c_str());
					unload();
					return;
				}

				global::vars::g_window = FindWindowA(XOR("grcWindow").c_str(), NULL);

				timeout++;
				Sleep(1000);
			}
		}

		//Auth_Check

		if (!Auth_Check()) {
			unload();
			return;
		}

		if (!menu::hooks::bypass()) {
			unload();
			return;
		}

		if (!menu::hooks::globals()) {
			unload();
			return;
		}

		if (!menu::hooks::hooks()) {
			unload();
			return;
		}

		while (*global::vars::g_game_state != GameStatePlaying) Sleep(500);

		if (!menu::hooks::globals_in_game()) {
			unload();
			return;
		}
	//	hooking::script(XOR("SGI"), XOR("main_persistent"), 0x767FBC2AC802EF3D, hooks::stat_get_int);

		//util::threads::add_thread(XOR("T_SH"), [](void*) {
		//	Sleep(1500);

		//	hooking::script(XOR("SCPW_P"), XOR("am_mp_property_int"), 0xADF692B254977C0C, &hooks::set_current_ped_weapon);
		//	hooking::script(XOR("DCA_P"), XOR("am_mp_property_int"), 0xFE99B66D079CF6BC, &hooks::disable_control_action);

		//	hooking::script(XOR("SCPW_C"), XOR("am_mp_smpl_interior_int"), 0xADF692B254977C0C, &hooks::set_current_ped_weapon);
		//	hooking::script(XOR("DCA_C"), XOR("am_mp_smpl_interior_int"), 0xFE99B66D079CF6BC, &hooks::disable_control_action);

		//	hooking::script(XOR("SCPW_F"), XOR("freemode"), 0xADF692B254977C0C, &hooks::set_current_ped_weapon);
		//	hooking::script(XOR("DCA_F"), XOR("freemode"), 0xFE99B66D079CF6BC, &hooks::disable_control_action);
		//	hooking::script(XOR("TTT"), XOR("freemode"), 0x1090044AD1DA76FA, &hooks::terminate_this_thread);
		//	// hooking::script(XOR("WAIT"), XOR("freemode"), 0x4EDE34FBADD967A6, &hooks::wait);
		
		//	hooking::script(XOR("SWM2"), XOR("shop_controller"), 0xDC38CC1E35B6A5D7, &hooks::set_warning_message_2);
		//	});

	}
}


BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		global::vars::g_module_handle = handle;
		/*DisableThreadLibraryCalls(handle);
		RemovePeHeader(handle);
		UnlinkModule((void*)handle);*/

		if (handle) {
			MODULEINFO module_info;
			GetModuleInformation(GetCurrentProcess(), handle, &module_info, sizeof(module_info));
			global::vars::g_cheat_address = { (uint64_t)module_info.lpBaseOfDll, module_info.SizeOfImage };
		}

		util::threads::add_job(menu::init::entry, handle);
		break;

	case DLL_PROCESS_DETACH:
		global::vars::g_unloading = true;
		break;
	}

	return TRUE;
}