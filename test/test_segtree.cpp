#include "segtree.hpp"
#include <bits/stdc++.h>

int main() {
	std::vector<int64_t> arr;
	{
		size_t n;
		std::cin >> n;
		arr.resize(n);
		for (auto &x : arr) {
			std::cin >> x;
		}
	}
	ds::lazy_sum_segtree<int64_t, 0, std::plus<int64_t>, std::minus<int64_t>, std::multiplies<int64_t>> st(arr.begin(), arr.end());
	std::string op;
	while (std::cin >> op) {
		if (op == "origin") {
			std::cout << '[';
			if (arr.size()) {
				std::cout << arr[0];
				for (size_t i = 1; i != arr.size(); ++i) {
					std::cout << ", " << arr[i];
				}
			}
			std::cout << "]" << std::endl;
		} else if (op == "size") {
			std::cout << st.size() << std::endl;
		} else if (op == "treesize") {
			std::cout << st.treesize() << std::endl;
		} else if (op == "lazysize") {
			std::cout << st.lazysize() << std::endl;
		} else if (op == "query") {
			size_t pos, n;
			std::cin >> pos >> n;
			std::cout << st.query(pos, n) << std::endl;
		} else if (op == "modify") {
			size_t pos, val;
			std::cin >> pos >> val;
			arr.at(pos) = val;
			st.modify(pos, val);
		} else if (op == "add") {
			size_t pos, n;
			int64_t diff;
			std::cin >> pos >> n >> diff;
			for (size_t i = pos; i != n && i != arr.size(); ++i) {
				arr.at(i) += diff;
			}
			st.add(pos, n, diff);
		} else if (op == "subtract") {
			size_t pos, n;
			int64_t diff;
			std::cin >> pos >> n >> diff;
			for (size_t i = pos; i != n && i != arr.size(); ++i) {
				arr.at(i) -= diff;
			}
			st.subtract(pos, n, diff);
		} else if (op == "clear") {
			arr.clear();
			st.clear();
		} else if (op == "build") {
			std::cout << "Overloads: \n"
					  << "\t1. build(_n);\n"
					  << "\t2. build(_n, _val);\n"
					  << "\t3. build(_begin, _end);\n";
			int oop;
			size_t n;
			std::cin >> oop >> n;
			if (oop == 1) {
				arr = std::vector<int64_t>(n);
				st.build(n);
			} else if (oop == 2) {
				int64_t val;
				std::cin >> val;
				arr = std::vector<int64_t>(n, val);
				st.build(n, val);
			} else if (oop == 3) {
				arr.resize(n);
				for (auto &x : arr) {
					std::cin >> x;
				}
				st.build(arr.begin(), arr.end());
			}
		}
	}
	return 0;
}