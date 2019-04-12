#pragma once

#include "MyInclude.h"
#include <random>

static std::random_device seed_gen;
static std::mt19937 mt(seed_gen());

struct Dice {
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
};
