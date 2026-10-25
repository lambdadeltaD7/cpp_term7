#include "domains.h"
#include "methods.h"
#include "stopping_criterion.h"
#include <random>


using vec = std::vector<double>;


double f1(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f1 expects 2 dimensions");
    }

    return x[0] * x[0] + x[1] * x[1];
}

double f2(const vec& x)
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f2 expects 2 dimensions");
    }

    return 100.0 * std::pow(x[1] - x[0] * x[0], 2)
         + std::pow(1.0 - x[0], 2);
}

double f3(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f3 expects 2 dimensions");
    }

    return std::pow(x[0] - 2.0, 2)
         + 2.0 * std::pow(x[1] + 1.0, 2);
}

double f4(const vec& x) 
{
    if (x.size() != 3) 
	{
        throw std::invalid_argument("f4 expects 3 dimensions");
    }

    return std::pow(x[0] - 1.0, 2)
         + std::pow(x[1] + 2.0, 2)
         + std::pow(x[2] - 0.5, 2);
}


void ex1()
{
	std::mt19937 gen(std::random_device{}());
	// std::mt19937 gen(67);

    Rect domain(
        vec{-6.0, -5.0},
        vec{5.0, 5.0}
    );

    RandomSearch method(800000, gen);

	std::vector<std::unique_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_unique<FuncVal>(0.001));

    const SearchResults result = method.optimize(
        domain,
        [](const vec& x) {
            return f2(x);
        },
		stopping_criteria
    );
    std::cout << '\n' << result << '\n';
}

void ex2()
{
    Rect domain(
        vec{-6.0, -5.0},
        vec{5.0, 5.0}
    );
	
	double alpha = 1.0;
	double beta = 0.5;
	double gamma = 2;
	double vertex_dist = 1;

    DeformPoly method(8000, alpha, beta, gamma, vertex_dist); 

	std::vector<std::unique_ptr<StoppingCriterion>> stopping_criteria;
	stopping_criteria.emplace_back(std::make_unique<FuncVal>(0.00001));

    const SearchResults result = method.optimize(
        domain,
        [](const vec& x) {
            return f2(x);
        },
		stopping_criteria
    );
    std::cout << '\n' << result << '\n';
}


int main()
{
	ex2();

    return 0;
}
