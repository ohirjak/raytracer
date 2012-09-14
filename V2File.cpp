#include "V2Base.h"
#include "V2File.h"

#if LINUX || MACOS
#include <fcntl.h>
#endif


using namespace V2;


FileMgr *V2::TheFileMgr = NULL;


const String FileException::GetTypeString(void) const
{
	return "FileException";
}


const String FileException::GetErrorString(void) const
{
	static const String error[] =
	{
		"kFileOkay",
		"kFileOpenFailed",
		"kFileIOFailed"
	};
	
	return error[fileResult];
}


const String File::FileName(const String &name)
{
	String fileName = name;

#if WIN

	for (char *c = fileName;; ++c)
	{
		long k = *c;
		if (k == 0) break;
		if (k == '/') *c = '\\';
	}

#endif

	return fileName;
}


File::File(const String name, FileOpenMode mode)
{
#if WIN

	DWORD access = (mode == kFileReadOnly) ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE;
	DWORD creation = (mode == kFileCreate) ? CREATE_ALWAYS : OPEN_EXISTING;

	fileHandle = CreateFileA(FileName(name), access, FILE_SHARE_READ, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
		throw FileException(kFileOpenFailed);

#elif LINUX || MACOS

	int flags = (mode == kFileReadOnly) ? O_RDONLY : O_RDWR;
	if (mode == kFileCreate)
		flags |= O_CREAT | O_TRUNC;

	fileDesc = open(FileName(name), flags, 0644);
	if (fileDesc == -1)
		throw FileException(kFileOpenFailed);

#endif

	filePosition = 0;
	fileComplete = true;
	fileResult = kFileOkay;
}


File::~File()
{
#if WIN

	CloseHandle(fileHandle);

#elif LINUX || MACOS

	close(fileDesc);

#endif
}


FileResult File::Read(void *buffer, long size)
{
#if WIN

	DWORD	actual;

	SetFilePointer(fileHandle, filePosition, NULL, FILE_BEGIN);
	if (ReadFile(fileHandle, buffer, size, &actual, NULL) && (actual == size))
	{
		filePosition += size;
		return (kFileOkay);
	}

	return (kFileIOFailed);

#elif LINUX || MACOS

	lseek(fileDesc, filePosition, SEEK_SET);

	if (read(fileDesc, buffer, size) >= size)
	{
		filePosition += size;
		return (kFileOkay);
	}

	return (kFileIOFailed);

#endif
}


FileResult File::Write(const void *buffer, long size)
{
#if WIN

	DWORD	actual;

	SetFilePointer(fileHandle, filePosition, NULL, FILE_BEGIN);
	if (WriteFile(fileHandle, buffer, size, &actual, NULL) && (actual == size))
	{
		filePosition += size;
		return (kFileOkay);
	}

	return (kFileIOFailed);

#elif LINUX || MACOS

	lseek(fileDesc, filePosition, SEEK_SET);

	if (write(fileDesc, buffer, size) == size)
	{
		filePosition += size;
		return (kFileOkay);
	}

	return (kFileIOFailed);

#endif
}


unsigned long File::GetSize() const
{
#if WIN

	return (GetFileSize(fileHandle, NULL));

#elif LINUX || MACOS

	struct stat buf;

	fstat(fileDesc, &buf);

	return (buf.st_size);

#endif
}


unsigned long File::SetPosition(long position, FilePositioningMode mode)
{
	switch (mode)
	{
		case kFileBegin:

			filePosition = position;
			break;

		case kFileCurrent:

			filePosition += position;
			break;

		case kFileEnd:

			filePosition = GetSize() - position;
			break;
	}

	return (filePosition);
}


FileMgr::~FileMgr()
{
}


FileMgr::FileMgr() : Singleton<FileMgr>(TheFileMgr)
{
}
