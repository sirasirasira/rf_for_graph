#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include "Database.h"

extern Setting setting;
extern Database db;

namespace Calculator {
	inline vector<ID> setDiff(const vector<ID>& a, const vector<ID>& b) {
		vector<ID> vec;
		std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(vec));
		return vec;
	}

	inline vector<ID> setIntersec(const vector<ID>& a, const vector<ID>& b) {
		vector<ID> vec;
		std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(vec));
		return vec;
	}

	inline double imp(const vector<double>& ys, const vector<ID>& train_targets) {
		// gini
		double impurity;
		int num = train_targets.size();
		int n_posi = 0;
		int n_nega = 0;
		for (ID id : train_targets) {
			if (ys[id] >= 0) {
				n_posi++;
			} else {
				n_nega++;
			}
		}
		impurity = 1 - (pow(n_posi/num, 2) + pow(n_nega/num, 2));
		return impurity;
	}

	// assert raw is sorted
	inline vector<ID> trainOnly(const vector<ID>& raw) {
		vector<ID> only;
		for (ID id : raw) {
			if (id > db.gdata.getLastTrainID()) {
				break;
			}
			only.push_back(id);
		}
		return only;
	}

	inline vector<ID> concat(vector<ID> a, vector<ID> b) {
		a.insert(a.end(), b.begin(), b.end());
		return a;
	}

	inline double score(const vector<double>& ys, const vector<ID>& raw_targets, const vector<ID>& raw_posi) {
		db.random_forest.incGainCount();
		vector<ID> targets = trainOnly(raw_targets);
		vector<ID> posi = setIntersec(targets, raw_posi);
		vector<ID> nega = setDiff(targets, posi);
		return posi.size() * imp(ys, posi) + nega.size() * imp(ys, nega);
	}

	inline double bound(const vector<double>& ys, const vector<ID>& raw_targets, const vector<ID>& raw_posi) {
		db.random_forest.incBoundCount();
		vector<ID> targets = trainOnly(raw_targets);
		vector<ID> posi = setIntersec(targets, raw_posi);
		vector<ID> nega = setDiff(targets, posi);
		vector<ID> posi_plus;
		vector<ID> posi_minus;
		for (ID id : posi) {
			if (ys[id] >= 0) {
				posi_plus.push_back(id);
			} else {
				posi_minus.push_back(id);
			}
		}
		double res1 = (posi_minus.size() + nega.size()) * imp(ys, concat(nega, posi_minus));
		double res2 = (posi_plus.size() + nega.size()) * imp(ys, concat(nega, posi_plus));
		return std::min(res1, res2);
	}
}
