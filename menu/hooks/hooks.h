#pragma once
#include "stdafx.h"
#include "rage/types/network_types.h"
#include "rage/invoker/invoker.h"

namespace menu::hooks {
	extern uint64_t g_rcs_caller_functions[5];


	void pack_clone_create(uint64_t _this, rage::network::net_object* net_obj, rage::network::net_game_player* player, uint64_t sync_data);
	inline decltype(&pack_clone_create) pack_clone_create_t;

	bool pack_clone_sync(uint64_t _this, rage::network::net_object* net_obj, rage::network::net_game_player* net_game_player);
	inline decltype(&pack_clone_sync) pack_clone_sync_t;

	void* get_font_id(uint64_t font, int id);
	inline decltype(&get_font_id) get_font_id_t;

	uint64_t format_engine_string(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack);
	inline decltype(&format_engine_string) format_engine_string_t;

	int get_engine_string_line_count(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack);
	inline decltype(&get_engine_string_line_count) get_engine_string_line_count_t;

	FARPROC get_proc_address(HMODULE mod, LPCSTR name);
	inline decltype(&get_proc_address) get_proc_address_t;

	bool shoot_bullet(rage::types::weapon* weapon, uint64_t shooter_info, math::vector3_<float>* start_pos, math::vector3_<float>* end_pos);
	inline decltype(&shoot_bullet) shoot_bullet_t;

	bool shoot_projectile(rage::types::weapon* weapon, uint64_t shooter_info, math::vector3_<float>* start_pos, math::vector3_<float>* end_pos);
	inline decltype(&shoot_projectile) shoot_projectile_t;

	bool render_script_textures(uint64_t table, uint32_t edx, uint32_t r8d);
	inline decltype(&render_script_textures) render_script_textures_t;

	uint64_t get_next_grc_texture(uint64_t table);
	inline decltype(&get_next_grc_texture) get_next_grc_texture_t;

	void reset_script_render_params();
	inline decltype(&reset_script_render_params) reset_script_render_params_t;

	int get_next_marker_texture_index(rage::types::marker_table* table);
	inline decltype(&get_next_marker_texture_index) get_next_marker_texture_index_t;

	void translate_marker_to_screen(rage::types::marker_table* table);
	inline decltype(&translate_marker_to_screen) translate_marker_to_screen_t;

	void render_markers_to_screen(rage::types::marker_table* table);
	inline decltype(&render_markers_to_screen) render_markers_to_screen_t;

	int add_marker_to_table(rage::types::marker_table* table, uint64_t marker);
	inline decltype(&add_marker_to_table) add_marker_to_table_t;

	bool receive_clone_create(uint64_t _this, rage::network::net_game_player* sender, uint64_t r8, eNetObjectTypes type, uint16_t network_id, uint16_t flag, rage::bit_buffer* bit_buffer, uint32_t timestamp);
	inline decltype(&receive_clone_create) receive_clone_create_t;

	int receive_clone_sync(uint64_t _this, rage::network::net_game_player* sender, uint64_t r8, eNetObjectTypes type, uint16_t network_id, rage::bit_buffer* bit_buffer, uint16_t sequence_id, int timestamp);
	inline decltype(&receive_clone_sync) receive_clone_sync_t;

	bool receive_clone_sync_caller(uint64_t _this, uint64_t rdx);
	inline decltype(&receive_clone_sync_caller) receive_clone_sync_caller_t;

	bool receive_clone_remove(uint64_t _this, rage::network::net_game_player* sender, uint64_t r8, uint16_t network_id, uint64_t unk);
	inline decltype(&receive_clone_remove) receive_clone_remove_t;

	bool receive_broadcast(uint64_t _this, rage::network::net_game_player* sender, rage::bit_buffer* bit_buffer, uint32_t bit_count, uint16_t network_id);
	inline decltype(&receive_broadcast) receive_broadcast_t;

	void receive_network_event(uint64_t _this, rage::network::net_game_player* sender, rage::network::net_game_player* receiver, uint16_t event_id, int event_index, int event_bitset, uint32_t bit_buffer_size, rage::bit_buffer* bit_buffer);
	inline decltype(&receive_network_event) receive_network_event_t;

	bool read_chat_message(rage::network::global_msg_context* context, rage::bit_buffer* bit_buffer);
	inline decltype(&read_chat_message) read_chat_message_t;

	bool receive_text_message(rage::network::net_msg_text_message* identifier, rage::bit_buffer* bit_buffer);
	inline decltype(&receive_text_message) receive_text_message_t;

	bool receive_session_info(uint64_t rcx);
	inline decltype(&receive_session_info) receive_session_info_t;

	bool receive_presence_event(uint64_t rcx, uint64_t rdx, const char** payload, uint64_t r9);
	inline decltype(&receive_presence_event) receive_presence_event_t;

	uint64_t process_game_event(uint64_t _this);
	inline decltype(&process_game_event) process_game_event_t;

	bool read_blacklist_message(uint64_t rcx, uint64_t rdx);
	inline decltype(&read_blacklist_message) read_blacklist_message_t;

	bool read_new_script_host_message(uint64_t rcx, uint64_t rdx);
	inline decltype(&read_new_script_host_message) read_new_script_host_message_t;

	void update_player_script_status(rage::script::game_script_handler_network_component* component, uint64_t rdx, uint64_t r8);
	inline decltype(&update_player_script_status) update_player_script_status_t;

	void process_relay_usage(uint8_t cl, uint64_t rdx, rage::network::event_added_gamer* gamer, uint32_t r9d, uint64_t stack);
	inline decltype(&process_relay_usage) process_relay_usage_t;

	void apply_player_physical_index(uint64_t network_player_manager, rage::network::net_game_player* player, uint8_t slot);
	inline decltype(&apply_player_physical_index) apply_player_physical_index_t;

	bool write_player_game_state_data_node(uint64_t rcx, uint64_t rdx);
	inline decltype(&write_player_game_state_data_node) write_player_game_state_data_node_t;

	void write_ped_health_data_node(uint64_t rcx, uint64_t rdx);
	inline decltype(&write_ped_health_data_node) write_ped_health_data_node_t;

	void write_gs_item(int index, rage::network::gs_session* data, const char* type);
	inline decltype(&write_gs_item) write_gs_item_t;

	bool read_matchmaking_response(uint64_t rcx, uint64_t rdx, uint64_t r8, uint64_t r9);
	inline decltype(&read_matchmaking_response) read_matchmaking_response_t;

	bool write_matchmaking_request(uint64_t rcx, uint32_t rs_index, uint32_t available_slots, uint64_t filter_info, uint32_t max_results, uint64_t stack_20, uint64_t stack_18);
	inline decltype(&write_matchmaking_request) write_matchmaking_request_t;

	bool write_encrypted_string_to_request(uint64_t rcx, const char* name, char* value, bool encrypted, uint64_t stack_3C8, uint64_t stack_3C0);
	inline decltype(&write_encrypted_string_to_request) write_encrypted_string_to_request_t;

	void send_global_chat_message(uint64_t manager, int key, rage::network::global_msg_context* context, uint64_t unk1, uint64_t unk2);
	inline decltype(&send_global_chat_message) send_global_chat_message_t;

	const char* get_gxt_label(uint64_t table, const char* label);
	inline decltype(&get_gxt_label) get_gxt_label_t;

	const char* get_gxt_label_from_hash(uint64_t table, uint32_t label_hash);
	inline decltype(&get_gxt_label_from_hash) get_gxt_label_from_hash_t;

	int check_chat_profanity(uint64_t rcx, uint64_t rdx, uint64_t r8);
	inline decltype(&check_chat_profanity) check_chat_profanity_t;


	const char* does_cam_exist(int cam);
	inline decltype(&does_cam_exist) does_cam_exist_t;

	char request_control(__int64 netGameEvent, __int64 sender, __int64 receiver);
	inline decltype(&request_control) request_control_t;


	__int64 read_from_sync_tree_buffer(unsigned __int64** sync_tree_ptr, __int64 a2, unsigned int a3, __int64 a4, __int64 a5);
	inline decltype(&read_from_sync_tree_buffer) read_from_sync_tree_buffer_t;

	char __cdecl script_event_protection(__int64 netGameEvent, __int64 sender);
	inline decltype(&script_event_protection) script_event_protection_t;


	int check_string_profanity(uint64_t rcx, uint64_t rdx, uint64_t r8);
	inline decltype(&check_string_profanity) check_string_profanity_t;

	bool get_peer_address(uint64_t* peer);
	inline decltype(&get_peer_address) get_peer_address_t;

	bool has_ros_privilege(uint64_t ros_table, int privilege);
	inline decltype(&has_ros_privilege) has_ros_privilege_t;

	bool write_player_gamer_data_node(rage::network::player_gamer_data_node* node, rage::bit_buffer_sync_instance* buffer);
	inline decltype(&write_player_gamer_data_node) write_player_gamer_data_node_t;

	void apply_weather_meta(uint64_t meta);
	inline decltype(&apply_weather_meta) apply_weather_meta_t;

	bool read_kick_player_message(uint64_t rcx, uint64_t rdx);
	inline decltype(&read_kick_player_message) read_kick_player_message_t;

	bool write_net_msg_identifier(rage::bit_buffer* buffer, uint64_t rdx, int r8d);
	inline decltype(&write_net_msg_identifier) write_net_msg_identifier_t;

	bool write_ulonglong_to_bit_buffer(rage::bit_buffer* buffer, uint64_t rdx, int r8);
	inline decltype(&write_ulonglong_to_bit_buffer) write_ulonglong_to_bit_buffer_t;

	// crash start
	int apply_vehicle_data__crash(rage::network::net_object* object, uint64_t node_data);
	inline decltype(&apply_vehicle_data__crash) apply_vehicle_data__crash_t;

	eThreadState tick_script_thread__crash(rage::script::gta_thread* _this, uint32_t instruction_count);
	inline decltype(&tick_script_thread__crash) tick_script_thread__crash_t;

	uint64_t attach_trailer__crash(uint64_t trailer, uint16_t flag);
	inline decltype(&attach_trailer__crash) attach_trailer__crash_t;

	uint64_t attach_trailer_audio__crash(uint64_t audio_entity, uint64_t rdx);
	inline decltype(&attach_trailer_audio__crash) attach_trailer_audio__crash_t;

	// resource_streamed_script* load_script_resource__crash(resource_streamed_script_handler* handler, uint16_t resource_id);
	// inline decltype(&load_script_resource__crash) load_script_resource__crash_t;

	void destruct_script_handler__crash(uint64_t _this);
	inline decltype(&destruct_script_handler__crash) destruct_script_handler__crash_t;


	void crash_protect_model(int64_t a1, int64_t a2, int a3, char a4);
	inline decltype(&crash_protect_model) crash_protect_model_t;

	bool shit_scripts2__crash(uint64_t rcx, uint16_t dx, uint32_t r8d, bool r9b);
	inline decltype(&shit_scripts2__crash) shit_scripts2__crash_t;

	uint64_t shit_scripts__crash(uint64_t rcx, uint64_t rdx);
	inline decltype(&shit_scripts__crash) shit_scripts__crash_t;

	uint64_t rid0__crash(uint64_t rcx, uint64_t rdx);
	inline decltype(&rid0__crash) rid0__crash_t;

	uint64_t sub_140AD8424__crash(uint64_t rcx, int edx, uint32_t r8d); // previously sub_140AFB59C
	inline decltype(&sub_140AD8424__crash) sub_140AD8424__crash_t;
	// crash end

	bool bypass();
	bool globals();
	bool globals_in_game();
	bool hooks();
}

