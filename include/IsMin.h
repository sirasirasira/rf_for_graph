#pragma once

#include "MyInclude.h"

class IsMin {
	public:
		bool run(const Pattern& pattern);

	private:
		bool minChecker(Pattern& comp, Graph& g, Tracers& tracers);
};
