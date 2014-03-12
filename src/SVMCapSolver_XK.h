#ifndef SVMCAPSOLVER_XK
#define SVMCAPSOLVER_XK

#include "SVMCapSolver.h"
class SVMCapSolver_XK :
	public SVMCapSolver
{
	int visit[300][300];
	Mat doClear(Mat &src, int pix_lim);

public:
	SVMCapSolver_XK(svm_model model);
	~SVMCapSolver_XK();
	string solve(Mat q);
};

#endif
