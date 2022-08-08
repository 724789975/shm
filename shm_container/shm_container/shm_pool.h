#ifndef __SHARE_MEM_POOL_H__
#define __SHARE_MEM_POOL_H__

#include <assert.h>
#include <string.h>
#include <iostream>
#include <exception>

//地址记录的是偏移 所以不能使用0作为无效地址判断
//#define INVALID_INDEX 0xFFFFFFFF

namespace ShareMemoryContainer
{
	template<typename T, unsigned int dwMaxNum>
	class ShmPool
	{
	public:
		enum
		{
			CAPACITY = dwMaxNum,
			INVALID_INDEX = dwMaxNum,
		};

		void create();
		unsigned int end() const;

		unsigned int alloc();
		unsigned int alloc(std::ostream& os);
		bool release(unsigned int dwIndex);
		bool release(unsigned int dwIndex, std::ostream& os);

		T& get_value(unsigned int dwIndex);
		const T& get_value(unsigned int dwIndex) const;

		T* get_head();
		const T* get_head()const;
		T* get_tail();
		const T* get_tail()const;
	private:
		struct PoolElement 
		{
			unsigned int	m_dwValueIndex;
			unsigned int	m_dwNextIndex;
			unsigned int	m_dwRefCount;
		};

		T					m_oValue[dwMaxNum];
		PoolElement			m_oPool[dwMaxNum];
		unsigned int		m_dwFreeIndex;
	};

	template<typename T, unsigned int dwMaxNum>
	inline void ShmPool<T, dwMaxNum>::create()
	{
#ifdef _WIN32
		static_assert(std::is_trivially_copyable<T>::value, "not trivially copyable");
#endif // _WIN32
		static_assert(std::is_standard_layout<T>::value, "not standard layout");

		this->m_dwFreeIndex = 0;
		for (unsigned int i = 0; i < CAPACITY; ++i)
		{
			m_oPool[i].m_dwValueIndex = i;
			m_oPool[i].m_dwNextIndex = i + 1;
			m_oPool[i].m_dwRefCount = 0;
		}
		memset(m_oValue, 0, sizeof(T) * CAPACITY);
	}

	template<typename T, unsigned int dwMaxNum>
	inline unsigned int ShmPool<T, dwMaxNum>::end() const
	{
		return CAPACITY;
	}

	template<typename T, unsigned int dwMaxNum>
	inline unsigned int ShmPool<T, dwMaxNum>::alloc()
	{
		if (m_dwFreeIndex >= INVALID_INDEX)
		{
			throw(std::exception());
		}

		unsigned int dwIndex = m_dwFreeIndex;
		m_dwFreeIndex = m_oPool[m_dwFreeIndex].m_dwNextIndex;
		m_oPool[dwIndex].m_dwNextIndex = INVALID_INDEX;
		assert(0 == m_oPool[dwIndex].m_dwRefCount);
		if (m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwRefCount = 1;
		new (&m_oValue[dwIndex]) T();
		return dwIndex;
	}

	template<typename T, unsigned int dwMaxNum>
	inline unsigned int ShmPool<T, dwMaxNum>::alloc(std::ostream& os)
	{
		if (m_dwFreeIndex >= INVALID_INDEX)
		{
			os << "[" << __FILE__ << ", " << __LINE__ << ", " << __FUNCTION__ << "]\n";
			throw(std::exception());
		}

		unsigned int dwIndex = m_dwFreeIndex;
		m_dwFreeIndex = m_oPool[m_dwFreeIndex].m_dwNextIndex;
		m_oPool[dwIndex].m_dwNextIndex = INVALID_INDEX;
		assert(0 == m_oPool[dwIndex].m_dwRefCount);
		if (m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwRefCount = 1;
		new (&m_oValue[dwIndex]) T();
		return dwIndex;
	}

	template<typename T, unsigned int dwMaxNum>
	inline bool ShmPool<T, dwMaxNum>::release(unsigned int dwIndex)
	{
		assert(dwIndex < CAPACITY);
		if (dwIndex >= CAPACITY)
		{
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwNextIndex = m_dwFreeIndex;
		m_dwFreeIndex = m_oPool[dwIndex].m_dwValueIndex;
		assert(1 == m_oPool[dwIndex].m_dwRefCount);
		if (1 != m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwRefCount = 0;
		m_oValue[dwIndex].~T();
		return true;
	}

	template<typename T, unsigned int dwMaxNum>
	inline bool ShmPool<T, dwMaxNum>::release(unsigned int dwIndex, std::ostream& os)
	{
		assert(dwIndex < CAPACITY);
		if (dwIndex >= CAPACITY)
		{
			os << "[" << __FILE__ << ", " << __LINE__ << ", " << __FUNCTION__ << "]\n";
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwNextIndex = m_dwFreeIndex;
		m_dwFreeIndex = m_oPool[dwIndex].m_dwValueIndex;
		assert(1 == m_oPool[dwIndex].m_dwRefCount);
		if (1 != m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		m_oPool[dwIndex].m_dwRefCount = 0;
		m_oValue[dwIndex].~T();
		return true;
	}

	template<typename T, unsigned int dwMaxNum>
	inline T& ShmPool<T, dwMaxNum>::get_value(unsigned int dwIndex)
	{
		if (dwIndex >= CAPACITY)
		{
			throw(std::exception());
		}
		assert(1 == m_oPool[dwIndex].m_dwRefCount);
		if (1 != m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		return m_oValue[dwIndex];
	}

	template<typename T, unsigned int dwMaxNum>
	inline const T& ShmPool<T, dwMaxNum>::get_value(unsigned int dwIndex) const
	{
		if (dwIndex >= CAPACITY)
		{
			throw(std::exception());
		}
		assert(1 == m_oPool[dwIndex].m_dwRefCount);
		if (1 != m_oPool[dwIndex].m_dwRefCount)
		{
			throw(std::exception());
		}
		return m_oValue[dwIndex];
	}

	template<typename T, unsigned int dwMaxNum>
	inline T* ShmPool<T, dwMaxNum>::get_head()
	{
		return m_oValue;
	}

	template<typename T, unsigned int dwMaxNum>
	inline const T* ShmPool<T, dwMaxNum>::get_head() const
	{
		return m_oValue;
	}

	template<typename T, unsigned int dwMaxNum>
	inline T* ShmPool<T, dwMaxNum>::get_tail()
	{
		return m_oValue + dwMaxNum - 1;
	}

	template<typename T, unsigned int dwMaxNum>
	inline const T* ShmPool<T, dwMaxNum>::get_tail() const
	{
		return m_oValue + dwMaxNum - 1;
	}

	template<typename T>
	class ShmPool<T, 0>;
};
#endif // __SHARE_MEM_POOL_H__

