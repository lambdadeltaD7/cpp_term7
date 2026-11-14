#include "stopping_criterion.h"
#include "methods.h"

#include <sstream>
#include <iomanip>
#include <string>

MaxIter::MaxIter(std::size_t max_iter):
	max_iter_(max_iter) {}

bool MaxIter::check(const IterativeMethod& method) const
{
	return method.get_search_res().iterations >= max_iter_;
}

void MaxIter::get_stop_msg([[maybe_unused]] const IterativeMethod& method) const
{
	std::cout 
		<< "stopped because reached max_iter = " + 
		   std::to_string(max_iter_)
		<< std::endl;
}


FuncVal::FuncVal(double func_val):
	func_val_(func_val) {}

bool FuncVal::check(const IterativeMethod& method) const
{
	return method.get_search_res().value <= func_val_;
}


void FuncVal::get_stop_msg(const IterativeMethod& method) const
{
	std::cout
		<< std::scientific 
		<< "stopped because found point where " 
		   "function value is <= than desired\n" 
		<< method.get_search_res().value
		<< " <= " 
		<< func_val_ << std::endl;
}

