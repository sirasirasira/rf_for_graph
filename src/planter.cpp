#include "Planter.h"
#define CLASS Planter

#include "Database.h"
#include "Calculator.h"

extern Setting setting;
extern Database db;

// each value of additive_ys must be overwritten only once
const vector<double>& CLASS::run(const vector<ID>& train_targets, const vector<ID>& test_targets) {
	// std::cout << "debug Planter run" << std::endl; // debug
	cout << "BEGIN_tree" << endl;
	db.spliter.prepare(train_targets);
	grow(train_targets, test_targets, 0);
	cout << "END_tree" << endl;
	return additive_ys;
}

void CLASS::grow(const vector<ID>& train_targets, const vector<ID>& test_targets, size_t depth) {
	// std::cout << "debug grow, depth " << depth << endl; // debug
	// Debug::IDs(targets, "targets "); // debug
	if (checkLeafConditions(train_targets, depth)) {
		makeLeaf(train_targets, test_targets, depth);
		return;
	}
	vector<ID> posi_train_targets = db.spliter.run(train_targets);
	if (db.spliter.valid() == false) {
		makeLeaf(train_targets, test_targets, depth);
		return;
	}
	const auto best_pattern = db.spliter.getBestPattern();
	db.gspan.updataFeatureImportance(best_pattern, db.spliter.getImportance());
	vector<ID> posi_test_targets = checkIsomorphism(best_pattern, test_targets);
	grow(posi_train_targets, posi_test_targets, depth + 1);
	cout << string(depth, '-') << "* " << best_pattern << endl;
	vector<ID> nega_train_targets = Calculator::setDiff(train_targets, posi_train_targets);
	vector<ID> nega_test_targets = Calculator::setDiff(test_targets, posi_test_targets);
	grow(nega_train_targets, nega_test_targets, depth + 1);
}

vector<ID> CLASS::checkIsomorphism(const Pattern& best_pattern, const vector<ID>& test_targets) {
//TODO
}

bool CLASS::checkLeafConditions(const vector<ID>& train_targets, size_t depth) {
	// if (train_targets.size() <= db.gspan.minsup * 2) return true;
	if (train_targets.size() < 2) return true;
	if (depth >= setting.max_depth) return true;
	return false;
}

// @change additive_ys
void CLASS::makeLeaf(const vector<ID>& train_targets, const vector<ID>& test_targets, size_t depth) {
	// std::cout << "debug makeLeaf" << std::endl; // debug
	// Debug::IDs(targets); // debug
	double sum = 0;
	size_t count = 0;
	for (ID id : train_targets) {
		count++;
		sum += db.ys[id];
	}
	double mean = sum / (double) count;
	// classification
	if (mean >= 0) {
		updateAdditiveYs(train_targets, test_targets, 1);
	} else {
		updateAdditiveYs(train_targets, test_targets, -1);
	}

	cout << string(depth, '-') << "* " << "[" << count << "] " << mean << endl;
}

void CLASS::updateAdditiveYs(const vector<ID>& train_targets, const vector<ID>& test_targets, double value) {
	for (auto id : train_targets) {
		additive_ys[id] = value;
	}
	for (auto id : test_targets) {
		additive_ys[id] = value;
	}
}
