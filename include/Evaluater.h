#pragma once

#include <MyInclude.h>

class Evaluater {
	public:
		void run(size_t t);

	private:
		void calcACC(size_t tree_count, string type, ID first_id, ID last_id, size_t num_all);
};
