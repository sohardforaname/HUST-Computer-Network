#pragma once
#include <cstdio>
#include "HttpParser.h"
class Configuration
{
private:
	HttpParser* configurationMap;

public:
	void ReadConfigurationFile(const char* filePath);
	std::string GetValue(const std::string& key);
	Configuration();
	~Configuration();
};