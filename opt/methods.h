#ifndef METHODS_H
#define METHODS_H


#include <memory>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include "domains.h"
#include "stopping_criterion.h"
#include "utils.h"

class StoppingCriterion;


using vec = std::vector<double>;


struct OptimizationContext
{
    std::shared_ptr<Domain> domain;
    std::function<double(const vec&)> function;
	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
};


class IterativeMethod 
{
protected:
    std::size_t max_iterations_;
    bool done_;
	SearchResults search_res_;
    OptimizationContext opt_ctxt_;

public:
    IterativeMethod(std::size_t max_iterations);

	void setup_optimization(OptimizationContext opt_ctxt);

	virtual void optimization_step() = 0;

    void update_done();

    bool is_done() const;

	std::size_t get_max_iters() const;

	SearchResults get_search_res() const;

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

	void optimization_step() override;
};


class DeformPoly: public IterativeMethod
{
private:
	double alpha_;
	double beta_;
	double gamma_;
	
	double vertex_dist_;

    std::vector<Pair> pairs_;

public:
	DeformPoly(	
		std::size_t max_iterations,
		double alpha = 1.0,
		double beta  = 0.5,
		double gamma = 2.0,
		double vertex_dist = 1.0
	);

	void optimization_step() override;
    
    std::vector<vec> get_poly() const;
};


#endif

