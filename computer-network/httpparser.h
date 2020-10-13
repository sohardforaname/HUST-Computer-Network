#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
typedef std::unordered_map<std::string,
	std::unordered_set<std::string>>::const_iterator Iter;

class HttpParser
{
private:
	std::unordered_map<std::string, std::unordered_set<std::string>> attributeMap;

public:
	Iter Get(const std::string& attribute);
	HttpParser(char* httprequestion);
	HttpParser();
	~HttpParser();
};