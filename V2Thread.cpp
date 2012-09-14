#include "V2Base.h"
#include "V2Thread.h"


using namespace V2;


ThreadPool *V2::TheThreadPool = NULL;


const String ThreadException::GetTypeString() const
{
	return "Thread exception";
}


const String ThreadException::GetErrorString() const
{
	static const String error[] =
	{
		"ThreadOkay",
		"ThreadInitFailed"
	};

	return error[threadResult];
}


#if WIN

DWORD WINAPI Thread::ThreadProc(LPVOID lpParameter)
{
	ASSERT(lpParameter);
	Thread *thread = static_cast<Thread *>(lpParameter);
	ASSERT(thread->threadRoutine);
	thread->threadRoutine(thread, thread->threadData);
	return 0;
}

#elif LINUX || MACOS

void * Thread::ThreadProc(void *data)
{
	Thread *thread = static_cast<Thread *>(data);
	(*thread->threadRoutine)(thread, thread->threadData);
	return NULL;
}

#endif


Thread::~Thread()
{
#if WIN

	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);

#elif LINUX || MACOS

	pthread_join(thread, NULL);

#endif
}


Thread::Thread(ThreadRoutineType routine, void *data, unsigned long stackSize)
{
	threadRoutine = routine;
	threadData = data;

#if WIN

	threadHandle = CreateThread(NULL, stackSize, &ThreadProc, this, 0, NULL);

	if (threadHandle == NULL)
		throw ThreadException(kThreadInitFailed);

#elif LINUX || MACOS

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, (size_t)stackSize);
	pthread_create(&thread, &attr, &ThreadProc, this);
	pthread_attr_destroy(&attr);

#endif
}

void Thread::SetThreadAffinity(int index) const
{
#if WIN

	SetThreadIdealProcessor(threadHandle, index);

#elif LINUX

	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(index, &cpuset);

	if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset))
		fprintf(stderr, "setaffinity error\n");

#elif MACOS

	// TODO: MAC impl

#endif
}


ThreadPool::~ThreadPool()
{
}


ThreadPool::ThreadPool() : Singleton<ThreadPool>(TheThreadPool)
{
}
