#include "domains.h"
#include "methods.h"
#include "stopping_criterion.h"
#include <random>


using vec = std::vector<double>;


void ex1()
{
	// std::mt19937 gen(std::random_device{}());
	std::mt19937 gen(67);

    RandomSearch method(800000, gen);

    
	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_shared<FuncVal>(0.001));

    OptimizationContext opt_ctxt;
    opt_ctxt.domain = std::make_shared<Rect>(
        vec{-6.0, -5.0},
        vec{5.0, 5.0}
    );
    opt_ctxt.function = [](const vec& x) {
                            return f2(x);
                        };
    opt_ctxt.stopping_criteria = stopping_criteria;

    method.setup_optimization(opt_ctxt);

    
    while( !method.is_done() )
    {
        method.optimization_step();
    }    

    std::cout << '\n' << method.get_search_res() << '\n';
}


void ex2()
{
	double alpha = 1.0;
	double beta = 0.5;
	double gamma = 2;
	double vertex_dist = 1;

    DeformPoly method(30, alpha, beta, gamma, vertex_dist); 

	std::vector<std::shared_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_shared<FuncVal>(0.0000000000001));

    OptimizationContext opt_ctxt;
    opt_ctxt.domain = std::make_shared<Rect>(
        vec{-6.0, -5.0, -5},
        vec{5.0, 5.0, 6}
    );
    opt_ctxt.function = [](const vec& x) {
                            return f4(x);
                        };
    opt_ctxt.stopping_criteria = stopping_criteria;

    method.setup_optimization(opt_ctxt);

    while( !method.is_done() )
    {
        method.optimization_step();
    }    

    std::cout << '\n' << method.get_search_res() << '\n';
}



int main()
{
	ex1();

    return 0;
}
