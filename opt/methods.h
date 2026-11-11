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

class StoppingCriterion;


using vec = std::vector<double>;


struct SearchResults 
{
    vec point;
    double value = 0.0;
    std::size_t iterations = 0;
};


struct OptimizationContext
{
    std::shared_ptr<Domain> domain;
    std::function<double(const vec&)> function;
	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
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
    bool done_;
	SearchResults search_res_;
    OptimizationContext opt_ctxt_;

public:
    IterativeMethod(std::size_t max_iterations);

	std::size_t get_max_iters() const 
	{
		return max_iterations_;
	}

	SearchResults get_search_res() const 
	{
		return search_res_;
	}

	void setup_optimization(OptimizationContext opt_ctxt)
    {
        opt_ctxt_ = opt_ctxt;
        opt_ctxt_.stopping_criteria.insert(
            opt_ctxt_.stopping_criteria.begin(),
            std::make_shared<MaxIter>(max_iterations_)
        );
    }

	virtual void optimization_step() = 0;

    bool is_done() const
    {
        return done_;
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

	void optimization_step() override;
};

/*
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
		const std::vector<std::shared_ptr<StoppingCriterion>>& stopping_criteria
    ) override;
};
*/

#endif

