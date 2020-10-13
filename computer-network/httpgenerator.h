#pragma once
#include <ctime>
#include <string>
class HttpGenerator
{
private:
	static std::string head;
	static std::string contentType;
	static std::string date;
	static std::string host;
	static std::string contentLength;
	static std::string newLine;

public:
	HttpGenerator();
	~HttpGenerator();
	std::string GeneratorHttpHead
	(
		const std::string& contentType,
		const std::string& contentSize
	);
};