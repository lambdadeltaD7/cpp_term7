#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include "domains.h"

using vec = std::vector<double>;

#ifndef METHODS_H
#define METHODS_H

struct SearchResults 
{
    vec point;
    double value = 0.0;
    std::size_t iterations = 0;
};


std::ostream& operator<<
(
	std::ostream& out,
	const SearchResults& result
); 


class IterativeMethod 
{
protected:
    std::size_t max_iterations_;
    double epsilon_;
	SearchResults search_res_;

public:
    IterativeMethod(std::size_t max_iterations, double epsilon);

    virtual SearchResults optimize(
        const Domain& domain,
        const std::function<double(const vec&)>& function
    ) = 0;

    virtual ~IterativeMethod() = default;
};

class RandomSearch : public IterativeMethod 
{
private:
	unsigned int seed_;
    std::mt19937& generator_;

public:
    RandomSearch(
        std::size_t max_iterations,
        double epsilon,
		std::mt19937& generator
    );

    SearchResults optimize(
        const Domain& domain,
        const std::function<double(const vec&)>& function
    ) override;
};

#endif
