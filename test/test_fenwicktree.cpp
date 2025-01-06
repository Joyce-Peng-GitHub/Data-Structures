#include <bits/stdc++.h>

#include "../ds/bigint.hpp"
#include "../ds/fenwicktree.hpp"

void test_bigint() {
	ds::bigint a(1 << 16), b(1 << 16);
	for (int i = 0; i < 100; ++i) {
		a <<= 1;
		testOut(std::cout, a) << std::endl;
	}
	for (int i = 0; i < 120; ++i) {
		a >>= 1;
		testOut(std::cout, a) << std::endl;
	}
}

void test_fenwicktree() {
	ds::fenwicktree<int64_t> *p;
	std::vector<int64_t> *q;
	std::string cmd, op;
	int64_t param[2];
	while (std::getline(std::cin, cmd)) {
		size_t cnt = 0;
		std::stringstream ss(cmd);
		ss >> op;
		if (ss >> param[0]) {
			cnt = 1;
			if (ss >> param[1]) {
				cnt = 2;
			}
		}
		if (op == "new") {
			if (cnt == 0) {
				p = new ds::fenwicktree<int64_t>();
				q = new std::vector<int64_t>();
			} else if (cnt == 1) {
				p = new ds::fenwicktree<int64_t>(param[0]);
				q = new std::vector<int64_t>(param[0]);
			} else {
				p = new ds::fenwicktree<int64_t>(param[0], param[1]);
				q = new std::vector<int64_t>(param[0], param[1]);
			}
		} else if (op == "tree_size") {
			std::cout << p->tree_size() << std::endl;
		} else if (op == "size") {
			std::cout << p->size() << std::endl
					  << q->size() << std::endl;
		} else if (op == "assign") {
			if (cnt == 0) {
				p->assign();
				q->assign(0, 0);
			} else if (cnt == 1) {
				p->assign(param[0]);
				q->assign(param[0], 0);
			} else {
				p->assign(param[0], param[1]);
				q->assign(param[0], param[1]);
			}
		} else if (op == "modify") {
			p->modify(param[0], param[1]);
			q->at(param[0]) += param[1];
		} else if (op == "query") {
			if (cnt == 0) {
				std::cout << p->query() << std::endl
						  << std::accumulate(q->begin(), q->end(), 0) << std::endl;
			} else {
				std::cout << p->query(param[0]) << std::endl
						  << std::accumulate(q->begin(), q->begin() + param[0], 0) << std::endl;
			}
		} else if (op == "delete") {
			delete p;
			delete q;
		} else {
			break;
		}
	}
}

int main() {
	test_bigint();
	// test_fenwicktree();
	return 0;
}