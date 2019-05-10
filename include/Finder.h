#pragma once

#include "MyInclude.h"
#include "StructuresGspan.h"
#include "patricia.h"

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

class Finder {
 public:
  unordered_map<int,string> g_tname;
  unordered_map<string,int> g_tid;
  map<int,string> fg_name;
  unordered_map<string,int> fg_id;
  vector<Graph>  gdata;
  vector<datapack>  trans;
  map<Triplet,GraphToTracers> gheap;
  patricia gpat_tree;
  patricia spat_tree;
  // member functions
  template<typename T> void read_features(T& ins);
};

int buildDFSCode(const string& str, vector<DFSCode>& query, int _cur, map<int,int>& label, vector<int>& addkeys){

  int cur = _cur;
  DFSCode tmp;
  std::istringstream in(str);
  char c;
  int i, j, elab=-200;
  while(in.get(c)){
    string token(1,c);
    unsigned int n = 1;
    while(in.get(c) && c != ' '){
      token += c;
      n += 1;
    }
    if (token[0] == '(' && token[n-1] == ')'){
      string sstr = token.substr(1,n-2);
      string::size_type r;
      if ((r = sstr.find("f")) != string::npos){
	i = atoi(sstr.substr(0,r).c_str());
	j = atoi(sstr.substr(r+1,string::npos).c_str());
	//std::cout << "call label[" << cur << "]=" << j << std::endl;
	addkeys.push_back(cur);
	label[cur] = j;
	tmp.time.a = i;
	tmp.time.b = cur;
	tmp.labels.x = label[i];
	tmp.labels.y = elab;
	tmp.labels.z = j;
	query.push_back(tmp);
	cur += 1;
      }else if ((r = sstr.find("b")) != string::npos){
	i = atoi(sstr.substr(r+1,string::npos).c_str());
	tmp.time.a = cur-1;
	tmp.time.b = i;
	tmp.labels.x = label[cur-1];
	tmp.labels.y = elab;
	tmp.labels.z = label[i];
	query.push_back(tmp);
      }else{
	i = atoi(sstr.c_str());
	label[cur] = i;
	addkeys.push_back(cur);
	cur += 1;
      }
    }else{
      elab = atoi(token.c_str());
    }
  }
  //std::cout << "done" << std::endl;
  
  return cur;
}
