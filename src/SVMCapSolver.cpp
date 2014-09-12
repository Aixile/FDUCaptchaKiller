#include "SVMCapSolver.h"

SVMCapSolver::SVMCapSolver(svm_model model) :model(model)
{
}


SVMCapSolver::~SVMCapSolver()
{
}

Mat SVMCapSolver::doThreshold(Mat& src, int v){
	Mat gray, bin;
	cvtColor(src, gray, CV_RGB2GRAY);
	threshold(gray, bin, v, 255, CV_THRESH_BINARY);
	return bin;
}

void SVMCapSolver::doDivide(Mat& bin, Mat binArr[], pII posInfo[]){
	for (int i = 0; i < length; i++){
		Mat aux = bin.colRange(posInfo[i].first, posInfo[i].second);
		binArr[i] = Mat::eye(aux.size(), aux.type());
		aux.copyTo(binArr[i]);
	}
}

Mat SVMCapSolver::convert2Standard(const Mat& m){
	int r = m.rows, c = m.cols;
	if (r>MAXROW || c>MAXCOL){
		cerr << "ERROR: Character size is too big. Row Size=" << r << " ,Col Size=" << c << endl;
		exit(1);
	}
	Mat s(MAXROW, MAXCOL, m.type());
	s = 255;
	int rs = (MAXROW - r) / 2, cs = (MAXCOL - c) / 2;
	m.copyTo(s(Rect(cs, rs, c, r)));
	return s;
}

int SVMCapSolver::getValue(Mat& a, int i, int j){
	if (a.ptr<uchar>(i)[j]<255)	return -1;
	else	return 1;
}

queue<pII> SVMCapSolver::doBFS(queue<pII> &que, Mat_<uchar> &m){
	int r = m.rows, c = m.cols;
	queue<pII> save;
	if (!que.empty())	visit[que.front().first][que.front().second] = -1;
	while (!que.empty()){
		pII a = que.front(); save.push(a); que.pop();
		for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)	if (i || j){
			int p = a.first + i, q = a.second + j;
			if (p<0 || p >= r || q<0 || q >= c)	continue;
			if (visit[p][q])	continue;
			if (m(p, q) != 255){
				que.push(pII(p, q));
				visit[p][q] = -1;
			}
		}
	}
	return save;
}

Mat SVMCapSolver::doClear(const Mat& src, int pix_lim){
	memset(visit, 0, sizeof(visit));
	int imgh = src.rows, imgw = src.cols;
	Mat_<uchar> m = src;
	queue<pII> que;
	for (int i = 0; i<imgh; i++)
	for (int j = 0; j<imgw; j++)	if (m(i, j) != 255 && visit[i][j] == 0){
		que.push(pII(i, j));
		que = doBFS(que, m);
		int v = que.size();
		while (!que.empty()){
			pII a = que.front();
			visit[a.first][a.second] = v;
			que.pop();
		}
	}
	for (int i = 0; i<imgh; i++)
	for (int j = 0; j<imgw; j++)	if (m(i, j) != 255 && visit[i][j]<pix_lim&&visit[i][j]>0){
		m(i, j) = 255;
	}
	return m;
}

Mat SVMCapSolver::findCharacter(const Mat& M){
	int r = M.rows, c = M.cols;
	Mat_<uchar> m = M;

	int minR = r, maxR = -1, minC = c, maxC = -1;
	for (int i = 0; i<r; i++){
		bool flag0 = true;
		for (int j = 0; j<c; j++)	if (m(i, j) != 255){
			flag0 = false;
			break;
		}
		if (!flag0){
			minR = min(minR, i);
			maxR = max(maxR, i);
		}
	}
	for (int j = 0; j<c; j++){
		bool flag0 = true;
		for (int i = 0; i<r; i++)	if (m(i, j) != 255){
			flag0 = false;
			break;
		}
		if (!flag0){
			minC = min(minC, j);
			maxC = max(maxC, j);
		}
	}
	if (minC >= 0 && minC<c){
		//cout<<"Find character at ("<<minR<<","<<minC<<") ("<<maxR<<","<<maxC<<")"<<endl;
		return M.rowRange(minR, maxR + 1).colRange(minC, maxC + 1);
	}else{
		throw "Error!: Unable to find any characters.";
	}
}
