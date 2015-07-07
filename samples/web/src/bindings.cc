/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2010 William Oliveira de Lagos <williamx64@gmail.com>
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

#include <zhockon.h>
using namespace Zhockon;

Binding::Binding(string n) : Module(n,status)
{
	name = n;
	status = true;
}


Python::Python(string n) : Binding(n)
{
	name = n;
}
	
int Python::helloPython()  
{
	Py_Initialize();
	PyRun_SimpleString("print 'Hello Python C/API!'");
	Py_Finalize();
	return 0;
}
int Python::loadModule(string modName)
{
	
	PyObject *name, *module;
	Py_Initialize();
	name = PyString_FromString(modName.c_str());
	module = PyImport_Import(name);
	Py_DECREF(name);
	if(!module){
		cout << "Failed to load module " << modName << endl;
		return 1;
	}
	Py_Finalize();
	return 0;
}
PyObject* Python::newObject(string className, string arguments)
{
	Py_Initialize();
	PyObject* name = PyString_FromString(className.c_str());
	PyObject* argument = PyString_FromString(arguments.c_str());
	PyObject* instance = PyInstance_New(name,argument,argument);
	return instance;
}
int Python::pythonCall(int argc, char *argv[])
{
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
	int i;
	if(argc < 3){
	    fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
	    return 1;
	}
	Py_Initialize();
	pName = PyString_FromString(argv[1]);
	/* Error checking of pName left out */
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if((pModule) != NULL){
		pFunc = PyObject_GetAttrString(pModule, argv[2]);
		/* pFunc is a new reference */
		if(pFunc && PyCallable_Check(pFunc)){
			pArgs = PyTuple_New(argc - 3);
			for(i = 0; i < argc - 3; ++i){
				pValue = PyInt_FromLong(atoi(argv[i + 3]));
				if(!pValue){
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if(pValue != NULL){
				printf("Result of call: %ld\n", PyInt_AsLong(pValue));
				Py_DECREF(pValue);
			}else{
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr,"Call failed\n");
				return 1;
			}
		}else{
			if(PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}else{
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	Py_Finalize();
	return 0;
}
