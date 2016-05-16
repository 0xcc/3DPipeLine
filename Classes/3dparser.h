#ifndef _3D_PARSER_H_
#define _3D_PARSER_H_

#include <string>
#include <vector>

// getLine操作模式
#define PARSER_STRIP_EMPTY_LINES     1   // strips all blank lines
#define PARSER_LEAVE_EMPTY_LINES     2   // leaves empty lines
#define PARSER_STRIP_WS_ENDS         4   // strips ws space at ends of line
#define PARSER_LEAVE_WS_ENDS         8   // leaves it
#define PARSER_STRIP_COMMENTS        16  // strips comments out
#define PARSER_LEAVE_COMMENTS        32  // leaves comments in

#define PARSER_BUFFER_SIZE           512 // size of parser line buffer

// pattern language
#define PATTERN_TOKEN_FLOAT   'f'
#define PATTERN_TOKEN_INT     'i'
#define PATTERN_TOKEN_STRING  's'
#define PATTERN_TOKEN_LITERAL '\''

// state machine defines for pattern matching
#define PATTERN_STATE_INIT       0

#define PATTERN_STATE_RESTART    1
#define PATTERN_STATE_FLOAT      2
#define PATTERN_STATE_INT        3 
#define PATTERN_STATE_LITERAL    4
#define PATTERN_STATE_STRING     5
#define PATTERN_STATE_NEXT       6

#define PATTERN_STATE_MATCH      7
#define PATTERN_STATE_END        8

#define PATTERN_MAX_ARGS         16
#define PATTERN_BUFFER_SIZE      80

//
class Parser_V1
{
public:	
	// 重置
	void reset();

	// 载入待分析的文件 -> mLines
	bool loadFile(char* filename);

	// 设置注释字符串
	void setComment(const std::string &comment);

	// 获取下一行, 返回空字符串为结束
	bool getLine(int mode);

	// find pattern in line
	bool match(const char *string, const char *pattern, ...);

private:
	std::string mComment; // single line comment string 注释字符串

	std::vector<std::string> mLines;
	std::vector<std::string>::size_type mLineIdx;

public:
	// pattern matching parameter storage, easier that variable arguments
	// anything matched will be stored here on exit from the call to match()

	std::string cur_line; // getLine()返回true时, 为本次调用获取的行
	                      // getLine()返回false时, 不再有意义

	char  pstrings[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE]; // any strings
	int   num_pstrings;

	float pfloats[PATTERN_MAX_ARGS];        // any floats
	int   num_pfloats;

	int   pints[PATTERN_MAX_ARGS];          // any ints
	int   num_pints;
};

#endif // _MZ_PARSER_H_