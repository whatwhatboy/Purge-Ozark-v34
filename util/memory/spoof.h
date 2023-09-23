#pragma once


#pragma warning(disable : 4530)
#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/MERGE:.pdata=.text")

//main includes
#include <windows.h>
#include <winternl.h>
#include <intrin.h>
#include <stdio.h>
#include <stdint.h>
#include <d3d11.h>
#include <cfloat>
#include <string>
#include "../log.h"

#define v2p(a) { fp(a.x); fp(a.y); }
#define v3p(a) { fp(a.x); fp(a.y); fp(a.z); }

#define RVA(Instr, InstrSize) ((DWORD64)Instr + InstrSize + *(LONG*)((DWORD64)Instr + (InstrSize - sizeof(LONG))))
#define ConstStrLen(Str) ((sizeof(Str) - sizeof(Str[0])) / sizeof(Str[0]))
#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)


namespace Module
{

	template <typename StrType, typename StrType2>
	bool StrCmp(StrType Str, StrType2 InStr, bool Two) {
		if (!Str || !InStr) return false;
		wchar_t c1, c2; do {
			c1 = *Str++; c2 = *InStr++;
			c1 = ToLower(c1); c2 = ToLower(c2);
			if (!c1 && (Two ? !c2 : 1)) return true;
		} while (c1 == c2); return false;
	}

	template <typename StrType> __declspec(noinline) constexpr unsigned short HashStr(StrType Data, int Len) {
		unsigned short CRC = 0xFFFF; while (Len--) {
			auto CurChar = *Data++; if (!CurChar) break;
			CRC ^= ToLower(CurChar) << 8; for (int i = 0; i < 8; i++)
				CRC = CRC & 0x8000 ? (CRC << 1) ^ 0x8408 : CRC << 1;
		} return CRC;
	}


#define ConstHashStr(Str) [](){ constexpr unsigned short CRC = Module::HashStr(Str, ConstStrLen(Str)); return CRC; }()

	//	template <typename PtrType>
	//	__forceinline PtrType EPtr( PtrType Ptr ) {
	//		typedef union
	//		{
	//			struct
	//			{
	//				USHORT Key1; USHORT Key2;
	//				USHORT Key3; USHORT Key4;
	//			}; ULONG64 Key;
	//		} CryptData;
	//		CryptData Key { ConstHashStr( __TIME__ ), ConstHashStr( __DATE__ ),
	//			ConstHashStr( __TIMESTAMP__ ), ConstHashStr( __TIMESTAMP__ ) };
	//		volatile LONG64 PtrData; volatile LONG64 VKey;
	//		InterlockedExchange64( &VKey, ( ULONG64 ) Key.Key );
	//		InterlockedExchange64( &PtrData, ( ULONG64 ) Ptr );
	//		PtrData ^= VKey; return ( PtrType ) PtrData;
	//	}
	//#define EPtr(Ptr) Module::EPtr(Ptr)

	__declspec(noinline) PBYTE GetModuleBase_Wrapper(const char* ModuleName) {
		PPEB_LDR_DATA Ldr = ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))->ProcessEnvironmentBlock->Ldr; void* ModBase = nullptr;
		for (PLIST_ENTRY CurEnt = Ldr->InMemoryOrderModuleList.Flink; CurEnt != &Ldr->InMemoryOrderModuleList; CurEnt = CurEnt->Flink) {
			LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(CurEnt, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			PUNICODE_STRING BaseDllName = (PUNICODE_STRING)&pEntry->Reserved4[0];
			if (!ModuleName || StrCmp(ModuleName, BaseDllName->Buffer, false))
				return (PBYTE)pEntry->DllBase;
		}

		return nullptr;
	}
#define GetModuleBase Module::GetModuleBase_Wrapper

	PBYTE FindPattern_Wrapper(const char* Pattern)
	{

		//LOG_SUCCESS("Signature %s Found :)", name);
#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))

		PBYTE ModuleStart = (PBYTE)GetModuleBase(nullptr);
		PIMAGE_NT_HEADERS NtHeader = ((PIMAGE_NT_HEADERS)(ModuleStart + ((PIMAGE_DOS_HEADER)ModuleStart)->e_lfanew));
		PBYTE ModuleEnd = (PBYTE)(ModuleStart + NtHeader->OptionalHeader.SizeOfImage - 0x1000); ModuleStart += 0x1000;

		PBYTE FirstMatch = nullptr;
		const char* CurPatt = Pattern;
		for (; ModuleStart < ModuleEnd; ++ModuleStart)
		{
			bool SkipByte = (*CurPatt == '\?');
			if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
				if (!FirstMatch) FirstMatch = ModuleStart;
				SkipByte ? CurPatt += 2 : CurPatt += 3;
				if (CurPatt[-1] == 0) return FirstMatch;
			}

			else if (FirstMatch) {
				ModuleStart = FirstMatch;
				FirstMatch = nullptr;
				CurPatt = Pattern;
			}
		}

		return nullptr;
	}

	//const wchar_t* process, std::string name, const char* signature
	PBYTE FindPattern_Wrapper2(const char* Pattern, const char* Module = nullptr)
	{
	//	LOG_SUCCESS("Signature %s Found :)", Pattern);
#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))

		PBYTE ModuleStart = (PBYTE)GetModuleBase(Module);
		PIMAGE_NT_HEADERS NtHeader = ((PIMAGE_NT_HEADERS)(ModuleStart + ((PIMAGE_DOS_HEADER)ModuleStart)->e_lfanew));
		PBYTE ModuleEnd = (PBYTE)(ModuleStart + NtHeader->OptionalHeader.SizeOfImage - 0x1000); ModuleStart += 0x1000;

		PBYTE FirstMatch = nullptr;
		const char* CurPatt = Pattern;
		for (; ModuleStart < ModuleEnd; ++ModuleStart)
		{
			bool SkipByte = (*CurPatt == '\?');
			if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
				if (!FirstMatch) FirstMatch = ModuleStart;
				SkipByte ? CurPatt += 2 : CurPatt += 3;
				if (CurPatt[-1] == 0) return FirstMatch;
			}

			else if (FirstMatch) {
				ModuleStart = FirstMatch;
				FirstMatch = nullptr;
				CurPatt = Pattern;
			}
		}

		return nullptr;
	}

	PBYTE FindPattern_Wrapper3(const char* Module, const char* signature)
	{
	//	LOG_SUCCESS("Signature %s Found :)", name);
#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))
		PBYTE ModuleStart = (PBYTE)GetModuleBase(Module);
		PIMAGE_NT_HEADERS NtHeader = ((PIMAGE_NT_HEADERS)(ModuleStart + ((PIMAGE_DOS_HEADER)ModuleStart)->e_lfanew));
		PBYTE ModuleEnd = (PBYTE)(ModuleStart + NtHeader->OptionalHeader.SizeOfImage - 0x1000); ModuleStart += 0x1000;

		PBYTE FirstMatch = nullptr;
		const char* CurPatt = signature;
		for (; ModuleStart < ModuleEnd; ++ModuleStart)
		{
			bool SkipByte = (*CurPatt == '\?');
			if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
				if (!FirstMatch) FirstMatch = ModuleStart;
				SkipByte ? CurPatt += 2 : CurPatt += 3;
				if (CurPatt[-1] == 0) return FirstMatch;
			}

			else if (FirstMatch) {
				ModuleStart = FirstMatch;
				FirstMatch = nullptr;
				CurPatt = signature;
			}
		}

		return nullptr;
	}
#define FindPattern Module::FindPattern_Wrapper
#define FindPattern2 Module::FindPattern_Wrapper2
#define FindPattern3 Module::FindPattern_Wrapper3


	bool execCheck(void* ptr)
	{
		MEMORY_BASIC_INFORMATION memInfo{};
		if (VirtualQuery(ptr, &memInfo, sizeof(memInfo))) {
			if ((memInfo.Protect & PAGE_EXECUTE) ||
				(memInfo.Protect & PAGE_EXECUTE_READ) ||
				(memInfo.Protect & PAGE_EXECUTE_READWRITE) ||
				(memInfo.Protect & PAGE_EXECUTE_WRITECOPY)) {
				return true;
			}
		}

		return false;
	}

	__forceinline void StackWalk()
	{
		return;

		auto highStack = (void**)__readgsqword(0x8);
		auto curStack = (void**)__readgsqword(0x10);

		while (highStack > curStack++) {
			*(volatile PVOID*)curStack;
		}

		//while (highStack > curStack++) {
		//	if (execCheck(*curStack)) {

		//		

		//		//hp(*curStack);
		//	}
		//}
	}


	extern "C" { PVOID legitTrampoline, legitTrampoline2; void _spoofer_stub(); }
	template<typename Ret = void*, typename a1t = void*, typename a2t = void*, typename a3t = void*, typename a4t = void*, typename a5t = void*, typename... Stack>
	__forceinline Ret SpoofCall(PVOID Func, a1t a1 = a1t{}, a2t a2 = a2t{}, a3t a3 = a3t{}, a4t a4 = a4t{}, a5t a5 = a5t{}, Stack... args)
	{
		StackWalk();

		struct callStruct
		{
			PVOID Fn; a5t Arg;
		} CallCtx(Func, a5);
		typedef Ret(*ShellFn)(a1t, a2t, a3t, a4t, callStruct*, Stack...);
		return ((ShellFn)_spoofer_stub)(a1, a2, a3, a4, &CallCtx, args...);

	}
	__forceinline void SetSpoofStub(PVOID Tramp, PVOID Tramp2) {
		legitTrampoline = Tramp; legitTrampoline2 = Tramp2;
	}
#define SpoofCall Module::SpoofCall

	template<typename Ret = void, typename... Args>
	__forceinline Ret VCall(PVOID Class, int Index, Args... AddArgs) {
		return SpoofCall<Ret>((*(PVOID**)Class)[Index], Class, AddArgs...);
	}
#define VCall Module::VCall

	__declspec(noinline) PVOID GetExportAddress(PBYTE hDll, const char* Name)
	{
		//

			//process image data
		PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
		PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

		//process list
		for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
		{
			//get ordinal & name
			USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
			const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];
			if (StrCmp(Name, ExpName, true))
				return (PVOID)(hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal]);
		}

		return nullptr;
	}
#define Module(Mod, Name, ...) [&](){ static PVOID FAddr = nullptr; \
	if (!FAddr) FAddr = (Module::GetExportAddress(Module::GetModuleBase_Wrapper(#Mod), (#Name))); \
	return SpoofCall<decltype(Name(__VA_ARGS__))>((FAddr), __VA_ARGS__); \
}()

	template<typename... Args>
	void Print(const char* Format, Args... args)
	{
		static bool Once = false;
		if (!Once) {
			Module(kernel32, AllocConsole);
			Once = true;
		}

		char Buff[128];
		Module(ntdll, sprintf, Buff, Format, args...);
		int StrLengthOut = Module(ntdll, strlen, Buff);
		HANDLE Out = Module(kernel32, GetStdHandle, STD_OUTPUT_HANDLE);
		Module(kernel32, WriteConsoleA, Out, Buff, StrLengthOut, NULL, NULL);
	}

	void ReadMemory(const void* address, void* buffer, size_t size)
	{

		DWORD back = NULL;

		DWORD instruction = 0x04;

		if (Module(kernel32, VirtualProtect, (LPVOID)address, size, instruction, &back))
		{
			memcpy(buffer, address, size);

			SpoofCall(VirtualProtect, (LPVOID)address, size, back, &back);
		}
	}

	const char* ReadASCII(uint64_t adr)
	{
		char buf[64];
		ReadMemory((const void*)adr, &buf, 64);
		return buf;
	}


}

//FACE STR Helper v2
#define FC_FORMAT_A(...) [&](){wchar_t Str[64];Module(ntdll, _swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_FORMAT_W(...) [&](){wchar_t Str[64];Module(ntdll, _swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_WCHAR(a) [&](){wchar_t Str[64];for(int i=0;;i++){if(!(Str[i]=a[i]))break;}return Str;}()
#define FC_CHAR(a) [&](){char Str[64];for(int i=0;;i++){if(!(Str[i]=a[i]))break;}return Str;}()

//FACE VMT Hook v5
class VMT_Hook11
{
private:
	DWORD Size = 0;
	PVOID* TblEnc = nullptr;

	__forceinline bool InExecMemory(PVOID Addr, DWORD64 ModBase, PIMAGE_NT_HEADERS NtHeader) {
		PIMAGE_SECTION_HEADER Sect = IMAGE_FIRST_SECTION(NtHeader);
		for (DWORD Cnt = 0; Cnt < NtHeader->FileHeader.NumberOfSections; Cnt++, Sect++) {
			if ((Sect->Characteristics & IMAGE_SCN_MEM_EXECUTE) &&
				((DWORD64)Addr >= (ModBase + Sect->VirtualAddress)) &&
				((DWORD64)Addr < (ModBase + Sect->VirtualAddress + Sect->Misc.VirtualSize)))
				return true;
		}
		return false;
	}

	__forceinline PVOID* FindTablePlace(DWORD Size, DWORD64 ModBase, PIMAGE_NT_HEADERS NtHeader) {
		PIMAGE_SECTION_HEADER Sect = IMAGE_FIRST_SECTION(NtHeader);
		for (DWORD Cnt = 0; Cnt < NtHeader->FileHeader.NumberOfSections; Cnt++, Sect++) {
			if (Sect->Characteristics & IMAGE_SCN_MEM_WRITE) {
				DWORD CurSize = 0; PBYTE CurAddr = nullptr;
				for (DWORD i = 0; i < Sect->Misc.VirtualSize; i++) {
					PBYTE Addr = (PBYTE)(ModBase + Sect->VirtualAddress + i);
					if (!*Addr) {
						if (CurAddr == nullptr)
							CurAddr = Addr;
						CurSize++;
						if (CurSize == Size)
							return (PVOID*)CurAddr;
					}
					else {
						CurSize = 0;
						CurAddr = nullptr;
					}
				}
			}
		}
		return 0;
	}

public:
	VMT_Hook11(PVOID Class, int OverSizeMP = 1)
	{


		//check class & find module
		if (!Class) return; Size = 0;
		PVOID* VFTable = *(PVOID**)Class; DWORD64 ModBase = 0;
		if (!VFTable || !VFTable[0]) return; TblEnc = nullptr;
		PTEB TEB = ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)));
		PPEB_LDR_DATA Ldr = TEB->ProcessEnvironmentBlock->Ldr;
		for (PLIST_ENTRY CurEnt = Ldr->InMemoryOrderModuleList.Flink;
			CurEnt != &Ldr->InMemoryOrderModuleList; CurEnt = CurEnt->Flink) {
			LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD(CurEnt, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			if (((DWORD64)VFTable[0] < ((DWORD64)pEntry->DllBase + (DWORD64)pEntry->Reserved3[1])) &&
				((DWORD64)VFTable[0] > ((DWORD64)pEntry->DllBase))) {
				ModBase = (DWORD64)pEntry->DllBase; break;
			}
		}

		//check & get nt headers
		if (ModBase == 0) return;
		PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)
			(ModBase + ((PIMAGE_DOS_HEADER)ModBase)->e_lfanew);

		//get vmt size
		while (InExecMemory(VFTable[Size], ModBase, NtHeader)) Size++;
		if (Size == 0) return;

		//get new vmt place
		PVOID* VFT_New = (PVOID*)Module(kernel32, VirtualAlloc, 0, (Size * 8), MEM_COMMIT, PAGE_READWRITE);//FindTablePlace((Size * 8) * OverSizeMP, ModBase, NtHeader);
		if (!VFT_New) return;

		//move old vmt to new & apply new vmt
		__movsq((PDWORD64)VFT_New, (const PDWORD64)VFTable, Size);
		*(PVOID**)Class = VFT_New; TblEnc = (PVOID*)(VFT_New);


	}

	template <typename T>
	__forceinline void SetupHook(T Orginal, PVOID Hook, int Index) {
		if (!Size || !TblEnc) return;
		PVOID* Tbl = (PVOID*)EPtr(TblEnc);
		Orginal = (T)Tbl[Index];
		Tbl[Index] = Hook;
	}
};


wchar_t DecodeUTF(const char* UTF)
{
	union wcharType
	{
		struct
		{
			UCHAR b1;
			UCHAR b2;
		}; wchar_t ch;
	};

	union charType
	{
		struct
		{
			UCHAR b1 : 4;
			UCHAR b2 : 4;
		}; UCHAR ch;
	};

	//1 byte
	charType b1;
	if ((UTF[0] >= '0') && (UTF[0] <= '9'))
		b1.b2 = UTF[0] - '0';
	else b1.b2 = UTF[0] - 'a' + 10;
	if ((UTF[1] >= '0') && (UTF[1] <= '9'))
		b1.b1 = UTF[1] - '0';
	else b1.b1 = UTF[1] - 'a' + 10;

	//2 byte
	charType b2;
	if ((UTF[2] >= '0') && (UTF[2] <= '9'))
		b2.b2 = UTF[2] - '0';
	else b2.b2 = UTF[2] - 'a' + 10;
	if ((UTF[3] >= '0') && (UTF[3] <= '9'))
		b2.b1 = UTF[3] - '0';
	else b2.b1 = UTF[3] - 'a' + 10;

	//build wchar
	wcharType wchar;
	wchar.b1 = b2.ch;
	wchar.b2 = b1.ch;

	//ret
	return wchar.ch;
}


