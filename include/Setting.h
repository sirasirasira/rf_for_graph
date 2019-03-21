#pragma once

#include "IncludeLib.h"

struct Setting {
	double minsup = 1; // 1~: support size, 0~1: support rate
	size_t maxpat = -1; // max graph size
	size_t num_of_trees = 500;
	size_t max_depth = -1;
	size_t min_samples_split = 2;
	double min_impurity_decrease = 0.0;
	double data_used = 0.36;
	size_t search_paths = 100;

	void print() {
		cout 
			<< " minsup " << minsup
			<< " maxpat " << maxpat
			<< " num_of_trees " << num_of_trees
			<< " max_depth " << max_depth
			<< " min_samples_split " << min_samples_split
			<< " min_impurity_decrease " << min_impurity_decrease
			<< " data_used " << data_used
			<< " search_paths " << search_paths
			<< endl;
	}
};
