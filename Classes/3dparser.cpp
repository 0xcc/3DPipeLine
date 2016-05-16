#include "3dParser.h"
#include "3dmath.h"
//#include "platform/CCFileUtils.h"
#include "fileutil.h"
#include "cocos2d.h"

void Parser_V1::reset()
{
	// clear and reset buffer
	mLines.clear();
	mLineIdx = 0;

	// set comment
	mComment = "#";
}

bool Parser_V1::loadFile(char* filename)
{
	// reset file system
	reset(); // mLines.clear();

	// opens a file
	//std::string filetext = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
	std::string filetext = FileUtils::getInstance()->getStringFromFile(filename);
	if (filetext.empty())
	{
		CCLOG("Couldn't load file: %s", filename);
		return false;
	}

	// 分解各行
	split(filetext, "\n", mLines);

	return true;
}

void Parser_V1::setComment(const std::string &comment)
{
	mComment = comment;
}

bool Parser_V1::getLine(int mode)
{
	// check translation mode
	if (mode & PARSER_STRIP_EMPTY_LINES) // 去除空行
	{
		// get lines until we get a real one with data on it
		while (1)
		{
			// have we went to the end of the file without getting anything?
			// if ((string = fgets(buffer,PARSER_BUFFER_SIZE, fstream))==NULL)	break;
			if (mLineIdx >= mLines.size()) // 没有了
				return false;

			// we have something, strip ws from it 
			cur_line = trim_left(mLines[mLineIdx++]); // 如果是空行 则trim_left得到的是空字符串

			// eat up space
			//while(isspace(string[sindex]))
			//	sindex++;

			// is there anything left?
			if (!cur_line.empty()) // 不是空行
			{
				// copy the string into place
				//memmove((void *)buffer, (void *)&string[sindex],  (slength - sindex)+1 );
				//string = buffer;
				//slength = strlen(string); 

				// strip comments also?
				if (mode & PARSER_STRIP_COMMENTS) // 去除注释
				{
					// does this begin with a comment or end with a comment?
					//char *comment_string = strstr(string, comment);
					std::string::size_type idx = cur_line.find_first_of(mComment);

					// 3 cases, no comment, comment at beginning, comment at end
					//if (comment_string == NULL)
					//	break; // line is valid exit with line
					if (idx == std::string::npos) // 没有注释
						break;

					// compute index into string from beginning where comment begins
					// int cindex = (int)(comment_string - string);

					// comment at beginning then continue

					if (idx == 0) // 全是注释
						continue; // this line is a comment, ignore completely, get another 阻止下面的break
					else
					{
						// comment at end, strip it, insert null where it begins
						// comment_string[0] = 0;
						cur_line.resize(idx); // ret.erase(idx) 去除注释
						break;
					}
				}

				// exit loop, we have something :)
				break;
			}
			else
			{
				// 空行 继续循环 取下一行
			}			
		}
	}
	else // 保留空行 (也保留注释)
	{
		// just get the next line, don't worry about stripping anything
		// string = fgets(buffer,PARSER_BUFFER_SIZE, fstream);

		if (mLineIdx >= mLines.size()) // 没有了
			return false;

		// we have something, strip ws from it 
		cur_line = mLines[mLineIdx++]; // TODO: 这里可能为""
	}

	// was the line valid?
	if (!cur_line.empty())
	{
		// final stripping of whitspace
		if (mode & PARSER_STRIP_WS_ENDS) // 去除行尾空白
		{
			//StringLtrim(buffer);
			//StringRtrim(buffer);
			cur_line = trim(cur_line); // trim_right()即可 同时结果不可能为空
		}

		// return the pointer, copy of data already in buffer
		// return(string);
	}
// 	else
// 	{
// #ifdef PARSER_DEBUG_ON
// 		printf("\nEOF");
// #endif
// 		return(NULL);
// 	}

	return true;
}

bool Parser_V1::match(const char *string, const char *pattern, ...)
{
	// this function tries to match the pattern sent in pattern with
	// the sent string, the results are sent back to the sender in the
	// variable arguments as well as stored in the parameter passing area

	// string literal                        = ['string']
	// floating point number                 = [f]
	// integer number                        = [i]
	// match a string exactly ddd chars      = [s=ddd] 
	// match a string less than ddd chars    = [s<ddd] 
	// match a string greater than ddd chars = [s>ddd]
	// for example to match "vertex: 34.234 56.34 12.4
	// ['vertex'] [f] [f] [f]

	char token_type[PATTERN_MAX_ARGS];         // type of token, f,i,s,l
	char token_string[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE];   // for literal strings this holds them
	char token_operator[PATTERN_MAX_ARGS];     // holds and operators for the token, >, <, =, etc.
	size_t token_numeric[PATTERN_MAX_ARGS];      // holds any numeric data to qualify the token

	char buffer[PARSER_BUFFER_SIZE]; // working buffer 有时候不够长

	// a little error testing
	if ( (!string || strlen(string)==0 || strlen(string) >= PARSER_BUFFER_SIZE) || (!pattern || strlen(pattern)==0) )
		return false;

	// copy line into working area 如果当前行过长,会有问题的!! Fixed by mazi
	strcpy(buffer, string);

	size_t tok_start      = 0;
	size_t tok_end        = 0;
	size_t tok_restart    = 0;
	size_t tok_first_pass = 0;

	int	num_tokens = 0;

	// step 1: extract token list
	while(1)
	{
		// eat whitepace
		while(isspace(pattern[tok_start]) )
			tok_start++;

		// end of line?
		if (tok_start >= strlen(pattern))
			break;    

		// look for beginning of token '['
		if (pattern[tok_start] == '[')
		{
			// now look for token code
			switch(pattern[tok_start+1])
			{
			case PATTERN_TOKEN_FLOAT:  // float
				{
					// make sure token is well formed
					if (pattern[tok_start+2]!=']')
						return false; // error

					// advance token scanner
					tok_start+=3;

					// insert a float into pattern
					token_type[num_tokens] = PATTERN_TOKEN_FLOAT;  // type of token, f,i,s,l
					strcpy(token_string[num_tokens],"");           // for literal strings this holds them
					token_operator[num_tokens] = 0;                // holds and operators for the token, >, <, =, etc.
					token_numeric[num_tokens]  = 0; 

					// increment number of tokens
					num_tokens++;
					
					//CCLOG("Found Float token");
				} break;

			case PATTERN_TOKEN_INT:  // integer
				{
					// make sure token is well formed
					if (pattern[tok_start+2]!=']')
						return false; // error

					// advance token scanner
					tok_start+=3;

					// insert a int into pattern
					token_type[num_tokens] = PATTERN_TOKEN_INT;  // type of token, f,i,s,l
					strcpy(token_string[num_tokens],"");         // for literal strings this holds them
					token_operator[num_tokens] = 0;              // holds and operators for the token, >, <, =, etc.
					token_numeric[num_tokens]  = 0; 

					// increment number of tokens
					num_tokens++;

					//CCLOG("Found Int token");
				} break;

			case PATTERN_TOKEN_LITERAL: // literal string
				{
					// advance token scanner to begining literal string
					tok_start+=2;
					tok_end = tok_start;

					// eat up string
					while(pattern[tok_end]!=PATTERN_TOKEN_LITERAL)
						tok_end++;

					// make sure string is well formed
					if (pattern[tok_end+1]!=']')
						return false;

					// insert a string into pattern              

					// literal string lies from (tok_start - (tok_end-1)
					memcpy(token_string[num_tokens], &pattern[tok_start], (tok_end - tok_start) );
					token_string[num_tokens][(tok_end - tok_start)] = 0; // null terminate

					token_type[num_tokens] = PATTERN_TOKEN_LITERAL;  // type of token, f,i,s,'
					token_operator[num_tokens] = 0;                 // holds and operators for the token, >, <, =, etc.
					token_numeric[num_tokens]  = 0; 

					//CCLOG("Found Literal token = %s",token_string[num_tokens]);

					// advance token scanner
					tok_start = tok_end+2;

					// increment number of tokens
					num_tokens++;

				} break;

			case PATTERN_TOKEN_STRING: // ascii string varying length
				{
					// look for comparator
					if (pattern[tok_start+2] == '=' || 
						pattern[tok_start+2] == '>' ||
						pattern[tok_start+2] == '<' )
					{
						// extract the number
						tok_end = tok_start+3;

						while( isdigit(pattern[tok_end] ) )
							tok_end++;

						// check for well formed
						if (pattern[tok_end]!=']')
							return false;

						// copy number in ascii to string and convert to real number
						memcpy(buffer, &pattern[tok_start+3], (tok_end - tok_start) );
						buffer[tok_end - tok_start] = 0;

						// insert a string into pattern
						token_type[num_tokens] = PATTERN_TOKEN_STRING;     // type of token, f,i,s,l
						strcpy(token_string[num_tokens],"");               // for literal strings this holds them
						token_operator[num_tokens] = pattern[tok_start+2]; // holds and operators for the token, >, <, =, etc.
						token_numeric[num_tokens]  = atoi(buffer);
					} // end if
					else
						return false; // not well formed

					//CCLOG("Found String token, comparator: %c, characters: %d", token_operator[num_tokens], token_numeric[num_tokens]);

					// advance token scanner
					tok_start = tok_end+1; 

					// increment number of tokens
					num_tokens++;

				} break;             

			default: break;

			} // end switch

		} // end if

		// end of line?
		if (tok_start >= strlen(pattern))
			break;    

	} // end while

	//CCLOG("string to parse: %s", string);
	//CCLOG("Pattern to scan for: %s", pattern);
	//CCLOG("number of tokens found %d", num_tokens);

	// at this point we have the pattern we need to look for, so look for it
	int pattern_state = PATTERN_STATE_INIT; // initial state for pattern recognizer
	int curr_tok = 0;                 // test for num_tokens
	char token[PATTERN_BUFFER_SIZE];  // token under consideration

	// enter scan state machine
	while (1)
	{
		switch(pattern_state)
		{
		case PATTERN_STATE_INIT:
			{
				// initial state for pattern
				strcpy(buffer, string);

				tok_start      = 0; 
				tok_end        = 0;
				tok_restart    = 0;
				tok_first_pass = 1;
				curr_tok       = 0;

				// reset output arrays
				num_pints = num_pfloats = num_pstrings = 0;

				// transition to restart         
				pattern_state = PATTERN_STATE_RESTART;

			} break;

		case PATTERN_STATE_RESTART:
			{
				// pattern may still be here?
				curr_tok       = 0;
				tok_first_pass = 1;

				// error detection
				if (tok_end >= strlen(buffer))
					return false;

				// restart scanner after first token from last pass
				tok_start = tok_end = tok_restart;

				// start validating tokens
				pattern_state = PATTERN_STATE_NEXT;

			} break;

		case PATTERN_STATE_NEXT:
			{
				// have we matched pattern yet?
				if (curr_tok >= num_tokens)
				{   
					pattern_state = PATTERN_STATE_MATCH;       
				}
				else
				{
					// get next token
					if (tok_end >= strlen(buffer))
						return false;

					tok_start = tok_end;
					while(isspace(buffer[tok_start])) tok_start++;
					tok_end = tok_start;

					while(!isspace(buffer[tok_end]) && tok_end < strlen(buffer)) tok_end++;

					// copy token
					memcpy(token, &buffer[tok_start], tok_end - tok_start);
					token[tok_end - tok_start] = 0;

					// check for error
					if (strlen(token)==0) 
						return false;

					// remember position of first token, so we can restart after it on next pass
					// if need
					if (tok_first_pass)
					{
						tok_first_pass = 0; 
						tok_restart = tok_end;
					} // end if

					// we have the token, set state to check for that token
					switch(token_type[curr_tok])
					{
					case PATTERN_TOKEN_FLOAT:
						{
							pattern_state = PATTERN_STATE_FLOAT;
						} break;
					case PATTERN_TOKEN_INT:    
						{
							pattern_state = PATTERN_STATE_INT;
						} break;
					case PATTERN_TOKEN_STRING: 
						{
							pattern_state = PATTERN_STATE_STRING;
						} break;
					case PATTERN_TOKEN_LITERAL:
						{
							pattern_state = PATTERN_STATE_LITERAL;
						} break;

					default: break;

					} // end switch

				} // end else

			} break;

		case PATTERN_STATE_FLOAT:
			{
				// simply validate this token as a float
				float f = toFloat(token);

				if (f!=FLT_MIN)
				{
					pfloats[num_pfloats++] = f;

					// get next token
					curr_tok++;
					pattern_state = PATTERN_STATE_NEXT;
				} // end if                
				else
				{
					// error pattern doesn't match, restart
					pattern_state = PATTERN_STATE_RESTART;
				} // end else

			} break;

		case PATTERN_STATE_INT:
			{
				// simply validate this token as a int
				int i = toInt(token);

				if (i!=INT_MIN)
				{
					pints[num_pints++] = i;

					// get next token
					curr_tok++;
					pattern_state = PATTERN_STATE_NEXT;
				} // end if                
				else
				{
					// error pattern doesn't match, restart
					pattern_state = PATTERN_STATE_RESTART;
				} // end else

			} break;

		case PATTERN_STATE_LITERAL:
			{
				// simply validate this token by comparing to data in table
				if (strcmp(token, token_string[curr_tok]) == 0)
				{
					// increment number of pstrings found and insert into table
					strcpy(pstrings[num_pstrings++], token);

					// get next token
					curr_tok++;
					pattern_state = PATTERN_STATE_NEXT;
				} // end if                
				else
				{
					// error pattern doesn't match, restart
					pattern_state = PATTERN_STATE_RESTART;
				} // end else

			} break;

		case PATTERN_STATE_STRING:
			{
				// need to test for non-space chars 
				// get comparator

				switch(token_operator[curr_tok])
				{
				case '=':
					{
						// we need exactly
						if (strlen(token) == token_numeric[curr_tok])
						{
							// put this string into table
							strcpy(pstrings[num_pstrings++], token);

							// get next token
							curr_tok++;
							pattern_state = PATTERN_STATE_NEXT;
						} // end if    
						else
						{
							// error pattern doesn't match, restart
							pattern_state = PATTERN_STATE_RESTART;
						} // end else

					} break; 

				case '>':
					{
						// we need greater than
						if (strlen(token) > token_numeric[curr_tok])
						{
							// put this string into table
							strcpy(pstrings[num_pstrings++], token);

							// get next token
							curr_tok++;
							pattern_state = PATTERN_STATE_NEXT;
						} // end if  
						else
						{
							// error pattern doesn't match, restart
							pattern_state = PATTERN_STATE_RESTART;
						} // end else

					} break; 

				case '<':
					{
						// we need less than
						if (strlen(token) < token_numeric[curr_tok])
						{
							// put this string into table
							strcpy(pstrings[num_pstrings++], token);

							// get next token
							curr_tok++;
							pattern_state = PATTERN_STATE_NEXT;
						} // end if    
						else
						{
							// error pattern doesn't match, restart
							pattern_state = PATTERN_STATE_RESTART;
						} // end else

					} break; 

				default: break;

				} // end switch

			} break;

		case PATTERN_STATE_MATCH:
			{
				// we have matched the string, output vars into variable arg list
				//CCLOG("Pattern: %s matched!", pattern);
				return true;
			} break;

		case PATTERN_STATE_END: 
			{

			} break;

		default: break;

		} // end switch

	} // end while

} // end Pattern_Match