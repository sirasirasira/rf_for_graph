#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include "Spliter.h"
#include "IsMin.h"

class Gspan {
	struct CacheRecord {
		vector<ID> posi;
		vector<DFSCode> childs;
		double feature_importance;
		CacheRecord() {
			feature_importance = 0;
		}
		CacheRecord(vector<ID> posi, vector<DFSCode> childs)
			: posi(posi), childs(childs) {
			feature_importance = 0;
		}
	};

	public:
		size_t minsup;
		size_t maxpat;

		void run(const vector<ID>& targets);
		
		inline const map<Pattern, CacheRecord>& getCache() {
			return cache;
		}

		inline void clearCache() {
			cache.clear();
		}

		inline const vector<Pattern>& gete1Patterns() {
			return e1patterns;
		}

		inline void cleare1Patterns() {
			e1patterns.clear();
		}

		inline void reportCache() {
			std::cout << "Report Cache Infomation" << std::endl;
			for (auto itr = cache.begin(); itr != cache.end(); ++itr) {
				std::cout << "pattern : " << itr->first << std::endl;
			}
		}

		inline void updataFeatureImportance(const Pattern& pattern, double importance) {
			cache.at(pattern).feature_importance += importance;
		}

		inline vector<ID> getPosiIds(const GraphToTracers& tracers) {
			vector<ID> vec(tracers.size());
			size_t i = 0;
			for (auto x : tracers) {
				vec[i] = x.first;
				i++;
			}
			return vec;
		}

		void setSpliterPtr(Spliter* ptr) {
			spliter = ptr;
		}

		void setMinsup(size_t _minsup) {
			minsup = _minsup;
		}

		void setMaxpat(size_t _maxpat) {
			maxpat = _maxpat;
		}

	private:
		Spliter* spliter;
		Pattern pattern;
		IsMin is_min;
		map<Pattern, CacheRecord> cache; // inserted data must keep pointer
		vector<Pattern> e1patterns;

		void edgeGrow(GraphToTracers& g2tracers, int pnum);
		size_t support(GraphToTracers& g2tracers);
		void report(GraphToTracers& g2tracers);
		bool check_pattern(Pattern pattern, GraphToTracers& g2tracers);
		int scanGspan(GraphToTracers& g2tracers, map<int, PairSorter, std::greater<int>>& b_heap, map<int, PairSorter, std::greater<int>>& f_heap) const ;
};
