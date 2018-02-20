#pragma once

#include <iostream>
#include <string>
#include <map>
using namespace std;
long vsize;
char* file;
long runs;

class Function
{
	public:
		char* operator()(char* filepath) {return this->set(filepath);}
		virtual ~Function() {}
	private:
		virtual char* set(char* filepath) = 0;
		
};
class Runs: public Function {virtual char* set(char* arg)
	{
		runs = atoi(arg);
		return arg;
	}
};
class FilePath: public Function {virtual char* set(char* arg) 
	{
		file=arg;
		return file;
	}
};
class VSize: public Function {virtual char* set(char* arg) 
	{
		vsize = atoi(arg);
		return arg;
	}
};
class FuncMap
{
	public:
		FuncMap()
		{
			functions["-file"] = &fileFunc;
			functions["-vsize"] = &vsizeFunc;
			functions["-runs"] = &runsFunc;
		}
		Function* getFunction(string command) const
		{
			Function* result = NULL;
			map<string,Function*>::const_iterator find;
			if ((find = functions.find(command)) != functions.end())
			{
				result = find->second;
			}
			return result;
		}
	private:
	FilePath fileFunc;
	VSize vsizeFunc;
	Runs runsFunc;
	map<string, Function*> functions;
};

FuncMap functions;