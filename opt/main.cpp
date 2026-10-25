#include "domains.h"
#include "methods.h"
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


int main()
{
	std::mt19937 gen(std::random_device{}());

    Rect domain(
        vec{-5.0, -5.0, -5.},
        vec{5.0, 5.0, 5}
    );

    RandomSearch method(900000, 1e-8, gen);
    const SearchResults result = method.optimize(
        domain,
        [](const vec& x) {
            return f4(x);
        }
    );
    std::cout << result << '\n';

    return 0;
}
