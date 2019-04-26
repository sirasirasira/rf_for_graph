#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"

class Planter {
	public:
		const vector<double>& run(const vector<ID>& train_targets, const vector<ID>& test_targets);

		void resizeAdditiveYs(size_t s) {
			additive_ys.resize(s);
		}

	private:
		vector<double> additive_ys;
		void grow(const vector<ID>& train_targets, const vector<ID>& test_targets, size_t depth);
		vector<ID> checkIsomorphism(const Pattern& best_pattern, const vector<ID>& test_targets);
		inline bool checkLeafConditions(const vector<ID>& train_targets, size_t depth);
		void makeLeaf(const vector<ID>& train_targets, const vector<ID>& test_targets, size_t depth);
		inline void updateAdditiveYs(const vector<ID>& train_targets, const vector<ID>& test_targets, double mean);
};
