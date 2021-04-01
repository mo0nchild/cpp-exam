#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

namespace task18
{

const string DIRNAME = "C:\\Users\\Byter\\Desktop\\CPPLang\\src\\";
struct FIELD
{
	vector<vector<int>> list;
	int max = 0, min = 0;
	vector<int> arr;

	void func(int x, int y, int sum)
	{
		if(list[y][x] < 500 && list[y][x] > 100) return;

		if(x == 11 && y == 11)
		{
			sum += list[y][x];
			arr.push_back(sum);
		}
		else
		{
			if(x < 11 && y < 11)
			{
				func(x + 1, y, sum + list[y][x]);
				func(x, y + 1, sum + list[y][x]);

			}
			else if(x < 11)func(x + 1, y, sum + list[y][x]);
			else if(y < 11)func(x, y + 1, sum + list[y][x]);
		}
		
	}

	FIELD(vector<vector<int>> list)
	{
		this->list = list;
		func(0,0,0);

		this->max = *max_element(arr.begin(), arr.end());
		this->min = *min_element(arr.begin(), arr.end());
	}

};

//____________ПЕРЕДВИЖЕНИЕ НА ЛЮБОЕ КОЛ-ВО КЛЕТОК____________________

//координаты стен : описывается как координаты клеток перед стенкой и после
typedef pair<pair<int, int>, pair<int, int>> Data;
vector<Data> walls = {
	make_pair(make_pair(6,4),make_pair(6,5)),
	make_pair(make_pair(7,4),make_pair(7,5)),
	make_pair(make_pair(8,4),make_pair(8,5)),
	make_pair(make_pair(9,4),make_pair(9,5)),
	make_pair(make_pair(10,4),make_pair(10,5)),
	make_pair(make_pair(11,4),make_pair(11,5)),
	make_pair(make_pair(12,4),make_pair(12,5)),

	make_pair(make_pair(12,5),make_pair(13,5)),
	make_pair(make_pair(12,6),make_pair(13,6)),
	make_pair(make_pair(12,7),make_pair(13,7)),
	make_pair(make_pair(12,8),make_pair(13,8)),
	make_pair(make_pair(12,9),make_pair(13,9)),
	make_pair(make_pair(12,10),make_pair(13,10)),
	make_pair(make_pair(12,11),make_pair(13,11)),

	
};

const int FIELDSIZE = 16;
struct FIELD_LOOP
{
	vector<vector<int>> list;
	vector<int> arr;
	map<pair<int,int>, pair<int,int>> picked;
	int max, min;

	pair<int,int> func(int x, int y, pair<int,int> past)
	{
		if(x == 0 && y == 0) return make_pair(list[y][x], list[y][x]);

		for(Data i : walls)
		{
			if((i.second == past && i.first == make_pair(x,y))||
				(i.first == past && i.second == make_pair(x,y)))
				return make_pair(0,0);
		}

		if(picked.find(make_pair(x,y)) != picked.end())
		{
			return picked.find(make_pair(x,y))->second;
		}
		else
		{
			vector<int> sums;
			pair<int,int> F;
			for(int _x = 0; _x < x; _x++)
			{
				F = func(_x,y, make_pair(x,y));
				if(F != pair<int,int>(0,0))
				{
					sums.push_back(F.first);
					sums.push_back(F.second);
				}
			}
			for(int _y = 0; _y < y; _y++)
			{
				F = func(x,_y, make_pair(x,y));
				if(F != pair<int,int>(0,0))
				{
					sums.push_back(F.first);
					sums.push_back(F.second);
				}
			}

			pair<int,int> result = make_pair(
				*max_element(sums.begin(), sums.end()) + list[y][x],
				*min_element(sums.begin(), sums.end()) + list[y][x]);
			picked.insert(make_pair(make_pair(x,y), result));

			return result;
		}
	}

	FIELD_LOOP(vector<vector<int>> list)
	{
		this->list = list;
		pair<int,int> result = func(FIELDSIZE - 1, FIELDSIZE - 1, make_pair(0,0));
		max = result.first;
		min = result.second;
	}
};

void run()
{
    ifstream file(DIRNAME + "file.txt");
	vector<vector<int>> arr;

	for(int y = 0; y < 12; y++)
	{
		arr.push_back(vector<int>{});
		for(int x = 0; x < 12; x++)
		{
			int buffer;
			file >> buffer;
			arr[y].push_back(buffer);
		}
	}

	FIELD F = FIELD(arr);
	cout << F.max << " " << F.min << endl;
}
}

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

	typedef vector<pair<int, int>> Data;

	Data getData(string filename) 
	{
		ifstream file(filename);
		Data arr;

		int items;
		file >> items;

		for (int i = 0; i < items; i++) 
		{
			pair<int, int> buffer;
			file >> buffer.first >> buffer.second;
			arr.push_back(buffer);
		}

		return arr;
	}

	struct AllSums
	{
	public: vector<int> sums;
	private:
		vector<int> nums;
		void get_all_sums(int index, int sum)
		{
			sum += nums[index];
			sums.push_back(sum);

			for (int i = index + 1; i < nums.size(); i++)
				get_all_sums(i, sum);
		}
	public:
		AllSums(vector<int> nums)
		{
			this->nums = nums;
			get_all_sums(0, 0);
		}
	};

	int getResult(Data list) 
	{
		vector<int> arr, delta;
		int sum = 0;

		for (Data::iterator i = list.begin(); i != list.end(); i++)
		{
			arr.push_back(max(i->first, i->second));
			if (abs(i->first - i->second))delta.push_back(abs(i->first - i->second));
			sum += max(i->first, i->second);
		}

		if (sum % 5 == 0) return sum;
		sort(delta.begin(), delta.end());

		for (int interval = 1; interval < arr.size(); interval++) 
		{
			vector<int> buffer = delta;
			buffer.erase(buffer.begin() + interval, buffer.end());

			vector<int> allsums = AllSums(buffer).sums;
			sort(allsums.begin(), allsums.end());

			for (int i = 0; i < allsums.size(); i++)
			{
				if ((sum - allsums[i]) % 5 == 0) return (sum - allsums[i]);
			}
		}
		
		return 0;
	}

	void Run()
	{
		cout << getResult(getData("data/27/27-4a.txt")) << endl;
		cout << getResult(getData("data/27/27-4b.txt")) << endl;
	}

	//________________________________________________________

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