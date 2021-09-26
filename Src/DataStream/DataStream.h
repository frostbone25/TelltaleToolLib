// This file was written by Lucas Saragosa. Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma once

#include <cstdio>
#include <memory>
#include <vector>
#include <list>

#ifndef _DATASTREAM
#define _DATASTREAM

typedef FILE* FileHandle;

//this is for windows, if on POSIX then include unistd and set the platform specific truncate function to truncate
#include <io.h>
#define PlatformSpecTrunc(handle, newsize) _chsize_s(_fileno(handle), newsize)
#define PlatformSpecOpenFile(file_path) fopen(file_path,mode == DataStreamMode::eMode_Write ? "wb" : "rb")

enum class DataStreamMode : unsigned char {
	eMode_Unset = 0,
	eMode_Read = 1,
	eMode_Write = 2
};

enum class DataStreamSeekType : unsigned char {
	eSeekType_Begin = 0,
	eSeekType_Current = 1,
	eSeekType_End = 2
};

class DataStreamSubStream;

/*
* A data stream. Abstract class which represents a stream of data (bytes) being read or written.
* Class is *not copyable* but is moveable.
*/
class DataStream {
public:
	DataStreamMode mMode;
	int mSubStreams;

	/*
	* Serialize bytes. First is the buffer, second is the size. The mode member variable decides if its write or reading into the buffer.
	* Returns if this function was successful. 
	*/
	virtual bool Serialize(void*, unsigned __int64) = 0;

	/*
	* Gets the size in bytes of this stream.
	*/
	virtual unsigned __int64 GetSize() const = 0;

	/*
	* Gets the position (offset) of this stream.
	*/
	virtual unsigned __int64 GetPosition() const = 0;

	/*
	* Sets the position or offset of this stream. The position is deduced by the seek type parameter.
	*/
	virtual bool SetPosition(signed __int64, DataStreamSeekType) = 0;

	/*
	* Truncates this stream to the given new size. If its over the stream size, adds zeros (if this stream can do that, otherwise will
	* return false) otherwise it will remove all excess bytes. Not available for all types of data stream.
	*/
	virtual bool Truncate(unsigned __int64) = 0;

	/*
	* Gets a sub-stream of this stream. A sub-stream is a READ-ONLY (will return null if its not read mode) stream which points
	* to a section of a data stream. Like a std::string_view for a std::basic_string/std::string
	*/
	virtual DataStreamSubStream* GetSubStream(unsigned __int64 off,unsigned __int64 size);
	
	/*
	* Sets the mode of this stream. If there are substreams attached to this one and you try to set it to not read then it fails.
	*/
	bool SetMode(DataStreamMode);

	virtual bool IsRead() { return mMode == DataStreamMode::eMode_Read; }
	virtual bool IsWrite() { return mMode == DataStreamMode::eMode_Write; }
	virtual bool IsInvalid() { return mMode == DataStreamMode::eMode_Unset; }

	DataStream& operator=(DataStream&&) ;
	DataStream& operator=(DataStream const&) = delete;
	DataStream(DataStream& const) = delete;
	DataStream(DataStream&&) ;
	DataStream() : mMode(DataStreamMode::eMode_Unset) {}
	DataStream(DataStreamMode mode) : mMode(mode) {}

};

/*
* A data stream implementation which comes from a file on disc (specificly for Windows, implement this for other platforms if you need!).
* This class can be optimized a lot (using runtime buffers like Java's BufferedReader and BufferedWriter), so feel free to reimplement that.
*/
class DataStreamFile_Win : public DataStream {
public:
	FileHandle mHandle;
	__int64 mStreamOffset, mStreamSize;

	bool Serialize(void*, unsigned __int64);
	unsigned __int64 GetSize() const { return mStreamSize; }
	unsigned __int64 GetPosition() const { return mStreamOffset; };
	bool SetPosition(signed __int64, DataStreamSeekType);
	bool Truncate(unsigned __int64);

	DataStreamFile_Win(FileHandle);
	DataStreamFile_Win(FileHandle, DataStreamMode);
	DataStreamFile_Win(DataStreamFile_Win&&);
	DataStreamFile_Win& operator=(DataStreamFile_Win&&);
	DataStreamFile_Win(DataStreamFile_Win const&) = delete;
	DataStreamFile_Win& operator=(DataStreamFile_Win& const) = delete;
	~DataStreamFile_Win() { fclose(mHandle); };

};

class DataStreamSubStream : public DataStream {
public:
	DataStream* mpBase;
	unsigned __int64 mOffset, mStreamOffset, mSize;

	bool Serialize(void*, unsigned __int64);

	unsigned __int64 GetSize() const { return mSize; }

	unsigned __int64 GetPosition() const { return mOffset; };

	bool SetPosition(signed __int64, DataStreamSeekType);

	bool Truncate(unsigned __int64 newz) {
		if (newz <= mSize) {
			mSize = newz;
			if (mOffset > mSize)
				mOffset = mSize;
			return true;
		}
		return false;
	};

	virtual DataStreamSubStream* GetSubStream(unsigned __int64 off, unsigned __int64 size);

	DataStreamSubStream(DataStream*, unsigned __int64  size);//substream starts at base stream current offset, size is size (param 2)
	DataStreamSubStream(DataStream*, unsigned __int64 size, unsigned __int64 baseOffset);
	DataStreamSubStream(DataStreamSubStream&&);
	DataStreamSubStream& operator=(DataStreamSubStream&&);
	DataStreamSubStream(DataStreamSubStream& const) = delete;
	DataStreamSubStream& operator=(DataStreamSubStream& const) = delete;

	~DataStreamSubStream();

};

/*
* A write only stream, which writes to pages of memory in RAM. Has no positioning implemented. Deprecated.
*/
class DataStreamMemory : public DataStream {
public:
	std::vector<char*> mPageTable;
	unsigned __int64 mPageSize;
	unsigned __int64 mSize;

	bool Serialize(void*, unsigned __int64);
	unsigned __int64 GetSize() const { return mSize; }
	unsigned __int64 GetPosition() const { return mSize; };
	bool SetPosition(signed __int64, DataStreamSeekType) { return true; }
	bool Truncate(unsigned __int64) { return false; }//no need to truncate in memory

	DataStreamMemory(unsigned __int64 pageSize);
	DataStreamMemory(DataStreamMemory&&);
	DataStreamMemory& operator=(DataStreamMemory&&);
	DataStreamMemory(DataStreamMemory const&) = delete;
	DataStreamMemory& operator=(DataStreamMemory& const) = delete;
	~DataStreamMemory();
};

/*
* Change this to the class you want to use as the actual file reading implementation of DataStream for your platform.
*/
typedef DataStreamFile_Win DataStreamFileDisc;

#endif