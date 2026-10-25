#include <random>
#include <vector>


using vec = std::vector<double>;

#ifndef DOMAINS_H
#define DOMAINS_H

class Domain 
{
public:
    virtual bool contains(const vec& point) const = 0;
    virtual vec sample(std::mt19937& generator) const = 0;

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
