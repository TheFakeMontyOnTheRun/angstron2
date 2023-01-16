///@brief Fixed Point Math class , originally from my 2005 N-Gage project - recently (2009) fixed and works almost good
///@author Daniel "NeoStrider" Monteiro
//==============================================================================================
#define MANTISSA 16     /// 7 bits for 32 bits forced
//=============================================================================================

namespace BZK_FastMath {


//----------------------------------------------------------------------------------------------
	inline static BZK_FixP Real32toFix32(float Num) {
		return (int) (Num * (1 << MANTISSA));
//	return (Num) << MANTISSA;
	}


//----------------------------------------------------------------------------------------------
	inline static float Fix32toReal32(BZK_FixP Num) {
		float _RealValue = Num;
		return (_RealValue / (1 << MANTISSA));
	}

//----------------------------------------------------------------------------------------------
	inline static BZK_FixP Int32toFix32(int Num) {
		return Num << (MANTISSA);
	}

//----------------------------------------------------------------------------------------------
	inline static int Fix32toInt32(BZK_FixP Num) {
		return Num >> (MANTISSA);
	}

//----------------------------------------------------------------------------------------------
	inline static BZK_FixP Div(BZK_FixP num1, BZK_FixP num2) {
		//+6 - -6 + 4
		return ((num1 << 6) / (num2 >> 6)) << 4;
	}

//----------------------------------------------------------------------------------------------
	inline static BZK_FixP Mul(BZK_FixP num1, BZK_FixP num2) {
		return ((num1 >> 6) * (num2 >> 6)) >> 4;
	}

//   int offset=((angle+22.5)/90.0); //+22.5 is a reasonable range
//----------------------------------------------------------------------------------------------
/// gets the current quadrant from a given angle
	inline static int GetDirection(BZK_FixP aAngle) {

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
///the biggest of two numbers
	inline static BZK_FixP Max(BZK_FixP aNumber1, BZK_FixP aNumber2) {
		if (aNumber1 >= aNumber2) return aNumber1;
		return aNumber2;
	}

//----------------------------------------------------------------------------------------------
/// get the quadrant of the inverse vector pointing towards a given angle
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
///get the quadrant to the left of a given quadrant
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
///get the quadrant to the right of a given quadrant
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
/// cartesian homogenous position of a cardinal point in X
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
/// cartesian homogenous position of a cardinal point in Y
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
/// cartesian homogenous position of a "cardinal" point in Z
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
	inline static BZK_FixP Min(BZK_FixP aNumber1, BZK_FixP aNumber2) {
		if (aNumber1 >= aNumber2) return aNumber2;
		return aNumber1;
	}

//----------------------------------------------------------------------------------------------
	inline static bool SmallerThanOne(BZK_FixP &aNumber) {
		return (aNumber < Int32toFix32(1));
	}

//----------------------------------------------------------------------------------------------
	inline static bool GreaterThanOne(BZK_FixP &aNumber) {
		return (aNumber > Int32toFix32(1));
	}

//----------------------------------------------------------------------------------------------
	inline static bool SmallerThanZero(BZK_FixP &aNumber) {
		return (aNumber < 0);
	}

//----------------------------------------------------------------------------------------------
	inline static bool GreaterThanZero(BZK_FixP &aNumber) {
		return (aNumber > 0);
	}

//----------------------------------------------------------------------------------------------
	inline static void Normalize(BZK_FixP &aNumber) {
		aNumber = Div(aNumber, Int32toFix32(2));
	}

//----------------------------------------------------------------------------------------------
	inline static void Dec(BZK_FixP &aNumber) {
		aNumber -= BZK_FastMath::Int32toFix32(1);
	}

//----------------------------------------------------------------------------------------------
	inline static void Inc(BZK_FixP &aNumber) {
		aNumber += BZK_FastMath::Int32toFix32(1);
	}

//----------------------------------------------------------------------------------------------
	inline static std::string Print(BZK_FixP aNumber) {
		std::string ToReturn;
		std::stringstream ss;
		ss << Fix32toReal32(aNumber);
		ToReturn = ss.str();
		return ToReturn;
	}

//----------------------------------------------------------------------------------------------
	inline static int Abs(BZK_FixP Num) {
		if (Num > 0) return Num;
		else return -Num;
	}



//----------------------------------------------------------------------------------------------
///implements heapsoft for integers


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
///implements heapsoft for fixed point numbers
#ifdef FIXP_INT_NOTTHESAME


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
#endif
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
