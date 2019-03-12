#pragma once

#include "Structures.h"

#include "RandomForest.h"
#include "Planter.h"
#include "Spliter.h"
#include "Gspan.h"
#include "Evaluater.h"

struct Database {
	GraphData gdata;
	vector<double> ys;
	vector<double> y_predictions;

	RandomForest random_forest;
	Plantar plantar;
	Spliter spliter;
	Gspan gspan;
	Evaluater evaluater;
};
