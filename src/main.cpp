
#include "SVMCapSolver.h"
#include "SVMCapSolver_XK.h"
#include "SVMCapSolver_MAIL.h"
#include "SVMCapSolver_FRESHMAN.h"
#include "utils.h"

string trained_model;
const string s_arg_mode = "-m";
const string s_xk = "xk";
const string s_mail = "mail";
const string s_freshman = "freshman";

int main(int argc, char *argv[])
{
	if(argc<3||argc>4){
		cerr<<"Usage: "<<argv[0]<<" xk|mail|freshman ImageFilePath "<<" [libsvm_model]"<<endl;
		cerr<<endl;
		cerr<<"Note that if the libsvm model file is not specified, program will use the default model of current working mode under svm model directory."<<endl;
		cerr<<endl;
		return 1;
	}
//	if (string(argv[1])!=s_arg_mode){
//		cerr << "ERROR: Please select working mode with -m" << endl;
//		return 1;
//	}
	if (string(argv[1]) == s_xk){
		trained_model = "model/xk.model";
	}else if (string(argv[1])==s_mail){
		trained_model = "model/mail.model";
	}else if (string(argv[1]) == s_freshman){
		trained_model = "model/freshman.model";
	}else{
		cerr << "ERROR: Unknown mode" << endl;
		return 1;
	}
	Mat src=imread(argv[2],CV_LOAD_IMAGE_UNCHANGED);
	if(src.empty()){
		cerr<<"ERROR: Could not load source file: "<<argv[2]<<endl;
		return 1;
	}
	if (argc == 4)	trained_model = argv[3]; 
	svm_model* model=svm_load_model(trained_model.c_str());
	if(model==0){
		cerr<<"ERROR: Could not load libsvm model: "<<trained_model<<endl;
		return 1;
	}
	if (string(argv[1]) == s_xk){
		SVMCapSolver_XK svmsolver(*model);
		cout << svmsolver.solve(src) << endl;
	}else if (string(argv[1]) == s_mail){
		SVMCapSolver_MAIL svmsolver(*model);
		cout << svmsolver.solve(src) << endl;
	}if (string(argv[1]) == s_freshman){
		SVMCapSolver_FRESHMAN svmsolver(*model);
		cout << svmsolver.solve(src) << endl;
	}

	return 0;
}
