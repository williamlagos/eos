/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2011 William Oliveira de Lagos <william.lagos1@gmail.com>
 *
 * Zhockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either infoVersionsion 3 of the License, or
 * (at your option) any later infoVersionsion.
 *
 * Zhockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zhockon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zhockon.h>
#include <Python.h>

namespace Zhockon{
Structure baseStr;
Info infoVer;

string Error::getMessage()
{
  
}


Module::Module(string modName, bool modStatus=true)
{
	name = modName;
	status = modStatus;
}

string Module::getName()
{
	return name;
}
bool Module::getStatus()
{
	return status;
}
void Module::setStatus()
{
	status = ~status;
}
int Module::operator > (Module mod)
{
	return name.compare(mod.getName());
}
int Module::operator < (Module mod)
{
	return mod.getName().compare(name);
}


int Package::addModule(Module* mod)
{
	modules.push_back((*mod));
	return 0;
}
int Package::numOfModules()
{
	return modules.size();
}

bool ZhockonInit()
{
	Node* root = baseStr.mainTree.getRoot();
	/*string Packages[3] = {"Zhockon","ZKN","Coronae"};
	newPackage((string*)Packages,3);*/
	readConfig();
	return true;
}

int newPackage(string* name, int times)
{
	for(int i = 0; i < times; i++){
		Package lay;
		baseStr.pakStack.push(lay);
	}
}
Package accessPackage()
{
	stack<Package> stackTemp = baseStr.pakStack;
	while(!stackTemp.empty()){
		//if(name.compare(stackTemp.top().getName()) == 0){
			return stackTemp.top();
		//}
	}
	stackTemp.pop();
}
void createGame(string gamName)
{
	Module *newGame = new Module(gamName,true);
	baseStr.mainTree.insert(newGame);
}
void deleteGame(string gamName)
{
	baseStr.mainTree.remove(gamName);
}
int exeModule(string name)
{
	Module mod(name,true);
	baseStr.modExec.push(mod);
}
void listModules()
{
	baseStr.mainTree.print(1);
}
void readConfig()
{
	/*Python p("Configuration");
	p.loadModule("ConfigParser");
	p.newObject("ConfigParser","");*/
}
};