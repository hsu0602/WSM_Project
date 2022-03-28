#ifndef __VECTORSPACEMODEL__H
#define __VECTORSPACEMODEL__H

#include <bits/stdc++.h>
using namespace std;

class VSM{
	
	public:
		
		VSM(){}
		
		vector<vector<double> > tf();
		
		vector<vector<double> > idf();
		
		vector<double> cosSim(vector<string>);
		
		vector<double> eudDis();
		
	private:

		map < string, int > index;
		vector < >

}


#endif
