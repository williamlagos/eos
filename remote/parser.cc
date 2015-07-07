/*
 * This file is part of Efforia Open Source Initiative.
 * 
 * Copyright (C) 2009-2015 William Oliveira de Lagos <william@efforia.com.br>
 *
 * Efforia OSI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Efforia OSI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Efforia OSI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "parser.h"

void load_json(const char* name)
{
	std::ifstream stream;
	stream.open("elements.json",std::ifstream::in);
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

Reader::Reader()
{

}

void Reader::parse(std::string str,Value &val)
{

}

FastWriter::FastWriter()
{

}

std::string FastWriter::write(Value &val)
{

}
