#include "handleresponse.h"

std::unordered_map<std::string, std::string> HandleResponse::contentTypeMap =
{
	{	"html", "text/html"			},
	{	"xml",  "text/xml"			},
	{	"ico",	"image/x-icon"		},
	{	"jpg",  "image/jpeg"		},
	{	"flac",	"audio/flac"		},
	{	"js",	"text/javascript"	}
};

std::string HandleResponse::GetTypeFromFilePath(const char* filePath)
{
	const char* ptr = filePath;
	int pos = 0;
	while (*ptr)
	{
		if (*ptr == '.')
			pos = ptr - filePath;
		++ptr;
	}
	return (*contentTypeMap.find(std::string(filePath + pos + 1))).second;
}

HandleResponse::HandleResponse(HandleRequest& request)
{
	buffer = new char[1 << 10];
	HttpGenerator* gen = new HttpGenerator;
	Reader* reader = new Reader;
	int fileSize = reader->OpenFile(request.GetFirstValue("URI").c_str());
	if (fileSize == -1)
		send(request.GetRequestSocket(), "HTTP/1.1 404 Not Found\r\n", 22, 0);
	else
	{
		std::string text = gen->GeneratorHttpHead(
			GetTypeFromFilePath(request.GetFirstValue("URI").c_str()),
			std::to_string(fileSize)
		);
		printf("generated text:\n%s", text.c_str());
		send(request.GetRequestSocket(), text.c_str(), text.size(), 0);
		while (!feof(reader->GetFilePointer()))
		{
			int blockSize = reader->ReadData();
			send(request.GetRequestSocket(), reader->GetBufferPointer(), blockSize, 0);
		}
		reader->CloseFile();
	}
	delete reader;
	delete gen;
}

HandleResponse::~HandleResponse()
{
	delete[]buffer;
}