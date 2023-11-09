#include <iostream>
#include <chrono>
#include <ctime>
#include "qusort.h"
#include "threadpool.h"

using namespace std;

int main()
{
	unsigned int start;
	unsigned int stop;
	int a_size = 100000;
	int* a = new int[a_size];
	int left = 0;
	int right = a_size - 1;

	promise<void> p;
	future<void> f = p.get_future();

	bool thread_on = false;   //Однопоточный режим
	cout << "Thread off" << endl;
	far(a, left, right);   //Заполнение массива
	cheksort(a, a_size);   //Проверка массива на сортировку
	start = clock();
	qusort(a, left, right, thread_on, p);   //Вызов сортировки
	stop = clock();
	cheksort(a, a_size);
	cout << "Run time: " << stop - start << endl;

	thread_on = true;   //Многопоточный режим
	cout << "Thread on" << endl;
	far(a, left, right);
	cheksort(a, a_size);
	start = clock();
	qusort(a, left, right, thread_on, p);
	f.wait();
	stop = clock();
	cheksort(a, a_size);
	cout << "Run time: " << stop - start << endl;

	delete[]a;
	return 0;
}