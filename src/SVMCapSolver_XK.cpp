#include "SVMCapSolver_XK.h"


SVMCapSolver_XK::SVMCapSolver_XK(svm_model model) :SVMCapSolver(model)
{
	length = 4;
	MAXROW = 16;
	MAXCOL = 12;
}

SVMCapSolver_XK::~SVMCapSolver_XK()
{
}

Mat SVMCapSolver_XK::doClear(Mat& M,int pix_lim){
	int r = M.rows, c = M.cols;
	Mat_<uchar> m = M;
	memset(visit, 0, sizeof(visit));
	queue<pII> que;
	for (int i = 0; i<r; i++)	if (m(i, 0) != 255){
		if (visit[i][0] == 0){
			que.push(pII(i, 0));
			que = doBFS(que, m);
			int v = que.size();
			while (!que.empty()){
				pII a = que.front();
				visit[a.first][a.second] = v;
				que.pop();
			}
		}
	}
	for (int i = 0; i<r; i++)	if (m(i, c - 1) != 255){
		if (visit[i][c - 1] == 0){
			que.push(pII(i, c - 1));
			que = doBFS(que, m);
			int v = que.size();
			while (!que.empty()){
				pII a = que.front();
				visit[a.first][a.second] = v;
				que.pop();
			}
		}
	}
	for (int i = 0; i<c; i++)	if (m(0, i) != 255){
		if (visit[0][i] == 0){
			que.push(pII(0, i));
			que = doBFS(que, m);
			int v = que.size();
			while (!que.empty()){
				pII a = que.front();
				visit[a.first][a.second] = v;
				que.pop();
			}
		}
	}
	for (int i = 0; i<c; i++)	if (m(r - 1, i) != 255){
		if (visit[r - 1][i] == 0){
			que.push(pII(r - 1, i));
			que = doBFS(que, m);
			int v = que.size();
			while (!que.empty()){
				pII a = que.front();
				visit[a.first][a.second] = v;
				que.pop();
			}
		}
	}
	for (int i = 0; i<r; i++)
	for (int j = 0; j<c; j++)	if (m(i, j) != 255 && visit[i][j]<pix_lim&&visit[i][j]>0){
		m(i, j) = 255;
	}
	return m;
}

string SVMCapSolver_XK::solve(Mat query){
	Mat bin = doThreshold(query, 180), std[4], binArr[4];
	pII posTable[4] = { pII(0, 12), pII(13, 22), pII(23, 32), pII(33, 43) };
	doDivide(bin, binArr, posTable);
	for (int i = 0; i<length; i++)	std[i] = convert2Standard(findCharacter(doClear(binArr[i],4)));
	string answer = "";
	struct svm_node * node[4];
	for (int i = 0; i<length; i++){
		int r = MAXROW, c = MAXCOL, tot = 0;
		int size = r*c + 2;

		node[i] = (struct svm_node *) malloc(size*sizeof(struct svm_node));

		for (int x = 0; x<r; x++){
			for (int y = 0; y<c; y++){
				node[i][tot].index = tot + 1;
				node[i][tot++].value = getValue(std[i], x, y);
			}
		}

		node[i][tot++].index = -1;
		answer += char(svm_predict(&model, node[i]));
		
		free(node[i]);
	}
	return answer;
}