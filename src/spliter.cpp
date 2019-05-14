#include "Spliter.h"
#define CLASS Spliter

#include "Database.h"
#include "Calculator.h" 

extern Setting setting;
extern Database db;

void CLASS::initMinScore() {
	parent_score = Calculator::imp(db.ys, targets);
	min_score = parent_score - setting.min_impurity_decrease - std::numeric_limits<double>::epsilon();
}

void CLASS::prepare(const vector<ID>& _targets) {
	// std::cout << "debug Spliter prepare" << std::endl; // debug
	// Debug::IDs(targets); // debug
	targets = _targets;
	db.gspan.clearCache();
	db.gspan.setSpliterPtr(this);
	db.gspan.run(targets);
}

vector<ID> CLASS::run(const vector<ID>& _targets) {
	// std::cout << "debug spliter run" << std::endl; // debug
	targets = _targets;
	best_pattern = {};
	initMinScore();
	if (min_score < 0) {
		goto G_INVALID;
	}
	queue = {};
	searchCache();
	// std::cout << "debug best_pattern " << best_pattern << std::endl; // debug
	if (best_pattern.size() == 0) {
G_INVALID:
		valid_flg = false;
		return {};
	}
	// std::cout << "debug parent_score " << parent_score << " min_score " << min_score << std::endl; // debug
	valid_flg = true;
	vector<ID> posi = db.gspan.getCache().at(best_pattern).posi;
	return Calculator::setIntersec(targets, posi);
}

void CLASS::searchCache() {
	// std::cout << "serch Cache" << std::endl;
	auto cache = db.gspan.getCache();
	const auto& e1patterns = db.gspan.gete1Patterns();

	// one edge
	for (auto pattern : e1patterns) {
		const auto& posi = cache[pattern].posi;
		double min_bound = Calculator::bound(db.ys, targets, posi);
		queue.push(std::make_pair(min_bound, pattern));
	}

	// queue_based search
	while (!queue.empty()) {
		if (min_score <= queue.top().first) {
			break;
		}

		Pattern pattern = queue.top().second;
		const auto& cacherecord = cache[pattern];
		const auto& posi = cacherecord.posi;
		const auto& childs = cacherecord.childs;
		queue.pop();
		update(pattern, posi);
		for (auto dcode : childs) {
			pattern.push_back(dcode);
			const auto& cposi = cache[pattern].posi;
			double min_bound = Calculator::bound(db.ys, targets, cposi);
			queue.push(std::make_pair(min_bound, pattern));
			pattern.pop_back();
		}
	}
}

void CLASS::update(Pattern pattern, vector<ID> posi) {
	double score = Calculator::score(db.ys, targets, posi);
	if (score < min_score ) { // old pattern may be used (this func is called from gspan)
		min_score = score;
		best_pattern = pattern;
	}
}
