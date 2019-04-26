#pragma once

#include "MyInclude.h"

class RandomForest {
	public:
		void run();

		void incGainCount() {
			gain_count++;
		}

		void incBoundCount() {
			bound_count++;
		}

		const vector<ID>& getTrainTargets() {
			return train_targets;
		}

		const vector<ID>& getTestTargets() {
			return test_targets;
		}

	private:
		vector<ID> train_targets;
		vector<ID> test_targets;
		size_t tree_count;
		int gain_count = 0;
		int bound_count = 0;
		int sum_gcount = 0;
		int sum_bcount = 0;
		int sum_csize = 0;

		void makeTargets();
		void plantFirst();
		void calcImprity();
		void plant();
		inline void report(const vector<double>& additive_ys);
		inline void report();
		inline void reportFeatureImportance();
};
