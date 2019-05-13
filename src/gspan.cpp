#include "Gspan.h"
#define CLASS Gspan

#include "Database.h"
#include "Calculator.h" 
#include "Spliter.h" 

extern Setting setting;
extern Database db;

void CLASS::run(const vector<ID>& targets) {
	// std::cout << "debug" << std::endl; // debug
	auto& gdata = db.gdata;
	map<Triplet, GraphToTracers> heap;
	for (auto gid : targets) {
		EdgeTracer cursor;
		Graph& g = gdata[gid];
		for (ID vid = 0; vid < (ID) g.size(); vid++) {
			for (auto e : g[vid]) {
				if (e.labels.x <= e.labels.z) {
					cursor.set(vid, e.to, e.id, nullptr);
					heap[e.labels][gid].push_back(cursor);
				}
			}
		}
	}
	pattern.resize(1);

	// random edge select
	vector<int> count = Dice::make_count(heap.size(), setting.search_paths);

	int i = 0;
	for (auto itr = heap.begin(); itr != heap.end(); itr++) {
		if (count[i] == 0) {
			i++;
			continue;
		}
		if (support(itr->second) < minsup) {
			i++;
			continue;
		}

		pattern[0].labels = itr->first;
		pattern[0].time.set(0, 1);
		e1patterns.push_back(pattern);
		edgeGrow(itr->second, count[i]);
		i++;
	}
}

size_t CLASS::support(GraphToTracers& g2tracers) {
	return g2tracers.size();
}

bool CLASS::check_pattern(Pattern pattern, GraphToTracers& g2tracers) {
		if (pattern.size() > maxpat) {
			return false;
		}
		if (support(g2tracers) < minsup) {
			return false;
		}
		/*
		if (is_min.run(pattern) == false) {
			return false;
		}
		*/
		return true;
}

void CLASS::edgeGrow(GraphToTracers& g2tracers, int pnum) {
	// std::cout << "debug edgeGrow" << std::endl; // debug
	// report(g2tracers);
	vector<ID> posi = getPosiIds(g2tracers);

	map<int, PairSorter, std::greater<int>> b_heap;
	map<int, PairSorter, std::greater<int>> f_heap;
	int maxtoc = scanGspan(g2tracers, b_heap, f_heap);
	// std::cout << "debug maxtoc " << maxtoc << std::endl; // debug

	int sum_of_edge = 0;
	for (auto it = b_heap.begin(); it != b_heap.end(); ++it) {
		sum_of_edge += it->second.size();
	}
	for (auto it = f_heap.begin(); it != f_heap.end(); ++it) {
		sum_of_edge += it->second.size();
	}
	if (sum_of_edge == 0) {
		return;
	}
	vector<int> count = Dice::make_count(sum_of_edge, pnum);

	// projecting
	DFSCode dcode;
	vector<DFSCode> childs;
	int i = 0;
	for (auto vitr = b_heap.begin(); vitr != b_heap.end(); ++vitr) {
		for (auto eitr = vitr->second.begin(); eitr != vitr->second.end(); ++eitr) {
			if (count[i] != 0) {
				dcode.labels = Triplet(-1,eitr->first.a,-1);
				dcode.time.set(vitr->first, eitr->first.b);
				childs.push_back(dcode);
				pattern.push_back(dcode);
				edgeGrow(eitr->second, count[i]);
				pattern.pop_back();
			}
			i++;
		}
	}
	for (auto vitr = f_heap.begin(); vitr != f_heap.end(); ++vitr) {
		for (auto eitr = vitr->second.begin(); eitr != vitr->second.end(); ++eitr) {
			if (count[i] != 0) {
				dcode.labels = Triplet(-1,eitr->first.a,eitr->first.b);
				dcode.time.set(vitr->first,maxtoc+1);
				childs.push_back(dcode);
				pattern.push_back(dcode);
				edgeGrow(eitr->second, count[i]);
				pattern.pop_back();
			}
			i++;
		}
	}

	cache.insert({pattern, CacheRecord(posi, childs)});
}

void CLASS::report(GraphToTracers& tracers) {
	cout << tracers.size() << " ||" << pattern << "|| ";
	for (auto x : tracers) {
		cout << x.first << " ";
	}
	cout << endl;
}

// not minDFS
int Gspan::scanGspan(GraphToTracers& g2tracers, map<int, PairSorter, std::greater<int>>& b_heap, map<int, PairSorter, std::greater<int>>& f_heap) const {
	int maxtoc = 0;
	for (auto it = pattern.rbegin(); it != pattern.rend(); it++) {
		if (it->time.a < it->time.b) {
			maxtoc = it->time.b;
			break;
		}
	}
	vector<VertexPair> vpairs(pattern.size());
	EdgeTracer *tracer;

	Pair pkey;
	EdgeTracer cursor;

	for (auto x = g2tracers.begin(); x != g2tracers.end(); ++x) {
		int gid = x->first;
		for (auto it = x->second.begin(); it != x->second.end(); ++it) {
			// an instance (a sequence of vertex pairs) as vector "vpair"
			tracer = &(*it);

			Graph& g = db.gdata[gid];
			vector<bool> tested(g.num_of_edges);
			vector<bool> discovered(g.size());
			vector<int> g_to_dfs(g.size(), -1);

			for (int i = vpairs.size()-1; i >= 0; --i, tracer = tracer->predec) {
				vpairs[i] = tracer->vpair;
				auto vidbase = vpairs[i].id - (vpairs[i].id % 2); // hit to_edge and from_edge
				tested[vidbase + 0] = true;
				tested[vidbase + 1] = true;
				discovered[vpairs[i].a] = discovered[vpairs[i].b] = true;

				g_to_dfs[vpairs[i].b] = pattern[i].time.b;
				if (i == 0) {
					g_to_dfs[vpairs[i].a] = pattern[i].time.a;
				}
			}

			// make heap
			for (unsigned int i = 0; i < g.size(); i++) {
				if (!discovered[i]) {
					continue;
				}
				for (unsigned int j = 0; j < g[i].size(); j++) {
					Edge& added_edge = g[i][j];
					if (discovered[added_edge.to]) {
						// backward
						if (tested[added_edge.id] == false and g_to_dfs[i] > g_to_dfs[added_edge.to]) {
							pkey.set(added_edge.labels.y,g_to_dfs[added_edge.to]);
							cursor.set(i,added_edge.to,added_edge.id,&(*it));
							b_heap[g_to_dfs[i]][pkey][gid].push_back(cursor);
						}
					} else {
						// forward
						pkey.set(added_edge.labels.y,added_edge.labels.z);
						cursor.set(i,added_edge.to,added_edge.id,&(*it));
						f_heap[g_to_dfs[i]][pkey][gid].push_back(cursor);
					}
				}
			}
		}
	}
	return maxtoc;
}
