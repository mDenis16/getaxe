#pragma once
class CSharedObjectTypeCache {
public:

	void AddObject ( void * obj );

	void RemoveObject ( void * obj );

	std::vector<CEconItem *> GetEconItems ( );


};