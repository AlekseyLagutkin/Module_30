#include "qusort.h"

shared_ptr<int> p1 = make_shared<int>(10);   //Счетчик рекурсивных функций

void qusort(int* a, int left, int right, bool thread_on, promise<void>& p)
{
	if (left >= right)
	{
		return;
	}
	shared_ptr<int> p2 = p1;
	int l = left;
	int r = right;
	static bool f = false;
	static RequestHandler req;
	int m = (l + r) / 2;
	int am = a[m];
	while (l < m)
	{
		if (a[l] >= am)
		{
			swap(a[l], a[m - 1]);
			swap(a[m - 1], a[m]);
			--m;
		}
		if (a[l] < am)
		{
			++l;
		}
	}
	while (r > m)
	{
		if (a[r] < am)
		{
			swap(a[r], a[m + 1]);
			swap(a[m + 1], a[m]);
			++m;
		}
		if (a[r] >= am)
		{
			--r;
		}
	}
	if (thread_on)
	{
		if (right - left > 10000)
		{
			req.pushRequest(qusort, a, left, m - 1, thread_on, p);
		}
		else
		{
			qusort(a, left, m - 1, thread_on, p);
		}
		qusort(a, m + 1, right, thread_on, p);
		if ((p1.use_count() == 2) && (!f))   //Проверка на закрытие рекурсивных функций
		{
			f = true;
			p.set_value();
		}
	}
	else
	{
		qusort(a, left, m - 1, thread_on, p);
		qusort(a, m + 1, right, thread_on, p);
	}
}

void far(int* a, int left, int size)
{
	for (int i = left; i < size; ++i)
	{
		a[i] = rand() % 100;
	}
}

void cheksort(int* a, int n)
{
	for (int i = 1; i < n; ++i)
	{
		if (a[i] < a[i - 1])
		{
			cout << "Not sorted" << endl;
			return;
		}
	}
	cout << "Sorted" << endl;
}

