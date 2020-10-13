#include "httpgenerator.h"

std::string HttpGenerator::head = "HTTP/1.1 200 OK";
std::string HttpGenerator::contentType = "Content-Type:";
std::string HttpGenerator::date = "Date:";
std::string HttpGenerator::host = "Server:";
std::string HttpGenerator::contentLength = "Content-Length:";
std::string HttpGenerator::newLine = "\r\n";

std::string HttpGenerator::GeneratorHttpHead
(
	const std::string& contentType,
	const std::string& contentSize
)
{
	time_t curTime;
	time(&curTime);
	std::string str("");
	str += head + newLine;
	str += (this->contentType + contentType + newLine);
	str += (this->contentLength + contentSize + newLine);
	str += (this->host + "localhost" + newLine);
	str += (this->date + ctime(&curTime) + newLine);
	return str;
}

HttpGenerator::HttpGenerator() {}
HttpGenerator::~HttpGenerator() {}
