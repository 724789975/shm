#ifndef __SHARE_MEM_CONTAINER_MAP_ALLOCTOR_H__
#define __SHARE_MEM_CONTAINER_MAP_ALLOCTOR_H__

#include "shm_container_less.h"
#include "shm_pool.h"

namespace ShareMemoryContainer
{
	template<typename K, typename V>
	class KVPair
	{
	public:
		K first;
		V second;
	};

	template<typename K, typename V>
	class _KVPair
	{
	public:
		const K first;
		V second;
	};

	template<typename K, typename V>
	class MapAllocator
	{
	public:
		typedef KVPair<K, V> ValueType;
		typedef _KVPair<K, V> _ValueType;

		typedef K CompareType;

		static const K& get_compare_val(const ValueType& refPair) { return refPair.first; }
		//static const K& get_compare_val(const _ValueType& refPair) { return refPair.first; }
		static void set_compare_val(ValueType& l, const ValueType r) { l.first = r.first; }
	};




};

#endif //__SHARE_MEM_CONTAINER_MAP_ALLOCTOR_H__
