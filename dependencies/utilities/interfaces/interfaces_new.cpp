#include "interfaces_new.h"
#include "Windows.h"
#include <Psapi.h>
#include <iostream>
#include <vector>
#include "..\modules\modules.hpp"
#include "..\fnv.hpp"

namespace interfaces_manager
{

	void on_server_receive(std::map < std::size_t, std::uint32_t> data)
	{
		INTERFACES_ADDRESS = data;
	}
	

	/// <summary>
	/// Ring loop 
	/// </summary>
	class InterfaceReg
	{
	public:
		void* func;
		const char* m_pName;
		InterfaceReg* m_pNext;
	};

	//void on_server_recieve(std::map<std::size_t, InterfaceObject>&)
	/// <summary>
	/// This should be never in RELEASE BUILD!
	///	Used only for uploading INTERFACES TABLE to server!
	/// </summary>
	///
	void init_prop_table()
	{
												

		InterfaceReg* pCur;

		for (auto& _module : modules_manager::modules){
			auto module_handle = GetModuleHandleA(_module.c_str());

			void* createinterface = GetProcAddress(module_handle, "CreateInterface");
			InterfaceReg* InterfaceRegistry = **(InterfaceReg***)((uintptr_t)(((uintptr_t)createinterface + 0x8) + (*(unsigned short*)((uintptr_t)createinterface + 0x5) - *(unsigned short*)((uintptr_t)createinterface + 0x7))) + 0x6);

			for (pCur = InterfaceRegistry; pCur; pCur = pCur->m_pNext)
			{
				/**out = reinterpret_cast<void*>(
					static_cast<uintptr_t(*)()>(interfaceList->func)());*/
				auto func_address = 
					static_cast<uintptr_t(*)()>(pCur->func)() - (std::uint32_t)module_handle;

				InterfaceObject obj = { _module, std::string{pCur->m_pName}, func_address };

				INTERFACES_LIST_SERVER[fnv::hash(pCur->m_pName)] = obj;


				std::cout << _module << " => " << pCur->m_pName <<  " = " << func_address << std::endl;
			}
		}

	
		
		simulate_server_computing();

	}

	/// <summary>
	/// This is only for local builds. Otherwise this should be run on server! 
	/// </summary>
	void simulate_server_computing()
	{

		std::map<std::size_t, std::uint32_t> serverData;

		for(auto const& [key, val] : INTERFACES_LIST_SERVER)
		{
			serverData[key] = val.addr + modules_manager::MODULES_ADDRESS[fnv::hash(val.mod.c_str())];
		}

		on_server_receive(serverData);

	}

}