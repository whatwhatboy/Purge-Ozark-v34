#pragma once
#include "stdafx.h"
#include "util/math.h"
#include "global/vars.h"

namespace rage::invoker {
	namespace crossmap {
		uint64_t search_crossmap(uint64_t hash);
		uint64_t search_crossmap_mapped(uint64_t hash);
		void foreach(void(*func)(uint64_t));
	}
}

#define NATIVE_CXT_MAX_ARGS 32
#define NATIVE_CXT_ARG_SIZE 8
typedef int Ped;
typedef int Entity;
typedef int Hash;
class native_call_cxt {
protected:
	void* m_ret_data;
	std::uint32_t m_arg_count;
	void* m_arg_data;
	std::uint32_t m_dat_count;
	std::uint32_t m_vec_data[48];
public:
	template <typename t>
	t get_arg(std::uint64_t index) {
		return *(t*)(&m_arg_data + (index * NATIVE_CXT_ARG_SIZE));
	}
	template <typename t>
	void set_arg(std::uint64_t index, t value) {
		return *(t*)(&m_arg_data + (index * NATIVE_CXT_ARG_SIZE)) = value;
	}
	void print_args() {
		std::cout << __FUNCTION__":";
		for (std::uint32_t i = 0; i < m_arg_count; i++)
			std::cout << " " << std::hex << ((std::uint64_t*)(m_arg_data))[i];
		std::cout << std::endl;
	}
};

class native_cxt
	: native_call_cxt {
private:
	std::uint8_t m_temp_stack[NATIVE_CXT_MAX_ARGS * NATIVE_CXT_ARG_SIZE];
public:
	native_cxt() {
		ZeroMemory(m_temp_stack, sizeof m_temp_stack);
		m_ret_data = &m_temp_stack;
		m_arg_data = &m_temp_stack;
	}
	template <typename t>
	void push_arg(t arg) {
		if (sizeof t > NATIVE_CXT_ARG_SIZE)
			throw "invalid argument size";
		if (sizeof t < NATIVE_CXT_ARG_SIZE)
			ZeroMemory(&m_temp_stack[m_arg_count * NATIVE_CXT_ARG_SIZE], NATIVE_CXT_ARG_SIZE);
		*(t*)(&m_temp_stack[m_arg_count * NATIVE_CXT_ARG_SIZE]) = arg;
		m_arg_count++;
	}
	template <typename t>
	t get_result() {
		spoof_call(reinterpret_cast<std::add_pointer_t<void>>(global::vars::GTA_JMP_RBX), ((void(*)(native_cxt*))global::vars::GTA_SET_VECTOR_RESULTS), this);
		return *(t*)(m_temp_stack);
	}
};

template<typename r, class...a>
static r invoke(std::uint64_t hash, a const&...args) {
	native_cxt cxt;
	((cxt.push_arg(args)), ...);

	std::uint64_t handler = spoof_call(reinterpret_cast<std::add_pointer_t<void>>(global::vars::GTA_JMP_RBX), ((std::uint64_t(*)(std::uint64_t, std::uint64_t))global::vars::GTA_GET_NATIVE_HANDLER), global::vars::GTA_NATIVE_REGISTRATION, rage::invoker::crossmap::search_crossmap(hash));
	if (handler)
		spoof_call(reinterpret_cast<std::add_pointer_t<void>>(global::vars::GTA_JMP_RBX), (native_cxt * (*)(native_cxt*))(handler), &cxt);
	return cxt.get_result<r>();
}
