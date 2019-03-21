#pragma once

#include "IncludeLib.h"
#include "Structures.h"

struct EdgeTracer {
	VertexPair vpair;
	EdgeTracer* predec;
	explicit EdgeTracer() {};
	explicit EdgeTracer(VertexPair vpair, EdgeTracer* predec) : vpair(vpair), predec(predec) {};
	void set(ID _a, ID _b, ID _id, EdgeTracer* _predec) {
		vpair.a = _a;
		vpair.b = _b;
		vpair.id = _id;
		predec = _predec;
	}
};

using Tracers = vector<EdgeTracer>;
using GraphToTracers = map<ID, Tracers>;
using PairSorter = map<Pair, GraphToTracers>;

struct DFSCode {
	Pair time;
	Triplet labels;
};

inline bool operator == (const DFSCode& l, const DFSCode& r) {
	if (l.time.a != r.time.a) return false;
	if (l.time.b != r.time.b) return false;
	if (l.labels.x != r.labels.x) return false;
	if (l.labels.y != r.labels.y) return false;
	return (l.labels.z == r.labels.z);
}

inline bool operator != (const DFSCode& l, const DFSCode& r) {
	return !(l == r);
}

inline bool operator < (const DFSCode& l, const DFSCode& r) {
	if (l.time < r.time) return true;
	if (r.time < l.time) return false;
	if (l.labels < r.labels) return true;
	if (r.labels < l.labels) return false;
	return false;
}

using Pattern = vector<DFSCode>;

inline bool operator < (const Pattern& l, const Pattern& r) {
	for (size_t i = 0; i < l.size() and i < r.size(); i++) {
		if (l[i] == r[i]) {
			continue;
		}
		if (l[i] < r[i]) return true;
		if (r[i] < l[i]) return false;
	}
	if (l.size() < r.size()) return true;
	if (r.size() < l.size()) return false;
	return false;
}

inline std::ostream& operator << (std::ostream& os, const Pattern& pattern) {
	if (pattern.empty()) return os;
	os << "(" << pattern[0].labels.x << ")" <<  pattern[0].labels.y << " (0f" << pattern[0].labels.z << ")";
	for (size_t i = 1; i < pattern.size(); i++) {
		if (pattern[i].time.a < pattern[i].time.b) {
			os << " " << pattern[i].labels.y << " (" << pattern[i].time.a << "f" << pattern[i].labels.z << ")";
		} else {
			os << " " << pattern[i].labels.y << " (b" << pattern[i].time.b << ")";
		}
	}
	return os;
}

inline Graph toGraph(const Pattern& codes) {
	Graph g;
	ID eid = 0;
	for (auto p : codes) {
		g.resize(std::max(p.time.a, p.time.b) + 1);
		if (p.labels.x != -1) g.label[p.time.a] = p.labels.x;
		if (p.labels.z != -1) g.label[p.time.b] = p.labels.z;
		Triplet labels(p.labels.x, p.labels.y, p.labels.z);
		g[p.time.a].push_back(Edge(eid++, p.time.b, labels));
		g[p.time.b].push_back(Edge(eid++, p.time.a, labels.reverse()));
	}
	g.num_of_edges = eid;
	return g;
}

inline void scan_rm(const Pattern& pattern, vector<size_t>& idx) {
	ID prev = -1;
	for (int i = pattern.size() - 1; i >= 0; i--) {
		if (pattern[i].time.a < pattern[i].time.b) {
			if (pattern[i].time.b == prev or prev == (ID) -1) {
				idx.push_back(i);
				prev = pattern[i].time.a;
			}
		}
	}
}
