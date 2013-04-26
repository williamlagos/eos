#include "elements.h"
using namespace std;

void load_json(const char* name)
{
	std::ifstream stream;
	stream.open("elements.json",ifstream::in);
	std::string line,buffer;
	if(stream.is_open()){
		while(stream.good()){
			getline(stream,line);
			buffer.append(line);
		}
		stream.close();
	}
	JSONNODE* node = json_parse_unformatted(buffer.c_str());
	JSONNODE_ITERATOR i = json_begin(node);
	while(i != json_end(node)){
		if(i == JSON_NULL) break;
		else if(json_type(*i) == JSON_NODE) continue;
		printf("%s",json_name(*i)); i++;
	}
}
