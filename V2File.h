#ifndef V2Files_h
#define V2Files_h


namespace V2
{
	/*
	 * =============================================
	 * Sposob otvorenia suboru
	 * =============================================
	*/

	enum FileOpenMode
	{
		kFileReadOnly,
		kFileReadWrite,
		kFileCreate
	};


	/*
	 * =============================================
	 * Urcenie pozicie v suboru
	 * =============================================
	*/

	enum FilePositioningMode
	{
		kFileBegin,
		kFileCurrent,
		kFileEnd
	};


	/*
	 * =============================================
	 * Vysledky chyb pri praci so subormi
	 * =============================================
	*/

	enum FileResult
	{
		kFileOkay,
		kFileOpenFailed,
		kFileIOFailed
	};


	/*
	 * =============================================
	 * Vynimka pri praci so subormi
	 * =============================================
	*/

	class FileException : public Exception
	{
	private:
		FileResult fileResult;

	public:
		FileException(FileResult result) : fileResult(result) {}

		const String GetTypeString() const;
		const String GetErrorString() const;
	};


	/*
	 * =============================================
	 * Trieda reprezentujuca subor
	 * =============================================
	*/

	class File
	{
	private:
	#if WIN

		HANDLE fileHandle;

	#elif LINUX

		int fileDesc;

	#endif

		unsigned long filePosition;
		volatile bool fileComplete;
		volatile FileResult fileResult;

	public:
		~File();
		File(const String name, FileOpenMode mode = kFileReadOnly);

		unsigned long GetPosition(void) const
		{
			return (filePosition);
		}

		FileResult GetResultCode(void) const
		{
			return (fileResult);
		}

		FileResult Read(void *buffer, unsigned long size);
		FileResult Write(const void *buffer, unsigned long size);

		unsigned long GetSize() const;
		unsigned long SetPosition(long position, FilePositioningMode mode = kFileBegin);

		static const String FileName(const String &name);
	};


	/*
	 * =============================================
	 * Hlavna trieda spravy suborov
	 * =============================================
	*/

	class FileMgr : public Singleton<FileMgr>
	{
	public:
		~FileMgr();
		FileMgr();

	};


	extern FileMgr *TheFileMgr;
}


#endif // V2Files_h
