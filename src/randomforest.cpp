#include "RandomForest.h"
#define CLASS RandomForest

#include "Database.h"
#include "Calculator.h" 

extern Setting setting;
extern Database db;

void CLASS::run() {
	// std::cout << "debug run" << std::endl; // debug
	for (tree_count = 1; tree_count <= setting.num_of_trees; tree_count++) {
		makeTargets();
		const vector<double>& test_ys = db.planter.run(targets);
		for (ID id = 0; id < db.gdata.num_of_test; id++) {
			db.y_predictions[id].push_back(test_ys[id]);
		}
		report();
	}
}

void CLASS::makeTargets() {
	// std::cout << "debug makeTargets" << std::endl; // debug
	ID id;
	targets.clear();
	targets = Dice::random_sample(db.gdata.num_of_train, setting.data_used);
	for (id = db.gdata.getFirstTestID(); id <= db.gdata.getLastTestID(); id++) {
		targets.push_back(id);
	}
	// Debug::IDs(targets); // debug
}

void CLASS::report() {
	reportFeatureImportance();
	db.evaluater.run(tree_count); // acc, auc
	cout << "REPORT " << tree_count << " cache_size " << db.gspan.getCache().size() << endl;
	cout << "REPORT " << tree_count << " gain_count " << gain_count << endl;
	cout << "REPORT " << tree_count << " bound_count " << bound_count << endl;
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

