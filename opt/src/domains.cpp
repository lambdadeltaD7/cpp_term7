#include "domains.h"
#include <stdexcept>


using vec = std::vector<double>;


Rect::Rect(const vec& lower, const vec& upper):
	Domain(lower.size()),
	lower_(lower),
	upper_(upper)
{
	for (std::size_t i = 0; i < lower_.size(); ++i) 
	{
		if (lower_[i] > upper_[i]) 
		{
			throw std::invalid_argument("Invalid rectangle bounds");
		}
	}
}


bool Rect::contains(const vec& point) const 
{
	if (point.size() != lower_.size())
	{
		throw std::invalid_argument("dim conflict in contains()");
	}

	for (std::size_t i = 0; i < point.size(); ++i)
	{
		if (point[i] < lower_[i] || point[i] > upper_[i]) 
		{
			return false;
		}
	}

	return true;
}

vec Rect::sample(std::mt19937& generator) const 
{
	vec point(lower_.size());

	for (std::size_t i = 0; i < point.size(); ++i) 
	{
		std::uniform_real_distribution<double> distribution(
			lower_[i], upper_[i]
		);
		point[i] = distribution(generator);
	}

	return point;
}

