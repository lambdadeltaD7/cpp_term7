#include "utils.h"


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

