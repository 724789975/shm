#ifndef __SHARE_MEM_CONTAINER_LESS_H__
#define __SHARE_MEM_CONTAINER_LESS_H__

namespace ShareMemoryContainer
{
	template<typename T>
	class Less
	{
	public:
		bool operator() (const T& refLeft, const T& refRight)const
		{
			return refLeft < refRight;
		}
	};

	template<typename T, class KeyCompare = Less<T> >
	class Equal
	{
	public:
		bool operator() (const T& refLeft, const T& refRight)const
		{
			return (!KeyCompare()(refLeft, refRight)) && (!KeyCompare()(refRight, refLeft));
		}
	};
};
#endif // !__SHARE_MEM_CONTAINER_LESS_H__
