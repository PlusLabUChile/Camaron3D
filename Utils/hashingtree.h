#ifndef HashingTree_H
#define HashingTree_H

#include <vector>
#include <algorithm>
#include <unordered_map>

/***
*  @brief Tree structure used to discard repeated polyhedrons based on vertex indices
*
*  The following structure is used by the model loader for the extensions M3D and Ele/Node,
*  both file formats represent their respective polyhedrons by using a sequence of vertices.
*  In our case Camaron must also generate the triangles that are composed by this vertices,
*  so in the case of a face that is shared by two polyhedrons, this face would be instanced
*  twice if we read the data without checking.
*
*  This structure is responsible to keep a registry of this vertex sequences to do this
*  checks, the data must be sorted, as the same face can be written with vertices in
*  different order. At the same time the structure must be able to represent sets of
*  variable length, which may vary depending on the file format (Ele/Node: 4, M3D: 4-6).
*
*  A sequence is represented through a chain of nodes, each node contains the index of
*  the specific vertex. The node containing the last vertex of a set stores the index of
*  the face in the "values" map.
*
*  Sequences can be queried through the getRegistry method, if the face exists, the structure
*  will return the index of the respective face and will remove the entry from the tree
*  (This is due to the fact that a face can only be present in two polyhedrons at maximum
*  , allowing us to reduce space allocation). If the face cannot be located, the structure
*  returns the value -1.
*
*  @note While this structure could be improved to reduce its RAM consumption, this
*  change has low priority as its footprint is significantly small compared to the memory
*  used in other parts of the program. At the same time the data allocated during the
*  loading process must be released at the end of the scope to prevent memory leaks.
*
*  @note Adaptation of the original HashTree, this version stores primitives rather than
*  raw pointers, as a way to prevent pointer invalidation during the loading process of
*  the models.
*/

namespace vis{
namespace ut{
template<class KEY,class T>
class HashingTree
{
	public:
		HashingTree();
		virtual ~HashingTree();
		T getRegistry(std::vector<KEY>);
		void setOrdered(bool);
		void registerValue(std::vector<KEY> keys, T value);

	private:
		bool hasChild(KEY);
		bool hasValue(KEY);
		bool isEmpty();
		T getRegistryIn(std::vector<KEY>&,unsigned int);
		void registerValueIn(std::vector<KEY>& keys, T value,unsigned int);
		std::unordered_map<KEY,HashingTree<KEY,T>* > children;
		std::unordered_map<KEY,T> values;
		bool ordered;
};
}
}


template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::HashingTree()
{
	ordered = true;
}

template <class KEY,class T>
vis::ut::HashingTree<KEY,T>::~HashingTree(){
	typename std::unordered_map<KEY, vis::ut::HashingTree<KEY,T>* >::iterator it =
			children.begin();
	while(it!=children.end()){
		vis::ut::HashingTree<KEY,T>* p = it->second;
		delete p;
		it++;
	}
}


template <class KEY,class T>
T vis::ut::HashingTree<KEY,T>::getRegistry(std::vector<KEY> keys){
	if(ordered)
		std::sort(keys.begin(),keys.end());
	return getRegistryIn(keys,0u);
}
template <class KEY,class T>
T vis::ut::HashingTree<KEY,T>::getRegistryIn(std::vector<KEY>& keys,unsigned int current){
	if(keys.size()-1==current){
		typename std::unordered_map<KEY, T>::iterator it = this->values.find( keys[current] );
		if( it == this->values.end() )
			return -1;
		T found = it->second;
		values.erase(keys[current]);
		return found;
	}
	typename std::unordered_map<KEY, vis::ut::HashingTree<KEY,T>* >::iterator it =
			this->children.find( keys[current] );
	if( it == this->children.end() )
		return -1;
	T found = it->second->getRegistryIn(keys,++current);
	vis::ut::HashingTree<KEY,T>* registryToTest = it->second;
	if(registryToTest->isEmpty()){
		children.erase(it->first);
		delete registryToTest;
	}
	return found;
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValue(std::vector<KEY> keys, T value){
	if(ordered)
		std::sort(keys.begin(),keys.end());
	registerValueIn(keys,value,0u);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::registerValueIn(std::vector<KEY>& keys, T value, unsigned int current){
	if(keys.size()-1==current){
		values[keys[current]] = value;
		return;
	}

	vis::ut::HashingTree<KEY,T>* hash;
	if(hasChild(keys[current])){
		hash = children[keys[current]];
	}
	else{
		hash = new vis::ut::HashingTree<KEY,T>();
		children[keys[current]] = hash;
	}
	hash->registerValueIn(keys,value,++current);
}

template <class KEY,class T>
void vis::ut::HashingTree<KEY,T>::setOrdered(bool b){
	ordered = b;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasChild(KEY key){
	return children.count(key)>0;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::hasValue(KEY key){
	return values.count(key)>0;
}
template <class KEY,class T>
bool vis::ut::HashingTree<KEY,T>::isEmpty(){
	return values.empty()&&children.empty();
}

#endif // HashingTree_H
