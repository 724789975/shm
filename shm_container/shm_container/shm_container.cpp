// shm_container.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "shm_pool.h"
#include "shm_container_tree.h"
#include "shm_container_map.h"
#include "shm_container_set.h"

#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>
#include <set>
#include <assert.h>
#include <iostream>

int main()
{
	ShareMemoryContainer::ShmPool<int, 1024> pool;
	pool.create();

	ShareMemoryContainer::map<int, int, 1024 > _map_test;
	_map_test.create();
	_map_test.cbegin();
	_map_test.cend();

	ShareMemoryContainer::set<int, 1024 > _set_test;
	_set_test.create();
	_set_test.cbegin();
	_set_test.cend();

	std::map<int, int> _map;
	//_map.cbegin();

	std::set<int> _set;

	std::vector<int> _vec;

	srand(time(0));
	//srand(0);
	while (true)
	{
		for (int i = 0; i < 1020; ++i)
		{
			if (_vec.size() + _map.size() >= 1020)
			{
				break;
			}
			int f = rand();
			int s = rand();
			//int f = i + 1;
			//int s = f + 1;
			//_vec.push_back(f);
			auto it_r1 = _map.insert({ f, s });
			auto it_r2 = _map_test.insert({f, s});

			auto it_set1 = _set.insert(f);
			auto it_set2 = _set_test.insert(f);
			assert(it_r1.second == it_r2.second);
			if (it_r1.second)
			{
				assert(it_r1.first->first == it_r2.first->first);
				assert(it_r1.first->second == it_r2.first->second);
			}
			assert(_map_test.size() == _map.size());

			assert(it_set1.second == it_set2.second);
			if (it_set1.second)
			{
				assert(*it_set1.first == *it_set2.first);
			}
			assert(_set_test.size() == _set.size());

			std::cout << f << "\n";
			//_map_test.debug_info(std::cout);
		}

		_vec.clear();
		for (auto p : _map)
		{
			assert(p.second == _map_test.find(p.first)->second);
			_vec.push_back(p.first);
		}

		for (auto p : _set)
		{
			assert(p == *_set_test.find(p));
		}

		for (auto p : _set_test)
		{
			assert(p == *_set.find(p));
		}

		auto it_1 = _map_test.begin();
		auto it_2 = _map.begin();
		while (true)
		{
			if (it_2 == _map.end())
			{
				assert(it_1 == _map_test.end());
				break;
			}
			assert(it_1->first == it_2->first);
			assert(it_1->second == it_2->second);
			++it_1;
			++it_2;
		}

		auto set_it_1 = _set_test.begin();
		auto set_it_2 = _set.begin();
		while (true)
		{
			if (set_it_2 == _set.end())
			{
				assert(set_it_1 == _set_test.end());
				break;
			}
			assert(*set_it_1 == *set_it_2);
			++set_it_1;
			++set_it_2;
		}

		std::random_shuffle(_vec.begin(), _vec.end());

		int rand_del = rand() % 20 + 1;
		while (!_vec.empty())
		{
			int p = _vec.back();
			_vec.pop_back();
			if (0 == p % rand_del)
			{
				continue;
			}
			std::cout << "erase " << p << "\n";

			auto it1 = _map_test.find(p);
			auto it2 = _map.find(p);
			if (_map.end() != it2)
			{
				auto it11 = _map_test.erase(it1);
				//_map_test._CheckNode();
				auto it22 = _map.erase(it2);
				if (it22 == _map.end())
				{
					assert(it11 == _map_test.end());
				}
				else
				{
					assert(it11->second == it22->second);
				}
				//_map_test.debug_info(std::cout);
			}
			assert(_map_test.size() == _map.size());

			auto set_it1 = _set_test.find(p);
			auto set_it2 = _set.find(p);
			if (_set.end() != set_it2)
			{
				auto it11 = _set_test.erase(set_it1);
				//_set_test._CheckNode();
				auto it22 = _set.erase(set_it2);
				if (it22 == _set.end())
				{
					assert(it11 == _set_test.end());
				}
				else
				{
					assert(*it11 == *it22);
				}
				//_map_test.debug_info(std::cout);
			}
			assert(_map_test.size() == _map.size());
			assert(_set_test.size() == _set.size());
		}

		if (rand_del == 11)
		{
			_map.clear();
			_map_test.clear();
			_set.clear();
			_set_test.clear();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	std::map<int, int> m;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
