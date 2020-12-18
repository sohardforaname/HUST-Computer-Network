#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
typedef std::unordered_set<std::string> attrType;

class HttpParser
{
private:
	std::unordered_map<std::string, std::unordered_set<std::string>> attributeMap;

public:
	attrType Get(const std::string& attribute);
	void Update(const std::string& key, const std::string& newvalue, const std::string& orivalue);
	HttpParser(char* httprequestion);
	HttpParser();
	~HttpParser();
};