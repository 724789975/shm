#ifndef __SHARE_MEM_CONTAINER_SET_ALLOCTOR_H__
#define __SHARE_MEM_CONTAINER_SET_ALLOCTOR_H__

#include "shm_container_less.h"
#include "shm_pool.h"

namespace ShareMemoryContainer
{
	template<typename K>
	class SetAllocator
	{
	public:
		typedef K ValueType;
		typedef const K _ValueType;

		typedef K CompareType;

		static const K& get_compare_val(const ValueType& refPair) { return refPair; }
		static void set_compare_val(ValueType& l, const ValueType r) { l = r; }
	};




};

#endif //__SHARE_MEM_CONTAINER_SET_ALLOCTOR_H__
