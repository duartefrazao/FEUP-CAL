#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

void preKmpMatcher(std::string pattern, int pi[]);
int kmpMatcher(std::string text, std::string pattern);
int editDistance(std::string pattern, std::string text);
