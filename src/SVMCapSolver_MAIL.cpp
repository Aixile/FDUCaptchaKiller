#include "SVMCapSolver_MAIL.h"


SVMCapSolver_MAIL::SVMCapSolver_MAIL(svm_model model) :SVMCapSolver(model)
{
	length = 4;
	MAXROW = 20;
	MAXCOL = 20;
}


SVMCapSolver_MAIL::~SVMCapSolver_MAIL()
{
}

string SVMCapSolver_MAIL::solve(Mat query){
	Mat bin = doClear(doThreshold(query, 150),10),  std[4], binArr[4];
	pII posTable[4] = { pII(0, 23), pII(24, 47), pII(48, 71), pII(72, 95) };
	doDivide(bin, binArr, posTable);
	for (int i = 0; i<length; i++)	std[i] = convert2Standard(findCharacter(binArr[i]));
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