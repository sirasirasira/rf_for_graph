#pragma once

#include "MyInclude.h"
#include <random>

struct Dice {
	static std::random_device seed_gen;
	static std::mt19937 mt;

	static vector<ID> random_sample(int num, double data_used) {
		int sample_num = num * data_used;

		std::vector<ID> vec(num);
		for (int i = 0; i < num; i++) {
			vec[i] = i;
		}

		vector<ID> res(sample_num);
		for (int i = 0; i < sample_num; i++) {
			int pos = mt() % (num-i);
			res[i] = vec[i + pos];
			std::swap(vec[i], vec[i + pos]);
		}
		std::sort(res.begin(), res.end());

		return res;
	}

	static vector<int> make_count(unsigned int size, int pnum) {
		vector<int> count(size, 0);
		std::uniform_int_distribution<int> rdm(0, size-1);
		for (int i = 0; i < pnum; i++) {
			count[rdm(mt)]++;
		}
		return count;
	}
};
