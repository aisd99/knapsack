#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <string>
#include <algorithm>

#include "test_runner.h"

class Knapsack {
public:
	size_t GetWeight() const {
		return optimised_weight;
	}

	size_t GetCost() const {
		return optimised_cost;
	}

	set<size_t> GetItems() const {
		return picked_items;
	}

	Knapsack(const std::vector<int>& _weights, const std::vector<int>& _costs, size_t _cap)
	{
		size_t items_number = _weights.size();
		static const size_t num = items_number + 1;
		size_t** sums_table = new size_t*[_cap + 1];
		for (size_t i = 0; i <= _cap; ++i) {
			sums_table[i] = new size_t[items_number + 1];
			for (size_t j = 0; j <= items_number; ++j) {
				sums_table[i][j] = 0;
			}
		}
		for (size_t q = 1; q <= items_number; ++q) {
			for (int w = 1; w <= _cap; ++w) {
				if (_weights[q - 1] <= w) {
					sums_table[w][q] =
					std::max(sums_table[w - _weights[q - 1]][q - 1] + _costs[q - 1], 
					sums_table[w][q - 1]);
				}
				else {
					sums_table[w][q] = sums_table[w][q - 1];
				}
			}
		}
		optimised_cost = sums_table[_cap][items_number];
		while (items_number != 0) {
			if (sums_table[_cap][items_number] != sums_table[_cap][items_number - 1]) {
				picked_items.insert(items_number);
				optimised_weight += _weights[items_number - 1];
				_cap -= _weights[items_number - 1];
			}
			--items_number;
		}
		for (size_t i = 0; i <= _cap; ++i) {
			delete[] sums_table[i];
		}
		delete[] sums_table;
	}
private:
	size_t optimised_weight = 0;
	size_t optimised_cost = 0;
	set<size_t> picked_items;
};

Knapsack RunOptimisation(istream& is) {
	vector<int> weights;
	vector<int> costs;
	int capacity = 0;
	is >> capacity;
	int first, second;
	while (is >> first >> second) {
		weights.push_back(first);
		costs.push_back(second);
	}
	return Knapsack(weights, costs, capacity);
}

void Test1() {
	const string input = R"(165
		23 92
		31 57
		29 49
		44 68
		53 60
		38 43
		63 67
		85 84
		89 87
		82 72)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 309);
	ASSERT_EQUAL(sack.GetWeight(), 165);
	set<size_t> expected_items = { 1, 2, 3, 4, 6 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test2() {
	const string input = R"(26
	12 24
	7 13
	11 23
	8 15
	9 16)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 51);
	ASSERT_EQUAL(sack.GetWeight(), 26);
	set<size_t> expected_items = { 2, 3, 4 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test3() {
	const string input = R"(190
	56 50
	59 50
	80 64
	64 46
	75 50
	17 5)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 150);
	ASSERT_EQUAL(sack.GetWeight(), 190);
	set<size_t> expected_items = { 1, 2, 5 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test4() {
	const string input = R"(750000000
	70000000 135
	73000000 139
	77000000 149
	80000000 150
	82000000 156
	87000000 163
	90000000 173
	94000000 184
	98000000 192
	106000000 201
	110000000 210
	113000000 214
	115000000 221
	118000000 229
	120000000 240)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 1458);
	ASSERT_EQUAL(sack.GetWeight(), 749000000);
	set<size_t> expected_items = { 1, 3, 5, 7, 8, 9, 14, 15 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test5() {
	const string input = R"(165
	23 92
	31 57
	29 49
	44 68
	53 60
	38 43
	0 10
	63 67
	85 84
	89 87
	0 20
	82 72)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 339);
	ASSERT_EQUAL(sack.GetWeight(), 165);
	set<size_t> expected_items = { 1, 2, 3, 4, 6, 7, 11 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test6() {
	const string input = R"(22
	23 92
	31 57
	29 49
	44 68
	53 60
	38 43
	63 67
	85 84
	89 87
	82 72)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 0);
	ASSERT_EQUAL(sack.GetWeight(), 0);
	set<size_t> expected_items = {};
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

void Test7() {
	const string input = R"(460870
	111151 442
	135550 525
	132839 511
	159949 593
	149105 546
	154527 564
	162660 617)";
	istringstream is(input);
	Knapsack sack = RunOptimisation(is);
	ASSERT_EQUAL(sack.GetCost(), 1735);
	ASSERT_EQUAL(sack.GetWeight(), 458159);
	set<size_t> expected_items = { 2, 4, 7 };
	ASSERT_EQUAL(sack.GetItems(), expected_items);
}

int main()
{
	TestRunner tr;
	RUN_TEST(tr, Test1);
	RUN_TEST(tr, Test2);
	RUN_TEST(tr, Test3);
	RUN_TEST(tr, Test4);
	RUN_TEST(tr, Test5);
	RUN_TEST(tr, Test6);
	RUN_TEST(tr, Test7);

	vector<int> weights;
	vector<int> costs;
	int capacity = 0;
	cin >> capacity;
	int first, second;
	while (cin >> first >> second) {
		weights.push_back(first);
		costs.push_back(second);
	}

	Knapsack sack(weights, costs, capacity);
	cout << sack.GetWeight() << " " << sack.GetCost() << endl;
	for (auto i : sack.GetItems()) {
		cout << i << endl;
	}
	return 0;
}