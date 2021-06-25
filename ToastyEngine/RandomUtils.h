#pragma once
#include <cstdlib>
#include <math.h>

double getRandomDouble(int max) {
	return static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / max));
};

int getRandomNumber(int max) {
	return (rand() % max) + 1;
};

double getRandomDoubleRange(int max, int min) {
	return min + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (max - min)));
};