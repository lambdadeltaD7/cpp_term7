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


double f1(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f1 expects 2 dimensions");
    }

    return x[0] * x[0] + x[1] * x[1];
}

double f2(const vec& x)
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f2 expects 2 dimensions");
    }

    return 100.0 * std::pow(x[1] - x[0] * x[0], 2)
         + std::pow(1.0 - x[0], 2);
}

double f3(const vec& x) 
{
    if (x.size() != 2) 
	{
        throw std::invalid_argument("f3 expects 2 dimensions");
    }

    return std::pow(x[0] - 2.0, 2)
         + 2.0 * std::pow(x[1] + 1.0, 2);
}

double f4(const vec& x) 
{
    if (x.size() != 3) 
	{
        throw std::invalid_argument("f4 expects 3 dimensions");
    }

    return std::pow(x[0] - 1.0, 2)
         + std::pow(x[1] + 2.0, 2)
         + std::pow(x[2] - 0.5, 2);
}

