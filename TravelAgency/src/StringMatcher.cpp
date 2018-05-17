#include "StringMatcher.h"



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

	int t=text.length(),p=pattern.length();

	int vecSize= t>p?t:p;

	std::vector<int> d (p+1,0);
	for(uint j = 1;j<=p;j++ ) d.at(j)=j;

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


float numApproximateStringMatching(std::string filename,std::string toSearch) {
        // TODO
        return 0;
}
