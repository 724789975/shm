#ifndef __SHARE_MEM_CONTAINER_SET_H__
#define __SHARE_MEM_CONTAINER_SET_H__

#include "shm_container_less.h"
#include "shm_pool.h"
#include "shm_container_tree.h"
#include "shm_container_set_alloctor.h"

//K, V 仅支持POD类型
namespace ShareMemoryContainer
{
	template<typename _Kty, unsigned int dwMaxNum, typename _Pr = Less<_Kty>, typename _Alloc = allocator<SetAllocator<_Kty>, dwMaxNum> >
	class set : public Tree<_Kty, _Alloc, _Pr>
	{
	public:
		typedef Tree<_Kty, allocator<SetAllocator<_Kty>, dwMaxNum>, _Pr> TreeType;
		typedef typename TreeType::iterator iterator;
		typedef typename TreeType::const_iterator const_iterator;
		typedef std::pair<iterator, bool> insert_ret;

		private:
	};
}

#endif // __SHARE_MEM_CONTAINER_SET_H__

