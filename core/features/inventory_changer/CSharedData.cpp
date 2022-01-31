#include "../../helpers/helpers.h"
#include "CSharedObject.h"
#include "CEconItem.h"

 
void CSharedObjectTypeCache::AddObject ( void * obj ) {
	typedef void ( __thiscall * tOriginal )( void *, void * );
	utilities::call_virtual<tOriginal> ( this, 1 )( this, obj );
}

void CSharedObjectTypeCache::RemoveObject ( void * obj ) {
	typedef void ( __thiscall * tOriginal )( void *, void * );
	utilities::call_virtual<tOriginal> ( this, 3 )( this, obj );
}

std::vector<CEconItem *> CSharedObjectTypeCache::GetEconItems ( ) {
	std::vector<CEconItem *> ret;

	auto size = *reinterpret_cast< size_t * >( this + 0x18 );

	auto data = *reinterpret_cast< uintptr_t ** >( this + 0x4 );

	for ( size_t i = 0; i < size; i++ )
		ret.push_back ( reinterpret_cast< CEconItem * >( data [ i ] ) );

	return ret;
}
