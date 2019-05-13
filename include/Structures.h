#pragma once

#include "IncludeLib.h"

using ID = size_t; // start from 0

struct Pair {
	ID a;
	ID b;
	
	explicit Pair() {};
	explicit Pair(ID a, ID b) : a(a), b(b) {};
	void set(ID _a, ID _b) {
		a = _a;
		b = _b;
	}
};

inline bool operator < (const Pair& l, const Pair& r) {
	if (l.a != r.a) return (l.a < r.a);
	return (l.b < r.b);
}

struct Triplet {
	int x;
	int y;
	int z;
	
	explicit Triplet() {};
	explicit Triplet(ID x, ID y, ID z) : x(x), y(y), z(z) {};
	void set(int _x, int _y, int _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	inline Triplet reverse() {
		return Triplet(z, y, x);
	}
};

inline std::ostream& operator << (std::ostream& os, const Triplet t) {
	os << t.x << "," << t.y << "," << t.z;
	return os;
}

inline bool operator < (const Triplet& l, const Triplet& r) {
	if (l.x != r.x) return (l.x < r.x);
	if (l.y != r.y) return (l.y < r.y);
	return (l.z < r.z);
}

inline bool operator <= (const Triplet& l, const Triplet& r) {
	return !(r < l);
}

inline bool operator == (const Triplet& l, const Triplet& r) {
	return (l.x == r.x and l.y == r.y and l.z == r.z);
}

struct IntPair {
public:
  int g;
  int s;
  int rank;
};

inline bool operator< (const IntPair& left, const IntPair& right){
  return (left.rank < right.rank);
}

struct Counterpart {
public:
  int target;
  int rank;
};

inline bool operator< (const Counterpart& left, const Counterpart& right){
  return (left.rank < right.rank);
}

typedef map<int,vector<int> > occurence;

struct datapack {
  int cmp;
  int seq;
  int label;
};

struct VertexPair : public Pair {
	ID id;
};

struct Edge {
	ID id; // edge id
	ID to; // to vertex id (from vertex id is AdjacentList key)
	Triplet labels;

	explicit Edge() {};
	explicit Edge(ID id, ID to, Triplet labels) : id(id), to(to), labels(labels) {};
};

using AdjacentList = vector<vector<Edge>>;

struct Graph : public AdjacentList {
	vector<int> label;
	size_t num_of_edges;

	inline void resize(size_t s) {
		AdjacentList::resize(s);
		label.resize(s);
	}
};

struct GraphData : vector<Graph> {
	size_t num_of_train;
	size_t num_of_test;

	inline ID getFirstTrainID() {
		return 0;
	}
	inline ID getLastTrainID() {
		return num_of_train - 1;
	}
	inline ID getFirstTestID() {
		return num_of_train;
	}
	inline ID getLastTestID() {
		return size() - 1;
	}
};
