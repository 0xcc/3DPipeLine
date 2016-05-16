#ifndef _FILEUTIL_H
#define _FILEUTIL_H
#include <string>


class FileUtils
{
public:
	static FileUtils* getInstance();
	std::string getStringFromFile(char* filename);
protected:
private:
};


#endif