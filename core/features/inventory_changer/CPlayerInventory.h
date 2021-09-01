#pragma once

class CPlayerInventory {

public:
	void RemoveItem ( uint64_t ID );
	void RemoveItem ( CEconItem * item );
	void ClearInventory ( );
	CSharedObjectTypeCache * GetBaseTypeCache ( );
	uint32_t GetSteamID ( );
	c_utl_vector< C_EconItemView * > * GetInventoryItems ( );
	bool AddEconItem ( CEconItem * item, int a3, int a4, char a5 );
	C_EconItemView * getInventoryItemByItemID ( unsigned long long itemID );
	C_EconItemView * GetItemInLoadout ( int team, int slot );
	C_EconItemView * findOrCreateReferenceEconItem ( unsigned long long itemID );

};