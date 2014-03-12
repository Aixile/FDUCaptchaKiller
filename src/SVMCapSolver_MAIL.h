#ifndef SVMCAPSOLVER_MAIL
#define SVMCAPSOLVER_MAIL

#include "SVMCapSolver.h"
class SVMCapSolver_MAIL :
	public SVMCapSolver
{
public:
	SVMCapSolver_MAIL(svm_model model);
	~SVMCapSolver_MAIL();
	string solve(Mat q);
};

#endif