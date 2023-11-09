#pragma once
#include <iostream>
#include <future>
#include "threadpool.h"

using namespace std;

void qusort(int* a, int left, int right, bool thread_on, promise<void>& p);   //Сортировка
void far(int* a, int left, int size);   //Заполнение массива
void cheksort(int* a, int n);   //Проверка сортировки