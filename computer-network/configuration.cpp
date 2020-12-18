#include "configuration.h"

void Configuration::ReadConfigurationFile(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb");
	if (!fp)
	{
		printf("Cannot read configuration file\n");
		exit(-1);
	}
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = new char[len + 10];
	int fileSize = fread(buffer, 1, len + 10, fp);
	buffer[fileSize] = 0;
	configurationMap = new HttpParser(buffer);
	delete[]buffer;
	fclose(fp);
}
std::string Configuration::GetValue(const std::string& key)
{
	return *(configurationMap->Get(key)).begin();
}
Configuration::Configuration()
{
	configurationMap = nullptr;
}
Configuration::~Configuration()
{
	delete configurationMap;
}