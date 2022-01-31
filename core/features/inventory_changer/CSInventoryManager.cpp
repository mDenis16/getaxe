#include "../../helpers/helpers.h"
#include "CSharedObject.h"
#include "CPlayerInventory.h"
#include "CSInventoryManager.h"


CPlayerInventory * CSInventoryManager::GetLocalPlayerInventory ( ) {
	static auto local_inventory_offset = *reinterpret_cast< uintptr_t * >( utilities::pattern_scan ( "client.dll", "8B 8B ? ? ? ? E8 ? ? ? ? 89 44 24 18" ) + 0x2 );
	return *reinterpret_cast< CPlayerInventory ** >( this + local_inventory_offset );
}