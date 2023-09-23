#pragma once
#include "stdafx.h"
#include "global/vars.h"

namespace menu {



	class scr_global {
	private:
		std::uint64_t m_index;
		static void* get_scr_global(std::uint64_t index) { return ((std::uint64_t**)(global::vars::g_global_cache))[index >> 18 & 0x3F] + (index & 0x3FFFF); }
	public:
		constexpr explicit scr_global(std::uint64_t index) : m_index(index) {}
		constexpr scr_global at(std::uint64_t index) { return scr_global(m_index + index); }
		constexpr scr_global at(std::uint64_t index, std::uint64_t size) { return at(1 + (index * size)); }
		template <typename T>
		std::enable_if_t<std::is_pointer<T>::value, T> as() const { return (T)(get_scr_global(m_index)); }
	};

	class script_global {
	public:
		script_global(int index)
			: m_handle(&global::vars::g_global_cache[index >> 18 & 0x3F][index & 0x3FFFF])
		{}

		script_global(void* ptr)
			: m_handle(ptr)
		{}

		script_global at(int index) const {
			return script_global((void**)(m_handle) + index);
		}

		script_global at(int index, int size) const {
			return at(1 + (index * size));
		}

		template<typename T>
		T* get() {
			return (T*)(m_handle);
		}

		template<typename T>
		T& as() {
			return *get<T>();
		}

	private:
		void* m_handle;
	};
}