#pragma once
#include "HandleRequest.h"
#include "HttpGenerator.h"
#include "Reader.h"

class HandleResponse
{
private:
	char* buffer;
	static std::unordered_map<std::string, std::string> contentTypeMap;

public:
	HandleResponse(HandleRequest& request);
	std::string GetTypeFromFilePath(const char* filePath);
	~HandleResponse();
};