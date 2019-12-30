#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <string>
#include <algorithm>

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
			for (size_t w = 1; w <= _cap; ++w) {
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

int main()
{
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