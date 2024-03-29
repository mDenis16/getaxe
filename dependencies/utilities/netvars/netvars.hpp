#pragma once
#include <windows.h>
#include <SDKDDKVer.h>
#include <string.h>
#include <vadefs.h>
#include <stdio.h>
#include <xstring>
#include <Psapi.h>
#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <winuser.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <assert.h>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <stdlib.h>
#include <process.h>
#include <Wincon.h>
#include <cstdint>
#include <chrono>
#include <Shlobj.h>
#include <future>
#include <Lmcons.h>
#include <tchar.h>
#include "../../../source-sdk/classes/recv_props.hpp"
#include "../../../source-sdk/classes/client_class.hpp"
#include "../../utilities/fnv.hpp"
template <typename T> T get_vfunc( void* v_table, const int i_index ) { return ( *static_cast< T** >( v_table ) ) [ i_index ]; }

#define  Assert( _exp )										((void)0)

struct stored_data
{
    recv_prop* prop_ptr;
    std::uint32_t class_relative_offset;
};

// table, prop, func_name, type
#define NETVAR2(type, func_name, table, prop ) \
	type& func_name( ) { \
      static uintptr_t offset = 0; \
      static constexpr  auto table_hash = fnv::hash( table ); \
      static constexpr  auto prop_hash = fnv::hash( prop ); \
      if(!offset) \
      { offset = netvar_manager::get_net_var(table_hash, prop_hash); } \
	  \
      return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define NETVAR(table, prop, func_name, type) \
	type& func_name( ) { \
      static uintptr_t offset = 0; \
      static constexpr auto table_hash = fnv::hash( table ); \
      static constexpr auto prop_hash = fnv::hash( prop ); \
      if(!offset) \
      { offset = netvar_manager::get_net_var(table_hash, prop_hash ); } \
	  \
      return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define NETVAR_LIST(table, prop, func_name, type) \
	type& func_name( ) { \
      static uintptr_t offset = 0; \
      static constexpr  auto table_hash = fnv::hash( table ); \
      static constexpr  auto prop_hash = fnv::hash( prop ); \
      if(!offset) \
      { offset = netvar_manager::get_net_var(table_hash, prop_hash ); } \
	  \
      return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define NETVAR_PTR(table, prop, func_name, type) \
	type* func_name( ) { \
      static uintptr_t offset = 0; \
      static constexpr auto table_hash = fnv::hash( table ); \
      static constexpr auto prop_hash = fnv::hash( prop ); \
      if(!offset) \
      { offset = netvar_manager::get_net_var(table_hash, prop_hash ); } \
	  \
      return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
    }

#define NETPROP(table, prop, func_name) \
static auto func_name() ->  RecvProp* \
{ \
	static uintptr_t offset = 0; \
    static constexpr auto table_hash = fnv::hash( table ); \
    static constexpr auto prop_hash = fnv::hash( prop ); \
	static recv_prop* prop_ptr; \
	if(!prop_ptr) prop_ptr = netvar_manager::get_prop(table_hash, prop_hash); \
	return prop_ptr; \
}


#define OFFSET(type, var, offset) \
	type& var() { \
		return *(type*)(uintptr_t(this) + offset); \
	} \

#define POFFSET(funcname, type, offset) type* funcname() \
{ \
	static uint16_t _offset = offset; \
	Assert(_offset); \
	return reinterpret_cast<type*>(uintptr_t(this) + _offset); \
}

#define MEMEBR_FUNC_ARGS(...) ( this, __VA_ARGS__ ); }
#define FUNCARGS(...) ( __VA_ARGS__ ); }
#define VFUNC( index, func, sig ) auto func { return get_vfunc< sig >( this, index ) MEMEBR_FUNC_ARGS
#define MFUNC(func, sig, offset) auto func { return reinterpret_cast< sig >( offset ) MEMEBR_FUNC_ARGS
#define FUNC(func, sig, offset) auto func { return reinterpret_cast< sig >( offset ) FUNCARGS
namespace netvar_manager {
	uintptr_t get_net_var(uint32_t table, uint32_t prop);
	recv_prop* get_prop(uint32_t table, uint32_t prop);
}

class recv_prop_hook
{
public:
	recv_prop_hook(recv_prop* prop, const recv_var_proxy_fn proxy_fn) :
		m_property(prop),
		m_original_proxy_fn(prop->proxy_fn)
	{
		set_proxy_function(proxy_fn);
	}

	~recv_prop_hook()
	{
		m_property->proxy_fn = m_original_proxy_fn;
	}

	auto get_original_function() const -> recv_var_proxy_fn
	{
		return m_original_proxy_fn;
	}

	auto set_proxy_function(const recv_var_proxy_fn proxy_fn) const -> void
	{
		m_property->proxy_fn = proxy_fn;
	}

private:
	recv_prop* m_property;
	recv_var_proxy_fn m_original_proxy_fn;
};