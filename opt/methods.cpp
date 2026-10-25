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


    out << std::fixed << std::setprecision(10);
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
    out << "    Function value: " 
		<< std::scientific << result.value << '\n';
    out << "    Iterations: " << result.iterations;

    out.flags(old_flags);
    out.precision(old_precision);

    return out;
}


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


struct Pair
{
	double val;
	vec point;
};


bool comp(const Pair& lhs, const Pair rhs)
{
	return lhs.val < rhs.val;
}


vec& operator+=(vec& a, const vec& b)
{
    for (std::size_t i = 0; i < a.size(); ++i)
        a[i] += b[i];

    return a;
}


vec operator+(const vec& a, const vec& b)
{
    vec result = a;

    return result += b;
}


vec operator*(const vec& v, double k)
{
    vec result(v.size());

    for (std::size_t i = 0; i < v.size(); ++i)
        result[i] = v[i] * k;

    return result;
}


vec operator*(double k, const vec& v) {
    return v * k;
}


vec operator-(const vec& a, const vec& b)
{
    vec result = a;

    return result += (-1) * b;
}


vec compute_mean(const std::vector<Pair>& pairs)
{
	std::size_t dim = pairs[0].point.size();
	vec res(dim);
	
	for(std::size_t j=0; j+1<pairs.size(); ++j)
		res += pairs[j].point;

	return res * (1.0 / dim);
}


SearchResults DeformPoly::optimize
(
	const Domain& domain,
	const std::function<double(const vec&)>& function,
	const std::vector<std::unique_ptr<StoppingCriterion>>& stopping_criteria
)
{
	std::cout << "Running DeformPoly method\n\n";

	// initialize polygon vertices 
	std::size_t n = domain.get_dim();
	double& t = vertex_dist_;
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
	std::vector<const StoppingCriterion*> stopping_criteria_internal;
	stopping_criteria_internal.reserve(1 + stopping_criteria.size());
	MaxIter max_iter_criterion{max_iterations_};
	stopping_criteria_internal.push_back(&max_iter_criterion);
	for(const auto& c : stopping_criteria)
		stopping_criteria_internal.push_back(c.get());
	
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


