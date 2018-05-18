

#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#define HIT_RATE 0.8

void preKmpMatcher(std::string pattern, int pi[]);
int kmpMatcher(std::string text, std::string pattern);
int editDistance(std::string pattern, std::string text);
std::string preProcessingChars(std::string text);
std::vector<std::string> preProcessString(std::string text);
bool isCommmonConjunction(std::string text);

