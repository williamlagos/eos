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

#include "coronae.h"
using namespace Json;

namespace Coronae{
Adapter::Adapter(string name){
	networking::init();
	adapterName = name;
}
Adapter::~Adapter()
{
	networking::cleanup();
}

bool Adapter::Init()
{
	pthread_t adapterServerThread;
	pthread_create(&adapterServerThread,NULL,
		       ManagerThread,NULL);
}
string Adapter::methodCall(char* methodName){
	//SubjectThread Init
	/*pthread_t adapterClientThread;
	pthread_create(&adapterClientThread,NULL,
		       SubjectThread,(void*)methodName);
	return string(methodName);*/
	
	FastWriter writer;
	Reader read;
	Value query;
	string quryString;
	string rpseString;
	query["jsonrpc"] = JSONRPC_VERSION; query["id"] = STD_ID;
	query["method"] = methodName;
	quryString = writer.write(query);
	Rpc::TcpClient* Sender = new Rpc::TcpClient(HOST,PORT);
	Sender->Connect();
	Sender->Send(quryString);
	Sender->Recv(rpseString);
	Value value;
	read.parse(rpseString,value);
	rpseString = value["result"].asString();
	return rpseString;
}
string Adapter::getAdapterName()
{
	return adapterName;
}


Creator::Creator(){
	
}


bool Connector::baseMethod(const Value& root, Value& response)
{
	response["jsonrpc"] = JSONRPC_VERSION; response["id"] = STD_ID;
	response["result"] = "Coronae has received the message.";
	return true;
}


bool CoronaeInit()
{
	networking::init();
	pthread_t coronaeServerThread;
	pthread_create(&coronaeServerThread,NULL,
		       ManagerThread,NULL);
	pthread_join(coronaeServerThread,NULL);
	networking::cleanup();
}
bool CoronaeCall()
{
	pthread_t coronaeClientThread;
	pthread_create(&coronaeClientThread,NULL,
		       SubjectThread,NULL);
}
void* ManagerThread(void* stdParam)
{
	Connector Conn;
	Rpc::TcpServer* Mngr = new Rpc::TcpServer(HOST,PORT);
	Mngr->AddMethod(new Rpc::RpcMethod<Connector>(Conn,
			&Connector::baseMethod,string("base_method")));
	Mngr->Bind();
	Mngr->Listen();
	while(1){ Mngr->WaitMessage(1000); }
	pthread_exit(Mngr);
}
void* SubjectThread(void* msgParam)
{	
	Message* Msg = (Message*)msgParam;
	Rpc::TcpClient* Subj = new Rpc::TcpClient(HOST,PORT);
	string queryString = string(Msg->queryMsg);
	string responseString;
	Subj->Connect();
	Subj->Send(queryString);
	Subj->Recv(responseString);
	strcpy(Msg->repseMsg,responseString.c_str());
	msgParam = Msg;
	pthread_exit(NULL);
}
};
