#include<bits/stdc++.h>
using namespace std;

string path = "../../docs/English/EnglishNewsStemmed-stop/";

string queryPath = "../../docs/Query/QueryStemmed/query-stop.txt";

struct article{
	string filename;
	long long int total_word = 0;
	long long int tfsize;
	double tfcosSim = 0.0;
	double tfeucDis = 0.0;
	double tfidfcosSim = 0.0;
	double tfidfeucDis = 0.0;
	vector <double> tf;
	vector <double> tfidf;
	double tfLength = 0.0;
	double tfidfLength = 0.0;
	vector <bool> exist;
};

struct query{
	long long int tfsize;
	vector <double> tf;
	vector <double> tfidf;
	double tfLength = 0.0;
	double tfidfLength = 0.0;
	vector <bool> exist;
};

map < string, long long int > idx;

vector <double> idf;

vector <article> articles;

long long int word_count = 0;

long long int file_count = 0;

long long int show;

bool tfcos(article a, article b){ return a.tfcosSim > b.tfcosSim; }

bool tfidfcos(article a, article b){ return a.tfidfcosSim > b.tfidfcosSim; }

bool tfeuc(article a, article b){ return a.tfeucDis < b.tfeucDis; }

bool tfidfeuc(article a, article b){ return a.tfidfeucDis < b.tfidfeucDis; }


int main(){

	cout << "Constructing vectors... " << flush;
	
	//constructing vectors
	//query
	ifstream fin(queryPath);
	
	string word;
	query q;
	
	while(fin >> word){
		if(idx.find(word) != idx.end()){
 			q.tf[idx[word]] += 1.0;
 			if(!q.exist[idx[word]]){
 				q.exist[idx[word]] = true;
 				idf[idx[word]] += 1.0;
 			}
		}else{
        	idx.insert({word, word_count});
        	q.tf.push_back(1.0);
        	q.exist.push_back(true);
        	idf.push_back(1.0);
        	word_count++;
        }
	}
	q.tfsize = word_count;
	fin.close();
	
	//articles
    for (const auto & entry : filesystem::directory_iterator(path)){
    	
    	file_count++;
    	
    	article art;
    	
    	string fname = entry.path();
    	fname.erase(0, path.size());
    	art.filename = fname;
    	
        ifstream fin(entry.path());
        
        art.tf.resize(word_count, 0);
        art.exist.resize(word_count, false);
        
        while(fin >> word){
        
        	art.total_word++;
        	
        	if(idx.find(word) != idx.end()){
        		art.tf[idx[word]] += 1.0;
        		if(!art.exist[idx[word]]){
 					art.exist[idx[word]] = true;
 					idf[idx[word]] += 1.0;
 				}
        	}else{
        		idx.insert({word, word_count});
        		art.tf.push_back(1.0);
        		art.exist.push_back(true);
        		idf.push_back(1.0);
        		word_count++;
        	}
        }
        art.tfsize = word_count;
        articles.push_back(art);
        fin.close();
    }
    cout << "done" << endl;
    
    cout << "Calaulating scores... " << flush;
    //calculate vectors and scores
    //idf
    for(long long int i=0; i<word_count; i++){
    	idf[i] = log((double)file_count/idf[i]);
    }
    
    //query
    q.tfidf.resize(word_count, 0);
    q.tf.resize(word_count,  0);
    for(long long int i=0; i<q.tfsize; i++){
    	q.tfidf[i] = idf[i] * q.tf[i];
    }
    
    //articles
    for(long long int i=0; i<file_count; i++){
    	articles[i].tfidf.resize(word_count, 0);
    	articles[i].tf.resize(word_count,  0);
    	double atfl = 0.0, atfidfl = 0.0, qtfl = 0.0, qtfidfl = 0.0;
    	for(long long int j=0; j<articles[i].tfsize; j++){
    		articles[i].tfidf[j] = idf[j] * articles[i].tf[j];
    		if(j<q.tfsize){
    			qtfl += pow(q.tf[j], 2.0);
    			qtfidfl += pow(q.tfidf[j], 2.0);
    			atfl += pow(articles[i].tf[j], 2.0);
    			atfidfl += pow(articles[i].tfidf[j], 2.0);
	    		articles[i].tfcosSim += articles[i].tf[j] * q.tf[j];
	    		articles[i].tfidfcosSim += articles[i].tfidf[j] * q.tfidf[j];
	    		articles[i].tfeucDis += pow((articles[i].tf[j] - q.tf[j]), 2.0);
	    		articles[i].tfidfeucDis += pow((articles[i].tfidf[j] - q.tfidf[j]), 2.0);
    		}else{
    			atfl += pow(articles[i].tf[j], 2.0);
    			atfidfl += pow(articles[i].tfidf[j], 2.0);
				articles[i].tfeucDis += pow(articles[i].tf[j], 2.0);
	    		articles[i].tfidfeucDis += pow(articles[i].tfidf[j], 2.0);
    		}
    	}
    	q.tfLength = sqrt(qtfl);
    	q.tfidfLength = sqrt(qtfidfl);
    	articles[i].tfLength = sqrt(atfl);
    	articles[i].tfidfLength = sqrt(atfidfl);
    	articles[i].tfcosSim /= (q.tfLength * articles[i].tfLength);
    	articles[i].tfidfcosSim /= (q.tfidfLength * articles[i].tfidfLength);
    	articles[i].tfeucDis = sqrt(articles[i].tfeucDis);
    	articles[i].tfidfeucDis = sqrt(articles[i].tfidfeucDis);
    }
    cout << "done" << endl << endl;
    
    cout << "Input the number of top X to show up (It's an poistive integer less than " << file_count << "): " << flush;
    cin >> show;
    
    //tf-cosSim
    cout << endl << endl << "-------------------------------------------------" << endl;
    cout << "Term Frequency (TF) Weighting + Cosine Similarity" << endl << endl;
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfcos);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfcosSim << endl;
    
    //tf-cosSim re-score
    cout << endl << "Using first result to re-score... " << flush;
    
    query newq = q;
    for(int i=0; i<articles[0].tfsize; i++){
    	if(articles[0].exist[i]){
    		newq.tf[i] += 0.5*articles[0].tf[i];
    	}
    }
    
    for(long long int i=0; i<file_count; i++){
    	articles[i].tfcosSim = 0.0;
    	double qtfl = 0.0;
    	for(long long int j=0; j<articles[i].tfsize; j++){
    		qtfl += pow(newq.tf[j], 2.0);
	    	articles[i].tfcosSim += articles[i].tf[j] * newq.tf[j];
    	}
    	articles[i].tfcosSim /= (sqrt(qtfl) * articles[i].tfLength);
    }
    cout << "done" << endl << endl;
    
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfcos);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfcosSim << endl;
    
    
    //tf-eucDis
    cout << endl << endl << "-------------------------------------------------" << endl;
    cout << "Term Frequency (TF) Weighting + Euclidean Distance" << endl << endl;
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfeuc);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfeucDis << endl;
    
    //tf-eucDis re-score
    cout << endl << "Using first result to re-score... " << flush;
    
    newq = q;
    for(int i=0; i<articles[0].tfsize; i++){
    	if(articles[0].exist[i]){
    		newq.tf[i] += 0.5*articles[0].tf[i];
    	}
    }
    
    for(long long int i=0; i<file_count; i++){
    	articles[i].tfcosSim = 0.0;
    	for(long long int j=0; j<articles[i].tfsize; j++){
    		articles[i].tfeucDis += pow((articles[i].tf[j] - newq.tf[j]), 2.0);
    	}
    	articles[i].tfeucDis = sqrt(articles[i].tfeucDis);
    }
    cout << "done" << endl << endl;
    
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfeuc);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfeucDis << endl;
    

	
    //tfidf-cosSim
    cout << endl << endl << "-------------------------------------------------" << endl;
    cout << "TF-IDF Weighting + Cosine Similarity" << endl << endl;
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfidfcos);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfidfcosSim << endl;
    
    
    //tfidf-cosSim re-score
    cout << endl << "Using first result to re-score... " << flush;
    
    newq = q;
    for(int i=0; i<articles[0].tfsize; i++){
    	if(articles[0].exist[i]){
    		newq.tfidf[i] += 0.5*articles[0].tfidf[i];
    	}
    }
    
    for(long long int i=0; i<file_count; i++){
    	articles[i].tfidfcosSim = 0.0;
    	double qtfidfl = 0.0;
    	for(long long int j=0; j<articles[i].tfsize; j++){
    		qtfidfl += pow(newq.tfidf[j], 2.0);
	    	articles[i].tfidfcosSim += articles[i].tfidf[j] * newq.tfidf[j];
    	}
    	articles[i].tfidfcosSim /= (sqrt(qtfidfl) * articles[i].tfidfLength);
    }
    cout << "done" << endl << endl;
    
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfidfcos);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfidfcosSim << endl;
    
    //tfidf-eucDis
    cout << endl << endl << "-------------------------------------------------" << endl;
    cout << "TF-IDF Weighting + Euclidean Distance" << endl << endl;
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfidfeuc);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfidfeucDis << endl;
    
    
    //tfidf-eucDis re-score
    cout << endl << "Using first result to re-score... " << flush;
    
    newq = q;
    for(int i=0; i<articles[0].tfsize; i++){
    	if(articles[0].exist[i]){
    		newq.tfidf[i] += 0.5*articles[0].tfidf[i];
    	}
    }
    
    for(long long int i=0; i<file_count; i++){
    	articles[i].tfidfcosSim = 0.0;
    	for(long long int j=0; j<articles[i].tfsize; j++){
    		articles[i].tfidfeucDis += pow((articles[i].tfidf[j] - newq.tfidf[j]), 2.0);
    	}
    	articles[i].tfidfeucDis = sqrt(articles[i].tfidfeucDis);
    }
    cout << "done" << endl << endl;
    
    cout << "NewsID" << "\t\t\t" << "Score" << endl;
    cout << "------------" << "\t\t" << "------------" << endl;
    sort (articles.begin(), articles.end(), tfidfeuc);
    for(int i=0; i<show; i++) cout << articles[i].filename << "\t\t" << articles[i].tfidfeucDis << endl;

}
