#ifndef SVMCAPSOLVER
#define SVMCAPSOLVER

#include "utils.h"

class SVMCapSolver
{
	int visit[300][300];

protected:
	int MAXROW,MAXCOL,length;
	svm_model model;
	Mat *binArr;

	Mat doThreshold(Mat& src, int v);
	void doDivide(Mat& bin, Mat binArr[], pII posInfo[]);
	int getValue(Mat& a, int i, int j);
	virtual Mat doClear(Mat &src, int pix_lim);
	virtual queue<pII> doBFS(queue<pII> &que, Mat_<uchar> &m);
	virtual Mat SVMCapSolver::findCharacter(Mat& M);
	virtual Mat convert2Standard(Mat& m);

public:
	SVMCapSolver(svm_model model);
	~SVMCapSolver();
	virtual string solve(Mat q) = 0;
};

#endif