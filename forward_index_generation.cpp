#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
	
	ifstream file;
	ofstream outputfile, outputlex;
	
	outputfile.open("forward_index.txt");
	outputlex.open("lexicon.txt");
	
	int docID = 1, fancy = -1, lastWordID = 0;
	map<string, int> lexicon;
	
	auto start = high_resolution_clock::now();
	
	while (docID < 16) {
		file.open(to_string(docID) + ".txt");
		
		if (file.fail()) {
			cout << "Error: Cannot open file: " << docID << endl;
			return -1;
		}
		
		if(docID % 5000 == 0) 
			cout << docID << endl;
		
		map<int,vector<int>> hits;
		int plain = 0, count = 0;
		string str, word;
		
		while (getline(file, str, '\n')) {
			
			stringstream str1(str);
			
			while (getline(str1, word, ' ')) {
				int wordID;
				map<string, int>::iterator lexitr = lexicon.find(word);
				
				if (lexitr != lexicon.end())
					wordID = lexitr->second;
				else {
					wordID = lastWordID + 1;
					lexicon.insert({ word, wordID });
					outputlex << word << endl << wordID << endl;
					lastWordID++;
				}
				
				map<int,vector<int>>::iterator hitsitr = hits.find(wordID);
				
				if (hitsitr == hits.end()) {
                    			hits.insert({wordID,vector<int>()});
                    			hitsitr = hits.find(wordID);
                		}
				
				if (count == 0 || count == 1 || count == 2 || count == 3)
					hitsitr->second.push_back(fancy);
				else if (count == 6) {
					hitsitr->second.push_back(plain);
					plain++;
				}
				
			}
			
			count++;	
		}
		
		outputfile << docID << endl;
		for(map<int,vector<int>>::iterator itr = hits.begin(); itr != hits.end(); itr++) {
            		outputfile << itr->first << endl;
            
            		for(int i = 0; i < itr->second.size(); i++)
                		outputfile << itr->second[i] << endl;
            
             		outputfile << "." << endl;
        	}
		outputfile << "_" << endl;

		docID++;
		file.close();
	}
	
	auto stop = high_resolution_clock::now();
    	auto duration = duration_cast<seconds>(stop - start);
    
    	cout << "Time taken: " << (duration.count()) << " seconds" << endl;
	
	outputfile.close();
	cout << "Forward index file finalized." << endl;
	
	outputlex << ".";
	outputlex.close();
	cout << "Lexicon file finalized." << endl;
	
	return 0;
}
