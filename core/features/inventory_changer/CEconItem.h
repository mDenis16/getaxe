#pragma once
enum ItemQuality {
	ITEM_QUALITY_DEFAULT,
	ITEM_QUALITY_GENUINE,
	ITEM_QUALITY_VINTAGE,
	ITEM_QUALITY_UNUSUAL,
	ITEM_QUALITY_SKIN,
	ITEM_QUALITY_COMMUNITY,
	ITEM_QUALITY_DEVELOPER,
	ITEM_QUALITY_SELFMADE,
	ITEM_QUALITY_CUSTOMIZED,
	ITEM_QUALITY_STRANGE,
	ITEM_QUALITY_COMPLETED,
	ITEM_QUALITY_UNK2,
	ITEM_QUALITY_TOURNAMENT
};

enum ItemRarity {
	ITEM_RARITY_DEFAULT,
	ITEM_RARITY_COMMON,
	ITEM_RARITY_UNCOMMON,
	ITEM_RARITY_RARE,
	ITEM_RARITY_MYTHICAL,
	ITEM_RARITY_LEGENDARY,
	ITEM_RARITY_ANCIENT,
	ITEM_RARITY_IMMORTAL
};

class CEconItem {
	unsigned short * GetEconItemData ( );
	void UpdateEquippedState ( unsigned int state );
public:
	uint32_t * GetInventory ( );
	uint32_t * GetAccountID ( );
	uint16_t * GetDefIndex ( );
	uint64_t * GetItemID ( );
	uint64_t * GetOriginalID ( );
	unsigned char * GetFlags ( );
	void SetQuality ( ItemQuality quality );
	void SetRarity ( ItemRarity rarity );
	void SetOrigin ( int origin );
	void SetLevel ( int level );
	void SetInUse ( bool in_use );
	void SetCustomName ( const char * name );
	void SetCustomDesc ( const char * name );
	void SetPaintSeed ( float seed );
	void SetPaintKit ( float kit );
	void SetPaintWear ( float wear );
	void SetStatTrak ( int val );
	void AddSticker ( int index, int kit, float wear, float scale, float rotation );
	uintptr_t get_item_schema ( ) {
		static auto fnGetItemSchema = reinterpret_cast< uintptr_t ( __stdcall * )( ) >( utilities::pattern_scan ( "client.dll", "A1 ? ? ? ? 85 C0 75 53" ) );
		return fnGetItemSchema ( );
	}

	template<typename TYPE>
	void SetAttributeValue ( int index, TYPE val ) {
	
		auto v15 = reinterpret_cast< DWORD * >( get_item_schema ( ) );
		auto v16 = *reinterpret_cast< DWORD * >( v15 [ 72 ] + 4 * index );

		static auto fn_set_dynamic_attribute_value = reinterpret_cast< int ( __thiscall * )( CEconItem *, DWORD, void * ) >( utilities::pattern_scan(  "client.dll" , "55 8B EC 83 E4 F8 83 EC 3C 53 8B 5D 08 56 57 6A 00" ) );
		fn_set_dynamic_attribute_value ( this, v16, &val );
	}
};