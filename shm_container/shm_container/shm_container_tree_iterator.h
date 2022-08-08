#ifndef __SHARE_MEM_CONTAINER_TREE_ITERATOR_H__
#define __SHARE_MEM_CONTAINER_TREE_ITERATOR_H__

namespace ShareMemoryContainer
{
	template<typename Tree, typename Node>
	class TreeIterator
	{
		friend Tree;
	public:
		TreeIterator() : m_pTreeNode(0), m_pTree(0) {}
		typename Tree::IteratorNode& operator*() { return m_pTree->_GetIteratorNode(m_pTreeNode); }
		typename Tree::IteratorNode* operator->() { return &m_pTree->_GetIteratorNode(m_pTreeNode); }
		//TreeIterator<Tree, Node>& operator=(const TreeIterator<Tree, Node>& r) { m_pTreeNode = r->m_pTreeNode; m_pTree = r->m_pTree; return *this; }
		bool operator==(const TreeIterator& it)
		{
			return (m_pTreeNode == it.m_pTreeNode) && (m_pTree == it.m_pTree);
		}
		bool operator!=(const TreeIterator& it)
		{
			return !(*this == it);
		}
		TreeIterator<Tree, Node>& operator++()
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			*this = m_pTree->_GetNext(*this);
			return *this;
		}
		TreeIterator<Tree, Node> operator++(int)
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			TreeIterator<Tree, Node> it;
			it.m_pTree = m_pTree;
			it.m_pTreeNode = m_pTreeNode;
			m_pTreeNode = m_pTree->_GetNext(*m_pTreeNode).m_pTreeNode;
			return it;
		}
		TreeIterator<Tree, Node>& operator--()
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			m_pTreeNode = m_pTree->_GetPrevious(*m_pTreeNode).m_pTreeNode;
			return *this;
		}
		TreeIterator<Tree, Node> operator--(int)
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			TreeIterator<Tree, Node> it;
			it.m_pTree = m_pTree;
			it.m_pTreeNode = m_pTreeNode;
			m_pTreeNode = m_pTree->_GetPrevious(*m_pTreeNode).m_pTreeNode;
			return it;
		}
	private:
		TreeIterator(Tree* pTree, Node* pNode) : m_pTreeNode(pNode), m_pTree(pTree) {}
		Node* m_pTreeNode;
		Tree* m_pTree;
	};

	template<typename Tree, typename Node>
	class TreeReverseIterator
	{
		friend Tree;
	public:
		TreeReverseIterator() : m_pTreeNode(0), m_pTree(0) {}
		typename Node::IteratorNode& operator*() { return m_pTree->_GetIteratorNode(m_pTreeNode); }
		typename Node::IteratorNode* operator->() { return &m_pTree->_GetIteratorNode(m_pTreeNode); }
		//TreeIterator<Tree, Node>& operator=(const TreeIterator<Tree, Node>& r) { m_pTreeNode = r->m_pTreeNode; m_pTree = r->m_pTree; return *this; }
		bool operator==(const TreeReverseIterator& it)
		{
			return (m_pTreeNode == it.m_pTreeNode) && (m_pTree == it.m_pTree);
		}
		bool operator!=(const TreeReverseIterator& it)
		{
			return !(*this == it);
		}
		TreeReverseIterator<Tree, Node>& operator++()
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			*this = m_pTree->_GetPrevious(*this);
			return *this;
		}
		TreeReverseIterator<Tree, Node> operator++(int)
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			TreeReverseIterator<Tree, Node> it;
			it.m_pTree = m_pTree;
			it.m_pTreeNode = m_pTreeNode;
			m_pTreeNode = m_pTree->_GetPrevious(*m_pTreeNode).m_pTreeNode;
			return it;
		}
		TreeReverseIterator<Tree, Node>& operator--()
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			m_pTreeNode = m_pTree->_GetNext(*m_pTreeNode).m_pTreeNode;
			return *this;
		}
		TreeReverseIterator<Tree, Node> operator--(int)
		{
			if (!m_pTree) { return *this; }
			if (!m_pTreeNode) { return *this; }
			TreeReverseIterator<Tree, Node> it;
			it.m_pTree = m_pTree;
			it.m_pTreeNode = m_pTreeNode;
			m_pTreeNode = m_pTree->_GetNext(*m_pTreeNode).m_pTreeNode;
			return it;
		}
	private:
		TreeReverseIterator(Tree* pTree, Node* pNode) : m_pTreeNode(pNode), m_pTree(pTree) {}
		Node* m_pTreeNode;
		Tree* m_pTree;
	};
}


#endif	//!__SHARE_MEM_CONTAINER_TREE_ITERATOR_H__

