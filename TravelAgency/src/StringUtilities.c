#include "StringUtilities.h"

using namespace std;

/*
 * Based on the fact that the characters we wanted to extract are represented by two bytes and the first one is negative we:
 *	-Convert the invalid chars to their code
 *	-Iterate through the string, if we find a negative char it means it's one of the strange characters
 *	-We discard that char (first byte)
 *	-Then substitute the second byte by the desired char replacer
 */
string preProcessingChars(string text){

	cout << "String before pre processing: "<< text << endl;

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
	cout << "String after  pre processing: "<< text << endl;
	cout << "---------"<< endl;

	return text;
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
		result.push_back(temp);
	}


	return result;
}