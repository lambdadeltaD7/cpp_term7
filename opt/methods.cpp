#include "methods.h"
#include "stopping_criterion.h"
#include "utils.h"

using vec = std::vector<double>;


IterativeMethod::IterativeMethod(std::size_t max_iterations): 
	max_iterations_{max_iterations}
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
	IterativeMethod{max_iterations},
	generator_{generator} 
{}


std::vector<const StoppingCriterion*> 
add_stopping_criterion(
	std::size_t max_iterations_,
	const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
)
{
	std::vector<const StoppingCriterion*> stopping_criteria_internal;
	stopping_criteria_internal.reserve(1 + stopping_criteria.size());
	MaxIter *max_iter_criterion = new MaxIter{max_iterations_};
	stopping_criteria_internal.push_back(max_iter_criterion);
	for(const auto& c : stopping_criteria)
		stopping_criteria_internal.push_back(c.get());
	return stopping_criteria_internal;
}


SearchResults RandomSearch::optimize(
	const Domain& domain,
	const std::function<double(const vec&)>& function,
	const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
) 
{
	std::cout << "Running RandomSearch method\n\n";


	vec& current = search_res_.point;
	current = domain.sample(generator_);

	double& current_value = search_res_.value;
	current_value = function(current);

	std::size_t& iterations = search_res_.iterations;
	iterations = 0;
	

	// explicitly add MaxIter criterion 
	std::vector<const StoppingCriterion*> stopping_criteria_internal = 
		add_stopping_criterion(max_iterations_, stopping_criteria);
	

	for (;;++iterations) 
	{
		vec candidate = domain.sample(generator_);
		const double candidate_value = function(candidate);

		if (candidate_value < current_value) {
			current = std::move(candidate);
			current_value = candidate_value;
		}

		for(const auto& st_cr : stopping_criteria_internal)
		{
			if(st_cr->check(*this))
			{
				st_cr->get_stop_msg(*this);
				goto end;
			}
		}

	}

	end:
		return search_res_;
}


DeformPoly::DeformPoly
(	
	std::size_t max_iterations,
	double alpha,
	double beta,
	double gamma,
	double vertex_dist
):
	IterativeMethod{max_iterations},
	alpha_{alpha},
	beta_{beta},
	gamma_{gamma},
	vertex_dist_{vertex_dist}
{}

std::vector<Pair> simplex_init(
	std::size_t n, //dim
	double t, // vertex_dist
	const std::function<double(const vec&)>& function
)
{
	double sqrt2 = std::sqrt(2);
	double d1 = t / (n * sqrt2) * (std::sqrt(n + 1) + n - 1);
	double d2 = t / (n * sqrt2) * (std::sqrt(n + 1) - 1);

	std::vector<Pair> pairs(1 + n);
	pairs[0].point = vec(n, 0);
	pairs[0].val = function(pairs[0].point);
    for (std::size_t i=1; i<=n; ++i)
	{
		pairs[i].point = vec(n, d2);
		pairs[i].point[i - 1] = d1;
		pairs[i].val = function(pairs[i].point);
	}

	return pairs;
}



SearchResults DeformPoly::optimize
(
	const Domain& domain,
	const std::function<double(const vec&)>& function,
	const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
)
{
	std::cout << "Running DeformPoly method\n\n";

	std::size_t n = domain.get_dim();

	// initialize polygon vertices 
	std::vector<Pair> pairs = 
		simplex_init(
			n,
			vertex_dist_,
			function
		);
		
	// initial sort
	std::sort(pairs.begin(), pairs.end(), comp);


	// some refs for convinience
	vec& current = search_res_.point;
	current = pairs[0].point;

	double& current_value = search_res_.value;
	current_value = function(current);

	std::size_t& iterations = search_res_.iterations;
	iterations = 0;


	// explicitly add MaxIter criterion 
	std::vector<const StoppingCriterion*> stopping_criteria_internal = 
		add_stopping_criterion(max_iterations_, stopping_criteria);


	// main loop
	for (;;++iterations) 
	{
		for(const auto& st_cr : stopping_criteria_internal)
		{
			if(st_cr->check(*this))
			{
				st_cr->get_stop_msg(*this);
				goto end;
			}
		}

		vec mass_c = compute_mean(pairs);
		
		// reflection
		vec x_refl = mass_c + alpha_ * (mass_c - pairs[n].point); 
		double f_refl = function(x_refl);

		if (f_refl < pairs[0].val)
		{
			// expansion
			vec x_exp = mass_c + gamma_ * (x_refl - mass_c); 
			double f_exp = function(x_exp);

			if (f_exp < pairs[0].val)
			{
				pairs[n].point = x_exp;
				pairs[n].val = f_exp;
			}
			else
			{
				pairs[n].point = x_refl;
				pairs[n].val = f_refl;
			}
		}
		else if (f_refl < pairs[n - 1].val)
		{
			pairs[n].point = x_refl;
			pairs[n].val = f_refl;
		}
		else
		{
			if (f_refl < pairs[n].val)
			{
				pairs[n].point = x_refl;
				pairs[n].val = f_refl;
			}

			// shrinkin'
			vec x_shr = mass_c + beta_ * (pairs[n].point - mass_c);
			double f_shr = function(x_shr);

			if (f_shr < pairs[n].val)
			{
				pairs[n].val = f_shr;
				pairs[n].point = x_shr;
			}
			else 
			{
				// reduction
				for (std::size_t i=0; i<pairs.size(); ++i)
					pairs[i].point = pairs[0].point + 
						0.5 * (pairs[i].point - pairs[0].point);
			}
		}
		
		// put pairs[n] on it's place (do sort)
		for (std::size_t i=0; i<=n; ++i)
		{
			if(pairs[n].val <= pairs[i].val)
			{
				pairs.insert(pairs.begin()+i, pairs[n]);
				pairs.pop_back();
				break;
			}
		}
		
		current = pairs[0].point;
		current_value = pairs[0].val;
	}
	
	end:
		return search_res_;
}


