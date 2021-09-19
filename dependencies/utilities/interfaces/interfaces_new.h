#pragma once
#include <iostream>
#include <map>
#include <string>


namespace interfaces_manager
{
	/// <summary>
	/// Server part
	/// </summary>
	struct InterfaceObject
	{
		std::string mod; 
		std::string inter;
		std::size_t addr;
	};
	inline std::map<std::size_t, InterfaceObject> INTERFACES_LIST_SERVER;
	/// <summary>
	/// 
	/// </summary>


	/// <summary>
	/// Client part
	/// </summary>

	inline std::map<std::size_t, std::uint32_t> INTERFACES_ADDRESS;

	void on_server_receive(std::map < std::size_t, std::uint32_t> data);

	void simulate_server_computing();
	void init_prop_table();
}