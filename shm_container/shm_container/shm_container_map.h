#ifndef __SHARE_MEM_CONTAINER_MAP_H__
#define __SHARE_MEM_CONTAINER_MAP_H__

#include "shm_container_less.h"
#include "shm_pool.h"
#include "shm_container_tree.h"
#include "shm_container_map_alloctor.h"

//K, V 仅支持POD类型
namespace ShareMemoryContainer
{

	template<typename _Kty, typename _Ty, unsigned int dwMaxNum, typename _Pr = Less<_Kty>, typename _Alloc = allocator<MapAllocator<_Kty, _Ty>, dwMaxNum> >
	class map : public Tree<_Kty, _Alloc, _Pr>
	{
	public:
		typedef Tree<_Kty, allocator<MapAllocator<_Kty, _Ty>, dwMaxNum>, _Pr> TreeType;
		typedef typename TreeType::iterator iterator;
		typedef typename TreeType::const_iterator const_iterator;
		typedef std::pair<iterator, bool> insert_ret;

		_Ty& operator[](const _Kty& k);
		private:
	};
	template<typename _Kty, typename _Ty, unsigned int dwMaxNum, typename _Pr, typename _Alloc>
	inline _Ty& map<_Kty, _Ty, dwMaxNum, _Pr, _Alloc>::operator[](const _Kty& k)
	{
		iterator it = find(k);
		if (TreeType::end() == it)
		{
			auto it_ret = insert({ k, _Ty() });
			if (it_ret)
			{
				it = it_ret.first;
			}
		}
		return it->second;
	}
}

#endif // __SHARE_MEM_CONTAINER_MAP_H__

