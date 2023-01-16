/**
   @brief Optimized math for embedded processors. (DM: w00t! quite naive coding here! I was surely drunk...)

   @author Daniel Monteiro
*/
namespace BZK_FastMath {


	/**
	   @brief Allows the modification of a value unless a magic number is said

	   @param int The candidate to be substituted

	   @author Daniel Monteiro
	*/
	///TODO: erradicate this crap


//----------------------------------------------------------------------------------------------
///@brief converts the float parameter to bzk math unit
	inline static BZK_FixP Real32toFix32(float Num) {
		return Num;
	}

//----------------------------------------------------------------------------------------------
///@brief converts the math unit to float
	inline static float Fix32toReal32(BZK_FixP Num) {
		return Num;
	}

//----------------------------------------------------------------------------------------------
///@brief convert the integer to math unit
	inline static BZK_FixP Int32toFix32(long Num) {
		return Num;
	}

//----------------------------------------------------------------------------------------------
///@brief converts the integer to math unit
	inline static long Fix32toInt32(BZK_FixP Num) {
		return Num;
	}

//   int offset=((angle+22.5)/90.0); //+22.5 is a reasonable range
//----------------------------------------------------------------------------------------------
	inline static BZK_FixP automodify(BZK_FixP aOriginal, std::string aTry) {
		float entry = -347.0;
		entry = atoi(aTry.c_str());
		if (entry == -347.0)
			return aOriginal;
		else
			return BZK_FastMath::Real32toFix32(entry);
	}

	/**
	   @brief Returns the quadrant of a given angle

	   @param BZK_FixP Angle to get the quadrant.

	   @author Daniel Monteiro
	*/
	inline static int GetDirection(BZK_FixP aAngle) {
/*
      int toReturn= (int) ((BZK_FastMath::Fix32toReal32(aAngle)+22.5)/90.0);
      if (toReturn==4) toReturn=0;
      return toReturn;
*/



		int toReturn = ((Fix32toInt32(aAngle) + 45) / 90) % 4;
/*
      if (toReturn==2) toReturn=0;
else if (toReturn==0) toReturn=2;
*/

		if (toReturn == 1) toReturn = 3;
		else if (toReturn == 3) toReturn = 1;
		if (toReturn < 0) toReturn += 4;

		return toReturn;//GetOpositeDirection(toReturn);

	}
//----------------------------------------------------------------------------------------------
	/**
	   @brief The bigger between two numbers


	   @author Daniel Monteiro
	*/
	inline static BZK_FixP Max(BZK_FixP aNumber1, BZK_FixP aNumber2) {
		if (aNumber1 >= aNumber2) return aNumber1;
		return aNumber2;
	}

//----------------------------------------------------------------------------------------------
///@brief get the oposite direction of a given direction
///TODO: Fix rendering , in order to use the optimized version.
	inline static int GetOpositeDirection(int aAngle) {
		/*
		  int valor=0;
		  if (aAngle<4)
			{
		  valor=aAngle-2;
		  while (valor<0) valor+=4;
			}
		  else
			{
		  valor=aAngle-2;
		  if (valor==5) valor=4;
		  else valor=5;
			}
		  if (valor==5) valor=0;
		  return valor;
		  */
		if (aAngle == 5) return 4;
		if (aAngle == 4) return 5;
		return (aAngle + 2) % 4;

	}

	//----------------------------------------------------------------------------------------------
///@brief get the direction to the left of the given direction
///TODO: Fix rendering , in order to use the optimized version.
	inline static int GetLeft(int aAngle) {
		/*
		  int valor;
		  if (aAngle<4)
			{
		  valor=aAngle-1;
		  while (valor<0) valor+=4;
			}
		  else
			{
			  valor=-1;
			}
		  if (valor==5) valor=0;
		  return valor;
		  */
		return (aAngle + 3) % 4;
	}

	//----------------------------------------------------------------------------------------------
///@brief get the direction to the right of the given direction
///TODO: Fix rendering , in order to use the optimized version.
	inline static int GetRight(int aAngle) {
		/*
		  int valor;
		  if (aAngle<4)
			{
		  valor=aAngle+1;
		  while (valor>3) valor-=4;

			}
		  else
			{
		  valor=-1;
			}
		  if (valor==5) valor=0;
		  return valor;
		  */
		return (aAngle + 1) % 4;
	}

	//----------------------------------------------------------------------------------------------
///@brief get X offset of a given direction
	inline static int Xoffset(int aAngle) {
		switch (aAngle) {
			case E:
				return -1;
			case W:
				return 1;
			default:
				return 0;
		}
	}

	//----------------------------------------------------------------------------------------------
///@brief get Y offset of a given direction
	inline static int Yoffset(int aAngle) {
		switch (aAngle) {
			case S:
				return -1;
			case N:
				return 1;
			default:
				return 0;
		}
	}

	//----------------------------------------------------------------------------------------------
///@brief get Z offset of a given direction
	inline static int Zoffset(int aAngle) {
		switch (aAngle) {
			case FLOOR:
				return -1;
			case CEILING:
				return 1;
			default:
				return 0;
		}
	}

//----------------------------------------------------------------------------------------------
///@brief get X smaller of two numbers
	inline static BZK_FixP Min(BZK_FixP aNumber1, BZK_FixP aNumber2) {
		if (aNumber1 >= aNumber2) return aNumber2;
		return aNumber1;
	}

//----------------------------------------------------------------------------------------------
///@brief tests if the given number is smaller than one
	inline static bool SmallerThanOne(BZK_FixP &aNumber) {
		return (aNumber < Int32toFix32(1));
	}

//----------------------------------------------------------------------------------------------
///@brief tests if the given number is bigger than one
	inline static bool GreaterThanOne(BZK_FixP &aNumber) {
		return (aNumber > Int32toFix32(1));
	}

//----------------------------------------------------------------------------------------------
///@brief tests if the given number is smaller than zero
	inline static bool SmallerThanZero(BZK_FixP &aNumber) {
		return (aNumber < 0);
	}

//----------------------------------------------------------------------------------------------
///@brief tests if the given number is bigger than zero
	inline static bool GreaterThanZero(BZK_FixP &aNumber) {
		return (aNumber > 0);
	}

///@brief decrease the given variable
	inline static void Dec(BZK_FixP &aNumber) {
		aNumber -= BZK_FastMath::Int32toFix32(1);
	}

//----------------------------------------------------------------------------------------------
///@brief increase the given variable
	inline static void Inc(BZK_FixP &aNumber) {
		aNumber += BZK_FastMath::Int32toFix32(1);
	}

//----------------------------------------------------------------------------------------------
///@brief convert to std::string the given value
	inline static std::string Tostring(BZK_FixP aNumber) {
		std::string ToReturn;
		std::stringstream ss;
		ss << Fix32toReal32(aNumber);
		ToReturn = ss.str();
		return ToReturn;
	}

///@deprecated
	inline static std::string Print(BZK_FixP aNumber) {
		return Tostring(aNumber); //deprecated
	}


//----------------------------------------------------------------------------------------------
///@deprecated
	inline static void _debug(int number) {
		//	  fprintf(stdout,"debug:%d",number);
	}

//----------------------------------------------------------------------------------------------
///@brief returns a positive value from the parameter
	inline static long Abs(BZK_FixP Num) {
		if (Num > 0) return Num;
		else return -Num;
	}

//----------------------------------------------------------------------------------------------
///@brief divides two number in math units
	inline static BZK_FixP Div(BZK_FixP num1, BZK_FixP num2) {
		return num1 / num2;
	}

//----------------------------------------------------------------------------------------------
///@brief multiply two numbers in math units
	inline static BZK_FixP Mul(BZK_FixP num1, BZK_FixP num2) {
		return num1 * num2;
	}

	inline static void heapfy(int **_dists, int **_sects, int size, int index) {
		int *dists = *_dists;
		int *sects = *_sects;
		int largest = 0;
		int tmp;
		if (2 * index < size && dists[2 * index] > dists[index])
			largest = 2 * index;
		else
			largest = index;

		if ((2 * index + 1) < size && dists[(2 * index + 1)] > dists[largest])
			largest = (2 * index + 1);

		if (largest != index) {
			tmp = dists[index];
			dists[index] = dists[largest];
			dists[largest] = tmp;

			tmp = sects[index];
			sects[index] = sects[largest];
			sects[largest] = tmp;

			heapfy(_dists, _sects, size, largest);
		}


	}

	inline static void build_heap(int **dists, int **sects, int size) {
		int c;
		for (c = size / 2; c > 0; c--)
			heapfy(dists, sects, size, c);
	}

	inline static void Order(int **_dists, int **_sects, int size) {
		int *dists = *_dists;
		int *sects = *_sects;
		int c;
		int tmp;
		build_heap(_dists, _sects, size);

		for (c = size - 1; c > 0; c--) {
			tmp = dists[0];
			dists[0] = dists[c];
			dists[c] = tmp;

			tmp = sects[0];
			sects[0] = sects[c];
			sects[c] = tmp;

			size--;
			heapfy(_dists, _sects, size, 0);
		}

	}
///implements heapsoft for floats


	inline static void heapfy(BZK_FixP **_dists, int **_sects, int size, int index) {
		BZK_FixP *dists = *_dists;
		int *sects = *_sects;
		int largest = 0;
		int tmp;
		BZK_FixP tmpfix;

		if (2 * index < size && dists[2 * index] > dists[index])
			largest = 2 * index;
		else
			largest = index;

		if ((2 * index + 1) < size && dists[(2 * index + 1)] > dists[largest])
			largest = (2 * index + 1);

		if (largest != index) {
			tmpfix = dists[index];
			dists[index] = dists[largest];
			dists[largest] = tmpfix;
			tmp = sects[index];
			sects[index] = sects[largest];
			sects[largest] = tmp;
			heapfy(_dists, _sects, size, largest);
		}


	}

	inline static void build_heap(BZK_FixP **dists, int **sects, int size) {
		int c;
		for (c = size / 2; c > 0; c--)
			heapfy(dists, sects, size, c);
	}

	inline static void Order(BZK_FixP **_dists, int **_sects, int size) {
		BZK_FixP *dists = *_dists;
		int *sects = *_sects;
		int c;
		int tmp;
		BZK_FixP tmpfix;
		build_heap(_dists, _sects, size);

		for (c = size - 1; c > 0; c--) {
			tmpfix = dists[0];
			dists[0] = dists[c];
			dists[c] = tmpfix;

			tmp = sects[0];
			sects[0] = sects[c];
			sects[c] = tmp;

			size--;
			heapfy(_dists, _sects, size, 0);
		}

	}
//----------------------------------------------------------------------------------------------
///implements heapsort for integers


	///Each case requires an appropriate approach

	static inline int FastSqrt(int aNum) {

		if (!aNum) return 0;

		int tmp;
		int old;
		tmp = aNum;
		old = 0xFFFF;

		while ((old - tmp) > 2) {
			if (!tmp) return 0;
			old = tmp;
			tmp = (tmp + (aNum / tmp)) / 2;
		}

		return tmp;
	}

	///from http://en.wikipedia.org/wiki/Methods_of_computing_square_roots
	static inline float FastSqrt(float val) {
		union {
			int tmp;
			float val;
		} u;
		u.val = val;
		u.tmp -= 1 << 23; /* Remove last bit so 1.0 gives 1.0 */
		/* tmp is now an approximation to logbase2(val) */
		u.tmp >>= 1; /* divide by 2 */
		u.tmp += 1 << 29; /* add 64 to exponent: (e+127)/2 =(e/2)+63, */
		/* that represents (e/2)-64 but we want e/2 */
		return u.val;
	}

	///from the same article
	static inline float InvSqrt(float x) {
		float xhalf = 0.5f * x;
		union {
			float x;
			int i;
		} u;
		u.x = x;
		u.i = 0x5f3759df - (u.i >> 1);
		x = u.x * (1.5f - xhalf * u.x * u.x);
		return x;
	}


};
