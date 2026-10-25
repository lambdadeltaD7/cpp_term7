#ifndef METHODS_H
#define METHODS_H


#include <memory>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include "domains.h"


class StoppingCriterion;


using vec = std::vector<double>;


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
	SearchResults search_res_;

public:
    IterativeMethod(std::size_t max_iterations);

    virtual SearchResults optimize(
        const Domain& domain,
        const std::function<double(const vec&)>& function,
		const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
    ) = 0;


	std::size_t get_max_iters() const 
	{
		return max_iterations_;
	}

	SearchResults get_search_res() const 
	{
		return search_res_;
	}

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
		std::mt19937& generator
    );

    SearchResults optimize(
        const Domain& domain,
        const std::function<double(const vec&)>& function,
		const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
    ) override;
};


class DeformPoly: public IterativeMethod
{
private:
	double alpha_;
	double beta_;
	double gamma_;
	
	double vertex_dist_;

public:
	DeformPoly(	
		std::size_t max_iterations,
		double alpha = 1.0,
		double beta  = 0.5,
		double gamma = 2.0,
		double vertex_dist = 1.0
	);

    SearchResults optimize(
        const Domain& domain,
        const std::function<double(const vec&)>& function,
		const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
    ) override;
};


#endif

