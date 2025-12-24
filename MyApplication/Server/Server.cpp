
#include "pch.h"
#include <thread>
#include <iostream>
#include <mutex>

using namespace std;
vector<int> v_data;
mutex loc;

void Push()
{
	unique_lock<mutex> lock(loc);
	for (int32 i = 0; i < 10000000; i++)
	{
		v_data.push_back(i);

		// 중간에 브레이크 포인트로 탈출하면 불행하게 unlock을 안하고 빠져나감.
		// 이건 큰 문제가 될 수 있다.
	}
}

int main()
{
	v_data.reserve(200000);

	std::thread a1(Push);
	std::thread a2(Push);

	a1.join();
	a2.join();

	cout << v_data.size();

}
