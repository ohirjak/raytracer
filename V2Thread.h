/*
 * @file: V2Windows.h
 * @author: Ondrej Hirjak, 2006 - 2007
 * @description: Windows includes.
 */

#ifndef V2Thread_h
#define V2Thread_h


namespace V2
{
	/*
	 * =============================================
	 * Trieda implementuje mutex
	 * =============================================
	*/

	class Mutex
	{
#if WIN

	private:
		HANDLE mutexHandle;

	public:
		~Mutex()
		{
			CloseHandle(mutexHandle);
		}

		Mutex()
		{
			mutexHandle = CreateMutex(NULL, FALSE, NULL);
		}

		void Acquire()
		{
			WaitForSingleObject(mutexHandle, INFINITE);
		}

		bool TryAcquire(void)
		{
			return (WaitForSingleObject(mutexHandle, 0) != WAIT_FAILED);
		}

		void Release()
		{
			ReleaseMutex(mutexHandle);
		}

#elif LINUX

	private:
		pthread_mutex_t	mutex;

	public:
		~Mutex()
		{
			pthread_mutex_destroy(&mutex);
		}

		Mutex()
		{
			pthread_mutex_init(&mutex, NULL);
		}

		void Acquire(void)
		{
			pthread_mutex_lock(&mutex);
		}

		bool TryAcquire(void)
		{
			return (pthread_mutex_trylock(&mutex) == 0);
		}

		void Release(void)
		{
			pthread_mutex_unlock(&mutex);
		}

#endif
	};

	enum
	{
		kThreadStackSize = 131072
	};


	/*
	 * =============================================
	 * Vysledky chyb pri praci s vlaknom
	 * =============================================
	*/

	enum ThreadResult
	{
		kThreadOkay,
		kThreadInitFailed
	};


	/*
	 * =============================================
	 * Vynimka pri praci s vlaknom
	 * =============================================
	*/

	class ThreadException : public Exception
	{
	private:
		ThreadResult threadResult;

	public:
		ThreadException(ThreadResult result) : threadResult(result) {}

		const String GetTypeString() const;
		const String GetErrorString() const;
	};


	/*
	 * =============================================
	 * Trieda implementujuca vlakno
	 * =============================================
	*/

	class Thread
	{
	public:
		typedef void (*ThreadRoutineType)(const Thread *, void *);

	private:
		ThreadRoutineType threadRoutine;
		void *threadData;

#if WIN

		HANDLE threadHandle;

		static DWORD WINAPI ThreadProc(LPVOID lpParameter);

#elif LINUX

		pthread_t	thread;

		static void * ThreadProc(void *data);

#endif

	public:
		~Thread();
		Thread(ThreadRoutineType routine, void *data, unsigned long stackSize = kThreadStackSize);

		void SetThreadAffinity(int index) const;

		static int GetProcessorNumber() {
#if WIN

			SYSTEM_INFO		systemInfo;
			
			GetSystemInfo(&systemInfo);

			return systemInfo.dwNumberOfProcessors;

#elif LINUX

			return sysconf(_SC_NPROCESSORS_ONLN);

#endif
		}
	};


	/*
	 * =============================================
	 * Hlavna trieda spravy vlaken, TODO
	 * =============================================
	*/

	class ThreadPool : public Singleton<ThreadPool>
	{
	public:
		~ThreadPool();
		ThreadPool();
	};


	extern ThreadPool *TheThreadPool;
}


#endif // V2Thread_h
