#pragma once
#include <iostream>
#include "Comparator.h"
#include <queue>

template <class T, class F = Comparator<T>> class Set;

template <class T, class F = Comparator<T>> 
std::ostream& operator<<(std::ostream&, const Set<T, F>&);


template <class T, class F = Comparator<T>>
class Set
{
	struct Node													//Setul va fi retinut sub forma unui Avl 
	{
		T val;														
		Node* left_son, * right_son;
		int height;

		Node(T);
		int getHeight() const;
		int getBalance() const;
	};

	F compare;
	int size;
	Node* root;
	
	Node* insert(Node*& node, T& val);						//functie recursiva pentru inserare
	Node* erase(Node* node, T& val);						//functie recursiva pentru stergere
	bool isPresent(Node* node, T& val) const;				//functie recurisiva pentruu a cauta o anumita valoare
	int noElem(Node*) const;								//functie recursiva care calculeaza nr de elemente
	Node* getMin(Node* node) const;							//returneaza valoarea minima din AVL

	Node* rr_rotate(Node*&);								//rotatia right-right
	Node* rl_rotate(Node*&);								//rotatia right-left
	Node* lr_rotate(Node*&);								//rotatie left-right
	Node* ll_rotate(Node*&);								//rotatie left-left

	void inorder(std::ostream&, Node*) const;				
	void preorder(std::ostream&, Node*) const;

public:
	Set();																//constructor fara parametri
	~Set();																//destructor
	Set(const Set&);													
	void insert(T );
	void erase(T );
	bool isPresent(T ) const;
	int noElem() const;

	template <typename U, typename F>
	friend std::ostream& operator<<(std::ostream&, const Set<U, F>&);		//afiseaza parcurgerea in inordine si in preordine

	Set& operator=(const Set&);
};

//constructor cu un parametru pentru nod
template<class T, class F>
inline Set<T, F>::Node::Node(T valoare) : val(valoare), left_son(NULL), right_son(NULL), height(0)
{

}

//calculez inaltimea fiecarui subarbore
template<class T, class F>
inline int Set<T, F>::Node::getHeight() const
{
	int left_side = 0, right_side = 0;
	if (left_son)
		left_side = left_son->height;
	if (right_son)
		right_side = right_son->height;
	
	//returnez max(left_side, right_side) + 1
	if (left_side > right_side)
		return left_side + 1;
	else
		return right_side + 1;
}

//pentru a vedea daca arborele este balansat sau nu, trebuie sa vedem care este diferenta dintre inaltimea subarborelui stand si inaltimea subarborelui drept
template<class T, class F>
inline int Set<T, F>::Node::getBalance() const
{
	int left_side = 0, right_side = 0;
	if (left_son)
		left_side = left_son->getHeight();
	if (right_son)
		right_side = right_son->getHeight();

	return left_side - right_side;
}

//constructor pentru Set fara parametri
template<class T, class F>
inline Set<T, F>::Set() : size(0), root(NULL)
{

}

//destructor
template<class T, class F>
inline Set<T, F>::~Set()
{
	if (root == NULL)
		return;

	//retinem nodurile intr-o coada pentru a ne fi mai usor sa le stergem
	std::queue<Node*>Q;
	Q.push(root);
	while (!Q.empty())
	{
		Node* curent = Q.front();
		Q.pop();
		if (curent->left_son)				//adaugam in coada fiul stang al nodului curent
			Q.push(curent->left_son);
		if (curent->right_son)
			Q.push(curent->right_son);		//adaugam in coada fiul drept al nodului curent
		delete curent;
	}
	root = NULL;
	size = 0;
}

//copy constructor
template<class T, class F>
inline Set<T, F>::Set(const Set& copie) : size(copie.size), compare(copie.compare)
{
	if (copie.root != NULL)
	{
		std::queue<Node*>Q;
		Q.push(copie.root);
		while (!Q.empty())
		{
			Node* curent = Q.front();
			Q.pop();
			this->insert(curent->val);
			if (curent->left_son)
				Q.push(curent->left_son);
			if (curent->right_son)
				Q.push(curent->right_son);
		}
	}
}

template<class T, class F>
inline void Set<T, F>::insert(T val)
{
		root = insert(root, val);
}

template<class T, class F>
inline void Set<T, F>::erase(T val)
{
	if (isPresent(val))
		root = erase(root, val);
	else
		throw "Nu exista valoarea in set";
}

template<class T, class F>
inline bool Set<T, F>::isPresent(T val) const
{
	return isPresent(root, val);
}

template<class T, class F>
inline int Set<T, F>::noElem() const
{
	int nr = 0;
	if (root)
		nr = noElem(root);
	return nr;
}

template<class T, class F>
inline Set<T,F>& Set<T, F>::operator=(const Set& copie)
{
	if (this == &copie)
		return *this;
	if (copie.root != NULL)
	{
		std::queue<Node*>Q;
		Q.push(copie.root);
		while (!Q.empty())
		{
			Node* curent = Q.front();
			Q.pop();
			this->insert(curent->val);
			if (curent->left_son)
				Q.push(curent->left_son);
			if (curent->right_son)
				Q.push(curent->right_son);
		}
	}
	return *this;
}

template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::insert(Node*& node, T& val)
{
	if (node == NULL)
	{
		node = new Node(val);
		return node;
	}
	
	if (this->compare(node->val, val) == 0)				//daca nodul deja exista, atunci nu il mai inseram
		return node;

	if (this->compare(node->val, val) == 1)				//daca valoarea este mai mica decat valoarea nodului curent, atunci vom merge pe ramura stanga	
	{
		node->left_son = insert(node->left_son, val);	//mergem pe ramura stanga
		
		if (node->getBalance() == 2)					
		{
			if (this->compare(node->left_son->val, val) == 1)							//daca avem o structura de tipul st
				node = ll_rotate(node);
			else if (this->compare(node->left_son->val, val) == -1)
				node = lr_rotate(node);
		}

	}
	else if (this->compare(node->val, val) == -1)
	{
		node->right_son = insert(node->right_son, val);
		
		if (node->getBalance() == -2)
		{
			if (this->compare(node->right_son->val, val) == -1)
				node = rr_rotate(node);
			else if (this->compare(node->right_son->val, val) == 1)
				node = rl_rotate(node);
		}

	}

	node->height = node->getHeight();
	return node;
}

template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::erase(Node* node, T& val)
{
	if (node == NULL)
		return NULL;

	if (compare(node->val, val) == 0)
		//are 2 copii
		if (node->left_son != NULL && node->right_son != NULL) 
		{
			Node* t = getMin(node->right_son);
			node->val = t->val;
			node->right_son = erase(node->right_son, t->val);
		}
		// 1 or 0 children
		else 
		{
			Node* u = node;
			if (node->left_son)
				node = node->left_son;
			else
				node = node->right_son;
			delete u;
		}
	else if (compare(node->val, val) == 1)
		node->left_son = erase(node->left_son, val);
	else if (compare(node->val, val) == -1)
		node->right_son = erase(node->right_son, val);

	if (node == NULL)
		return NULL;
	if (node->getBalance() == 2) 
	{
		if (node->left_son->left_son == NULL)
			node = lr_rotate(node);
		else
			node = ll_rotate(node);
	}
	else if (node->getBalance() == -2) 
	{
		if (node->right_son->right_son == NULL)
			node = rl_rotate(node);
		else
			node = rr_rotate(node);
	}

	return node;
}

//urmaresc 
template<class T, class F>
inline bool Set<T, F>::isPresent(Node* node, T& val) const
{
	if (node == NULL)
		return false;
	else if (this->compare(node->val, val) == 0)
		return true;
	else if (this->compare(node->val, val) == 1)
		return isPresent(node->left_son, val);
	else
		return isPresent(node->right_son, val);
}

//obtin numarul de elemente
template<class T, class F>
inline int Set<T, F>::noElem(Node* node) const
{
	int nr = 1;
	if (node->left_son != NULL)
		nr += noElem(node->left_son);							//numarul de elemente mergand pe subarborele stang
	if (node->right_son != NULL)
		nr += noElem(node->right_son);							//numarul de elemente mergand pe subarborele drept
	return nr;
}

//obtin valoarea minima din AVL, mergand pe ramura stanga
template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::getMin(Node* node) const
{
	if (node == NULL)
		return NULL;
	if (node->left_son)
		return getMin(node->left_son);
	else 
		return node;
}

//rotatie right-right
template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::rr_rotate(Node*&pivot)
{
	Node* aux = pivot->right_son;

	pivot->right_son = aux->left_son;
	pivot->height = pivot->getHeight();

	aux->left_son = pivot;
	aux->height = aux->getHeight();

	return aux;
}

//rotatie right-left
template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::rl_rotate(Node*&pivot)
{
	pivot->right_son = ll_rotate(pivot->right_son);
	return rr_rotate(pivot);
}

//rotatie left-right
template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::lr_rotate(Node*&pivot)
{
	pivot->left_son = rr_rotate(pivot->left_son);
	return ll_rotate(pivot);
}

//rotatie left-left
template<class T, class F>
typename inline Set<T, F>::Node* Set<T, F>::ll_rotate(Node*&pivot)
{
	Node* aux = pivot->left_son;

	pivot->left_son = aux->right_son;
	pivot->height = pivot->getHeight();

	aux->right_son = pivot;
	aux->height = aux->getHeight();

	return aux;
}

//afisare in inordine
template<class T, class F>
inline void Set<T, F>::inorder(std::ostream& os, Node* nod) const
{
	if (nod == NULL) 
		return;
	inorder(os, nod->left_son);
	os << nod->val << ' ';
	inorder(os, nod->right_son);
}

//afisare in preordine
template<class T, class F>
inline void Set<T, F>::preorder(std::ostream& os, Node* nod) const
{
	if (nod == NULL)
		return;
	os << nod->val << ' ';
	preorder(os, nod->left_son);
	preorder(os, nod->right_son);
}

//operatorul de afisare : afiseaza parcurgerea in inordinde si apoi o afiseaza pe cea in preordine
template<class T, class F>
std::ostream& operator<<(std::ostream& os, const Set<T, F>& set)
{
	os << "Inordine: "; set.inorder(os, set.root);
	os << std::endl;
	os << "Preordine: "; set.preorder(os, set.root);
	return os;
}
