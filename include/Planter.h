#pragma once

#include "MyInclude.h"

class Planter {
	public:
		const vector<double>& run(const vector<ID>& targets);
	private:
		void grow(vector<ID>& targets, size_t depth);
		inline bool checkLeafCondition(vector<ID>& targets, size_t depth);
		void makeLeaf(vector<ID>& targets, size_t depth);
};
