#include "methods.h"
#include "stopping_criterion.h"
#include "utils.h"

using vec = std::vector<double>;


IterativeMethod::IterativeMethod(std::size_t max_iterations): 
	max_iterations_{max_iterations},
    done_{false}
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



void RandomSearch::optimization_step() 
{
    if (search_res_.iterations == 0)
    {
        search_res_.point = opt_ctxt_.domain->sample(generator_); 
        search_res_.value = opt_ctxt_.function(search_res_.point);
    }

	vec& current = search_res_.point;
	double& current_value = search_res_.value;

    vec candidate = opt_ctxt_.domain->sample(generator_);
    double candidate_value = opt_ctxt_.function(candidate);

    if (candidate_value < current_value) {
        current = std::move(candidate);
        current_value = candidate_value;
    }
    
	++search_res_.iterations;

    update_done();
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



void DeformPoly::optimization_step()
{
    std::size_t n = opt_ctxt_.domain->get_dim();

	// some refs for convinience
	vec& current = search_res_.point;
	double& current_value = search_res_.value;
	std::size_t& iterations = search_res_.iterations;
    auto& function = opt_ctxt_.function;

    if( iterations == 0 )
    {
        // initialize polygon vertices 
        pairs_ = 
            simplex_init(
                n,
                vertex_dist_,
                function
            );
            
        // initial sort
        std::sort(pairs_.begin(), pairs_.end(), comp);

        current = pairs_[0].point;
	    current_value = function(current);
    }


    vec mass_c = compute_mean(pairs_);
    
    // reflection
    vec x_refl = mass_c + alpha_ * (mass_c - pairs_[n].point); 
    double f_refl = function(x_refl);

    if (f_refl < pairs_[0].val)
    {
        // expansion
        vec x_exp = mass_c + gamma_ * (x_refl - mass_c); 
        double f_exp = function(x_exp);

        if (f_exp < pairs_[0].val)
        {
            pairs_[n].point = x_exp;
            pairs_[n].val = f_exp;
        }
        else
        {
            pairs_[n].point = x_refl;
            pairs_[n].val = f_refl;
        }
    }
    else if (f_refl < pairs_[n - 1].val)
    {
        pairs_[n].point = x_refl;
        pairs_[n].val = f_refl;
    }
    else
    {
        if (f_refl < pairs_[n].val)
        {
            pairs_[n].point = x_refl;
            pairs_[n].val = f_refl;
        }

        // shrinkin'
        vec x_shr = mass_c + beta_ * (pairs_[n].point - mass_c);
        double f_shr = function(x_shr);

        if (f_shr < pairs_[n].val)
        {
            pairs_[n].val = f_shr;
            pairs_[n].point = x_shr;
        }
        else 
        {
            // reduction
            for (std::size_t i=0; i<pairs_.size(); ++i)
                pairs_[i].point = pairs_[0].point + 
                    0.5 * (pairs_[i].point - pairs_[0].point);
        }
    }
    
    // put pairs_[n] on it's place (do sort)
    for (std::size_t i=0; i<=n; ++i)
    {
        if(pairs_[n].val <= pairs_[i].val)
        {
            pairs_.insert(pairs_.begin()+i, pairs_[n]);
            pairs_.pop_back();
            break;
        }
    }
    
    current = pairs_[0].point;
    current_value = pairs_[0].val;
	++iterations;

    update_done();	
}


