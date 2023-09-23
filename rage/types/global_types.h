#pragma once
#include "stdafx.h"

namespace rage::global {
	constexpr int host_drop_kick = 1883751;//1.63
	constexpr int apartment_teleport = -1390976345;// 1.63
	constexpr int island_teleport = 1361475530;	// 1.63
	constexpr int block_passive = 65268844;// 1.63
	constexpr int give_off_the_radar = -1973627888;// 1.63
	constexpr int give_cops_turn_blind_eye = -2095232746;// 1.63
	constexpr int remove_wanted_level = 1449852136;;// 1.63
	constexpr int invite = 1111927333;// 1.63
	constexpr int network_bail = 1674887089;// 1.63
	constexpr int kick_from_ceo = -1425016400;// 1.63
	constexpr int ban_from_ceo = 1240068495;// 1.63
	constexpr int set_vehicle_can_be_locked_on = -1409079442;// 1.63
	constexpr int set_weather = 315658550;		//gone from the game rip					// 1
	constexpr int message_handler = -1529596656;// 1.63
	constexpr int destroyed_personal_vehicle = -2126830022;// 1.63
	constexpr int kick_from_vehicle = -714268990;// 1.63
	constexpr int force_into_mission = -283041276;// 1.63
	constexpr int transition_session = 1575015;// 1.63
	constexpr int transition_session_flag = 1574589;// 1.63

	// NEW
	constexpr int _155_give_off_the_radar_global = 1894573;// 1.64
	constexpr int _155_give_off_the_radar_global_size = 608;// 1.64
	constexpr int _155_give_off_the_radar_global_index = 510;// 1.64

	constexpr int _155_sc_crash_1 = -555356783;// 1.63
	constexpr int _155_sc_crash_2 = 526822748;;// 1.63
	constexpr int _155_sc_crash_3 = 1926582096;

	constexpr int _155_force_into_moc_mission = -1147284669;
	constexpr int _155_screen_flicker = 1376436345;
	constexpr int _155_transaction_error = -2041535807;
	constexpr int _155_spawn_dlc_in_sp = 4540726;

	constexpr int _155_disable_phone_type = 19798;
	constexpr int _155_disable_phone_bool_1 = 19984;
	constexpr int _155_disable_phone_phone_pos_set = 19726;
	constexpr int _155_disable_phone_phone_pos_get = 19735;
	constexpr int _155_disable_phone_bitset_1 = 7669;
	constexpr int _155_disable_phone_bitset_2 = 7668;
	constexpr int _155_disable_phone_unk_1 = 21125;

	constexpr int _155_outfit_editor_base = 2097152;
	constexpr int _155_outfit_editor_base_offset = 675;
	constexpr int _155_outfit_editor_offset_1 = 1879;		// from 1878
	constexpr int _155_outfit_editor_offset_2 = 2090;		// from 2089
	constexpr int _155_outfit_editor_offset_3 = 1331;		// from 1330
	constexpr int _155_outfit_editor_offset_4 = 1605;		// from 1604
	constexpr int _155_outfit_editor_offset_5 = 2301;		// from 2300
	constexpr int _155_outfit_editor_offset_6 = 2455;		// from 2454

	constexpr int _155_outfit_editor_base2 = 2533708;		// from 2530205
	constexpr int _155_outfit_editor_base2_offset = 9241;	// from 9146

	constexpr int _155_convert_rp_to_level = 295824;//1.63;		// from 293361
	constexpr int _155_get_player_blip_handle = 2678393;//1.63;		//

constexpr int _155_player_stats_struct = 1853910;		// 1.64
constexpr int _155_player_stats_size = 862;				// 1.64
constexpr int _155_player_stats_start = 205;			// 1.64


constexpr int _155_garage_base = 1586468;				// 1.64
constexpr int _155_garage_size = 142;					// 1.64
constexpr int _155_tunables = 262145;                   // 1.64
constexpr int _155_property_int_offset1 = 14734;        // 1.64
constexpr int _155_property_int_offset2 = 14735;        // 1.64
constexpr int _155_property_int_offset3 = 14733;        // 1.64
constexpr int _155_property_int_offset4 = 14736;        // 1.64
constexpr int _155_property_int_offset5 = 14735;        // 1.64
constexpr int _155_property_int_offset6 = 14737;        // 1.64

constexpr int _155_property_int_offset7 = 19141;        // 1.64
constexpr int _155_property_int_offset8 = 19143;        // 1.64
constexpr int _155_property_int_offset9 = 19147;        // 1.64
constexpr int _155_property_int_offset10 = 19144;       // 1.64
constexpr int _155_property_int_offset11 = 19151;       // 1.64
constexpr int _155_property_int_offset12 = 19149;       // 1.64
constexpr int _155_property_int_offset13 = 19154;       // 1.64
constexpr int _155_property_int_offset14 = 21100;       // 1.64
constexpr int _155_property_int_offset15 = 21101;       // 1.64

constexpr int _155_property_int_offset_16 = 267;		//1.64
constexpr int _155_property_int_offset_16_1 = 372;		//1.64
constexpr int _155_property_int_offset_16_2 = 278;		//1.61
constexpr int _155_property_int_offset_16_3 = 273;	    //1.61
constexpr int _155_property_int_offset_17 = 10629;		//1.61

constexpr int _155_property_base2 = 2689235;			//1.61
constexpr int _155_property_base2_size = 453;			//1.61
constexpr int _155_property_base2_flag = 197;           //1.61
constexpr int _155_property_base2_offset1 = 318;		//1.61
constexpr int _155_property_base3 = 1853184;			//1.61
constexpr int _155_property_base4 = 1312193;			//1.61
constexpr int _155_property_base4_size = 1951;          //1.61
constexpr int _155_property_base4_flag = 33;            //1.61 MIGHT NEED CHECKING
constexpr int _155_property_base5 = 1940666;			//1.61

constexpr int _155_customs_base = 2815059;				//1.61
	constexpr int _155_customs_base2 = 102139;				// from 101708
	constexpr int _155_time_base = 2703735;		//1.63			// from 2440277
}