#ifndef __SHARE_MEM_CONTAINER_TREE_H__
#define __SHARE_MEM_CONTAINER_TREE_H__

#include "shm_container_map_alloctor.h"
#include "shm_container_less.h"
#include "shm_pool.h"
#include "shm_container_tree_iterator.h"

#include <iostream>
#include <exception>
#include <type_traits>
#include <algorithm>
#include <deque>

//K, V ��֧��POD����
namespace ShareMemoryContainer
{
	struct TreeInfo
	{
		unsigned int								m_dwNodeIndex;		//����¼���ݵ�ƫ��
		unsigned int								m_dwMyIndex;		//��������ƫ��
		unsigned int								m_dwHeight;			//����
		unsigned int								m_dwParentIndex;
		unsigned int								m_dwLeftIndex;
		unsigned int								m_dwRightIndex;
	};

	template<typename Ty, unsigned int dwMaxNum>
	class allocator
	{
	public:
		enum { CAPACITY = dwMaxNum, };
		typedef typename Ty::CompareType CompareType;
		typedef typename Ty::ValueType ValueType;
		typedef typename Ty::_ValueType _ValueType;

		typedef ShmPool<TreeInfo, CAPACITY> TreePool;
		typedef ShmPool<ValueType, CAPACITY> NodePool;
	
		//static const CompareType& get_compare_val(const _ValueType& value_type);
		static const CompareType& get_compare_val(const ValueType& value_type);
		static void set_compare_val(ValueType& l, const ValueType& r);
	};

	//template<typename Ty, unsigned int dwMaxNum>
	//inline const typename allocator<Ty, dwMaxNum>::CompareType&
	//	allocator<Ty, dwMaxNum>::get_compare_val(
	//	const typename allocator<Ty, dwMaxNum>::_ValueType& value_type)
	//{
	//	return Ty::get_compare_val(value_type);
	//}

	template<typename Ty, unsigned int dwMaxNum>
	inline const typename allocator<Ty, dwMaxNum>::CompareType&
		allocator<Ty, dwMaxNum>::get_compare_val(
			const typename allocator<Ty, dwMaxNum>::ValueType& value_type)
	{
		return Ty::get_compare_val(value_type);
	}

	template<typename Ty, unsigned int dwMaxNum>
	inline void allocator<Ty, dwMaxNum>::set_compare_val(ValueType& l, const ValueType& r)
	{
		return Ty::set_compare_val(l ,r);
	}

	template<typename _K, typename Alloc = allocator<MapAllocator<_K, int>, 0>, typename KeyLess = Less<_K> >
	class Tree
	{
	public:
		typedef typename Alloc::ValueType Node;
		typedef typename Alloc::_ValueType IteratorNode;

		typedef TreeIterator<Tree<_K, Alloc, KeyLess>, TreeInfo> iterator;
		typedef TreeIterator<const Tree<_K, Alloc, KeyLess>, const TreeInfo> const_iterator;
		friend iterator;
		friend const_iterator;
		typedef std::pair<iterator, bool> insert_ret;

		typedef TreeReverseIterator<Tree<_K, Alloc, KeyLess>, TreeInfo> reverse_iterator;
		typedef TreeReverseIterator<const Tree<_K, Alloc, KeyLess>, const TreeInfo> const_reverse_iterator;
		friend reverse_iterator;
		friend const_reverse_iterator;

		void create();

		insert_ret insert(const typename Alloc::ValueType& refValue);
		unsigned int erase(const _K& k);
		iterator erase(iterator it);
		iterator erase(const_iterator it);

		iterator find(const _K& k);
		iterator begin();
		const_iterator cbegin()const;
		iterator end();
		const_iterator cend()const;
		reverse_iterator rbegin();
		const_reverse_iterator crbegin()const;
		reverse_iterator rend();
		const_reverse_iterator crend()const;
		iterator lower_bound(const _K& k);
		iterator upper_bound(const _K& k);
		void clear();
		unsigned int size()const;

		void debug_info(std::ostream& os);

		void _CheckNode();
	private:
		/// <summary>
		/// ��ȡ�ڵ�
		/// </summary>
		/// <param name="dwIndex"></param>
		/// <returns></returns>
		IteratorNode & _GetIteratorNode(TreeInfo* pTreeInfo);
		const IteratorNode& _GetIteratorNode(TreeInfo* pTreeInfo)const;

		/// <summary>
		/// ����ڵ�
		/// </summary>
		/// <returns></returns>
		unsigned int _BuyNode();
		unsigned int _BuyNode(std::ostream os);
		/// <summary>
		/// �ͷŽڵ�
		/// </summary>
		/// <param name="dwTreeNodeIndex"></param>
		/// <returns></returns>
		bool _FreeNode(unsigned int dwNodeIndex);
		bool _FreeNode(unsigned int dwNodeIndex, std::ostream& os);
		/// <summary>
		/// ��С�Ƚ�
		/// </summary>
		/// <param name="dwLeftNodeIndex"></param>
		/// <param name="dwRoightNodeIndex"></param>
		/// <returns></returns>
		bool _Less(unsigned int dwLeftNodeIndex, unsigned int dwRoightNodeIndex)const;
		bool _Less4K(const _K& l, const _K& r)const;
		/// <summary>
		/// k�Ƿ���ͬ
		/// </summary>
		/// <param name="l"></param>
		/// <param name="r"></param>
		/// <returns></returns>
		bool _Equal4K(const _K& l, const _K& r)const;
		/// <summary>
		/// �Ƴ��ڵ�
		/// </summary>
		/// <param name="refTreeNode"></param>
		void _RemoveNode(TreeInfo& refTreeNode);
		/// <summary>
		/// ��ȡ������
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		iterator _GetIterator(TreeInfo& refTreeNode);
		const_iterator _GetIterator(TreeInfo& refTreeNode)const;
		/// <summary>
		/// ��ȡ������
		/// </summary>
		/// <param name="dwTreeNodeIndex"></param>
		/// <returns></returns>
		iterator _GetIterator(unsigned int dwTreeNodeIndex);
		const_iterator _GetIterator(unsigned int dwTreeNodeIndex)const;
		/// <summary>
		/// ��ȡ����
		/// </summary>
		/// <param name="dwIndex"></param>
		/// <returns></returns>
		int _GetHeight(unsigned int dwIndex);
		/// <summary>
		/// ���½ڵ�ĸ߶�
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		void _UpdateHeight(TreeInfo& refTreeNode);
		/// <summary>
		/// ��ýڵ�������ֵ
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		const TreeInfo& _FindMax(const TreeInfo& refTreeNode) const;
		TreeInfo& _FindMax(TreeInfo& refTreeNode);
		/// <summary>
		/// ��ýڵ�����С��ֵ
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		const TreeInfo& _FindMin(const TreeInfo& refTreeNode) const;
		TreeInfo& _FindMin(TreeInfo& refTreeNode);
		/// <summary>
		/// ����
		/// </summary>
		/// <param name="dwTreeNodeIndex"></param>
		/// <param name="k"></param>
		/// <returns></returns>
		iterator _Find(unsigned int dwTreeNodeIndex, const _K& k);
		/// <summary>
		/// _LowerBound
		/// </summary>
		/// <param name="refIterator">����ֵ</param>
		/// <param name="dwTreeNodeIndex"></param>
		/// <param name="k"></param>
		/// <returns></returns>
		iterator _LowerBound(iterator& refIterator, unsigned int dwTreeNodeIndex, const _K& k);
		/// <summary>
		/// _uPPERbOund
		/// </summary>
		/// <param name="refIterator">����ֵ</param>
		/// <param name="dwTreeNodeIndex"></param>
		/// <param name="k"></param>
		/// <returns></returns>
		iterator _UpperBound(iterator& refIterator, unsigned int dwTreeNodeIndex, const _K& k);
		/// <summary>
		/// r��l������ڵ�
		/// </summary>
		/// <param name="l"></param>
		/// <param name="r"></param>
		void _SwapNode4Erase(TreeInfo& l, TreeInfo& r);
		/// <summary>
		/// ��ȡ��һ��
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		iterator _GetNext(const iterator& ite);
		const_iterator _GetNext(const const_iterator& cite)const;
		/// <summary>
		/// ���ϲ�����һ��(��ǰ�ڵ�Ϊ���ӽڵ�ʱ�ĸ��ڵ�)
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		iterator _GetNext4Parent(TreeInfo& refTreeNode);
		const_iterator _GetNext4Parent(TreeInfo& refTreeNode)const;
		/// <summary>
		///	��ȡ��һ��
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		iterator _GetPrevious(const TreeInfo& refTreeNode);
		/// <summary>
		/// ���ϲ�����һ��(��ǰ�ڵ�Ϊ���ӽڵ�ʱ�ĸ��ڵ�)
		/// </summary>
		/// <param name="refTreeNode"></param>
		/// <returns></returns>
		iterator _GetPrevious4Parent(const TreeInfo& refTreeNode);
		/// <summary>
		/// ��ȡ�ýڵ��������ӽڵ������
		/// </summary>
		/// <param name="dwTreeNodeIndex"></param>
		/// <returns></returns>
		unsigned int _GetNodeNum(unsigned int dwTreeNodeIndex) const;
		/// <summary>
		/// �������
		/// </summary>
		/// <param name="dwCompareIndex">��ѯ�ȽϵĽڵ�</param>
		/// <param name="refValue"></param>
		/// <returns></returns>
		insert_ret _Insert(unsigned int dwCompareIndex, const typename Alloc::ValueType& refValue);
		/// <summary>
		/// �������
		/// </summary>
		/// <param name="it_ret"></param>
		/// <param name="dwSearchIndex">��ѯ�ȽϵĽڵ�</param>
		/// <param name="dwIndex">������ڵ�</param>
		/// <returns></returns>
		void _Insert(insert_ret& it_ret, unsigned int& dwCompareIndex, unsigned int dwCompareParentIndex, unsigned int dwIndex);
		/// <summary>
		/// ɾ���ڵ�
		/// </summary>
		/// <param name="dwCompareIndex">������ڵ㿪ʼ����</param>
		/// <param name="k"></param>
		/// <returns></returns>
		iterator _Erase(unsigned int dwCompareIndex, const _K k);
		/// <summary>
		/// ɾ���ýڵ������нڵ�
		/// </summary>
		/// <param name="refTreeNode"></param>
		void _Clear(TreeInfo& refTreeNode);
		/// <summary>
		/// ����ת ��
		/// </summary>
		/// <param name="refTreeNode">Ҫ��ת�Ľڵ�</param>
		/// <returns>��ת��������λ�õĽڵ�</returns>
		unsigned int _LL(TreeInfo& refTreeNode);
		/// <summary>
		/// ����ת ��
		/// </summary>
		/// <param name="refTreeNode">Ҫ��ת�Ľڵ�</param>
		/// <returns></returns>
		unsigned int _RR(TreeInfo& refTreeNode);
		/// <summary>
		/// ˫��ת ����
		/// </summary>
		/// <param name="refTreeNode">Ҫ��ת�Ľڵ�</param>
		/// <returns></returns>
		unsigned int _LR(TreeInfo& refTreeNode);
		/// <summary>
		/// ˫��ת ����
		/// </summary>
		/// <param name="refTreeNode">Ҫ��ת�Ľڵ�</param>
		/// <returns></returns>
		unsigned int _RL(TreeInfo& refTreeNode);

		typename Alloc::TreePool m_oTreePool;
		typename Alloc::NodePool m_oNodePoolNew;

		unsigned int				m_dwRootIndex;
		unsigned int				m_dwSize;
	};

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::create()
	{
		m_dwSize = 0;
		m_oTreePool.create();
		m_oNodePoolNew.create();
		m_dwRootIndex = Alloc::CAPACITY;

		//for (int i = 0; i < Alloc::CAPACITY; ++i)
		//{
		//	m_oTreePool.get_value(i).m_dwMyIndex = Alloc::CAPACITY;
		//	m_oTreePool.get_value(i).m_dwNodeIndex = Alloc::CAPACITY;
		//	m_oTreePool.get_value(i).m_dwParentIndex = Alloc::CAPACITY;
		//	m_oTreePool.get_value(i).m_dwLeftIndex = Alloc::CAPACITY;
		//	m_oTreePool.get_value(i).m_dwRightIndex = Alloc::CAPACITY;
		//}
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::insert_ret Tree<_K, Alloc, KeyLess>::insert(const typename Alloc::ValueType& refValue)
	{
		iterator it = find(Alloc::get_compare_val(refValue));
		if (it != end())
		{
			typename Tree<_K, Alloc, KeyLess>::insert_ret it_ret;
			it_ret.first = it;
			it_ret.second = false;
			return  it_ret;
		}
		return _Insert(m_dwRootIndex, refValue);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::erase(const _K& k)
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return 0;
		}
		if (end() != _Erase(m_dwRootIndex, k))
		{
			return 1;
		}
		return 0;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::erase(iterator it)
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return end();
		}
		return _Erase(m_dwRootIndex, Alloc::get_compare_val(m_oNodePoolNew.get_value(it.m_pTreeNode->m_dwNodeIndex)));
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::erase(const_iterator it)
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return end();
		}
		return _Erase(m_dwRootIndex, Alloc::get_compare_val(m_oNodePoolNew.get_value(it.m_pTreeNode->m_dwNodeIndex)));
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::find(const _K& k)
	{
		return _Find(m_dwRootIndex, k);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::begin()
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return end();
		}

		TreeInfo& refTreeInfo = m_oTreePool.get_value(m_dwRootIndex);
		TreeInfo& refBeginTreeNode = _FindMin(refTreeInfo);
		iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refBeginTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::cbegin() const
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return cend();
		}
		const TreeInfo& refTreeInfo = m_oTreePool.get_value(m_dwRootIndex);
		const TreeInfo& refBeginTreeNode = _FindMin(refTreeInfo);
		const_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refBeginTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::end()
	{
		iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = m_oTreePool.get_head() + Alloc::CAPACITY;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::cend() const
	{
		const_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = m_oTreePool.get_head() + Alloc::CAPACITY;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::reverse_iterator Tree<_K, Alloc, KeyLess>::rbegin()
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return rend();
		}
		TreeInfo& refTreNode = m_oTreePool.get_value(m_dwRootIndex);
		TreeInfo& refBeginTreeNode = _FindMax(refTreNode);
		reverse_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refBeginTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_reverse_iterator Tree<_K, Alloc, KeyLess>::crbegin() const
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return crend();
		}
		const TreeInfo& refTreNode = m_oTreePool.get_value(m_dwRootIndex);
		const TreeInfo& refBeginTreeNode = _FindMax(refTreNode);
		const_reverse_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refBeginTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::reverse_iterator Tree<_K, Alloc, KeyLess>::rend()
	{
		reverse_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &m_oTreePool.get_value(0) - 1;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_reverse_iterator Tree<_K, Alloc, KeyLess>::crend() const
	{
		const_reverse_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &m_oTreePool.get_value(0) - 1;
		return it;
	}
	
	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::lower_bound(const _K& k)
	{
		iterator it = end();
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return it;
		}
		_LowerBound(it, m_dwRootIndex, k);
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::upper_bound(const _K& k)
	{
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			return end();
		}
		iterator it = end();
		_UpperBound(it, m_dwRootIndex, k);
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::clear()
	{
		if (Alloc::CAPACITY != m_dwRootIndex)
		{
			_Clear(m_oTreePool.get_value(m_dwRootIndex));
		}
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::size() const
	{
		assert(m_dwSize == _GetNodeNum(m_dwRootIndex));
		return m_dwSize;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::debug_info(std::ostream& os)
	{
		std::deque<unsigned int> tree_node_indexs[2];
		int deque_index = 0;
		tree_node_indexs[deque_index].push_back(m_dwRootIndex);
		while (!tree_node_indexs[deque_index].empty())
		{
			int next_deque_index = (deque_index + 1) % 2;
			unsigned int dwIndex = *tree_node_indexs[deque_index].begin();
			if ((dwIndex & 0x0000FFFF) == Alloc::CAPACITY)
			{
				unsigned int dwHeight = (dwIndex & 0xFFFF0000) >> 16;
				os << "0-" << dwHeight << ",";
				tree_node_indexs[deque_index].pop_front();
				if (tree_node_indexs[deque_index].empty())
				{
					deque_index = next_deque_index;
					os << "\n";
				}
				continue;
			}
			TreeInfo& refTreeNode = m_oTreePool.get_value(dwIndex);
			tree_node_indexs[deque_index].pop_front();
			if (Alloc::CAPACITY == refTreeNode.m_dwLeftIndex)
			{
				tree_node_indexs[next_deque_index].push_back(((refTreeNode.m_dwHeight - 1) << 16) | refTreeNode.m_dwLeftIndex);
			}
			else
			{
				tree_node_indexs[next_deque_index].push_back(refTreeNode.m_dwLeftIndex);
			}
			if (Alloc::CAPACITY == refTreeNode.m_dwRightIndex)
			{
				tree_node_indexs[next_deque_index].push_back(((refTreeNode.m_dwHeight - 1) << 16) | refTreeNode.m_dwRightIndex);
			}
			else
			{
				tree_node_indexs[next_deque_index].push_back(refTreeNode.m_dwRightIndex);
			}
			os << Alloc::get_compare_val(refTreeNode) << "-" << refTreeNode.m_dwHeight << ", ";
			if (tree_node_indexs[deque_index].empty())
			{
				deque_index = next_deque_index;
				os << "\n";
			}
		}
		os << "\n";
		os << "\n";
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_CheckNode()
	{
		for (int i = 0; i < Alloc::CAPACITY; ++i)
		{
			TreeInfo& refInfo = m_oTreePool.get_value(i);
			if (Alloc::CAPACITY != refInfo.m_dwMyIndex)
			{
				TreeInfo* pP = nullptr;
				TreeInfo* pL = nullptr;
				TreeInfo* pR = nullptr;
				if (Alloc::CAPACITY != refInfo.m_dwParentIndex)
				{
					pP = &m_oTreePool.get_value(refInfo.m_dwParentIndex);
				}
				if (Alloc::CAPACITY != refInfo.m_dwLeftIndex)
				{
					pL = &m_oTreePool.get_value(refInfo.m_dwLeftIndex);
				}
				if (Alloc::CAPACITY != refInfo.m_dwRightIndex)
				{
					pR = &m_oTreePool.get_value(refInfo.m_dwRightIndex);
				}
				if (pP)
				{
					assert(pP->m_dwLeftIndex == refInfo.m_dwMyIndex || pP->m_dwRightIndex == refInfo.m_dwMyIndex);
					assert(refInfo.m_dwParentIndex == pP->m_dwMyIndex);
				}
				else
				{
					assert(Alloc::CAPACITY == refInfo.m_dwParentIndex);
				}
				if (pL)
				{
					assert(pL->m_dwParentIndex == refInfo.m_dwMyIndex);
					assert(refInfo.m_dwLeftIndex == pL->m_dwMyIndex);
				}
				else
				{
					assert(Alloc::CAPACITY == refInfo.m_dwLeftIndex);
				}
				if (pR)
				{
					assert(pR->m_dwParentIndex == refInfo.m_dwMyIndex);
					assert(refInfo.m_dwRightIndex == pR->m_dwMyIndex);
				}
				else
				{
					assert(Alloc::CAPACITY == refInfo.m_dwRightIndex);
				}
			}
		}
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::IteratorNode& Tree<_K, Alloc, KeyLess>::_GetIteratorNode(TreeInfo* pTreeInfo)
	{
		return *(IteratorNode*)(&m_oNodePoolNew.get_value(pTreeInfo->m_dwNodeIndex));
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline const typename Tree<_K, Alloc, KeyLess>::IteratorNode& Tree<_K, Alloc, KeyLess>::_GetIteratorNode(TreeInfo* pTreeInfo) const
	{
		return *(IteratorNode*)(&m_oNodePoolNew.get_value(pTreeInfo->m_dwNodeIndex));
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_BuyNode()
	{
		unsigned int dwTreeIndex = m_oTreePool.alloc();
		unsigned int dwNodeIndex = m_oNodePoolNew.alloc();

		m_oTreePool.get_value(dwNodeIndex).m_dwMyIndex = dwTreeIndex;
		m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex = dwNodeIndex;
		m_oTreePool.get_value(dwNodeIndex).m_dwParentIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwLeftIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwRightIndex = Alloc::CAPACITY;

		++m_dwSize;
		return dwNodeIndex;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_BuyNode(std::ostream os)
	{
		unsigned int dwTreeIndex = m_oTreePool.alloc(os);
		unsigned int dwNodeIndex = m_oNodePoolNew.alloc(os);

		m_oTreePool.get_value(dwNodeIndex).m_dwMyIndex = dwTreeIndex;
		m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex = dwNodeIndex;
		m_oTreePool.get_value(dwNodeIndex).m_dwParentIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwLeftIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwRightIndex = Alloc::CAPACITY;

		++m_dwSize;
		return dwNodeIndex;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline bool Tree<_K, Alloc, KeyLess>::_FreeNode(unsigned int dwNodeIndex)
	{
		m_oNodePoolNew.release(m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex);
		m_oTreePool.get_value(dwNodeIndex).m_dwMyIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwParentIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwLeftIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwRightIndex = Alloc::CAPACITY;
		m_oTreePool.release(dwNodeIndex);

		--m_dwSize;
		return true;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline bool Tree<_K, Alloc, KeyLess>::_FreeNode(unsigned int dwNodeIndex, std::ostream& os)
	{
		m_oNodePoolNew.release(m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex, os);
		m_oTreePool.get_value(dwNodeIndex).m_dwMyIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwNodeIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwParentIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwLeftIndex = Alloc::CAPACITY;
		m_oTreePool.get_value(dwNodeIndex).m_dwRightIndex = Alloc::CAPACITY;
		m_oTreePool.release(dwNodeIndex, os);
	
		--m_dwSize;
		return true;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline bool Tree<_K, Alloc, KeyLess>::_Less(unsigned int dwLeftNodeIndex, unsigned int dwRoightNodeIndex) const
	{
		const TreeInfo& refLeftNode = m_oTreePool.get_value(dwLeftNodeIndex);
		const TreeInfo& refRightNode = m_oTreePool.get_value(dwRoightNodeIndex);

		return KeyLess()(Alloc::get_compare_val(m_oNodePoolNew.get_value(refLeftNode.m_dwNodeIndex))
			, Alloc::get_compare_val(m_oNodePoolNew.get_value(refRightNode.m_dwNodeIndex)));
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline bool Tree<_K, Alloc, KeyLess>::_Less4K(const _K& l, const _K& r) const
	{
		return KeyLess()(l, r);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline bool Tree<_K, Alloc, KeyLess>::_Equal4K(const _K& l, const _K& r) const
	{
		return !KeyLess()(l, r) && !KeyLess()(r, l);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_RemoveNode(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY != refTreeNode.m_dwParentIndex)
		{
			TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				refParentTreeNode.m_dwLeftIndex = Alloc::CAPACITY;
			}
			else if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				refParentTreeNode.m_dwRightIndex = Alloc::CAPACITY;
			}
			else
			{
				assert(0);
			}
		}
		_FreeNode(refTreeNode.m_dwMyIndex);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetIterator(TreeInfo& refTreeNode)
	{
		iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::_GetIterator(TreeInfo& refTreeNode) const
	{
		const_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetIterator(unsigned int dwTreeNodeIndex)
	{
		if (dwTreeNodeIndex == Alloc::CAPACITY)
		{
			return end();
		}
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::_GetIterator(unsigned int dwTreeNodeIndex) const
	{
		if (dwTreeNodeIndex == Alloc::CAPACITY)
		{
			return cend();
		}
		const TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		const_iterator it;
		it.m_pTree = this;
		it.m_pTreeNode = &refTreeNode;
		return it;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline int Tree<_K, Alloc, KeyLess>::_GetHeight(unsigned int dwIndex)
	{
		if (Alloc::CAPACITY == dwIndex)
		{
			return 0;
		}
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwIndex);
		return refTreeNode.m_dwHeight;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_UpdateHeight(TreeInfo& refTreeNode)
	{
		refTreeNode.m_dwHeight = std::max(_GetHeight(refTreeNode.m_dwLeftIndex), _GetHeight(refTreeNode.m_dwRightIndex)) + 1;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline const TreeInfo& Tree<_K, Alloc, KeyLess>::_FindMax(const TreeInfo& refTreeNode) const
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwRightIndex)
		{
			return refTreeNode;
		}
		const TreeInfo& refRightTreeNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
		return _FindMax(refRightTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline TreeInfo& Tree<_K, Alloc, KeyLess>::_FindMax(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwRightIndex)
		{
			return refTreeNode;
		}
		TreeInfo& refRightTreeNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
		return _FindMax(refRightTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline const TreeInfo& Tree<_K, Alloc, KeyLess>::_FindMin(const TreeInfo& refTreeNode) const
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwLeftIndex)
		{
			return refTreeNode;
		}
		const TreeInfo& refLeftTreeNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
		return _FindMin(refLeftTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline TreeInfo& Tree<_K, Alloc, KeyLess>::_FindMin(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwLeftIndex)
		{
			return refTreeNode;
		}
		TreeInfo& refLeftTreeNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
		return _FindMin(refLeftTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_Find(unsigned int dwTreeNodeIndex, const _K& k)
	{
		if (Alloc::CAPACITY == dwTreeNodeIndex)
		{
			return end();
		}
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		if (_Less4K(k, Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex))))
		{
			return _Find(refTreeNode.m_dwLeftIndex, k);
		}
		else if (_Less4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex)), k))
		{
			return _Find(refTreeNode.m_dwRightIndex, k);
		}
		return _GetIterator(refTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_LowerBound(iterator& refIterator, unsigned int dwTreeNodeIndex, const _K& k)
	{
		if (Alloc::CAPACITY == dwTreeNodeIndex)
		{
			return refIterator;
		}
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		if (_Less4K(k, Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex))))
		{
			refIterator = _GetIterator(refTreeNode);
			return _LowerBound(refIterator, refTreeNode.m_dwLeftIndex, k);
		}
		else if (_Less4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex)), k))
		{
			return _LowerBound(refIterator, refTreeNode.m_dwRightIndex, k);
		}

		refIterator = _GetIterator(refTreeNode);

		return refIterator;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_UpperBound(iterator& refIterator, unsigned int dwTreeNodeIndex, const _K& k)
	{
		if (Alloc::CAPACITY == dwTreeNodeIndex)
		{
			return refIterator;
		}
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		if (_Less4K(k, Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex))))
		{
			refIterator = _GetIterator(refTreeNode);
			return _UpperBound(refIterator, refTreeNode.m_dwLeftIndex, k);
		}
		else
		{
			return _UpperBound(refIterator, refTreeNode.m_dwRightIndex, k);
		}

		assert(0);
		return refIterator;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_SwapNode4Erase(TreeInfo& l, TreeInfo& r)
	{
		if (m_dwRootIndex == l.m_dwMyIndex)
		{
			m_dwRootIndex = r.m_dwMyIndex;
		}
		TreeInfo* pLP = nullptr;
		TreeInfo* pLL = nullptr;
		TreeInfo* pLR = nullptr;
		TreeInfo* pRP = nullptr;
		TreeInfo* pRL = nullptr;
		TreeInfo* pRR = nullptr;
		if (Alloc::CAPACITY != l.m_dwParentIndex)
		{
			pLP = &m_oTreePool.get_value(l.m_dwParentIndex);
		}
		if (Alloc::CAPACITY != l.m_dwLeftIndex)
		{
			pLL = &m_oTreePool.get_value(l.m_dwLeftIndex);
		}
		if (Alloc::CAPACITY != l.m_dwRightIndex)
		{
			pLR = &m_oTreePool.get_value(l.m_dwRightIndex);
		}

		if (Alloc::CAPACITY != r.m_dwParentIndex)
		{
			pRP = &m_oTreePool.get_value(r.m_dwParentIndex);
		}
		if (Alloc::CAPACITY != r.m_dwLeftIndex)
		{
			pRL = &m_oTreePool.get_value(r.m_dwLeftIndex);
		}
		if (Alloc::CAPACITY != r.m_dwRightIndex)
		{
			pRR = &m_oTreePool.get_value(r.m_dwRightIndex);
		}

		if (pLP)
		{
			if (pLP->m_dwLeftIndex == l.m_dwMyIndex)
			{
				pLP->m_dwLeftIndex = Alloc::CAPACITY;
			}
			else if (pLP->m_dwRightIndex == l.m_dwMyIndex)
			{
				pLP->m_dwRightIndex = Alloc::CAPACITY;
			}
			else
			{
				throw(std::exception());
			}
			l.m_dwParentIndex = Alloc::CAPACITY;
		}
		if (pLL)
		{
			pLL->m_dwParentIndex = Alloc::CAPACITY;
			l.m_dwLeftIndex = Alloc::CAPACITY;
		}
		if (pLR)
		{
			pLR->m_dwParentIndex = Alloc::CAPACITY;
			l.m_dwRightIndex = Alloc::CAPACITY;
		}
		if (pRP)
		{
			if (pRP->m_dwLeftIndex == r.m_dwMyIndex)
			{
				pRP->m_dwLeftIndex = Alloc::CAPACITY;
			}
			else if (pRP->m_dwRightIndex == r.m_dwMyIndex)
			{
				pRP->m_dwRightIndex = Alloc::CAPACITY;
			}
			//else
			//{
			//	throw(std::exception());
			//}
			r.m_dwParentIndex = Alloc::CAPACITY;;
		}
		if (pRL)
		{
			pRL->m_dwParentIndex = Alloc::CAPACITY;
			r.m_dwLeftIndex = Alloc::CAPACITY;
		}
		if (pRR)
		{
			pRR->m_dwParentIndex = Alloc::CAPACITY;
			r.m_dwRightIndex = Alloc::CAPACITY;
		}
		
		if (pLP)
		{
			r.m_dwParentIndex = pLP->m_dwMyIndex;

			if (_Less4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(r.m_dwNodeIndex))
				, Alloc::get_compare_val(m_oNodePoolNew.get_value(pLP->m_dwNodeIndex))))
			{
				assert(Alloc::CAPACITY == pLP->m_dwLeftIndex);
				pLP->m_dwLeftIndex = r.m_dwMyIndex;
			}
			else
			{
				assert(Alloc::CAPACITY == pLP->m_dwRightIndex);
				pLP->m_dwRightIndex = r.m_dwMyIndex;
			}
		}
		if (pRL)
		{
			l.m_dwLeftIndex = pRL->m_dwMyIndex;
			pRL->m_dwParentIndex = l.m_dwMyIndex;
		}
		if (pRR)
		{
			l.m_dwRightIndex = pRR->m_dwMyIndex;
			pRR->m_dwParentIndex = l.m_dwMyIndex;
		}

		assert(pRP);
		if (pRP == &l)
		{
			l.m_dwParentIndex = r.m_dwMyIndex;
			if (pLL == &r)
			{
				r.m_dwLeftIndex = l.m_dwMyIndex;
				if (pLR)
				{
					r.m_dwRightIndex = pLR->m_dwMyIndex;
					pLR->m_dwParentIndex = r.m_dwMyIndex;
				}
			}
			else if (pLR == &r)
			{
				r.m_dwRightIndex = l.m_dwMyIndex;
				if (pLL)
				{
					r.m_dwLeftIndex = pLL->m_dwMyIndex;
					pLL->m_dwParentIndex = r.m_dwMyIndex;
				}
			}
			else
			{
				throw(std::exception());
			}
		}
		else
		{
			l.m_dwParentIndex = pRP->m_dwMyIndex;
			if (_Less4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(l.m_dwNodeIndex))
				, Alloc::get_compare_val(m_oNodePoolNew.get_value(pRP->m_dwNodeIndex))))
			{
				assert(Alloc::CAPACITY == pRP->m_dwLeftIndex);
				pRP->m_dwLeftIndex = l.m_dwMyIndex;
			}
			else
			{
				assert(Alloc::CAPACITY == pRP->m_dwRightIndex);
				pRP->m_dwRightIndex = l.m_dwMyIndex;
			}
			if (pLL)
			{
				r.m_dwLeftIndex = pLL->m_dwMyIndex;
				pLL->m_dwParentIndex = r.m_dwMyIndex;
			}
			if (pLR)
			{
				r.m_dwRightIndex = pLR->m_dwMyIndex;
				pLR->m_dwParentIndex = r.m_dwMyIndex;
			}
		}

	
		//----------------------------------------------------------------------//
		//if (pLP)
		//{
		//	assert(pLP->m_dwLeftIndex == r.m_dwMyIndex || pLP->m_dwRightIndex == r.m_dwMyIndex);
		//}
		//else
		//{
		//	assert(Alloc::CAPACITY == r.m_dwParentIndex);
		//}
		//if (pRP != &l)
		//{
		//	assert(pRP->m_dwLeftIndex == l.m_dwMyIndex || pRP->m_dwRightIndex == l.m_dwMyIndex);
		//}

		//if (pLL)
		//{
		//	if (pLL != &r)
		//	{
		//		assert(pLL->m_dwParentIndex == r.m_dwMyIndex);
		//		assert(r.m_dwLeftIndex == pLL->m_dwMyIndex);
		//	}
		//}
		//else
		//{
		//	assert(Alloc::CAPACITY == r.m_dwLeftIndex);
		//}
		//if (pRL)
		//{
		//	assert(pRL->m_dwParentIndex == l.m_dwMyIndex);
		//	assert(l.m_dwLeftIndex == pRL->m_dwMyIndex);
		//}

		//if (pLR)
		//{
		//	if (pLR != &r)
		//	{
		//		assert(pLR->m_dwParentIndex == r.m_dwMyIndex);
		//		assert(r.m_dwRightIndex == pLR->m_dwMyIndex);
		//	}
		//}
		//if (pRR)
		//{
		//	assert(pRR->m_dwParentIndex == l.m_dwMyIndex);
		//	assert(l.m_dwRightIndex == pRR->m_dwMyIndex);
		//}

		//_CheckNode();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetNext(const iterator& ite)
	{
		const TreeInfo& refTreeNode = *ite.m_pTreeNode;
		if (Alloc::CAPACITY != refTreeNode.m_dwRightIndex)
		{
			return _GetIterator(_FindMin(m_oTreePool.get_value(refTreeNode.m_dwRightIndex)));
		}
		else
		{
			if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
			{
				return end();
			}
			TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetIterator(refParentTreeNode);
			}
			else if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetNext4Parent(refParentTreeNode);
			}
			else
			{
				assert(0);
			}
		}

		assert(0);
		return iterator();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::_GetNext(const const_iterator& cite) const
	{
		const TreeInfo& refTreeNode = cite.m_pTreeNode;
		if (Alloc::CAPACITY != refTreeNode.m_dwRightIndex)
		{
			return _GetIterator(_FindMin(m_oTreePool.get_value(refTreeNode.m_dwRightIndex)));
		}
		else
		{
			if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
			{
				return cend();
			}
			TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetIterator(refParentTreeNode);
			}
			else if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetNext4Parent(refParentTreeNode);
			}
			else
			{
				assert(0);
			}
		}

		assert(0);
		return iterator();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetNext4Parent(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
		{
			return end();
		}
		TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
		if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetIterator(refParentTreeNode);
		}
		else if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetNext4Parent(refParentTreeNode);
		}
		assert(0);
		return iterator();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::const_iterator Tree<_K, Alloc, KeyLess>::_GetNext4Parent(TreeInfo& refTreeNode) const
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
		{
			return cend();
		}
		TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
		if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetIterator(refParentTreeNode);
		}
		else if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetNext4Parent(refParentTreeNode);
		}
		assert(0);
		return const_iterator();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetPrevious(const TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY != refTreeNode.m_dwLeftIndex)
		{
			return _GetIterator(_FindMax(m_oTreePool.get_value(refTreeNode.m_dwLeftIndex)));
		}
		else
		{
			if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
			{
				return end();
			}
			TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetIterator(refParentTreeNode);
			}
			else if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				return _GetPrevious4Parent(refParentTreeNode);
			}
			else
			{
				assert(0);
			}
		}

		assert(0);
		return iterator();
	}
	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_GetPrevious4Parent(const TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY == refTreeNode.m_dwParentIndex)
		{
			return end();
		}
		TreeInfo& refParentTreeNode = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
		if (refParentTreeNode.m_dwRightIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetIterator(refParentTreeNode);
		}
		else if (refParentTreeNode.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
		{
			return _GetNext4Parent(refParentTreeNode);
		}
		assert(0);
		return iterator();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_GetNodeNum(unsigned int dwTreeNodeIndex) const
	{
		if (Alloc::CAPACITY == dwTreeNodeIndex)
		{
			return 0;
		}
		const TreeInfo& refTreeNode = m_oTreePool.get_value(dwTreeNodeIndex);
		return _GetNodeNum(refTreeNode.m_dwLeftIndex) + _GetNodeNum(refTreeNode.m_dwRightIndex) + 1;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::insert_ret Tree<_K, Alloc, KeyLess>::_Insert(
		unsigned int dwCompareIndex, const typename Alloc::ValueType& v)
	{
		unsigned int dwNodeIndex = _BuyNode();
		TreeInfo& refTreeInfoNew = m_oTreePool.get_value(dwNodeIndex);
		m_oNodePoolNew.get_value(refTreeInfoNew.m_dwNodeIndex) = v;

		typename Tree<_K, Alloc, KeyLess>::insert_ret it_ret;
		it_ret.first.m_pTreeNode = &refTreeInfoNew;
		it_ret.first.m_pTree = this;
		it_ret.second = false;
		_Insert(it_ret, dwCompareIndex, Alloc::CAPACITY, dwNodeIndex);

		return it_ret;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_Insert(
		typename Tree<_K, Alloc, KeyLess>::insert_ret& it_ret
		, unsigned int& dwCompareIndex, unsigned int dwCompareParentIndex, unsigned int dwIndex)
	{
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwIndex);
		if (Alloc::CAPACITY == m_dwRootIndex)
		{
			m_dwRootIndex = dwIndex;
			refTreeNode.m_dwHeight = 1;
			it_ret.second = true;
		}
		else if (Alloc::CAPACITY == dwCompareIndex)
		{
			dwCompareIndex = dwIndex;
			refTreeNode.m_dwParentIndex = dwCompareParentIndex;
			refTreeNode.m_dwHeight = 1;
			it_ret.second = true;
		}
		else if (_Less(dwIndex, dwCompareIndex))
		{
			TreeInfo& refCompareTreeNode = m_oTreePool.get_value(dwCompareIndex);
			_Insert(it_ret, refCompareTreeNode.m_dwLeftIndex, dwCompareIndex, dwIndex);

			//�ж�ƽ�����
			if (_GetHeight(refCompareTreeNode.m_dwLeftIndex) - _GetHeight(refCompareTreeNode.m_dwRightIndex) > 1)
			{
				//��������� ���������
				if (_Less(dwIndex, refCompareTreeNode.m_dwLeftIndex))	//����
				{
					_LL(refCompareTreeNode);
				}
				else													//����
				{
					_LR(refCompareTreeNode);
				}
			}
			else
			{
				_UpdateHeight(refCompareTreeNode);
			}
		}
		else if (_Less(dwCompareIndex, dwIndex))
		{
			TreeInfo& refCompareTreeNode = m_oTreePool.get_value(dwCompareIndex);
			_Insert(it_ret, refCompareTreeNode.m_dwRightIndex, dwCompareIndex, dwIndex);

			if (_GetHeight(refCompareTreeNode.m_dwRightIndex) - _GetHeight(refCompareTreeNode.m_dwLeftIndex) > 1)
			{

				if (_Less(refCompareTreeNode.m_dwRightIndex, dwIndex))
				{
					_RR(refCompareTreeNode);
				}
				else
				{
					_RL(refCompareTreeNode);
				}
			}
			else
			{
				_UpdateHeight(refCompareTreeNode);
			}
		}
		else
		{
			//�ظ�����
			it_ret.second = false;
		}
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline typename Tree<_K, Alloc, KeyLess>::iterator Tree<_K, Alloc, KeyLess>::_Erase(unsigned int dwCompareIndex, const _K k)
	{
		TreeInfo& refTreeNode = m_oTreePool.get_value(dwCompareIndex);
		//�ҵ���Ҫɾ���Ľ��
		if (_Equal4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex)), k))
		{
			//�����������ǿ�
			if (refTreeNode.m_dwLeftIndex != Alloc::CAPACITY && refTreeNode.m_dwRightIndex != Alloc::CAPACITY)
			{//�ڸ߶ȸ�����Ǹ������Ͻ���ɾ������

				//�������߶ȴ�ɾ����������ֵ���Ľ�㣬���丳�������
				if (_GetHeight(refTreeNode.m_dwLeftIndex) > _GetHeight(refTreeNode.m_dwRightIndex))
				{
					TreeInfo& refLeftNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
					TreeInfo& refLeftMaxNode = _FindMax(refLeftNode);

					_SwapNode4Erase(refTreeNode, refLeftMaxNode);
					Alloc::set_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwMyIndex), m_oNodePoolNew.get_value(refLeftMaxNode.m_dwMyIndex));
					_Erase(refLeftMaxNode.m_dwLeftIndex, Alloc::get_compare_val(m_oNodePoolNew.get_value(refLeftMaxNode.m_dwNodeIndex)));
					_UpdateHeight(refLeftMaxNode);
					//std::cout << _GetIterator(_FindMin(m_oTreePool.get_value(refLeftMaxNode.m_dwRightIndex)))->first << "\n";
					//assert(_GetIterator(_FindMin(m_oTreePool.get_value(refLeftMaxNode.m_dwRightIndex)))->first > k);
					return _GetIterator(_FindMin(m_oTreePool.get_value(refLeftMaxNode.m_dwRightIndex)));
				}
				else//�������߶ȸ���ɾ����������ֵ��С�Ľ�㣬���丳�������
				{
					TreeInfo& refRightNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
					TreeInfo& refRightMinNode = _FindMin(refRightNode);

					_SwapNode4Erase(refTreeNode, refRightMinNode);
					Alloc::set_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwMyIndex), m_oNodePoolNew.get_value(refRightMinNode.m_dwMyIndex));
					_Erase(refRightMinNode.m_dwRightIndex, Alloc::get_compare_val(m_oNodePoolNew.get_value(refRightMinNode.m_dwNodeIndex)));
					_UpdateHeight(refRightMinNode);
					//std::cout << "++++++++++++" << _GetIterator(refRightMinNode)->first << "\n";
					//assert(_GetIterator(refRightMinNode)->first > k);
					return _GetIterator(refRightMinNode);
				}
			}
			else
			{//����������һ����Ϊ�գ�ֱ������Ҫɾ���Ľ����ӽ���滻����
				iterator it = _GetNext(iterator(this, &refTreeNode));
				if (Alloc::CAPACITY != refTreeNode.m_dwLeftIndex)
				{
					iterator it = _GetNext(iterator(this, &refTreeNode));
					TreeInfo& refLeftNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
					_SwapNode4Erase(refTreeNode, refLeftNode);
					_RemoveNode(refTreeNode);
					//std::cout << it->first << "\n";
					//assert((it->first >= k) || (it == end()));
					return it;
				}
				else if (Alloc::CAPACITY != refTreeNode.m_dwRightIndex)
				{
					TreeInfo& refRightNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
					_SwapNode4Erase(refTreeNode, refRightNode);
					_RemoveNode(refTreeNode);
					//std::cout << it->first << "\n";
					//assert((it->first >= k) || (it == end()));
					return _GetIterator(refRightNode);
				}
				else if (m_dwRootIndex == dwCompareIndex)
				{
					m_dwRootIndex = Alloc::CAPACITY;
					//std::cout << "aaaddd" << "\n";
					_RemoveNode(refTreeNode);
					return it;
				}
				_RemoveNode(refTreeNode);
				//std::cout << it->first << "\n";
				//assert((it->first >= k) || (it == end()));
				return it;
			}
		}
		else if (_Less4K(k, Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex))))//Ҫɾ���Ľ������������
		{
			//�ݹ�ɾ���������ϵĽ��
			if (Alloc::CAPACITY == refTreeNode.m_dwLeftIndex)
			{
				return end();
			}
			iterator it = _Erase(refTreeNode.m_dwLeftIndex, k);
			//�ж��Ƿ���Ȼ����ƽ������
			if (_GetHeight(refTreeNode.m_dwRightIndex) - _GetHeight(refTreeNode.m_dwLeftIndex) > 1)
			{
				TreeInfo& refRightNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
				if (_GetHeight(refRightNode.m_dwLeftIndex) > _GetHeight(refRightNode.m_dwRightIndex))
				{
					//RL˫��ת
					_RL(refTreeNode);
				}
				else
				{//RR����ת
					_RR(refTreeNode);
				}
			}
			else//����ƽ������ �����߶���Ϣ
			{
				_UpdateHeight(refTreeNode);
			}
			return it;
		}
		else if (_Less4K(Alloc::get_compare_val(m_oNodePoolNew.get_value(refTreeNode.m_dwNodeIndex)), k))//Ҫɾ���Ľ������������
		{
			//�ݹ�ɾ�����������
			if (Alloc::CAPACITY == refTreeNode.m_dwRightIndex)
			{
				return end();
			}
			iterator it = _Erase(refTreeNode.m_dwRightIndex, k);
			//�ж�ƽ�����
			if (_GetHeight(refTreeNode.m_dwLeftIndex) - _GetHeight(refTreeNode.m_dwRightIndex) > 1)
			{
				TreeInfo& refLeftNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
				if (_GetHeight(refLeftNode.m_dwRightIndex) > _GetHeight(refLeftNode.m_dwLeftIndex))
				{
					//LR˫��ת
					_LR(refTreeNode);
				}
				else
				{
					//LL����ת
					_LL(refTreeNode);
				}
			}
			else//����ƽ���� �����߶�
			{
				_UpdateHeight(refTreeNode);
			}
			return it;
		}
		return end();
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline void Tree<_K, Alloc, KeyLess>::_Clear(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY != refTreeNode.m_dwRightIndex)
		{
			TreeInfo& refRightTreeNode = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
			_Clear(refRightTreeNode);
		}
		if (Alloc::CAPACITY != refTreeNode.m_dwLeftIndex)
		{
			TreeInfo& refLeftTreeNode = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
			_Clear(refLeftTreeNode);
		}
		if (refTreeNode.m_dwMyIndex == m_dwRootIndex)
		{
			m_dwRootIndex = Alloc::CAPACITY;
		}
		_RemoveNode(refTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_LL(TreeInfo& refTreeNode)
	{
		//refTreeNodeΪ�������������ʧ��Ľ��

		//��ȡʧ���������
		TreeInfo& refLeftSon = m_oTreePool.get_value(refTreeNode.m_dwLeftIndex);
		//����son����Һ��ӵĸ�ָ��
		if (Alloc::CAPACITY != refLeftSon.m_dwRightIndex)
		{
			TreeInfo& refSonRightChild = m_oTreePool.get_value(refLeftSon.m_dwRightIndex);
			//����son����Һ��ӵĸ�ָ��
			refSonRightChild.m_dwParentIndex = refTreeNode.m_dwMyIndex;
		}
		//ʧ��������ӱ��Ϊson���Һ���
		refTreeNode.m_dwLeftIndex = refLeftSon.m_dwRightIndex;
		//����ʧ����ĸ߶���Ϣ
		_UpdateHeight(refTreeNode);
		//ʧ������son���Һ���
		refLeftSon.m_dwRightIndex = refTreeNode.m_dwMyIndex;
		//����son�ĸ����Ϊԭʧ����ĸ����
		refLeftSon.m_dwParentIndex = refTreeNode.m_dwParentIndex;
		//���ʧ���㲻�Ǹ���㣬��ʼ���¸��ڵ�
		if (Alloc::CAPACITY != refTreeNode.m_dwParentIndex)
		{
			//��ȡʧ����ĸ��ڵ�
			TreeInfo& refParent = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			//������ڵ��������ʧ���㣬ָ�����ڸ��º���º���son
			if (refParent.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				refParent.m_dwLeftIndex = refLeftSon.m_dwMyIndex;
			}
			else if (refParent.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				//���ڵ���Һ�����ʧ����
				refParent.m_dwRightIndex = refLeftSon.m_dwMyIndex;
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			m_dwRootIndex = refLeftSon.m_dwMyIndex;
		}
		//����ʧ����ĸ���
		refTreeNode.m_dwParentIndex = refLeftSon.m_dwMyIndex;
		//����son���ĸ߶���Ϣ
		_UpdateHeight(refLeftSon);
		return refLeftSon.m_dwMyIndex;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_RR(TreeInfo& refTreeNode)
	{
		//refTreeNodeΪ�������������ʧ��Ľ��

		//��ȡʧ������Һ���
		TreeInfo& refRightSon = m_oTreePool.get_value(refTreeNode.m_dwRightIndex);
		//����son������ӵĸ�ָ��
		if (Alloc::CAPACITY != refRightSon.m_dwLeftIndex)
		{
			TreeInfo& refSonLeftChild = m_oTreePool.get_value(refRightSon.m_dwLeftIndex);
			//����son������ӵĸ�ָ��
			refSonLeftChild.m_dwParentIndex = refTreeNode.m_dwMyIndex;
		}
		//ʧ������Һ��ӱ��Ϊson������
		refTreeNode.m_dwRightIndex = refRightSon.m_dwLeftIndex;
		//����ʧ����ĸ߶���Ϣ
		_UpdateHeight(refTreeNode);
		//ʧ������son������
		refRightSon.m_dwLeftIndex = refTreeNode.m_dwMyIndex;
		//����son�ĸ����Ϊԭʧ����ĸ����
		refRightSon.m_dwParentIndex = refTreeNode.m_dwParentIndex;
		//���ʧ���㲻�Ǹ���㣬��ʼ���¸��ڵ�
		if (Alloc::CAPACITY != refTreeNode.m_dwParentIndex)
		{
			//��ȡʧ����ĸ��ڵ�
			TreeInfo& refParent = m_oTreePool.get_value(refTreeNode.m_dwParentIndex);
			//������ڵ��������ʧ���㣬ָ�����ڸ��º���º���son
			if (refParent.m_dwLeftIndex == refTreeNode.m_dwMyIndex)
			{
				refParent.m_dwLeftIndex = refRightSon.m_dwMyIndex;
			}
			else if (refParent.m_dwRightIndex == refTreeNode.m_dwMyIndex)
			{
				//���ڵ���Һ�����ʧ����
				refParent.m_dwRightIndex = refRightSon.m_dwMyIndex;
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			m_dwRootIndex = refRightSon.m_dwMyIndex;
		}
		//����ʧ����ĸ���
		refTreeNode.m_dwParentIndex = refRightSon.m_dwMyIndex;
		//����son���ĸ߶���Ϣ
		_UpdateHeight(refRightSon);
		return refRightSon.m_dwMyIndex;
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_LR(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY != refTreeNode.m_dwLeftIndex)
		{
			_RR(m_oTreePool.get_value(refTreeNode.m_dwLeftIndex));
		}
		return _LL(refTreeNode);
	}

	template<typename _K, typename Alloc, typename KeyLess>
	inline unsigned int Tree<_K, Alloc, KeyLess>::_RL(TreeInfo& refTreeNode)
	{
		if (Alloc::CAPACITY != refTreeNode.m_dwRightIndex)
		{
			_LL(m_oTreePool.get_value(refTreeNode.m_dwRightIndex));
		}
		return _RR(refTreeNode);
	}

}

#endif // __SHARE_MEM_CONTAINER_TREE_H__

