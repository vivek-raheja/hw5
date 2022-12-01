// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
#include <cstddef>
#include <cstring>

using namespace std;


// Add prototypes of helper functions here
uint8_t* convertToBytes(string str);
set<string> findMatches(set<string> arrMatch, string floating);
bool findOneMatch(string str, char ch);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floatVal,
    const std::set<std::string>& dict)
{
	set<string> outSet;
	
	set<string> strMatch;
	
	uint8_t count;
	for(auto& str: dict)
	{
		//Get the same length strings
		if(str.length() == in.length()) {
			//Now match the patterns
			bool match = true;
			for(count=0;count<str.length();count++) {
				if(str[count] != in[count] && in[count] != '-') {
					match = false;
					break;
				}
			}
			if(match) {
				strMatch.insert(str);
			}
		}
	}
	
	outSet = findMatches(strMatch, floatVal);
	
	// Add your code here
	return outSet;
}

// Define any helper functions here
uint8_t* convertToBytes(string str) {
	uint8_t bytes[str.length()];
    memcpy(bytes, str.data(), str.length());
    uint8_t* newBytes = bytes;	
    return newBytes;
}

bool findOneMatch(string str, char ch) {
	size_t found = str.find(ch);
	return found != string::npos;
}


set<string> findMatches(set<string> arrMatch, string floating) {
	set<string> matches;
	
	uint8_t count = 0;
	
	for(auto& str: arrMatch) {
		bool match = true;
		for(count=0;count<floating.length();count++) {
			match &= findOneMatch(str, floating[count]);
			if(match == false)
				break;
		}
		if(match)
			matches.insert(str);
	}
	
	return matches;
}
