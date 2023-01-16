
template<class T>

///@brief generic vector class, good for "fill once and only lookup after that". Not good for modifying.
///@author Daniel Monteiro
class BZK_Vector {
	std::vector<T> *_vector; //the STL vector itself
	int size; //we cant trust STL...
public:
///@brief constructor
	BZK_Vector() : size(0), _vector(new std::vector<T>()) {
//      size=0;
		//    _vector=new std::vector<T>;
	}

///@brief generic random acess method. No f*cking getAt()
///@note If out of bounds, it will wrap around.
	T &operator[](long aIndex) {
		return (*_vector)[aIndex];
	}

///@brief erase item from list
	void Remove(int aIndex) {
		size--;
		_vector->erase(_vector->begin() + aIndex);
	}


///@brief this is how we add things to the vector
	long Add(T aItem) {
		size++;
		_vector->push_back(aItem);
		return size;
		//return _vector->size();
	}

///@brief wrapper to allow exchanging this with the other containers
	void Push(T aItem) {
		Add(aItem);
	}

///@brief tests for the index of a given element. SLOW. dont use it unless for debugging purporses.
	int indexof(T aItem) {
		for (int c = TotalItems() - 1; c >= 0; c--)
			if (this[c] == aItem)
				return c;
		return -1;
	}


///@brief constructs already inserting a item on the "list"
	BZK_Vector(T aItem) {
		BZK_Vector();
		Add(aItem);
	}

///@brief make the list empty, releasing all its elements. Its on the programmer to clean up everything else.
	void ReleaseAll() {
		size = 0;
		delete _vector;
		_vector = new std::vector<T>;
	}

///@brief not only make the list empty, but destroy everything
	void EraseAll() {
		size = 0;
		_vector->clear();
		//_vector->erase(_vector->begin(),_vector->end());

	}

///@brief returns the size of the list
	long TotalItems() {
		return size;//_vector->size();
	}

///@brief tests whatever the "group" is empty
	bool Empty() {
		return TotalItems() == 0;
	}

///@brief we're shutting down. clean up everything
	~BZK_Vector() {
//	ReleaseAll(); ///???
		delete _vector;
	}

///@brief reserve a certain number of elements, to make allocation faster.
	void Reserve(long aQtd) {
		_vector->reserve(aQtd);
	}
};
