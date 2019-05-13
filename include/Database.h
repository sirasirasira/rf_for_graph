#pragma once

#include "Structures.h"

#include "RandomForest.h"
#include "Planter.h"
#include "Spliter.h"
#include "Finder.h"
#include "Gspan.h"
#include "Evaluater.h"

struct Database {
	GraphData gdata;
	vector<double> ys;
	vector<double> y_predictions;

	RandomForest random_forest;
	Planter planter;
	Spliter spliter;
	Finder finder;
	Gspan gspan;
	Evaluater evaluater;
};
