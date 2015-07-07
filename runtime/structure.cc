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

#include <zhockon.h>
using namespace Zhockon;

template<typename T>
Matrix<T>::Matrix(int size)
{
	this->numOfCols = size;
	this->numOfRows = size;
}

template<typename T>
void Matrix<T>::createRow()
{
	typename vector<vector<T>*>::iterator it;
	for(it = this->base.begin(); it != this->base.end(); it++){
		(*it)->resize(numOfRows+1);
	}
}
template<typename T>
vector<T> Matrix<T>::createCol()
{
	vector<T> col;
	this->base.push_back(&col);
	this->numOfCols += 1;
	return col;
}
template<typename T>
void Matrix<T>::clean()
{
	
}
template<typename T>
void Matrix<T>::insert(T* item)
{
	vector<T> column = this->createCol();
	for(int i = 0; i < this->numOfRows; i++){
		column.push_back((*item));
		item++;
	}
}
template<typename T>
void Matrix<T>::remove(T*)
{
	
}
template<typename T>
void Matrix<T>::search(string)
{
	
}

void Tree::insert(Module* module)
{
	Node* nod = new Node;
	Node* parent;
	nod->data = module;
	nod->left = NULL;
	nod->right = NULL;
	parent = NULL;
	if(isEmpty()) root = nod;
	else{
		Node* current;
		current = root;
		while(current){
			parent = current;
			if(nod->data > current->data) current = current->right;
			else current = current->left;
		}
		if(nod->data < parent->data)
			parent->left = nod;
		else
			parent->right = nod;
	}
}
void Tree::remove(string name)
{
	bool found = false;
	if(isEmpty()){
		cout << "This Tree is empty! " << endl;
		return;
	}
	Node* current;
	Node* parent;
	current = root;    
	while(current != NULL){
		if(current->data->getName() == name){
			found = true;
			break;
		}else{
			parent = current;
			if(name > current->data->getName()) current = current->right;
			else current = current->left;
		}
	}
	if(!found){
		cout << "Data not found! " << endl;
		return;
	}
	if((current->left == NULL && current->right != NULL) || (current->left != NULL && current->right == NULL)){
		if(current->left == NULL && current->right != NULL){
			if(parent->left == current){
				parent->left = current->right;
				delete current;
			}else{
				parent->right = current->right;
				delete current;
			}
		}else{
			if(parent->left == current){
				parent->left = current->left;
				delete current;
			}else{
				parent->right = current->left;
				delete current;
			}
		}
		return;
	}
	if(current->left == NULL && current->right == NULL){
		if(parent->left == current) parent->left = NULL;
		else parent->right = NULL;
		delete current;
		return;
	}
	if(current->left != NULL && current->right != NULL){
		Node* checker;
		checker = current->right;
		if((checker->left == NULL) && (checker->right == NULL)){
			current = checker;
			delete checker;
			current->right = NULL;
		}else{
			if((current->right)->left != NULL){
				Node* lCurrent;
				Node* lCurrentParent;
				lCurrentParent = current->right;
				lCurrent = (current->right)->left;
				while(lCurrent->left != NULL){
					lCurrentParent = lCurrent;
					lCurrent = lCurrent->left;
				}
				current->data = lCurrent->data;
				delete lCurrent;
				lCurrentParent->left = NULL;
			}else{
				Node* temporary;
				temporary = current->right;
				current->data = temporary->data;
				current->right = temporary->right;
				delete temporary;
			}
		}
		return;
	}
}
void Tree::print(int type)
{
	switch(type){
	  case IN_ORDER: inOrder(root);
			 break;
	  case PRE_ORDER: preOrder(root);
			  break;
	  case POST_ORDER: postOrder(root);
			   break;
	}
}
void Tree::inOrder(Node* p)
{
	if(p != NULL){
		if(p->left) inOrder(p->left);
		cout << " " << p->data->getName() << " ";
		if(p->right) inOrder(p->right);
	}
	else return;
}
void Tree::preOrder(Node* p)
{
	if(p != NULL){
		cout << " " << p->data->getName() << " ";
		if(p->left) preOrder(p->left);
		if(p->right) preOrder(p->right);
	}
	else return;
}
void Tree::postOrder(Node* p)
{
	if(p != NULL){
		if(p->left) postOrder(p->left);
		if(p->right) postOrder(p->right);
		cout << " " << p->data->getName() << " ";
	}
	else return;
}

template class Matrix<int>;
template class Matrix<float>;
template class Matrix<bool>;