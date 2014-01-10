#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include "svm.h"

using namespace std;
using namespace cv;

typedef pair<int,int> pII;
#define MAXROW 16
#define MAXCOL 12

Mat doThreshold(Mat src,int v){
	Mat gray,bin;
	cvtColor(src,gray,CV_RGB2GRAY);
	threshold(gray, bin,v,255,CV_THRESH_BINARY);
	return bin;
}

void doDivide(Mat bin,Mat bin4[]){
	Mat aux=bin.colRange(0,12);
	bin4[0]=Mat::eye(aux.size(),aux.type());
	aux.copyTo(bin4[0]);
	aux=bin.colRange(13,22);
	bin4[1]=Mat::eye(aux.size(),aux.type());
	aux.copyTo(bin4[1]);
	aux=bin.colRange(23,32);
	bin4[2]=Mat::eye(aux.size(),aux.type());
	aux.copyTo(bin4[2]);
	aux=bin.colRange(33,43);
	bin4[3]=Mat::eye(aux.size(),aux.type());
	aux.copyTo(bin4[3]);
}

# define COMPONENT_PIXEL_LIMMIT 4
int connectedComponentWeight[100][100];
queue<pII> doBFS(queue<pII> &que,Mat_<uchar> &m){
	int r=m.rows,c=m.cols;
	queue<pII> save;
	if(!que.empty())	connectedComponentWeight[que.front().first][que.front().second]=-1;
	while(!que.empty()){
		pII a=que.front();save.push(a);que.pop();
		for(int i=-1;i<=1;i++)
			for(int j=-1;j<=1;j++)	if(i||j){
				int p=a.first+i,q=a.second+j;
				if(p<0||p>=r||q<0||q>=c)	continue;
				if(connectedComponentWeight[p][q])	continue;
				if(m(p,q)!=255){
					que.push(pII(p,q));
					connectedComponentWeight[p][q]=-1;
				}
			}
	}
	return save;
}

Mat_<uchar> doCleaning(Mat& M){
	int r=M.rows,c=M.cols;
	Mat_<uchar> m=M;
	memset(connectedComponentWeight,0,sizeof(connectedComponentWeight));
	queue<pII> que;
	for(int i=0;i<r;i++)	if(m(i,0)!=255){	
		if(connectedComponentWeight[i][0]==0){
			que.push(pII(i,0));
			que=doBFS(que,m);
			int v=que.size();
			while(!que.empty()){
				pII a=que.front();
				connectedComponentWeight[a.first][a.second]=v;
				que.pop();
			}
		}
	}
	for(int i=0;i<r;i++)	if(m(i,c-1)!=255){	
		if(connectedComponentWeight[i][c-1]==0){
			que.push(pII(i,c-1));
			que=doBFS(que,m);
			int v=que.size();
			while(!que.empty()){
				pII a=que.front();
				connectedComponentWeight[a.first][a.second]=v;
				que.pop();
			}
		}
	}
	for(int i=0;i<c;i++)	if(m(0,i)!=255){	
		if(connectedComponentWeight[0][i]==0){
			que.push(pII(0,i));
			que=doBFS(que,m);
			int v=que.size();
			while(!que.empty()){
				pII a=que.front();
				connectedComponentWeight[a.first][a.second]=v;
				que.pop();
			}
		}
	}
	for(int i=0;i<c;i++)	if(m(r-1,i)!=255){	
		if(connectedComponentWeight[r-1][i]==0){
			que.push(pII(r-1,i));
			que=doBFS(que,m);
			int v=que.size();
			while(!que.empty()){
				pII a=que.front();
				connectedComponentWeight[a.first][a.second]=v;
				que.pop();
			}
		}
	}
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)	if(m(i,j)!=255&&connectedComponentWeight[i][j]<COMPONENT_PIXEL_LIMMIT&&connectedComponentWeight[i][j]>0){
			m(i,j)=255;
		}
	return m;
}

Mat findCharacter(Mat& M){
	int r=M.rows,c=M.cols;
	Mat_<uchar> m=doCleaning(M);

	int minR=r,maxR=-1,minC=c,maxC=-1;
	for(int i=0;i<r;i++){
		bool flag0=true;
		for(int j=0;j<c;j++)	if(m(i,j)!=255){
			flag0=false;
			break;
		}
		if(!flag0){
			minR=min(minR,i);
			maxR=max(maxR,i);
		}
	}
	for(int j=0;j<c;j++){
		bool flag0=true;
		for(int i=0;i<r;i++)	if(m(i,j)!=255){
			flag0=false;
			break;
		}
		if(!flag0){
			minC=min(minC,j);
			maxC=max(maxC,j);
		}
	}
	if(minC>=0&&minC<c){
		//cout<<"Find character at ("<<minR<<","<<minC<<") ("<<maxR<<","<<maxC<<")"<<endl;
		return M.rowRange(minR,maxR+1).colRange(minC,maxC+1);
	}else{
		cerr<<"Error!: Unable to find any characters."<<endl;
		exit(1);
		//return M;
	}
}

Mat convert2Standard(Mat m){
	int r=m.rows,c=m.cols;
	if(r>MAXROW||c>MAXCOL){
		cerr<<"ERROR: Character size is too big. Row Size="<<r<<" ,Col Size="<<c<<endl;
		exit(1);
	}
	Mat s(MAXROW,MAXCOL,m.type());
	s=255;
	int rs=(MAXROW-r)/2,cs=(MAXCOL-c)/2;
	m.copyTo(s(Rect(cs,rs,c,r)));
	return s;
}

int getValue(Mat &a,int i,int j){
	if(a.ptr<uchar>(i)[j]<255)	return -1;
	else	return 1;
}

struct svm_model* model;
struct svm_node * node[4];
string trained_model("train.model");
string answer="";

int main(int argc, char *argv[])
{
	if(argc<2||argc>3){
		cerr<<"Usage: "<<argv[0]<<" ImageFilePath "<<" [libsvm_model]"<<endl;
		cerr<<endl;
		cerr<<"Note that if the libsvm model file is not specified, program will use train.model under current directory as default model."<<endl;
		cerr<<endl;
		return 1;
	}
	Mat src=imread(argv[1],CV_LOAD_IMAGE_UNCHANGED),bin,bin4[4],std[4];
	if(src.empty()){
		cerr<<"ERROR: Could not load source file: "<<argv[1]<<endl;
		return 1;
	}
	if(argc==3)	trained_model=argv[2];
	model=svm_load_model(trained_model.c_str());
	if(model==0){
		cerr<<"ERROR: Could not load libsvm model: "<<trained_model<<endl;
		exit(1);
	}

	bin=doThreshold(src,180);
	doDivide(bin,bin4);
	for(int i=0;i<4;i++)
		std[i]=convert2Standard(findCharacter(bin4[i]));
	
	for(int i=0;i<4;i++){
		int r=MAXROW,c=MAXCOL,tot=0;
		int size=r*c+2;
		
		node[i]=(struct svm_node *) malloc(size*sizeof(struct svm_node));
		for(int x=0;x<r;x++){
			for(int y=0;y<c;y++){
				node[i][tot].index=tot+1;
				node[i][tot++].value=getValue(std[i],x,y);
			}
		}
		node[i][tot++].index=-1;
		answer+=char(svm_predict(model,node[i]));
	}
	cout<<answer<<endl;
	for(int i=0;i<4;i++)	free(node[i]);

	return 0;
}
