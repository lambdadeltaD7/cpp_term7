#include <random>
#include <vector>


using vec = std::vector<double>;

#ifndef DOMAINS_H
#define DOMAINS_H

class Domain 
{
private:
	std::size_t dim_;

public:
	Domain(std::size_t dim):
		dim_{dim} {}

    virtual bool contains(const vec& point) const = 0;
    
	virtual vec sample(std::mt19937& generator) const = 0;
	
	char get_dim() const
	{
		return dim_;
	}

    virtual ~Domain() = default;
};


class Rect : public Domain
{
private:
    vec lower_;
    vec upper_;

public:
	Rect(const vec& lower, const vec& upper);

	bool contains(const vec& point) const override;

	vec sample(std::mt19937& generator) const override;
};

#endif
