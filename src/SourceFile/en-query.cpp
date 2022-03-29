#include<bits/stdc++.h>
using namespace std;

int main(){
	ofstream fout("../../docs/Query/Query/query.txt");
	string query;
	cout << "Enter a query (in English): ";
	
	getline(cin, query);
	istringstream iss(query);
	
	vector<string> words;
	string word;
	while(iss >> word) fout << word << endl;
	
	fout.close();
	
	return 0;
}
