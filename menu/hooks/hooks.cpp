#include "hooks.h"
#include "util/memory/memory.h"
#include "util/hooking/hooking.h"
#include "util/log.h"
#include "util/caller.h"
#include "global/vars.h"
#include "rage/invoker/natives.h"
#include "menu/base/util/helpers.h"
#include "util/memory/pattern.h"
#include "util/util.h"
#include "util/threads.h"
#include <intrin.h>
#include "menu/base/submenus/main/network/network_host_toolkit.h"
#include "menu/base/submenus/main/network/network_spoofing.h"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_rid.h"
#include "menu/base/submenus/main/network/spoofing/network_spoofing_ip.h"
#include "menu/base/submenus/main.h"
#include "menu/base/util/global.h"
#include "util/memory/spoof.h"
#include <Psapi.h>

std::uint64_t tmp;




typedef int Cam;




namespace menu::hooks {





	bool bypass() {
		bool successful = true;


		MODULEINFO module_info;
		if (K32GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &module_info, sizeof(MODULEINFO)) == 0)
		{
			LOG_ERROR("unexpected exception - retrieving modules info");
			return FALSE;
		}

		gta5::baseAddress = module_info.lpBaseOfDll;
		gta5::moduleSize = module_info.SizeOfImage;


		mem::module_ctx scan_ctx;
		scan_ctx.start = (DWORD_PTR)module_info.lpBaseOfDll;
		scan_ctx.end = ((DWORD_PTR)module_info.lpBaseOfDll + module_info.SizeOfImage);

		LPVOID rbp_gadget = mem::find(XOR("FF 27").c_str(), &scan_ctx).get_addr();
		if (rbp_gadget == NULL)
		{
			return FALSE;
		}

		size_t spoofer_length = sizeof(spoofer_payload) + sizeof(spoofer_cleanup);
		x64::spoofer_size = spoofer_length;


		LPVOID location = NULL;
		x64::spoofer_cave = NULL;
		if ((x64::spoofer_cave = VirtualAlloc(NULL, spoofer_length, MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
		{
			return FALSE;
		}


		LPVOID ret0 = NULL;
		memcpy(x64::spoofer_cave, spoofer_payload, sizeof(spoofer_payload));
		memcpy((LPVOID)((DWORD_PTR)x64::spoofer_cave + sizeof(spoofer_payload)), spoofer_cleanup, sizeof(spoofer_cleanup));
		memset(spoofer_payload, 0, sizeof(spoofer_payload));
		memset(spoofer_cleanup, 0, sizeof(spoofer_cleanup));

		DWORD_PTR cleanup_address = (DWORD_PTR)x64::spoofer_cave + sizeof(spoofer_payload);
		for (DWORD i = 0; i < sizeof(spoofer_payload); i++)
		{
			if (*reinterpret_cast<PBYTE>((LPVOID)((DWORD_PTR)x64::spoofer_cave + i)) == (BYTE)0xCC)
			{
				memcpy((LPVOID)((DWORD_PTR)x64::spoofer_cave + i), &cleanup_address, sizeof(DWORD_PTR));
				break;
			}
		}

		x64::fastcall_ctx.gadget = rbp_gadget;
		x64::fastcall_ctx.payload = x64::spoofer_cave;

		location = NULL;

		if ((location = mem::find(XOR("48 85 C0 0F 84 ? ? ? ? 8B 48 50").c_str(), &scan_ctx).get_addr()) == NULL)
		{
			return FALSE;
		}
		if (location) memset(reinterpret_cast<void*>((std::uintptr_t)location), 0x90, 24);

		location = NULL;

		if ((location = mem::find(XOR("48 8B C8 FF 52 30 84 C0 74 05 48").c_str(), &scan_ctx).get_addr()) == NULL)
		{
			return FALSE;
		}
		if (location) memset(reinterpret_cast<void*>((std::uintptr_t)location + 0x8), 0x90, 2);

		location = NULL;

		if ((location = mem::find(XOR("8B 0D ? ? ? ? 49 8D 56 28").c_str(), &scan_ctx).add(2)->rip()->get_addr()) == NULL)
		{
			return FALSE;
		}
		global::vars::tick_count = (uint32_t*)location;

		location = NULL;

		if ((location = mem::find(XOR("48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41").c_str(), &scan_ctx).add(-15)->get_addr()) == NULL)
		{
			return FALSE;
		}
		global::vars::trigger_script_event = (global::vars::fpTriggerScriptEvent)location;

		location = NULL;

		if ((location = mem::find(XOR("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").c_str(), &scan_ctx).add(3)->rip()->get_addr()) == NULL)
		{
			return FALSE;
		}
		global::vars::GTA_GAME_GLOBALS = (std::uintptr_t)location;

		location = NULL;



		const uint8_t patch[7]{ 0xBB, 0x03, 0x00, 0x00, 0x00, 0x39, 0x1D };


		if ((location = mem::find(XOR("BB 02 00 00 00 39 1D").c_str(), &scan_ctx).get_addr()) == NULL)
		{
			return FALSE;
		}
		tmp = (std::uintptr_t)location;
		tmp = (uint8_t)patch;
		tmp = NULL;

		return successful;
	}

	bool globals() {
		bool successful = true;

		Module::SetSpoofStub(FindPattern2(XOR("48 83 C4 68 C3").c_str()), FindPattern2(XOR("FF E3").c_str()));

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern3(XOR("socialclub.dll").c_str(), XOR("48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85").c_str()));
		global::vars::g_social_club_game_info_table = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern3(XOR("socialclub.dll").c_str(), XOR("E8 ? ? ? ? 85 C0 78 27 3B 47 14 7D 1E 48 98 48 8B D6 48 69 C8").c_str()));
		global::vars::g_social_club_get_game_info_table = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("8B 05 ? ? ? ? 85 C0 0F 84 ? ? ? ? 83 F8 06 74 08 83 C0 F8").c_str()));
		global::vars::g_game_state = (game_state*)memory::read_instruction(tmp, 2, 6);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18").c_str()));
		global::vars::GTA_SET_VECTOR_RESULTS = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("23 C3 C1 E0 0C").c_str()));
		global::vars::GTA_JMP_RBX = tmp - 1;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 48 8B 14 FA E8").c_str()));
		global::vars::GTA_NATIVE_REGISTRATION = memory::read_instruction(tmp);
		global::vars::GTA_GET_NATIVE_HANDLER = memory::read_instruction(tmp + 0xB, 1, 5);
		global::vars::g_native_registration = (rage::invoker::native_registration**)memory::read_instruction(tmp);
		global::vars::g_translate_native = memory::read_instruction(tmp + 0xB, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 48 8B C2 48 8B D9 33 D2 48 8B C8 E8 ? ? ? ? 33 D2 44 8B D0 F7 35 ? ? ? ? 44 8B C2 48 8B 15 ? ? ? ? 46 8B 0C 82 41").c_str()));
		global::vars::g_get_store_module_extension = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("89 54 24 10 55 53 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 83 79 ? ? 41 8B F8 48 8B F1 75 07 32 C0 E9 ? ? ? ? 48 8B 06").c_str()));
		global::vars::g_request_streaming_file = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 C7 41 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 C7 41 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 01").c_str()));
		global::vars::g_construct_gfx_font = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 EC 28 F0 FF 49 08 75 0F 48 85 C9 74 0A 48 8B 01 BA ? ? ? ? FF 10 48 83 C4 28 C3").c_str()));
		global::vars::g_destruct = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 55 48 8B EC 48 81 EC ? ? ? ? 49 8B 00 F3 0F 10 02 F3 0F 10 4A ? 83 65 B8 00 83 65 C0 00 83 65 C4 00 F3").c_str()));
		global::vars::g_create_id_for_font = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 48 8B 41 40 4D 63 C8 4C 8B D9 46 0F B6 14 08 48 8B DA 41 81 E2 ? ? ? ? 41 8B C2 45 8B CA 49 F7 D9 4C 0B").c_str()));
		global::vars::g_add_font_lib = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? F3 0F 10 35 ? ? ? ? F3 0F 10 44 1D ? F3 0F 10 4C 1D ? 8B 47 28 41 0F 28 DB 41 0F 28").c_str()));
		global::vars::g_vertex_begin = memory::read_instruction(tmp, 1, 5);
		global::vars::g_vertex_add = memory::read_instruction(tmp + 0x4D, 1, 5); 
		global::vars::g_vertex_end = memory::read_instruction(tmp + 0x5C, 1, 5); 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 10 55 56 57 41 54 41 56 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 C8 0F 29 78 B8 44 0F 29 40 ? 44 0F").c_str()));
		global::vars::g_render_script_texture = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 48 8B CB 48 69 C9 ? ? ? ? 40 38 AC 01 ? ? ? ? 74 0F E8 ? ? ? ? 0F 28 CE 8B CB E8 ? ? ? ? F3 0F 10 05 ? ? ? ? B2 01 E8").c_str()));
		global::vars::g_font_table = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D 3F 6D A2 01").c_str()));
		global::vars::g_font_memory_helper = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 85 D2 74 10 49 83 C8 FF 49 FF C0 42 80 3C 02 ? 75 F6 EB 03 45 33 C0 E9").c_str()));
		global::vars::g_get_font_id_table = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 45 33 C0 48 8B 01 41 8D 50 20 FF 50 50 48 85 C0 74 0D 48 8B C8 E8").c_str()));
		global::vars::g_memory_heap = *(rage::types::memory_heap_pt**)(memory::read_instruction(tmp));
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 03 D3 E8 ? ? ? ? 66 44 39 7D ? 74 09 48 8B 4D E8 E8").c_str()));
		global::vars::g_store_manager = (rage::types::store_manager*)(memory::read_instruction(tmp));
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 57 52 8F 01").c_str()));
		global::vars::g_d3d11_device = *(uint64_t*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 45 33 C0 48 8B 01 33 D2 FF 90 ? ? ? ? 48 8B 0D ? ? ? ? 83 64 24").c_str()));
		global::vars::g_grc_texture_factory_dx11 = *(uint64_t*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 63 FA 48 8B D9 E8 ? ? ? ? C7 43 ? ? ? ? ? 48 8D 05 ? ? ? ? 48 89 03").c_str()));
		global::vars::g_thread_alloc = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 ? ? ? ? 33 D2 48 89 44 24 ? 48 8D 05 ? ? ? ? 48 8B CB 48 89 44 24 ? 48 8D 05 ? ? ? ? 48 89 44 24").c_str()));
		global::vars::g_setup_dictionary = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 8B 00 48 8B 0D ? ? ? ? 48 63 D0 0F AF 05 ? ? ? ? 44 0F B6 04 0A 48 63 D0 41 FF C7 48 03 15").c_str()));
		global::vars::g_setup_store_item = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 33 D2 4C 8B D9 85 D2 78 3A 48 8B 41 40 4C 63 C2 46 0F B6 0C 00 8B 41 4C 41 81 E1 ? ? ? ? 45 8B C1 0F AF").c_str()));
		global::vars::g_is_valid_pool_slot = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("89 54 24 10 48 83 EC 28 48 8B 41 40 4C 63 CA 46 0F B6 14 08 8B 41 4C 41 81 E2 ? ? ? ? 45 8B CA 0F AF C2").c_str()));
		global::vars::g_add_dictionary_to_pool = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 0F B7 41 28 45 33 D2 8B DA 4C 8B C9 49 8B F8 41 8B CA 8B D0 41 8D 72 01").c_str()));
		global::vars::g_add_grc_texture_to_dictionary = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? E8 ? ? ? ? 39 05 ? ? ? ? 74 0B 39 05").c_str()));
		global::vars::g_get_weather_id = memory::read_instruction(tmp + 7, 1, 5);
		global::vars::g_weather_table = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? E8 ? ? ? ? 83 38 FF 74 2D").c_str()));
		global::vars::g_game_streamed_scripts = memory::read_instruction(tmp);
		global::vars::GTA_STREAMED_SCRIPTS = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B EA 48 8B F1 49 8B D0 B9 ? ? ? ? 49 8B F8 E8").c_str()));
		global::vars::g_get_streamed_script = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B").c_str()));
		global::vars::g_ped_factory = *(rage::types::ped_factory**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").c_str()));
		global::vars::g_global_cache = (uint64_t**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("8B 05 ? ? ? ? 44 8B D3 8D 48 FF 85 C9 78 35 46").c_str()));
		global::vars::g_weapon_components = { memory::read_instruction(tmp, 2, 6), memory::read_instruction(tmp + 0x14) }; 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("44 0F B7 05 ? ? ? ? 44 8B DD 41 FF C8 78 28 48 8B 1D").c_str()));
		global::vars::g_weapon_info = { memory::read_instruction(tmp, 4, 8), memory::read_instruction(tmp + 0x10) }; 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 4A 8B 1C F0 48 85 DB 0F 84").c_str()));
		global::vars::g_game_camera_angles = (rage::network::game_camera_angles*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B D3 48 8B F8 E8 ? ? ? ? 48 85 FF 74 3B 48 8B 17 48 8B CF FF 52").c_str()));
		global::vars::g_get_third_person_camera = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 8B A8 ? ? ? ? E8 ? ? ? ? 48 8B F8").c_str()));
		global::vars::g_third_person_camera = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 48 8B D8 48 85 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 8B D7 E8 ? ? ? ? 48 8B F8 48 85 C0").c_str()));
		global::vars::g_get_entity_address = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 F7 F9 49 8B 48 08 48 63 D0 C1 E0 08 0F B6 1C 11 03 D8").c_str()));
		global::vars::g_get_entity_handle_from_address = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 EC 28 33 C0 38 05 ? ? ? ? 74 0A 83 F9 1F 77 05 E8 ? ? ? ? 48 83 C4 28").c_str()));
		global::vars::g_get_net_game_player_from_index = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 0F B7 45 58 66 89 45 50 8B 45 50 41 0B C7 41 23 C6 0F").c_str()));
		global::vars::g_get_model_info = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("8B 91 ? ? ? ? F3 0F 10 05 ? ? ? ? 8D 42 FD A9").c_str()));
		global::vars::g_set_vehicle_gravity = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8B 0D ? ? ? ? 45 33 C0 4D 85 C9 75 03 33 C0 C3").c_str()));
		global::vars::g_get_vehicle_paint_array = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 EC 28 45 8B C8 44 8B C2 8B D1 48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 05").c_str()));
		global::vars::g_draw_origin_index = memory::read_instruction(tmp, 2, 6);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 0F B7 45 58 66 89 45 50 8B 45 50 41 0B C7 41 23 C6 0F").c_str()));
		global::vars::g_add_to_clock_time = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 3D 24 BF F3 01").c_str()));
		global::vars::g_dispatch_service_table = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 33 C9 48 85 C0 74 2A 44 8D 41 01 66 89 4C 24 ? 48 89 4C 24 ? 4C 8D 4C 24 ? 48").c_str()));
		global::vars::g_get_host_net_game_player = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 8B ? ? ? ? E8 ? ? ? ? 8B D7 B9 ? ? ? ? 4C 8B C0 E8").c_str()));
		global::vars::g_send_increment_stat_event = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 60 ? ? 48 8D 05 ? ? ? ? 48 89 03 EB 02 33 DB 48 85 DB 0F 84 ? ? ? ? 48").c_str()));
		global::vars::g_update_net_object_owner = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? E8 ? ? ? ? 4C 8B 13 4C 8D 05 ? ? ? ? 48").c_str()));
		global::vars::g_network_object_manager = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? E8").c_str()));
		global::vars::g_network_player_manager = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 33 C9 45 85 C0 74 56 41 FF C8 74 4C 41 FF C8 74 42 41 FF C8 74 38 41 FF C8 74 2E 41 FF").c_str()));
		global::vars::g_handle_rotation_values_from_order = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B F0 E8 ? ? ? ? 33 FF 48").c_str()));
		global::vars::g_get_net_game_player_from_handle = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B FA 4C 8B C1 48 8D 54 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B D8 83").c_str()));
		global::vars::g_get_sprite_texture = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 9C 24 ? ? ? ? E8 ? ? ? ? 48 8D 8C 24 ? ? ? ? 48 8B D8 E8 ? ? ? ? 48 8D 94 24 ? ? ? ? 0F 10").c_str()));
		global::vars::g_send_text_message_net_msg = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 30 80 3D ? ? ? ? ? 0F 84 ? ? ? ? 48 8D 1D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05").c_str()));
		global::vars::g_set_traffic_lights = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 A1 48 81 EC ? ? ? ? 83 3D").c_str()));
		global::vars::g_update_traffic_lights = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 55 48 8D 68 B8 48 81 EC ? ? ? ? 0F 28 05 ? ? ? ? 0F 28 2A 0F 29 70 E8").c_str()));
		global::vars::g_set_traffic_light_colors = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 80 3D ? ? ? ? ? 0F 84 ? ? ? ? 48 8D 3D ? ? ? ? 48 8D 1D").c_str()));
		global::vars::g_set_vehicle_lights = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("0F B7 CA 83 F9 07 7F 5E 74 54 85 C9 74 48 FF C9 74 3C FF C9 74 30").c_str()));
		global::vars::g_get_sync_tree_from_clone_type = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 66 89 54 24 ? 56 57 41 56 48 83 EC 20 45").c_str()));
		global::vars::g_get_network_object_from_network_id = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 83 64 24 ? ? 48 8B FA BB ? ? ? ? 48 8D 54 24 ? 44 8B C3 E8").c_str()));
		global::vars::g_read_bool_from_bit_buffer = tmp;
		global::vars::g_read_uint_from_bit_buffer = memory::read_instruction(tmp + 0x1F, 1, 5); 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 48 8B 4C 24 ? 48 8D 53 34 41 B8 ? ? ? ? E8 ? ? ? ? 40 84 FF 74 1E 8B 0E E8").c_str())); 
		global::vars::g_read_int_from_bit_buffer = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 70 10 57 48 83 EC 60 48 8B F2 48 8B F9 48 8D 50 C8 48 8D 48 E8 41 8B D9 E8").c_str())); 
		global::vars::g_read_vector3_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C 8B D2 48 8B D9 45 85 C0 74 4F 8A 41 1C A8 02 75 48 A8").c_str())); 
		global::vars::g_read_array_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F8 8A 43 1C 48 8B F2 A8 02 75 45 A8 01 74 05").c_str())); 
		global::vars::g_read_short_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 56 57 41 56 48 83 EC 20 41 8B F8 33 DB 41 B8 ? ? ? ? 4C 8B F2 48 8B F1 89 58 18 41 3B F8 7E 38 48 8D 50 20 89 58 20 E8").c_str()));
		global::vars::g_read_ulonglong_from_bit_Buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8A 42 1C 40 32 FF 48 8B DA 48 8B F1 A8 02 0F 85 ? ? ? ? A8 01 74 05 8B 4A 0C EB 03 8B 4A 14 8B 42 10 83").c_str()));
		global::vars::g_read_msg_header_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 44 8B DA 83 CB FF 4D 63 D1 49 C1 FA 03 41 83 E1 07 4C 03 D1 B9 ? ? ? ? 41 2B C8 D3 E3 41 D3 E3 41 8B C9 8B D3 41 8B C3 C1 EA").c_str()));
		global::vars::g_write_uint_to_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B 01 48 8D 71 0C 48 8B FA 66 83 3E 08 48 8B D9 0F 93 44 24 ? FF 50 10 48").c_str())); 
		global::vars::g_read_pickup_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 48 8B D9 48 81 C1 ? ? ? ? 48 8B EA 33 FF E8 ? ? ? ? 48 3B E8 75 09 48 8D BB ? ? ? ? EB 3D 8B F7 39 BB").c_str())); 
		global::vars::g_get_network_config_from_peer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D").c_str()));
		global::vars::g_network_event_registration = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 48 98 4C 8B C6 41 8B 44 85 ? 0D ? ? ? ? 89 44 24 30 83 64 24 ? ? C6 44 24").c_str()));
		global::vars::g_waypoint_data = (rage::types::waypoint_data*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 41 8A D9").c_str()));
		global::vars::g_net_shop_queue = *(rage::network::net_shop_queue**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 EC 18 48 8B 0D ? ? ? ? 4C 8D 05 ? ? ? ? F3 0F 10 89 ? ? ? ? F3 0F 10 81 ? ? ? ? F3 0F 10 99 ? ? ? ? 0F 14 D9").c_str()));
		global::vars::g_update_ui_values = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 53 48 81 EC ? ? ? ? 80 B9 ? ? ? ? ? 0F 29 70 E8 0F 29 78 D8 48 8B D9 44 0F 29 40 ? 44 0F 29 48 ? 44").c_str())); 
		global::vars::g_update_ui_values_2 = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 0F 29 44 24 ? E8 ? ? ? ? 0F 28 05 ? ? ? ? 0F 59 44 24 ? F3 48 0F 2C C0 0F C6 C0 55").c_str())); 
		global::vars::g_ui_visual_settings = (rage::types::ui_visual_settings*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 30 F3 0F 10 15 ? ? ? ? 48 8D 15 ? ? ? ? 41 B1 01 48 8B D9 E8 ? ? ? ? 48 8D 15 ? ? ? ? F3 0F").c_str()));
		global::vars::g_set_vehicle_neon_ui = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 ? ? ? ? 4C 8D 1C 7F 48 8B 4A 10 49 C1 E3 05 4C 03 D8 0F B6 85 ? ? ? ? 48 23 C8 8B 42 18 48 3B").c_str()));
		global::vars::g_vfx_liquid = (rage::types::vfx_liquid*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 8A D9 8A D1 48 8D 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 88 1D ? ? ? ? 48 83 C4 20").c_str()));
		global::vars::g_set_seethrough = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 30 45 8B F9 41 8B E8 4C 8B F2 48 8B F1 33 DB E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 33 D2").c_str()));
		global::vars::g_add_basket_to_queue = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F2 48 8D 54 24").c_str())); 
		global::vars::g_add_item_to_basket = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 30 48 83 3D ? ? ? ? ? 8B").c_str())); 
		global::vars::g_send_session_info_request = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 ? ? ? ? 4D 69 C0 ? ? ? ? 4C 03 C0 41 83 B8 ? ? ? ? ? 0F 86 ? ? ? ? 83 CD FF 48 8D 15").c_str()));
		global::vars::g_rs_info = (rage::network::rs_info*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B F2 33 D2 49 8B F8 4C 8B F1 44 8D 42 7C 48 8D 4C 24 ? 41 8A E9 C6 44 24 ? ? E8").c_str()));
		global::vars::g_get_session_info_from_gs = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 83 EC 60 4C 8B F9 48 8D 48 C8 49 8B F1 49").c_str()));
		global::vars::g_send_network_event_reply = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 83 EC 60 4C 8B F9 48 8D 48 C8 49 8B F1 49").c_str()));
		global::vars::g_send_network_event_reply = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 8B 6C 24 60 8B 74 24 68 49 8B C1 4D 8B D0").c_str()));
		global::vars::g_send_network_event_ack = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8B DC 49 89 5B 08 49 89 73 10 49 89 7B 18 41 56 48 83 EC 40 48 8B F1 48 8B C2 49 89 53 F0 48 8D 0D").c_str())); 
		global::vars::g_setup_give_control_event = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("80 3D ? ? ? ? ? 75 03 32 C0 C3 4C 8B CA 8B 15 ? ? ? ? 4C 8B C1 48 8D 0D ? ? ? ? E9").c_str())); 
		global::vars::g_send_friend_request = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 89 03 85 C0 78 21 8B 13 48 8B 8F ? ? ? ? 4C 8D 84 24 ? ? ? ? 41 B9 ? ? ? ? 4C 89 6C 24 ? E8").c_str())); 
		global::vars::g_get_client_player_key = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("F3 0F 10 05 ? ? ? ? 0F 57 C9 0F 2F C1 76 1B 48 8B 05 ? ? ? ? 0F 2F 48 70 73 0E 48 8D 0D ? ? ? ? 33").c_str()));
		global::vars::g_render_water_on_map_intensity = memory::read_instruction(tmp, 4, 8);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("0F 2F 35 ? ? ? ? 0F 86 ? ? ? ? 0F 28 05 ? ? ? ? 48 8D 4D B0 41 0F 28 D9 48 8B D3 0F 28 D7 0F 29 45 A0").c_str()));
		global::vars::g_map_fog_intensity = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 ? ? ? ? 48 6B FF 45 F3 0F 59 0D ? ? ? ? F3 41 0F 59 9E ? ? ? ? F3 0F 10 BD ? ? ? ? 48 03 FE 48 69 FF ? ? ? ? F3").c_str()));
		global::vars::g_vfx_wheel = { (rage::types::vfx_wheel*)memory::read_instruction(tmp), *(uint8_t*)(tmp + 0xA) }; 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 03 0D ? ? ? ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 84 C0 74 10 48 8D 4C 24 ? 44 8B").c_str())); 
		global::vars::g_friends = *(rage::network::friends**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 48 8B EA 44 8B F1 49 8B D9 41").c_str())); 
		global::vars::g_send_presence_event = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 50 48 8B FA 48 8B D9 E8 ? ? ? ? 83 7B 10 08").c_str())); 
		global::vars::g_migrate_script_host = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 83 FB 01 75 2D 8B 45 7F 48 8D 55 CF 48 8D 4D EF 44 8B CE").c_str()));
		global::vars::g_is_reported_for_reason = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 45 EC 4C 8D 45 F0 48 8D 55 EC 48 8D 0D ? ? ? ? 89 45 F0 E8").c_str())); 
		global::vars::g_texture_store = (rage::types::texture_store*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 18 44 89 48 20 89 50 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81").c_str())); 
		global::vars::g_send_matchmaking_request = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9 B2").c_str())); 
		global::vars::g_register_streaming_file = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 E8 ? ? ? ? 48 8B 43 60 48 89 47 60 48 8B 43 68 48 89 47 68 0F 10 43").c_str())); 
		global::vars::g_create_game_invite_presence = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 0F B7 D9 48 8B 0D ? ? ? ? B2 01 E8 ? ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 33 C0 48 85 C9 74 08 0F B7 D3 E8 ? ? ? ? 48 8B 0D").c_str())); 
		global::vars::g_send_ragdoll_event = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 07 48 8B CF FF 90 ? ? ? ? 84 C0 74 07 41 C7 06 ? ? ? ? 41 39 36 76 5E 4C 8B 17 45 33 C9 48").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 11 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("33 F6 39 75 00 0F 86 ? ? ? ? 48 8B 03 45 33 C9 44 8B FE 45 8D 41 03 48 8B CB 4D 6B FF 64 4D 8D 34 3F 49").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 2 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 07 48 8B CF FF 90 ? ? ? ? 84 C0 74 07 41 C7 06 ? ? ? ? 44 38 23 74 14 48 8B 07 48 8B CF FF 90").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 10 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 06 4C 8D B5 ? ? ? ? 49 8B D6 45 33 C0 48 8B CE FF 50 38 48 8B 06 48 8B CE FF 90 ? ? ? ? 33").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 32 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 8B F4 45 39 27 76 7C 48 8B 07 45 33 C9 48 8D 96 ? ? ? ? 41 8B DE 45 8D 41 20 48 8B CF 48 8D 14").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 11 });
		global::vars::g_return_address_node_iterations.push_back({ tmp + 0xBB, 3 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 06 48 8B CE FF 90 ? ? ? ? 33 DB 84 C0 74 06 C7 07 ? ? ? ? 39 1F 76 21 4C 8B 0E 48 8D 95 ? ? ? ? 45 33 C0").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 32 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 48 8B 07 48 8D 9E ? ? ? ? 48 8B D3 45 33 C0 48 8B CF FF 50 38 45 33 FF 44 38 3B 75 1C 48 8B 07").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 30 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 06 45 33 C9 4D 8D AE ? ? ? ? 45 8D 41 08 49 8B D5 48 8B CE FF 50 68 33 ED 4D 8D A6 ? ? ? ? 41").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 3 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("33 ED 41 39 2E 76 2D 48 8D 96 ? ? ? ? 45 33 C0 48 8B CF 48 8D 14 AA E8 ? ? ? ? C6 84 35 ? ? ? ? ? C6 84 35").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 105 });
		global::vars::g_return_address_node_iterations.push_back({ tmp + 0x4D, 65 }); 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("B8 ? ? ? ? 41 39 06 41 0F 42 06 33 ED 41 89 06 85 C0 74 7A 44 8B FD 45 33 C0 48 8B CF").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 10 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("44 8B E7 41 39 3F 76 66 48 8B 06 45 33 C9 41 8B FC 49 8D 96 ? ? ? ? 48 8D 1C 7F 45 8D 41 03").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 3 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 8B F7 44 39 3B 76 24 4C 8B 17 45 33 C9 41 8B D6 48 83 C2 4A 45 8D 41 20 48 8B CF 48 8D 14 96 41 FF 52 68 41 FF C6 44 3B 33 72 DC 48 8B").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 11 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 07 48 8B CF FF 90 ? ? ? ? 84 C0 74 06 C7 03 ? ? ? ? 41 BF ? ? ? ? 44 38 65 00 75 33 45 38 26 75 2E 48 8B 07 48 8B CF").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 10 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 33 E4 41 8B EC 45 39 26 76 44 48 8B 07 44 8B FD 45 33 C0 49 8D 1C 37 48 8B CF 48 8D 93 ? ? ? ? FF 50 38 44 38 A3 ? ? ? ? 74 1A 48 8B 07").c_str())); 
		global::vars::g_return_address_node_iterations.push_back({ tmp, 16 });
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 4C 8D 44 24 ? 48 8D 95 ? ? ? ? 48 89 85 ? ? ? ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D").c_str())); 
		global::vars::g_global_chat_ptr = *(uint64_t*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 44 38 60 14 75 06 44 39 60 04 74 10 33 C9 E8 ? ? ? ? 83 B8").c_str())); 
		global::vars::g_chat_config = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("44 8B CA 4C 8B C1 0F B7 C2 85 D2 78 31 3B 81 ? ? ? ? 73 29 8A 89 ? ? ? ? 8B D0 48 D3 EA 49 8B 88").c_str())); 
		global::vars::g_get_player_info_from_net_msg = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 2D ? ? ? ? 0F 2F C6 0F 97 C0 88 44 24 40 84 C0 74 4A 8B 0D ? ? ? ? 83 CB FF 44").c_str())); 
		global::vars::g_drop_shaders = memory::read_instruction(tmp);
		global::vars::g_drop_shader_count = memory::read_instruction(tmp + 0x15, 2, 6); 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 4C 8B E8 48 85 C0 0F 84 ? ? ? ? 48 8B C8 E8 ? ? ? ? 84 C0 0F 84").c_str())); 
		global::vars::g_get_net_msg_net_player = memory::read_instruction(tmp, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8B 15 ? ? ? ? F3 0F 11 45 ? F3 0F 10 05 ? ? ? ? BF ? ? ? ? BB ? ? ? ? 41 BE ? ? ? ? F3 0F 11").c_str())); 
		global::vars::g_ui_3d_draw_manager = memory::read_instruction(tmp);
		global::vars::g_push_scene_preset_to_manager = memory::read_instruction(tmp + 0x5D, 1, 5);
		global::vars::g_add_element_to_scene = memory::read_instruction(tmp + 0x9F, 1, 5);
		global::vars::g_set_scene_element_lighting = memory::read_instruction(tmp + 0xE4, 1, 5);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 41 B8 ? ? ? ? 48 8B D7 48 8B C8 E8 ? ? ? ? EB 07 48 8D 05 ? ? ? ? 48 8B 5C 24 ? 48 83 C4 20 5F C3").c_str())); 
		global::vars::g_invalid_string_address = memory::read_instruction(tmp + 0x18);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 55 56 57 48 83 EC 20 33 C0 48 8B EA 48 8B D9 8D 70 01 C6 44 24 ? ? 89 44 24 51 66 89 44 24 ? 88").c_str())); 
		global::vars::g_read_token_from_bit_buffer = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 20 48 8B F2 45 8B F0 32 DB 88 1A 48 8D 54 24 ? 41 B8 ? ? ? ? 48 8B").c_str())); 
		global::vars::g_read_string_from_bit_buffer = tmp;
		tmp = NULL;


		return successful;
	}

	bool globals_in_game() {
		bool successful = true;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 8B 78 10 85 FF").c_str())); 
		global::vars::g_object_pool = *(guid_pool**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 41 0F BF C8 0F BF 40 10").c_str())); 
		global::vars::g_ped_pool = *(guid_pool**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 45 33 C9 44 8B C5 BA ? ? ? ? E8 ? ? ? ? 48").c_str())); 
		global::vars::g_vehicle_pool = *(vehicle_pool**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 0F B7 48 10 66 03 CA 66 44 03 C1 41 80 F9 04").c_str())); 
		global::vars::g_pickup_pool = *(guid_pool**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D ? ? ? ? ? C1 EF 12 8B D0 89 85 88 00 00 00 40 80 E7 01 E8").c_str())); 
		global::vars::g_explosion_data = *(rage::types::explosion_data**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 41 0F 28 D9 45 33 C0 48 8B D3 E8 ? ? ? ? 44").c_str())); 
		global::vars::g_explosion_fx = *(rage::types::explosion_fx**)(memory::read_instruction(tmp) + 0x10);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 0D ? ? ? ? 33 D2 E8 ? ? ? ? 84 C0 75 ? 48 8B 05 ? ? ? ? BA").c_str())); 
		global::vars::g_network_base_config = *(rage::network::network_base_config**)(memory::read_instruction(tmp));
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 33 DB 44 8D 43 01 41 3B C8 75").c_str()));
		global::vars::g_ocean_quads.m_ocean = (rage::types::ocean_quad_info*)memory::read_instruction(tmp);
		global::vars::g_ocean_quads.m_calming = (rage::types::ocean_quad_info*)memory::read_instruction(tmp + 0xC);
		global::vars::g_ocean_quads.m_wave = (rage::types::ocean_quad_info*)memory::read_instruction(tmp + 0x25);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8D 0D ? ? ? ? 48 0F 45 D1 48 8B 0D ? ? ? ? 48 83 64 24 ? ? 4C 8D 05 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85").c_str())); 
		global::vars::g_water_tune = (rage::types::water_tune*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 0D ? ? ? ? 44 8A CE 44 8A C5 8B D3 44 89 64 24 ? 89 7C 24 20 E8 ? ? ? ? 8D").c_str())); 
		global::vars::g_ui_weather = (rage::types::ui_weather*)(memory::read_instruction(tmp) + 0x60);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 05 ? ? ? ? 83 20 00 48 8B 0D ? ? ? ? 48 83 C4 28 E9").c_str())); 
		global::vars::g_ui_puddle = *(rage::types::ui_puddle**)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("44 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8D 15 ? ? ? ? 48 8D 05 ? ? ? ? 48 0F 45 C1 44 39 2D ? ? ? ? 48 8D 0D").c_str())); 
		uint64_t info = memory::read_instruction(tmp);
		if (info) {
			RECT desktop;
			GetWindowRect(GetDesktopWindow(), &desktop);
			global::vars::g_resolution = { *(int*)(info), *(int*)(info + 4) };
			global::vars::g_desktop_resolution = { (int)desktop.right, (int)desktop.bottom };
			LOG_WARN("Game Resolution: %ix%i, Screen Resolution: %ix%i",
				global::vars::g_resolution.x, global::vars::g_resolution.y,
				global::vars::g_desktop_resolution.x, global::vars::g_desktop_resolution.y);

		}
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("4C 8D 05 ? ? ? ? 0F B7 C1").c_str())); 
		global::vars::g_blip_list = (rage::types::blip_list*)memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("0F B7 15 ? ? ? ? 45 33 F6 8B E9 85 C9 B8 ? ? ? ? 45 8D 7E 01 41 8A F6 41 8B DE 0F 44 E8").c_str())); 
		global::vars::g_script_handlers = { memory::read_instruction(tmp), *(uint64_t*)(memory::read_instruction(tmp) - 8) };
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 49 8B D9 41 8B F8 48 8B").c_str())); 
		global::vars::g_invite_player = tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC ? 33 DB 85 C9 78 10 8B D1 48 8B 0D ? ? ? ? E8 ? ? ? ? EB 03").c_str())); 
		hooking::detour(XOR("DCE").c_str(), tmp, &does_cam_exist, &does_cam_exist_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 81 EC ? ? ? ? 44 8B 81").c_str())); 
		hooking::detour(XOR("SEP").c_str(), tmp, &script_event_protection, &script_event_protection_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 74 24 ? 57 48 83 EC 20 48 8B EA 0F B7 51 30 48 8B F9").c_str())); 
		hooking::detour(XOR("RCE").c_str(), tmp - 0x10, &request_control, &request_control_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 85 C0 0F 84 ? ? ? ? 8B 48 50").c_str())); 
		global::vars::m_world_model_spawn_bypass = (PVOID)tmp;
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 40 48 8B B1 ? ? ? ? 8A 9A ? ? ? ? 32").c_str())); 
		hooking::detour(XOR("C1").c_str(), tmp, &apply_vehicle_data__crash, &apply_vehicle_data__crash_t);/*1.64*/
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 48 8B D9 48 8B 49 30 45 33").c_str())); 
		hooking::detour(XOR("C2").c_str(), tmp, &attach_trailer__crash, &attach_trailer__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 81 EC ? ? ? ? 48 8B D9 48 8B 89 ? ? ? ? 0F 29 70 E8 0F 29 78 D8 48 8B F2 E8").c_str())); 
		hooking::detour(XOR("C3").c_str(), tmp, &attach_trailer_audio__crash, &attach_trailer_audio__crash_t);
		tmp = NULL;

		hooking::detour(XOR("C5").c_str(), tmp, &shit_scripts__crash, &shit_scripts__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F9 48 83 C1 30 41 8A F1 48 8B 19 EB 10").c_str())); 
		hooking::detour(XOR("C6").c_str(), tmp, &shit_scripts2__crash, &shit_scripts2__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 3D ? ? ? ? ? 41 8B D8 8B F2 48 8B F9 74 2F E8").c_str())); 
		hooking::detour(XOR("C7").c_str(), tmp, &sub_140AD8424__crash, &sub_140AD8424__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8D 05 ? ? ? ? 48 8D 8E ? ? ? ? 48 8D 54 24 ? 48 89 5C 24 ? 48 89 44 24").c_str())); 
		global::vars::g_sync_data_reader_vtable = memory::read_instruction(tmp);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F9 48 8B CA 49 8B E9").c_str())); 
		hooking::detour(XOR("PCC").c_str(), tmp, &pack_clone_create, &pack_clone_create_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 02").c_str())); 
		hooking::detour(XOR("PCS").c_str(), tmp, &pack_clone_sync, &pack_clone_sync_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B F9 48 8D 0D ? ? ? ? E8 ? ? ? ? 80 3D ? ? ? ? ? 75 56 33 C9 E8 ? ? ? ? 48 8B D8 E8").c_str())); 
		hooking::detour(XOR("GPT").c_str(), tmp, &get_peer_address, &get_peer_address_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 41 54 41 56 48 8D 68 88 48 81 EC 60 01 00 00 33").c_str())); 
		hooking::detour(XOR("RSIR").c_str(), tmp, &receive_session_info, &receive_session_info_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B 05 ? ? ? ? 33 DB 48 85 C0 74 39 8B 50 20 8B 40 10 C1 E2 02 C1 FA 02 3B C2").c_str())); 
		hooking::detour(XOR("HNGE").c_str(), tmp, &process_game_event, &process_game_event_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 84 C0 74 60 4C 39 77 40 74 29 48 8B 4C 24 ? 48 8B 01 FF 50 28 48 8B 4F 40 44 0F B7").c_str())); 
		global::vars::g_read_new_script_host_message = memory::read_instruction(tmp, 1, 5);
		hooking::detour(XOR("HMNSH").c_str(), tmp - 0x6D, &read_new_script_host_message, &read_new_script_host_message_t); 
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 30 48 8B 05 ? ? ? ? 4C 8B D1 41 83 7A ? ? 48 8B 48 40 7D 23 48 85 C9 0F 84 ? ? ? ? 48 8B 01 4C 8D 05").c_str())); 
		hooking::detour(XOR("UPSS").c_str(), tmp, &update_player_script_status, &update_player_script_status_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 5A FD 01 00").c_str())); 
		hooking::detour(XOR("HPE").c_str(), memory::read_instruction(tmp, 1, 5), &receive_presence_event, &receive_presence_event_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 33 DB 49 8B F9 41 8B").c_str())); 
		hooking::detour(XOR("FGES").c_str(), tmp, &format_engine_string, &format_engine_string_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 60 20 55 41 56 41 57 48 8D 68 C8 48 81 EC ? ? ? ? 0F 29 70 D8 49 8B F9 4C 8B FA 0F 28").c_str())); 
		hooking::detour(XOR("GGESLC").c_str(), tmp, &get_engine_string_line_count, &get_engine_string_line_count_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 20 41 8A E8 4C 8B F2 48 8B F1").c_str())); 
		hooking::detour(XOR("APPI").c_str(), tmp, &apply_player_physical_index, &apply_player_physical_index_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 30 0F B7 81 ? ? ? ? 48 8B B1").c_str())); 
		hooking::detour(XOR("WPGDN").c_str(), tmp, &write_player_game_state_data_node, &write_player_game_state_data_node_t);
		tmp = NULL;
	
		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B 02 8B F1 48 8D 4C 24 ? 48 89 44 24 ? 48 8B 42").c_str())); 
		global::vars::g_write_gs_item = tmp;
		hooking::detour(XOR("WGSITEM").c_str(), tmp, &write_gs_item, &write_gs_item_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("41 80 78 28 ? 0F 85 ? ? ? ? 49 8B 80").c_str())); 
		global::vars::GTA_STICKY_BOMBS = memory::read_instruction(tmp);
		memory::nop(tmp + 0x4, 99);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44").c_str())); 
		hooking::detour(XOR("GGXTL").c_str(), tmp, &get_gxt_label, &get_gxt_label_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 83 EC 28 E8 ? ? ? ? 48 85 C0 75 34 8B 0D ? ? ? ? 65 48 8B 04 25 ? ? ? ? BA ? ? ? ? 48 8B 04 C8 8B 0C 02 D1").c_str())); 
		hooking::detour(XOR("GGXTLFH").c_str(), tmp, &get_gxt_label_from_hash, &get_gxt_label_from_hash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 41 83 21 00 49 8B C0 49 8B D9 48 8B F1 48 8D 15 ? ? ? ? 41 B1 01 48 8B C8 45 33 C0 E8").c_str())); 
		hooking::detour(XOR("RMMR").c_str(), tmp, &read_matchmaking_response, &read_matchmaking_response_t);
		tmp = NULL;
	
		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 81 EC ? ? ? ? 4D 8B F1 45 8B").c_str())); 
		hooking::detour(XOR("WMMR").c_str(), tmp, &write_matchmaking_request, &write_matchmaking_request_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 BD ? ? ? ? ? ? ? ? 4D 8B C7 49 8D 04 2F 48 83 F8").c_str())); 
		hooking::detour(XOR("WESTR").c_str(), memory::read_instruction(tmp, 1, 5), &write_encrypted_string_to_request, &write_encrypted_string_to_request_t);
		tmp = NULL;
	
		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("45 33 C9 4C 8B D1 44 39 89 ? ? ? ? 76 60 83 FA 1C 7D 5B 44 8B C2 41 FF C8 74 36 41 83 F8 06 74 30").c_str())); 
		hooking::detour(XOR("HRP").c_str(), tmp, &has_ros_privilege, &has_ros_privilege_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC 20 48 8D 99 ? ? ? ? 33 ED 48 8B FA 48 8B F1 48 39 2B 74").c_str())); 
		hooking::detour(XOR("PGDN").c_str(), tmp, &write_player_gamer_data_node, &write_player_gamer_data_node_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 30 0F 29 74 24 ? 48 8B D9 E8 ? ? ? ? F3 0F 10 83 ? ? ? ? F3 0F 10 35").c_str())); 
		hooking::detour(XOR("AWM").c_str(), tmp, &apply_weather_meta, &apply_weather_meta_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 41 B8 ? ? ? ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84 C0 74 57 48 8D").c_str())); 
		hooking::detour(XOR("HMKP").c_str(), tmp, &read_kick_player_message, &read_kick_player_message_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 89 5C 24 ? 57 48 83 EC 20 48 8B FA 48 8B D9 E8 ? ? ? ? 48 8B 47 60 4C 8B C7 48 8D 8B ? ? ? ? 48 89 43 60").c_str())); 
		hooking::detour(XOR("RID0").c_str(), tmp, &rid0__crash, &rid0__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("40 53 48 83 EC 20 48 83 79 ? ? 48 8D 05 ? ? ? ? 48 8B D9 48 89 01 74 05 E8 ? ? ? ? 48").c_str())); 
		global::vars::g_game_script_handler_manager = memory::read_instruction(tmp + 0x1F);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("48 8B 01 4C 8B C1 48 3B D0 75 21 48 8B 40 18 48 89 01 33 C0 48 89 42 18 48 8B 09 48 85 C9 74 06 48 89 41 20 EB 4E 49").c_str())); 
		hooking::detour(XOR("C5").c_str(), tmp, &shit_scripts__crash, &shit_scripts__crash_t);
		tmp = NULL;

		tmp = reinterpret_cast<decltype(tmp)>(FindPattern(XOR("66 41 83 F9 ? 0F 83 ? ? ? ? 4C 8B DC 49 89 5B 08 49 89 6B 10 49 89 73 18 57 41 56 41 57 48 83 EC 50 8B").c_str())); 
		hooking::detour(XOR("HNE").c_str(), tmp, &receive_network_event, &receive_network_event_t);
		tmp = NULL;

		LOG_SUCCESS(XOR("Menu loaded enjoy your time!").c_str());

		return successful;
	}

	bool hooks() {
		bool successful = true;
		return successful;
	}
}