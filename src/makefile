all : main.o svm.o SVMCapSolver.o SVMCapSolver_FRESHMAN.o SVMCapSolver_MAIL.o SVMCapSolver_XK.o
	g++ `pkg-config opencv --libs --cflags opencv` -o FDUCaptchaKiller svm.o SVMCapSolver.o SVMCapSolver_FRESHMAN.o SVMCapSolver_MAIL.o SVMCapSolver_XK.o main.o

main.o : main.cpp SVMCapSolver.h SVMCapSolver_XK.h SVMCapSolver_MAIL.h SVMCapSolver_FRESHMAN.h utils.h
	g++ -c main.cpp

svm.o : svm.cpp 
	g++ -c svm.cpp

SVMCapSolver.o : SVMCapSolver.cpp
	g++ -c SVMCapSolver.cpp

SVMCapSolver.o : SVMCapSolver_FRESHMAN.cpp
	g++ -c SVMCapSolver.cpp

SVMCapSolver.o : SVMCapSolver_MAIL.cpp
	g++ -c SVMCapSolver.cpp

SVMCapSolver.o : SVMCapSolver_XK.cpp
	g++ -c SVMCapSolver.cpp

clean :
	rm FDUCaptchaKiller main.o svm.o SVMCapSolver.o SVMCapSolver_FRESHMAN.o SVMCapSolver_MAIL.o SVMCapSolver_XK.o
