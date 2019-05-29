#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include "Database.h"

extern Setting setting;
extern Database db;

namespace Calculator {
	inline bool isSameSign(double a, double b) {
		if (a * b > 0) {
			return true;
		} else {
			return false;
		}
	}

	inline double calcDeviation(double ans, double pred) {
		using namespace std;
		return log(1 + exp(-2 * ans * pred));
	}

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
		//cout << "calc impurity" << endl;
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
		// gini
		//impurity = 1 - (pow(double(n_posi)/num, 2) + pow(double(n_nega)/num, 2));
		// entropy
		impurity = 1 - ((double(n_posi)/num) * log2(double(n_posi)/num) + (double(n_nega)/num) * log2(double(n_nega)/num));
		//cout << impurity << endl;
		return impurity;
	}

	inline vector<ID> concat(vector<ID>& a, vector<ID>& b) {
		vector<ID> ab;
		ab.reserve(a.size() + b.size()); // preallocate memory
		ab.insert(ab.end(), a.begin(), a.end());
		ab.insert(ab.end(), b.begin(), b.end());
		return ab;
	}

	inline double score(const vector<double>& ys, const vector<ID>& targets, const vector<ID>& raw_posi) {
		//cout << "calc score" << endl;
		db.random_forest.incGainCount();
		vector<ID> posi = setIntersec(targets, raw_posi);
		vector<ID> nega = setDiff(targets, posi);
		//cout << (posi.size() * imp(ys, posi) + nega.size() * imp(ys, nega)) / (posi.size() + nega.size()) << endl;
		return (posi.size() * imp(ys, posi) + nega.size() * imp(ys, nega)) / (posi.size() + nega.size());
	}

	inline double bound(const vector<double>& ys, const vector<ID>& targets, const vector<ID>& raw_posi) {
		//cout << "calc bound" << endl;
		db.random_forest.incBoundCount();
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
		double res1 = ((posi_minus.size() + nega.size()) * imp(ys, concat(nega, posi_minus))) / (posi.size() + nega.size());
		double res2 = ((posi_plus.size() + nega.size()) * imp(ys, concat(nega, posi_plus))) / (posi.size() + nega.size());
		//cout << "res1: " << res1 << " res2: " << res2 << " min: " << std::min(res1, res2) << endl;
		return std::min(res1, res2);
	}
}
