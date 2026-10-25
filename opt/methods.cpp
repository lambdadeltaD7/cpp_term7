#include "methods.h"
#include "stopping_criterion.h"


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
    out << "SearchResults:\n";
	out << "    Point: [";

    for (std::size_t i = 0; i < result.point.size(); ++i) 
	{
        if (i > 0) 
		{
            out << ", ";
        }
        out << result.point[i];
    }

    out << "]\n";
    out << "    Function value: " << result.value << '\n';
    out << "    Iterations: " << result.iterations;

    out.flags(old_flags);
    out.precision(old_precision);

    return out;
}


IterativeMethod::IterativeMethod(std::size_t max_iterations): 
	max_iterations_(max_iterations)
{
	if (max_iterations_ == 0)
	{
		throw std::invalid_argument("N must be positive");
	}

}


RandomSearch::RandomSearch
(
	std::size_t max_iterations,
	std::mt19937& generator
):    
	IterativeMethod(max_iterations),
	generator_(generator) 
{}

SearchResults RandomSearch::optimize(
	const Domain& domain,
	const std::function<double(const vec&)>& function,
	const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
) 
{
	vec& current = search_res_.point;
	current = domain.sample(generator_);

	double& current_value = search_res_.value;
	current_value = function(current);

	std::size_t& iterations = search_res_.iterations;
	iterations = 0;

	std::vector<const StoppingCriterion*> stopping_criteria_internal;
	stopping_criteria_internal.reserve(1 + stopping_criteria.size());
	MaxIter max_iter_criterion{max_iterations_};
	stopping_criteria_internal.push_back(&max_iter_criterion);
	for(const auto& c : stopping_criteria)
		stopping_criteria_internal.push_back(c.get());
	
	for (;;++iterations) 
	{
		vec candidate = domain.sample(generator_);
		const double candidate_value = function(candidate);

		const double previous_value = current_value;

		if (candidate_value < current_value) {
			current = std::move(candidate);
			current_value = candidate_value;
		}

		for(const auto& st_cr : stopping_criteria_internal)
		{
			if(st_cr->check(*this))
			{
				std::cout << st_cr->get_stop_msg(*this) << std::endl;
				goto end;
			}
		}

	}

	end:
		return search_res_;
}

