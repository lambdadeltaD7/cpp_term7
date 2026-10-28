#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include "methods.h"

using vec = std::vector<double>;


struct Pair
{
	double val;
	vec point;
};

bool comp(const Pair& lhs, const Pair rhs);


vec& operator+=(vec& a, const vec& b);

vec operator+(const vec& a, const vec& b);

vec operator*(const vec& v, double k);

vec operator*(double k, const vec& v);

vec operator-(const vec& a, const vec& b);


vec compute_mean(const std::vector<Pair>& pairs);


std::ostream& operator<<
(
	std::ostream& out,
	const SearchResults& result
); 
#endif

