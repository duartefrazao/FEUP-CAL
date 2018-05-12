#include "StringMatcher.h"


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
        // TODO
        return 0;
}

float numApproximateStringMatching(std::string filename,std::string toSearch) {
        // TODO
        return 0;
}
