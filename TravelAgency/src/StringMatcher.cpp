#include "StringMatcher.h"

using namespace std;

/*
 * Based on the fact that the characters we wanted to extract are represented by two bytes and the first one is negative we:
 *	-Convert the invalid chars to their code
 *	-Iterate through the string, if we find a negative char it means it's one of the strange characters
 *	-We discard that char (first byte)
 *	-Then substitute the second byte by the desired char replacer
 */
string preProcessingChars(string text){

	//cout << "String before pre processing: "<< text << endl;

	//Get ascii code from the invalid characters
	string invalidChars= "ãâáàÁÀÃÂéêÉÊíÍóòôõÓÒÔÕũûúŨÛÚçÇ";
	string valid=  		 "aaaaaaaaeeeeiioooooooouuuuuucc";
	vector<char> invalid;
	for(int i = 1; i < invalidChars.length();i+=2)invalid.push_back((int) invalidChars[i]);

	//Transform valid letters to lower case
	transform(text.begin(), text.end(), text.begin(), ::tolower);


	//Remove invalid chars
	for(auto it=text.begin(); it !=text.end();it++){
		for(uint j = 0; j < valid.length();j++){
			if((int) (*it)<0 && (int) (*(it +1))==invalid.at(j)){

				it=text.erase(it);
				(*it)=valid[j];
				break;
			}
		}
	}
	//cout << "String after  pre processing: "<< text << endl;
	//cout << "---------"<< endl;

	return text;
}

bool isCommmonConjunction(string text){
	vector<string> conjs{"de","da","e", "a","do","o"};


	for(int i = 0; i < conjs.size();i++){
		if(conjs.at(i).length() ==text.length() &&kmpMatcher(text,conjs.at(i))>0) return true;
	}
	return false;
}

vector<string> preProcessString(string text){

	for(int i =0; i < text.length();i++){
		if(text[i]=='-')text[i]=' ';
	}

	vector<string> result;
	char trash;
	stringstream s(text);
	while(!s.eof()){
		string temp;
		s>>temp;
		if(!isCommmonConjunction(temp))
			result.push_back(temp);
	}


	return result;
}

std::vector<unsigned int> naiveMatcher(std::string text, std::string pattern){
	std::vector<unsigned int> occurrences;

	for(int i = 0; i < text.length(); i++){
		bool match = true;
		for(int j = 0; j < pattern.length(); j++){
			if (text[i + j] != pattern[j]){
				match = false;
				break;
			}
			if (match)
				occurrences.push_back(i);
		}
	}
	return occurrences;
}

void preKmpMatcher(std::string pattern, int pi[]) {
	int m = pattern.length(), k = 0;
	pi[0] = 0;
	for (int q = 1; q < m; q++) {
		if (pattern[q] == pattern[k]) {
			k++;
			pi[q] = k;
		} else {
			if (k != 0)
				k = pi[k - 1];
			else
				pi[q] = 0;
		}

	}
}

int kmpMatcher(std::string text, std::string pattern) {
	int length = pattern.length();
	int pi[length];
	memset(pi, 0, length * sizeof(int));
	preKmpMatcher(pattern, pi);
	int nOccurrences = 0;

	int n = text.length(), m = pattern.length();
	int q = 0, i = 0;

	while( i < n){
		if (pattern[q] == text[i]){
			q++;
			i++;
		}
		if (q == m){
			q = pi[q-1];
			nOccurrences++;
		}
		else if (i < n && pattern[q] != text[i]){
			if (q != 0)
				q= pi[q-1];
			else i++;
		}

	}
	return nOccurrences;
}


int numStringMatching(std::string filename,std::string toSearch) {
        // TODO
}


int editDistance(std::string pattern, std::string text) {

	int t = text.length(), p = pattern.length();

	int vecSize = t>p ? t : p;

	std::vector<int> d (p+1,0);
	for(uint j = 1; j <= p; j++) d.at(j)=j;

	int old,newV;


	for(uint i =1;i<= t;i++){
		old=d.at(0);
		d.at(0)= i;

		for(uint j=1; j<=p;j++){
			if(pattern.at(j-1) == text.at(i-1))newV=old;
			else newV=1+std::min(old,std::min(d.at(j),d.at(j-1)));

			old=d.at(j);
			d.at(j) = newV;
		}
	}
	return d.at(p);
}
