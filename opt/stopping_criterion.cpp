#include "stopping_criterion.h"
#include "methods.h"


MaxIter::MaxIter(std::size_t max_iter):
	max_iter_(max_iter) {}

bool MaxIter::check(const IterativeMethod& method) const
{
	return method.get_search_res().iterations >= max_iter_;
}

std::string MaxIter::get_stop_msg(const IterativeMethod& method) const
{
	return "stopped because reached max_iter = " + 
			std::to_string(max_iter_);
}


FuncVal::FuncVal(double func_val):
	func_val_(func_val) {}

bool FuncVal::check(const IterativeMethod& method) const
{
	return method.get_search_res().value <= func_val_;
}

std::string FuncVal::get_stop_msg(const IterativeMethod& method) const
{
	return "stopped because found point where function value is <= than desired\n" + 
			std::to_string(method.get_search_res().value) +
			" <= " +
			std::to_string(func_val_);
}

