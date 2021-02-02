#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

namespace task21
{
	typedef tuple<bool, int> Data;

	enum Events { AND, OR };
	const int WinScore = 34, MaxInputs = 33;
	bool checker = false;

	Data Task(int x, int level)
	{
		auto F = [&](Events E)
		{
			if (x < WinScore)
			{
				Data a, b, c, d;
				a = Task(x + 1, level + 1);
				b = Task(x + 2, level + 1);
				c = Task(x + 3, level + 1);
				d = Task(x * 2, level + 1);
				int max1 = (get<1>(a) > get<1>(b)) ? get<1>(a) : get<1>(b);
				int max2 = (get<1>(c) > get<1>(d)) ? get<1>(c) : get<1>(d);
				int max = (max1 > max2) ? max1 : max2;

				if (E == Events::AND && (get<0>(a) && get<0>(b)
					&& get<0>(c) && get<0>(d))) return Data{ true, max };

				else if (E == Events::OR && (get<0>(a) || get<0>(b)
					|| get<0>(c) || get<0>(d))) return Data{ true, max };

				else return Data{ false, level };
			}
			else
			{
				if (level == 2) return Data{ true, level };
				else return Data{ false, level };
			}
		};

		Data result;
		switch (level)
		{
		case 0: result = F(Events::AND);break;
		case 1: result = F(Events::OR);break;
		case 2: result = F(Events::AND);break;
		case 3: result = F(Events::OR);break;
		default:
			if (x >= WinScore) result = Data{ true, level };
			else result = Data{ false, level };
		}

		return result;
	}

	int run()
	{

		for (int i = 1; i <= MaxInputs; i++)
		{
			Data value = Task(i, 0);
			if (get<0>(value) && (get<1>(value) == 4)) cout << i << endl;
		}

		return 0;
	}

}

namespace task23
{
	struct Task23_1
	{
	private:
		int Begin, End;
	public:
		Task23_1(int Begin, int End)
		{
			this->Begin = Begin;
			this->End = End;
		}
		int func(int i)
		{
			if (i == Begin) return 1;
			else if (i < Begin) return 0;
			else
			{
				if (i % 3 == 0) return func(i / 3) + func(i - 1) + func(i - 2);
				else return func(i - 1) + func(i - 2);
			}
		}
		int run()
		{
			return func(End);
		}
	};

	int run()
	{
		// 2 -> 15
		// проходит через 4, 11
		cout << Task23_1(2, 4).run() * Task23_1(4, 11).run()
			* Task23_1(11, 15).run() << endl;

		return 0;
	}

	//Сколько существует программ минимальной длины, 
	//в результате выполнения которых при исходном числе 1 
	//результатом является число 28?

	int begin = 1;
	int end = 28;

	struct Task23_2
	{
	private: vector<int> arr;
	public:
		void func(int i, int step)
		{   
			if(i == end)
			{
				arr.push_back(step);
				return;
			}
			else if(i > end) return;
			else 
			{
				func(i + 1, step + 1);
				func(i + 2, step + 1);
				func(i * 2, step + 1);
				
				return;
			}
		}
		int get()
		{
			func(begin, 0);
			sort(arr.begin(), arr.end());

			int counter = 0;
			for(int i : arr)
			{
				if(i == arr[0]) counter++;
				else break;
			}
			return counter;
		}
	};

}

namespace task24
{
	void GetTextFromFile(string* text, const char* filename)
	{
		ifstream file(filename);
		string str;

		while (getline(file, str)) *text += str;
		file.close();
	}


	int RunProgram()
	{
		string text;
		GetTextFromFile(&text, "24_demo.txt");

		int counter = 0, max = 0;
		bool trigger = false;

		for (int i = 0; i < text.length(); i++)
		{

			if (text[i] == 'X' && !trigger) trigger = true;

			if (((text[i] == 'X' && counter % 3 == 0) ||
				(text[i] == 'Y' && counter % 3 == 1) ||
				(text[i] == 'Z' && counter % 3 == 2)) && trigger)
			{
				counter++;
			}
			else
			{
				trigger = false;

				if (max < counter) max = counter;
				counter = 0;
			}

		}

		if (counter > 0 && !max) max = counter;

		cout << max << endl;

		return 0;

	}
}

namespace task26
{
	void GetFileData(vector<int>* arr, tuple<int, int>* T, string filename)
	{
		ifstream file(filename);
		file >> get<0>(*T) >> get<1>(*T);

		for (int i = 0; i < get<1>(*T); i++)
		{
			int buffer; file >> buffer;
			arr->push_back(buffer);
		}

		file.close();
		sort(arr->begin(), arr->end());

	}

	int RunProgram()
	{
		int MaxUsersAmount = 0, MaxDataWeight = 0;

		tuple<int, int> T;
		vector<int> arr;

		GetFileData(&arr, &T, "file.txt");

		int sum = 0, lastindex;
		for (int i = 0; i < get<1>(T); i++)
		{
			if (sum + arr[i] < get<0>(T))
			{
				lastindex = i;
				MaxUsersAmount++;
				sum += arr[i];

			}
		}

		for (int i = get<1>(T) - 1; i >= 0; i--)
		{
			if ((sum - arr[lastindex] + arr[i]) < get<0>(T))
			{
				MaxDataWeight = arr[i];
				break;
			}
		}

		cout << MaxUsersAmount << " " << MaxDataWeight << endl;

		return 0;
	}
}

namespace task27
{
	typedef vector<tuple<int, int>> array_of_tuple;

	void GetTextFromFile(array_of_tuple* arr, int* n, string filename)
	{
		ifstream file(filename);
		file >> *n;
		arr->clear();

		for (int i = 0; i < *n; i++)
		{
			string str1, str2;
			file >> str1 >> str2;

			arr->push_back(make_tuple(stoi(str1), stoi(str2)));
		}

		file.close();
	}

	int GetSummaryCounter(array_of_tuple* arr, int n)
	{
		int sum = 0;
		for (int i = 0; i < n; i++)
		{
			if (get<0>((*arr)[i]) > get<1>((*arr)[i]))
			{
				if (sum + get<0>((*arr)[i]) % 3 != 0)
					sum += get<0>((*arr)[i]);
				else sum += get<1>((*arr)[i]);
			}
			else
			{
				if (sum + get<1>((*arr)[i]) % 3 != 0)
					sum += get<1>((*arr)[i]);
				else sum += get<0>((*arr)[i]);
			}
		}
		return sum;
	}

	void RunProgram()
	{
		int n, sum1, sum2;
		array_of_tuple arr;

		auto GetSummary = [&]()
			// copy GetSummaryCounter function as lambda 
		{
			int sum = 0;
			for (int i = 0; i < n; i++)
			{
				tuple<int, int> T = arr[i];
				if (get<0>(T) > get<1>(T))
				{
					if (sum + get<0>(T) % 3 != 0) sum += get<0>(T);
					else sum += get<1>(T);
				}
				else
				{
					if (sum + get<1>(T) % 3 != 0) sum += get<1>(T);
					else sum += get<0>(T);
				}
			}
			return sum;
		};

		GetTextFromFile(&arr, &n, "fileA.txt");
		sum1 = GetSummary();

		GetTextFromFile(&arr, &n, "fileB.txt");
		sum2 = GetSummary();

		cout << sum1 << " " << sum2 << endl;
	}
}