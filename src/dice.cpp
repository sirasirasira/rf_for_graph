#include "Dice.h"
#define CLASS Dice

std::random_device CLASS::seed_gen;
//std::mt19937 CLASS::mt(seed_gen());
std::mt19937 CLASS::mt(0);
