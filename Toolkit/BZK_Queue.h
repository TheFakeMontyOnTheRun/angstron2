///brief makeshift queue using STL (good to abstract the ADT, so if we're porting to Symbian, we dont have to modify the whole system)
///author Daniel "NeoStrider" Monteiro
template<class T>
class BZK_Queue {

private:
	std::queue<T> _queue;
	long qtd;  ///for some unknown reason, we can't trust STL on counting (maybe Im screwing somewere)
public:

	long TotalItems() ///the size of our queue
	{
		return qtd;
	}


///checkout who's at the top, but without popping
	const T Peek() {

		if (!Empty()) {
			T theData = _queue.front();
			return theData;
		}
		return NULL;
	}

///default constructor
	BZK_Queue() : qtd(0) {
		/// make it empty
		EraseAll();
	}

///default destructor. for now, assuming the structure is already empty
///TODO: fix Queue destructor
	~BZK_Queue() {
		// EraseAll();
//      ReleaseAll();
		//  delete _queue;
	}

/// supposed to erase everybody inside. Its not doing what it is supposed to do for now
///TODO: fix Queue EraseAll
	void EraseAll() {
		while (qtd > 0) {
			_queue.pop();
			qtd--;
		}

	}

/// just let everybody go
	void ReleaseAll() {
		while (qtd > 0)//!_queue->empty())
		{
			_queue.pop();
			qtd--;
		}
	}

///insert element at the end of the line  
	void Push(T aItem) {
		_queue.push(aItem);
		qtd++;
	}

///concatenates two queues . The other goes at the end of these
	void Push(BZK_Queue<T> &aAnother) {
		while (!aAnother.Empty())
			Push(aAnother.Pop());
	}

///concatenates the two queues, but without modifying the other
	void PushCopy(const BZK_Queue<T> aAnother) {
		while (!aAnother.Empty())
			Push(aAnother.Pop());

	}

///retrieves the front element of the queue
	T Pop() {
		if (!Empty()) {
			T theData = _queue.front();
			_queue.pop();
			qtd--;
			return theData;
		}
	}

///checks what ever it is empty
	bool Empty() {
		return qtd == 0;
	}


};
