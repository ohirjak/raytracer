/*
 * @file: V2Base.h
 * @author: Ondrej Hirjak, 2006 - 2010
 * @description: Basic definitionss and functions.
 */

#ifndef V2Base_h
#define V2Base_h


/*
 * =============================================
 * Definovanie konstant opisujucich aplikaciu
 * =============================================
*/

#include "V2Version.inc"

#define PROGRAM_NAME "Raytracer"
#define PROGRAM_AUTHOR "Hiro, 2010"
#define PROGRAM_ENGINE "V2 Engine v" V2_VERSION " (http://www.ak2studios.com/hiro/), 2006 - 2010"


/*
 * =============================================
 * Definovanie pouziteho OS
 * OS specificke includy
 * =============================================
*/

#ifdef V2WIN

	#define WIN		1
	#define LINUX	0

	#define BIG_ENDIAN	0
	#define LITTLE_ENDIAN	1

	#include "V2Windows.h"

	#define WINDOW_CLASS	"V2RocketWindowClass"

#elif V2LINUX

	#define WIN		0
	#define LINUX	1

	#define BIG_ENDIAN	0
	#define LITTLE_ENDIAN	1

	#include "V2Linux.h"

#else

	#error OS must be specified

#endif


/*
 * =============================================
 * OS nezavisle
 * =============================================
*/

#define WINDOW_TITLE PROGRAM_NAME

#ifdef _DEBUG
#define ASSERT(X) if (!(X)) throw GenericException("Assertion failed", __FILE__, __LINE__);
#else
#define ASSERT(X)
#endif


namespace V2
{
	/*
	 * =============================================
	 * Zapuzdrenie retazca
	 * =============================================
	*/

	enum
	{
		kStringLength = 256
	};


	class String
	{
	private:
		// TODO: allow longer strings, unicode support
		char buffer[kStringLength];

	public:
		String();
		String(const String& s);
		String(const char *s);

		unsigned long Length()
		{
			return (unsigned long)strlen(buffer);
		}

		const bool operator ==(const String &s);
		const bool operator !=(const String &s)
		{
			return !(operator ==(s));
		}

		const String operator +(const String s) const;
		const String operator +(const char *s);

		const String operator +(const int i) const;

		bool operator <(const String& s) const;

		operator const char *() const
		{
			return buffer;
		}

		operator char *()
		{
			return buffer;
		}

		const char* Ptr() const
		{
			return buffer;
		}
	};


	/*
	 * =============================================
	 * Trieda zakladnej vynimky (abstraktna trieda)
	 * =============================================
	*/

	class Exception
	{
	protected:
		Exception() {}

	public:
		virtual const String GetTypeString() const = 0;
		virtual const String GetErrorString() const = 0;
	};


	/*
	 * =============================================
	 * Trieda vseobecnej vynimky
	 * =============================================
	*/

	class GenericException : public Exception
	{
		String error;
		String file;
		int line;
		bool info;

	public:
		GenericException(String _error, String _file, int _line) : error(_error), file(_file), line(_line), info(true) {}
		GenericException(String _error) : error(_error), info(false) {}

		const String GetTypeString() const;
		const String GetErrorString() const;
	};


	/*
	 * =============================================
	 * Hlavna trieda pre objekty s iba jednou instanciou
	 * =============================================
	*/

	template <class T>
	class Singleton
	{
	private:
		T **singleton;

	protected:
		~Singleton()
		{
			*singleton = NULL;
		}

		Singleton(T *& instance)
		{
			instance = static_cast<T *>(this);
			singleton = &instance;
		}
	};


	/*
	 * =============================================
	 * Matematicke funkcie a konstanty
	 * =============================================
	*/

	const float PI = 3.1415926535897932384626433832795f;


	inline float Deg2Rad(float angle)
	{
		return (angle * PI / 180);
	}


	inline float Rad2Deg(float angle)
	{
		return (angle * 180 / PI);
	}


	inline float Sin(float angle)
	{
		return ((float) sin(angle));
	}


	inline float Cos(float angle)
	{
		return ((float) cos(angle));
	}


	inline float Sqrt(float x)
	{
		return ((float)sqrt(x));
	}


	inline float InvSqrt(float x)
	{
		return (1.0f / (float)sqrt(x));
	}


	inline float Random()
	{
		return (float)rand() / RAND_MAX;
	}


	/*
	 * =============================================
	 * Vrati cas v milisekundach od spustenia systemu
	 * =============================================
	*/

	inline float GetTime()
	{
	#if WIN

		LARGE_INTEGER freq, counter;

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&counter);

		return (counter.QuadPart * 1000.0f / freq.QuadPart);

	#elif LINUX

		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (ts.tv_sec * 1000.0f + tv.tv_nsec / 1000000.0f);

		/*struct timeval tv;

		gettimeofday(&tv, NULL);
	      
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);*/

	#endif
	}
}


#endif // V2Base_h
