/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2010 William Oliveira de Lagos <william.lagos1@gmail.com>
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

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include<string>
#include<queue>
#include<stack>
#include<vector>
using std::string;
using std::queue;
using std::stack;
using std::vector;

namespace Zhockon{
class Module;
enum treePrints{
	IN_ORDER = 1,
	PRE_ORDER = 2,
	POST_ORDER = 3
};
struct Node
{
	Node* left;
	Node* right;
	Module* data;
};

class Module{
protected:
	string name;
	bool status;
public:
	Module(string,bool);
	string getName();
	bool getStatus();
	int operator > (Module);
	int operator < (Module);
	void setStatus();
};

template<typename T>
class Matrix{
private:
	vector<vector<T>*> base;
	int numOfCols;
	int numOfRows;
public:
	Matrix(int);
	vector<T> createCol();
	void createRow();
	void clean();
	void insert(T*);
	void remove(T*);
	void search(string);
};

class Tree{
private:
	Node* root;
public:
	Tree(){ root = NULL; }
	Node* getRoot(){ return root; }
	bool isEmpty() const { return root==NULL; }
	void print(int);
	void inOrder(Node*);
	void preOrder(Node*);
	void postOrder(Node*);
	void insert(Module*);
	void remove(string);
};

class Library : public Module{
public:
	Library(string);
};

class Binding : public Module{
public:
	Binding(string);
};

class Package{
private:
	Module* init;
	Module* last;
	vector<Module> modules;
	int addModule(Module*);
public:
	Package(){};
	int numOfModules();
};

struct Structure
{
	Tree mainTree;
	queue<Module> modExec;
	stack<Package> pakStack;
};

extern Structure baseStr;
};
#endif