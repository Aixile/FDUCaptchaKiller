#ifndef SVMCAPSOLVER_FRESHMAN
#define SVMCAPSOLVER_FRESHMAN

#include "SVMCapSolver.h"
class SVMCapSolver_FRESHMAN :
	public SVMCapSolver
{
public:
	SVMCapSolver_FRESHMAN(svm_model model);
	~SVMCapSolver_FRESHMAN();
	string solve(Mat q);
};

#endif
