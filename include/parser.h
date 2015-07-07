#include <libjson.h>
#include <fstream>
#include <map>

void load_json(const char* name);

typedef std::map<std::string,const char*> Value;

class FastWriter{
public:
	FastWriter();
	std::string write(Value&);
};

class Reader{
public:
	Reader();
	void parse(std::string,Value&);
};
