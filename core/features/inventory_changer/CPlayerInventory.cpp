#include "../features.hpp"

void CPlayerInventory::RemoveItem ( uint64_t ID ) {
	static auto fnRemoveItem
		= reinterpret_cast< int ( __thiscall * )( void *, int64_t ) >(
			utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 56 57 FF 75 0C 8B F1" )
			);

	fnRemoveItem ( this, ID );
}

void CPlayerInventory::RemoveItem ( CEconItem * item ) {
	RemoveItem ( *item->GetItemID ( ) );
	GetBaseTypeCache ( )->RemoveObject ( item );
}

void CPlayerInventory::ClearInventory ( ) {
	auto BaseTypeCache = this->GetBaseTypeCache ( );
	auto items = BaseTypeCache->GetEconItems ( );
	for ( auto item : items ) {
		RemoveItem ( *item->GetItemID ( ) );
		BaseTypeCache->RemoveObject ( item );
	}
}

CSharedObjectTypeCache * CPlayerInventory::GetBaseTypeCache ( ) {
	static auto fnGCSDK_CGCClient_FindSOCache
		= reinterpret_cast< uintptr_t ( __thiscall * )( uintptr_t, uint64_t, uint64_t, bool ) >(
			utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 83 EC 1C 0F 10 45 08" )
			);

	static auto fnGCSDK_CSharedObjectCache_CreateBaseTypeCache
		= reinterpret_cast< CSharedObjectTypeCache * ( __thiscall * )( uintptr_t, int ) >(
			utilities::pattern_scan ( "client.dll", "55 8B EC 51 53 56 8B D9 8D 45 08" )
			);

	auto g_GCClientSystem = **reinterpret_cast<uintptr_t**>(utilities::pattern_scan("client.dll", "8B 0D ? ? ? ? 6A 00 83 EC 10") + 0x2);
	auto SOCahce = fnGCSDK_CGCClient_FindSOCache ( g_GCClientSystem + 0x70, *reinterpret_cast< uint64_t * >( this + 0x8 ), *reinterpret_cast< uint64_t * >( this + 0x10 ), 0 );

	return fnGCSDK_CSharedObjectCache_CreateBaseTypeCache ( SOCahce, 1 );
}

uint32_t CPlayerInventory::GetSteamID ( ) {
	return *reinterpret_cast< uint32_t * >( this + 0x8 );
}

c_utl_vector< C_EconItemView * > * CPlayerInventory::GetInventoryItems ( ) {
	return reinterpret_cast< c_utl_vector<C_EconItemView *>* >( this + 0x2C );
}


C_EconItemView * CPlayerInventory::getInventoryItemByItemID ( unsigned long long itemID ) {
	static auto oGetInventoryItemByItemID = ( C_EconItemView * ( __thiscall * )( void *, unsigned long long ) )( utilities::pattern_scan ( ( "client.dll" ),  ( "55 8B EC 8B 55 08 83 EC 10 8B C2" ) ) );
	auto econ = oGetInventoryItemByItemID ( this, itemID );
	if ( !econ || !*( BYTE * ) ( ( unsigned int ) econ + 0x204 ) )
		return nullptr;
	else
		return econ;
}

C_EconItemView * CPlayerInventory::GetItemInLoadout ( int team, int slot ) {
	typedef C_EconItemView * ( __thiscall * tOriginal )( void *, unsigned int, signed int );
	return utilities::get_virtual_function<tOriginal> ( this, 8 )( this, team, slot );
}
C_EconItemView * CPlayerInventory::findOrCreateReferenceEconItem ( unsigned long long itemID ) {
	static auto oFindOrCreateReferenceEconItem = ( C_EconItemView * ( __thiscall * )( void *, int64_t ) )( utilities::pattern_scan (  ( "client.dll" ),  ( "55 8B EC 51 8B 55 0C 53 56" ) ) );
	return oFindOrCreateReferenceEconItem ( this, itemID );
}
bool CPlayerInventory::AddEconItem ( CEconItem * item, int a3, int a4, char a5 ) {
	static auto fnAddEconItem
		= reinterpret_cast< C_EconItemView * ( __thiscall * )( void *, CEconItem *, int, int, char ) >(
			utilities::pattern_scan ( "client.dll", "55 8B EC 83 E4 F8 A1 ? ? ? ? 83 EC 14 53 56 57 8B F9 8B 08" )
			);

	GetBaseTypeCache ( )->AddObject ( item );


	auto ret = fnAddEconItem ( this, item, a3, a4, a5 );

	if ( ret ) {
		auto i = getInventoryItemByItemID ( *item->GetItemID ( ) );

		*reinterpret_cast< bool * >( ( uintptr_t ) i + 0xA1 ) = 1;
	}

	return ret;
}