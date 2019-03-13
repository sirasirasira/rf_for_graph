#pragma once

#include "MyInclude.h"

class TreeEnsemble {
	public:
		void run();

		void incGainCount() {
			gain_count++;
		}

		void incBoundCount() {
			bound_count++;
		}

		void printGainCount() {
			cout << "gain_count " << gain_count << endl;
		}

		void printBoundCount() {
			cout << "bound_count " << bound_count << endl;
		}

		const vector<ID>& getTargets() {
			return targets;
		}

	private:
		vector<ID> targets;
		size_t tree_count;
		int gain_count = 0;
		int bound_count = 0;

		void makeTargets();
		void plantFirst();
		void calcImprity();
		void plant();
		inline void report();
		inline void reportFeatureImportance();
};
