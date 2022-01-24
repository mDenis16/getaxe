#include "netvars.hpp"
#include "../../interfaces/interfaces.hpp"


namespace netvar_manager {
	using netvar_key_value_map = std::unordered_map< uint32_t, stored_data >;
	using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;
	void initialize_props(netvar_table_map& table_map);

	recv_prop* get_prop(const uint32_t table,
		const uint32_t prop)
	{
		static netvar_table_map map = {};
		if (map.empty())
			initialize_props(map);

		if (map.count(table) == 0)
			return nullptr;

		netvar_key_value_map& table_map = map.at(table);
		if (table_map.count(prop) == 0)
			return nullptr;

		return table_map.at(prop).prop_ptr;
	}

   uintptr_t get_net_var(const uint32_t table,
		const uint32_t prop) {
		static netvar_table_map map = {};
		if (map.empty())
			initialize_props(map);

		if (map.count(table) == 0)
			return 0;

		netvar_key_value_map & table_map = map.at(table);
		if (table_map.count(prop) ==  0)
			return 0;

		return table_map.at(prop).class_relative_offset;
	}

	void add_props_for_table(netvar_table_map & table_map, const uint32_t table_name_hash, const std::string & table_name, recv_table * table, const bool dump_vars, std::map< std::string, std::map< uintptr_t, std::string > > & var_dump, const size_t child_offset = 0) {
		for (auto i = 0; i < table->props_count; ++i) {
			auto prop = &table->props[i];

			if (prop->data_table && prop->elements_count > 0) {
				if (std::string(prop->prop_name).substr(0, 1) == std::string("0"))
					continue;

				add_props_for_table(table_map, table_name_hash, table_name, prop->data_table, dump_vars, var_dump, prop->offset + child_offset);
			}

			auto name = std::string(prop->prop_name);

			if (name.substr(0, 1) != "m" /*&& name.substr( 0, 1 ) != "b"*/)
				continue;

			const auto name_hash = fnv::hash(prop->prop_name);
			const auto offset = uintptr_t(prop->offset) + child_offset;

			table_map[table_name_hash][name_hash] = {
				prop,
					offset
			};;

			/*{
				prop_ptr,
					total_offset
			};*/
			//if (dump_vars)
			//	var_dump[table_name][offset] = prop.prop_name;
		}
	}

	void initialize_props(netvar_table_map & table_map) {
		const auto dump_vars = true;  //true if netvar dump

		std::map< std::string, std::map< uintptr_t, std::string > > var_dump;
		for (auto client_class = interfaces::client->get_client_classes();
			client_class;
			client_class = client_class->next_ptr) {
			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			const auto table_name = table->table_name;
			const auto table_name_hash = fnv::hash(table_name);

			if (table == nullptr)
				continue;

			add_props_for_table(table_map, table_name_hash, table_name, table, dump_vars, var_dump);
		}
	}
}