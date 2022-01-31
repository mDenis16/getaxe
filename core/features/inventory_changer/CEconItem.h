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
union AttributeDataUnion {
	float asFloat;
	std::uint32_t asUint32;
	char* asBlobPointer;
};

struct StaticAttrib {
	std::uint16_t defIndex;
	AttributeDataUnion value;
	bool forceGCToGenerate;
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
	int  GetPainKit();
	void SetPaintKit ( float kit );
	void SetPaintWear ( float wear );
	void SetStatTrak ( int val );
	void AddSticker ( int index, int kit, float wear, float scale, float rotation );
	uintptr_t get_item_schema ( ) {
		static auto fnGetItemSchema = reinterpret_cast< uintptr_t ( __stdcall * )( ) >( utilities::pattern_scan ( "client.dll", "A1 ? ? ? ? 85 C0 75 53" ) );
		return fnGetItemSchema ( );
	}

	const c_utl_vector<StaticAttrib>& getStaticAttributes() noexcept
	{
		return *reinterpret_cast<const c_utl_vector<StaticAttrib>*>(std::uintptr_t(this) + 0x30);
	}
	std::uint32_t getAttributeValue(std::uint16_t attributeDefinitionIndex) noexcept
	{
		const auto& staticAttributes = getStaticAttributes();
		for (int i = 0; i < staticAttributes.count(); ++i) {
			if (staticAttributes[i].defIndex == attributeDefinitionIndex)
				return staticAttributes[i].value.asUint32;
		}
		return 0;
	}
	template<typename TYPE>
	void SetAttributeValue(int index, TYPE val);

};