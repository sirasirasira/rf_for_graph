#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"

class Spliter {
	public:
		void prepare(const vector<ID>& targets);
		vector<ID> run(const vector<ID>& targets);
		void update(Pattern pattern, vector<ID> posi);
		inline bool valid() {
			return valid_flg;
		}
		inline const Pattern& getBestPattern() {
			assert(valid_flg);
			return best_pattern;
		}
		inline double getImportance() {
			assert(valid_flg);
			return parent_score - min_score;
		}
		inline vector<ID> getTargets() {
			return  targets;
		}

	private:
		bool valid_flg;
		vector<ID> targets;
		double parent_score;
		double min_score;
		Pattern best_pattern;
		priority_queue<std::pair<double, Pattern>, vector<std::pair<double, Pattern>>, std::greater<std::pair<double, Pattern>>> queue;

		void initMinScore();
		void searchCache();
};
