#pragma once
#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)
#define PI 3.141592654

#include <vector>

namespace PolynomialSolver
{
	int SolveQuadratic(float a, float b, float c, std::vector<double>& Sol);
	int SolveCubic(float a, float b, float c, float d, std::vector<double>& Sol);
	int SolveQuartic(float a, float b, float c, float d, float e, std::vector<double>& Sol);
}
