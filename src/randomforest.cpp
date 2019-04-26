#include "RandomForest.h"
#define CLASS RandomForest

#include "Database.h"
#include "Calculator.h" 

extern Setting setting;
extern Database db;

void CLASS::run() {
	// std::cout << "debug run" << std::endl; // debug
	for (tree_count = 1; tree_count <= setting.num_of_trees; tree_count++) {
		gain_count = 0;
		bound_count = 0;
		makeTargets();
		const vector<double>& additive_ys = db.planter.run(train_targets, test_targets);
		for (auto id : test_targets) {
			db.y_predictions[id] += (double) additive_ys[id] / (double) setting.num_of_trees;
		}
		report(additive_ys);
	}

	report();
}

void CLASS::makeTargets() {
	// std::cout << "debug makeTargets" << std::endl; // debug
	ID id;
	train_targets.clear();
	test_targets.clear();
	train_targets = Dice::random_sample(db.gdata.num_of_train, setting.data_used);
	for (id = db.gdata.getFirstTestID(); id <= db.gdata.getLastTestID(); id++) {
		test_targets.push_back(id);
	}
	// Debug::IDs(target_trains); // debug
	// Debug::IDs(target_tests); // debug
}

void CLASS::report(vector<double>& additive_ys) {
	reportFeatureImportance();
	db.evaluater.run(tree_count, train_targets, test_targets, additive_ys); // acc, auc
	cout << "REPORT " << tree_count << " cache_size " << db.gspan.getCache().size() << endl;
	cout << "REPORT " << tree_count << " gain_count " << gain_count << endl;
	cout << "REPORT " << tree_count << " bound_count " << bound_count << endl;
	sum_gcount += gain_count;
	sum_bcount += bound_count;
	sum_ccount += db.gspan.getCache().size();
}

void CLASS::report() {
	reportFeatureImportance();
	db.evaluater.run(test_targets); // acc, auc
	cout << "REPORT " << tree_count << " RESULT cache_size " << sum_ccount << endl;
	cout << "REPORT " << tree_count << " RESULT gain_count"  << sum_gcount << endl;
	cout << "REPORT " << tree_count << " RESULT bound_count" << sum_bcount << endl;
}

void CLASS::reportFeatureImportance() {
	size_t count = 0;
	for (const auto& pair : db.gspan.getCache()) {
		const auto& pattern = pair.first;
		double importance = pair.second.feature_importance;
		if (importance > 0) {
			count++;
			cout << "REPORT " << tree_count << " FI " << pattern << " : " << importance << endl;
		}
	}
	cout << "REPORT " << tree_count << " used_features " << count << endl;
}

