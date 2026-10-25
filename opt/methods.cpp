#include "methods.h"


using vec = std::vector<double>;


std::ostream& operator<<
(
	std::ostream& out,
	const SearchResults& result
) 
{
    const std::ios::fmtflags old_flags = out.flags();
    const std::streamsize old_precision = out.precision();

    out << std::fixed << std::setprecision(6);
    out << "Point: [";

    for (std::size_t i = 0; i < result.point.size(); ++i) 
	{
        if (i > 0) 
		{
            out << ", ";
        }
        out << result.point[i];
    }

    out << "]\n";
    out << "Function value: " << result.value << '\n';
    out << "Iterations: " << result.iterations;

    out.flags(old_flags);
    out.precision(old_precision);

    return out;
}


IterativeMethod::IterativeMethod(std::size_t max_iterations, double epsilon): 
	max_iterations_(max_iterations),
	epsilon_(epsilon) 
{
	if (max_iterations_ == 0)
	{
		throw std::invalid_argument("N must be positive");
	}

	if (epsilon_ < 0.0)
	{
		throw std::invalid_argument("Invalid epsilon");
	}
}


RandomSearch::RandomSearch
(
	std::size_t max_iterations,
	double epsilon,
	std::mt19937& generator
):    
	IterativeMethod(max_iterations, epsilon),
	generator_(generator) 
{}

SearchResults RandomSearch::optimize(
	const Domain& domain,
	const std::function<double(const vec&)>& function
) 
{
	vec& current = search_res_.point;
	current = domain.sample(generator_);

	double& current_value = search_res_.value;
	current_value = function(current);

	std::size_t& iterations = search_res_.iterations;
	iterations = 0;

	for (; iterations < max_iterations_; ++iterations) 
	{
		vec candidate = domain.sample(generator_);
		const double candidate_value = function(candidate);

		const double previous_value = current_value;

		if (candidate_value < current_value) {
			current = std::move(candidate);
			current_value = candidate_value;
		}

		const double difference =
			std::abs(current_value - previous_value);

	}

	return search_res_;
}

