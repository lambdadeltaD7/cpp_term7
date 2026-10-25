#ifndef STOPPING_CRITERION_H
#define STOPPING_CRITERION_H


#include <string>


class IterativeMethod;


class StoppingCriterion
{
public:

	virtual bool check(const IterativeMethod& method) const = 0;
	virtual std::string get_stop_msg(const IterativeMethod& method) const = 0;

	virtual ~StoppingCriterion() = default;

};


class MaxIter : public StoppingCriterion
{
private:
	std::size_t max_iter_;

public:
	MaxIter(std::size_t max_iter);

	bool check(const IterativeMethod& method) const override;
	std::string get_stop_msg(const IterativeMethod& method) const override;
};


class FuncVal : public StoppingCriterion
{
private:
	double func_val_;

public:
	FuncVal(double func_val);

	bool check(const IterativeMethod& method) const override;
	std::string get_stop_msg(const IterativeMethod& method) const override;
};

#endif
