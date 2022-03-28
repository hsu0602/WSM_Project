#include<bits/stdc++.h>
using namespace std;

int main(){
	ofstream fout("../docs/Query/query.txt");
	string query;
	cout << "Enter a query: ";
	
	getline(cin, query);
	istringstream iss(query);
	
	vector<string> words;
	string word;
	while(iss >> word) fout << word << endl;
	
	return 0;
}
