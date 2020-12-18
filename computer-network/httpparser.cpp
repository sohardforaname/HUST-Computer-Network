#include "httpparser.h"


HttpParser::HttpParser()
{

}

const std::string strtable[3] = { "function","URI","httpVersion" };

HttpParser::HttpParser(char* httprequestion)
{
	attributeMap.clear();
	char* beg = httprequestion, * ptr = beg;
	int cnt = 0;
	while (cnt < 3 && *ptr)
	{
		if (*ptr == ' ' || *ptr == '\r')
		{
			if (*ptr == '\r')
				*(ptr++) = 0;
			*(ptr++) = 0;
			attributeMap.insert({
				strtable[cnt++],
				std::unordered_set<std::string>{
					std::string(beg)
				}
				});
			beg = ptr;
		}
		else
			++ptr;
	}
	std::string attrkey;
	while (*ptr)
	{
		if (*ptr == ':')
		{
			*(ptr++) = 0;
			attrkey = std::string(beg);
			attributeMap.insert({ attrkey, std::unordered_set<std::string>() });
			beg = ptr;
		}
		else if (*ptr == ',' || *ptr == '\r')
		{
			if (*ptr == '\r')
				*(ptr++) = 0;
			*(ptr++) = 0;
			attributeMap[attrkey].insert(std::string(beg));
			beg = ptr;
		}
		else
			++ptr;
	}
}
HttpParser::~HttpParser()
{

}

attrType HttpParser::Get(const std::string& attribute)
{
	auto it = attributeMap.find(attribute);
	if (it == attributeMap.end())
		return attrType();
	return (*it).second;
}

void HttpParser::Update(const std::string& key, const std::string& newvalue, const std::string& orivalue)
{
	auto it = attributeMap.find(key);
	if (it == attributeMap.end())
		return;
	(*it).second.erase(orivalue);
	(*it).second.insert(newvalue);
}

#undef func