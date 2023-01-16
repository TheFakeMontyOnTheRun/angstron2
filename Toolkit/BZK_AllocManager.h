template<class T>
class BZK_AllocManager {

	BZK_Stack<T *> iPool;

public:

	T &Request() {
		T *ptr;
		if (iPool.Empty()) {
			ptr = new T();
		} else
			ptr = iPool.Pop();
		return *ptr;
	}

	void Dispose(T &aInstance) {
		iPool.Push(&aInstance);
	}

	void EraseAllocs() {
		while (!iPool.Empty())
			delete iPool.Pop();
	}
};
