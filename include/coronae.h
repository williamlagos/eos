/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2011 William Oliveira de Lagos <william.lagos1@gmail.com>
 *
 * Zhockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zhockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zhockon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <pthread.h>
#include <json/reader.h>
#include <jsonrpc.h>
#include <string>
#include <cstring>

#define HOST "127.0.0.1"
#define PORT 4004
#define JSONRPC_VERSION "2.0"
#define STD_ID 1
using namespace Json;
using std::string;

namespace Coronae{
struct Message{
	char queryMsg[100];
	char repseMsg[100];
};

class Extensor;

class Adapter{
private:
	string adapterName;
public:
	Adapter(string);
	~Adapter();
	bool Init();
	string methodCall(char*);
	string getAdapterName();
};

class Creator{
public:
	Creator();
};

class Connector{
public:
	Connector(){};
	bool baseMethod(const Value&,Value&);
};


//bool RemMethod(string);
bool CoronaeInit();
bool CoronaeCall();
void* ManagerThread(void*);
void* SubjectThread(void*);
//void* SendMethod(void*);
};
