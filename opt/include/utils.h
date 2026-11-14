#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using vec = std::vector<double>;

struct SearchResults 
{
    vec point;
    double value = 0.0;
    std::size_t iterations = 0;
};

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


double f1(const vec& x);

double f2(const vec& x);

double f3(const vec& x);

double f4(const vec& x);

#endif

