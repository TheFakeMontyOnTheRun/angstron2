template <class T>
/**
@brief Stack class for all purposes.
@author Daniel Monteiro
*/
class BZK_Stack
{
 private:
  std::stack<T> *_stack; ///we just encapsulate, for the cases where STL is not available.
  long _qtd; ///we cant trust STL on this
 public:
  /////////////////
///@brief constructor
  BZK_Stack():_qtd(0),_stack(_stack=new std::stack<T>())
    {
//	_qtd=0;
  //    _stack=new std::stack<T>;
    }
  ////////////////////////
///@brief destructor. Take extra care to cleanup all alocated data after deleting the stack
  ~BZK_Stack()
    {

	///I know this one is wrong, but I cant help it.
	if (_stack!=NULL)
	{
             while (_qtd>0) _stack->pop();	 ///be careful when using this. Can lead to memory leaks
	     delete _stack;
	}
    }
  //////////////
///@brief self-explanatory. Adds item to the top of the stack
  inline void Push(T aItem)
    {  
	_qtd++;
      _stack->push(aItem);
    }
  ////////////////////
///@brief self-explanatory. Takes the top of the stack.
  inline  T Pop()
    {
	if (Empty()) return NULL;
      T _data=_stack->top();
      _stack->pop();
	_qtd--;
      return _data;
    }
  ///////////////////
///@bief check if the stack is empty
  inline  bool Empty()
    {
      return _stack->empty();
    }
  ///////////////////
///@brief returns the size of the stack
  inline long TotalItems()
	{return _qtd;}


};
