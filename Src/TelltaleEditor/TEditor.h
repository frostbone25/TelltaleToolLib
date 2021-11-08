#include "../TelltaleToolLibrary.h"

// Copyright Lucas Saragosa - Part of the Telltale Editor project. Some Job semantics from Telltale. Implementation is by me.

// This header should not be included in any files.

#ifndef TEDITOR_H
#define TEDITOR_H

#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>

#include "../TTArchive2.hpp"
#include "../Meta.hpp"

template<typename T> T* GetPtr(void* pPtr, int offset) {
	return *static_cast<T**>((void*)(((char*)pPtr) + offset));
}

template<typename T> T GetAt(void* pPtr, int offset) {
	return *static_cast<T*>((void*)(((char*)pPtr) + offset));
}

enum class JobResult {
	eJobResult_Succeeded = 0,
	eJobResult_Canceled = 1,
	eJobResult_Failed = 2,
	eJobResult_OutOfMemory = 3,
	eJobResult_Count = 4,
	eJobResult_Uninitialized = 0xFFFFFFFF,
};

//normal job waiting, state
enum class TryWaitResult {
	eResult_NotDone = 0x0,
	eResult_DoneSuccess = 0x1,
	eResult_DoneError = 0xFFFFFFFF
};

class CriticalJobAccess {
	std::recursive_mutex mAccessLock;
	TryWaitResult mState;
	JobResult mResult;
	float mProgress;
public:

	CriticalJobAccess() : mState(TryWaitResult::eResult_NotDone), mResult(JobResult::eJobResult_Uninitialized), mProgress(0.0f) {}

	TryWaitResult PollState() {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		return mState;
	}

	void SetState(TryWaitResult pState) {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		mState = pState;
		if (mState != TryWaitResult::eResult_NotDone)
			SetProgress(1.0f);
	}

	void SetProgress(float pProgress) {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		if (pProgress > 1 || pProgress < 0)return;
		mProgress = pProgress;
	}

	float GetProgress() {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		return mProgress;
	}

	JobResult GetResult() {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		return mResult;
	}

	void SetResult(JobResult pResult) {
		std::scoped_lock<std::recursive_mutex> _Lock(mAccessLock);
		mResult = pResult;
	}

	CriticalJobAccess(const CriticalJobAccess&) = delete;
	CriticalJobAccess(CriticalJobAccess&&) = delete;
	CriticalJobAccess& operator=(const CriticalJobAccess&) = delete;
	CriticalJobAccess& operator=(CriticalJobAccess&&) = delete;

};

#define IFFAILMSG(object,msg) if ((object) == NULL) {\
mAccessLock.SetResult(JobResult::eJobResult_Failed);\
TelltaleToolLib_RaiseError(msg, ERR);\
return;\
}\

#define IFFAIL(object) if ((object) == NULL) {\
mAccessLock.SetResult(JobResult::eJobResult_Failed);\
return;\
}\

#define FAIL mAccessLock.SetResult(JobResult::eJobResult_Failed)
#define SUCCEED mAccessLock.SetResult(JobResult::eJobResult_Succeeded)

#define JOB_MAX_SIZE (sizeof(Job) + 0x18)
#define JOB_POOL_SIZE 50

class Job {
public:

	//if true this job should be async
	bool mbProgressStatAvailable;
	bool mbExternMem;
	CriticalJobAccess mAccessLock;
	void* mResult;
	
	virtual void PerformOperation(void* pInput) = 0;
	virtual void* GetOutput() = 0;
	virtual ~Job() {}
	
};

void _DispatchJobAsync(Job* pJob,void* pInput) {
	pJob->PerformOperation(pInput);
	pJob->mAccessLock.SetState(pJob->mAccessLock.GetResult() == JobResult::eJobResult_Succeeded ?
		TryWaitResult::eResult_DoneSuccess : TryWaitResult::eResult_DoneError);
}

class Job_CreateObject : public Job {
public:

	Job_CreateObject() { mbProgressStatAvailable = false; mResult = NULL; }

	void PerformOperation(void* pInput) override {
		const char* type = *static_cast<const char**>(pInput);
		if (!_stricmp(type, "TTArchive2")) {
			mResult = new TTArchive2;
		}
		else if (!_stricmp(type, "MetaStream")) {
			mResult = new MetaStream(NULL);
		}
		else if (!_stricmp(type, "DataStreamFileDisc")) {
			const char* filepath = *static_cast<const char**>((void*)((char*)pInput + 8));
			IFFAIL(filepath);
			DataStreamMode mode = *static_cast<DataStreamMode*>((void*)((char*)pInput + 16));
			mResult = OpenDataStreamFromDisc(filepath, mode);
		}
		else {
			MetaClassDescription* typeClass = TelltaleToolLib_FindMetaClassDescription(type, true);
			if (!typeClass) {
				mAccessLock.SetResult(JobResult::eJobResult_Failed);
				return;
			}
			FuncNew func = (FuncNew)typeClass->mpVTable[VTableFunction::eVTableNew];
			if (!func) {
				mAccessLock.SetResult(JobResult::eJobResult_Failed);
				return;
			}
			mResult = func();
			FuncConstruct func1 = (FuncConstruct)typeClass->mpVTable[VTableFunction::eVTableConstruct];
			if (!func1) {
				mAccessLock.SetResult(JobResult::eJobResult_Failed);
				return;
			}
			func1(mResult);
		}
		mAccessLock.SetResult(mResult ? JobResult::eJobResult_Succeeded : JobResult::eJobResult_OutOfMemory);
	}

	void* GetOutput() override {
		return mResult;
	}

};

class Job_DeleteObject : public Job {
public:
	Job_DeleteObject() { mbProgressStatAvailable = false; }

	void* GetOutput() override { return NULL; }

	void PerformOperation(void* pInput) {
		const char* type = *static_cast<const char**>(pInput);
		void* obj = *((void**)((char*)pInput + sizeof(size_t)));
		if (obj == NULL) {
			mAccessLock.SetResult(JobResult::eJobResult_Failed);
			return;
		}
		if (!_stricmp(type, "TTArchive2")) {
			delete static_cast<TTArchive2*>(obj);
		}
		else if (!_stricmp(type, "MetaStream")) {
			delete static_cast<MetaStream*>(obj);
		}
		else if (!_stricmp(type, "DataStreamFileDisc")) {
			delete static_cast<DataStreamFileDisc*>(obj);
		}
		else if (!_stricmp(type, "DataStreamSubStream")) {
			delete static_cast<DataStreamSubStream*>(obj);
		}
		else {
			mAccessLock.SetResult(JobResult::eJobResult_Failed);
			return;
		}
		mAccessLock.SetResult(JobResult::eJobResult_Succeeded);
	}

};

class Job_SetHashDB : public Job {
public:
	Job_SetHashDB() { mbProgressStatAvailable = false; }

	void* GetOutput() override { return NULL; }

	void PerformOperation(void* pInput) {
		const char* fp = static_cast<const char*>(pInput);
		IFFAIL(fp);
		DataStream* dbstream = OpenDataStreamFromDisc(fp, DataStreamMode::eMode_Read);
		IFFAIL(dbstream->IsInvalid());
		TelltaleToolLib_SetGlobalHashDatabaseFromStream(dbstream);
		SUCCEED;
	}

};

class Job_GetArchiveInfo : public Job {
public:

	enum InfoOp {
		eNumFiles,
		eVersion,
		eEntryAt,
		eFindEntry,
		eEntryName,
		eEntrySize,
		eEntriesNameTable,
		eEntryNameTableIndex,
		eGetResourceStream,
		eEntryCrc,
	};
	Job_GetArchiveInfo() { mbProgressStatAvailable = false; mResult = NULL; }

	void* GetOutput() override { return mResult; }

	void PerformOperation(void* pInput) override {
		InfoOp op = *static_cast<InfoOp*>((void*)((char*)pInput + 8));
		if (op == InfoOp::eNumFiles) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			IFFAIL(arc);
			mResult = (void*)arc->mResources.size();
		}
		else if (op == InfoOp::eVersion) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			IFFAIL(arc);
			mResult = (void*)arc->mVersion;
		}
		else if (op == InfoOp::eEntryAt) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			IFFAIL(arc);
			int index = *static_cast<int*>((void*)((char*)pInput + 12));
			IFFAIL(index >= arc->mResources.size());
			mResult = arc->mResources.data() + index;
		}
		else if (op == InfoOp::eFindEntry) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			IFFAIL(arc);
			Symbol sym = *static_cast<u64*>((void*)((char*)pInput + 12));//unaligned!
			for (int i = 0; i < arc->mResources.size(); i++) {//O(n)
				if ((arc->mResources.data() + i)->mNameCRC == sym.GetCRC()) {
					mResult = arc->mResources.data() + i;
					return;
				}
			}
			FAIL;
			return;
		}
		else if (op == InfoOp::eEntrySize) {
			TTArchive2::ResourceEntry* e = *static_cast<TTArchive2::ResourceEntry**>(pInput);
			IFFAIL(e);
			mResult = (void*)e->mSize;
		}
		else if (op == InfoOp::eEntryName) {//8 byte crc, 4 byte operation, 8 byte archive ptr, 8 byte dest buf ptr, 28 bytes.
			u64 entrycrc = *static_cast<u64*>(pInput);
			TTArchive2* arc = *static_cast<TTArchive2**>((void*)((char*)pInput + 12));
			IFFAIL(arc);
			String result;
			arc->GetResourceName(entrycrc, &result);
			const char* cstr = result.c_str();
			char* destbuf = *static_cast<char**>((void*)((char*)pInput + 20));//this destbuf could be the buffer itself! this is allowed!
			memcpy(destbuf, cstr, result.size()+1);
			mResult = destbuf;//for convienience
		}  
		else if (op == InfoOp::eEntryNameTableIndex) {
			TTArchive2::ResourceEntry* e = *static_cast<TTArchive2::ResourceEntry**>(pInput);
			IFFAIL(e);
			mResult = (void*)(e->mNamePageIndex * 0x10000 + e->mNamePageOffset);
		}
		else if (op == InfoOp::eEntryCrc) {
			TTArchive2::ResourceEntry* e = *static_cast<TTArchive2::ResourceEntry**>(pInput);
			IFFAIL(e);
			mResult = (void*)(e->mNameCRC);
		}
		else if (op == InfoOp::eEntriesNameTable) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			IFFAIL(arc);
			int ntsize = arc->mNamePageCount * 0x10000;
			arc->mpNameStream->SetPosition(0, DataStreamSeekType::eSeekType_Begin);
			mResult = malloc(ntsize);
			if (!mResult) {
				FAIL;
				return;
			}
			arc->mpNameStream->Serialize((char*)mResult, ntsize);
		}
		else if (op == InfoOp::eGetResourceStream) {
			TTArchive2* arc = *static_cast<TTArchive2**>(pInput);
			TTArchive2::ResourceEntry* entry = static_cast<TTArchive2::ResourceEntry*>((void*)((char*)pInput + 12));//unaligned!
			IFFAIL(arc);
			IFFAIL(entry);
			mResult = arc->GetResourceStream(entry);
		}
		else {
			FAIL;
			return;
		}
		SUCCEED;
	}

};

class Job_MetaStreamOpen : public Job {
public:
	Job_MetaStreamOpen() { mbProgressStatAvailable = false; }


	void PerformOperation(void* pInput) override {
		MetaStream* stream = *static_cast<MetaStream**>(pInput);
		DataStream* data = *static_cast<DataStream**>((void*)((char*)pInput + 8));
		IFFAIL(stream);
		IFFAIL(data);
		stream->Open(data, data->mMode == DataStreamMode::eMode_Read ?
			MetaStreamMode::eMetaStream_Read : MetaStreamMode::eMetaStream_Write, { 0 });
		SUCCEED;
	}

	void* GetOutput() override {
		return NULL;
	}

};

class Job_MetaStreamClose : public Job {
public:
	Job_MetaStreamClose() { mbProgressStatAvailable = false; }


	void PerformOperation(void* pInput) override {
		MetaStream* stream = static_cast<MetaStream*>(pInput);
		IFFAIL(stream);
		stream->Close();
		SUCCEED;
	}

	void* GetOutput() override {
		return NULL;
	}

};

class Job_MetaSerialize : public Job {
public:
	Job_MetaSerialize() { 
		mbProgressStatAvailable = false; 
		mResult = 0;
	}

	void PerformOperation(void* pInput) override {
		MetaStream* stream = *static_cast<MetaStream**>(pInput);
		void* obj = *static_cast<void**>((void*)((char*)pInput + 8));
		const char* type = *static_cast<const char**>((void*)((char*)pInput + 16));
		bool by_ext = *static_cast<bool*>((void*)((char*)pInput + 24));
		IFFAIL(stream);
		IFFAIL(obj);
		IFFAIL(type);
		MetaClassDescription* clazz = TelltaleToolLib_FindMetaClassDescription(type, by_ext);
		if (!clazz) {
			FAIL;
			return;
		}
		mResult = (void*)PerformMetaSerializeFull(stream, obj, clazz);
		SUCCEED;
	}

	void* GetOutput() override {
		return mResult;
	}

};

class Job_PropertySet : public Job {
public:

	enum Op {
		eClear,
		eAddProp,
		eRemoveProp,
		eGetProp,
		eCopyPropSet,
		eGetData
	};

	Job_PropertySet() { mbProgressStatAvailable = false; mResult = NULL; }

	void PerformOperation(void* pInput) override;

	void* GetOutput() override {
		return mResult;
	}

};

class Job_PropertyValue : public Job {
public:

	enum Op {
		eGetValue = 0,
		eSetValue = 1,
		eExists = 2
	};

	Job_PropertyValue() { mbProgressStatAvailable = false;  mResult = NULL; }

	void PerformOperation(void* pInput) override;

	void* GetOutput() override {
		return mResult;
	}

};


class Job_ReadTTArchive2 : public Job {
public:
	Job_ReadTTArchive2() { mbProgressStatAvailable = true; }

	void PerformOperation(void* pInput) override {
		TTArchive2* archive = *static_cast<TTArchive2**>(pInput);
		DataStream* instream = *static_cast<DataStream**>((void*)((char*)pInput + sizeof(size_t)));
		IFFAIL(archive);
		IFFAIL(instream);
		archive->Activate(instream);
		SUCCEED;
	}

	void* GetOutput() override {
		return NULL;
	}

};

class Job_DataStream : public Job {
public:
	Job_DataStream() { mbProgressStatAvailable = false; }

	enum Op {
		eRead,
		eSetPos,
		eGetPos,
	};

	void PerformOperation(void* pInput) override {
		DataStream* ds = GetPtr<DataStream>(pInput, 0);
		IFFAIL(ds);
		int op = GetAt<int>(pInput, 8);
		if (op == Op::eRead) {
			int toread = GetAt<int>(pInput, 12);
			
		}
		else if (op == Op::eSetPos) {
			u64 pos = GetAt<u64>(pInput, 12);
			ds->SetPosition(pos, DataStreamSeekType::eSeekType_Begin);
		}
		else if (op == Op::eGetPos) {
			mResult = (void*)ds->GetPosition();
		}
		else {
			FAIL;
		}
		SUCCEED;
	}

	void* GetOutput() override {
		return mResult;
	}

};

class Job_WriteTTArchive2 : public Job {
public:
	Job_WriteTTArchive2() { mbProgressStatAvailable = true; }

	static void _cb(const char*, float) {}

	void PerformOperation(void* pInput) override {
		DataStream* dst = *static_cast<DataStream**>(pInput);
		TTArchive2::ResourceCreateEntry* entries = *static_cast<TTArchive2::ResourceCreateEntry**>((void*)((char*)pInput + sizeof(size_t)));
		int numentries = *static_cast<int*>((void*)((char*)pInput + sizeof(size_t) + sizeof(size_t)));
		bool compress = *static_cast<bool*>((void*)((char*)pInput + sizeof(size_t) + sizeof(size_t) + sizeof(int)));
		bool encrypt = *static_cast<bool*>((void*)((char*)pInput + sizeof(size_t) + sizeof(size_t) + sizeof(int) + sizeof(bool)));
		Compression::Library lib = *static_cast<Compression::Library*>((void*)((char*)pInput + sizeof(size_t) + 
			sizeof(size_t) + sizeof(int) + sizeof(bool) + sizeof(bool)));
		u32 version = *static_cast<u32*>((void*)((char*)pInput + sizeof(size_t) +
			sizeof(size_t) + sizeof(int) + sizeof(bool) + sizeof(bool) + sizeof(Compression::Library)));
		if (version > 2)version = 2;
		IFFAIL(dst);
		IFFAIL(entries);
		mResult = (void*)TTArchive2::Create(_cb,dst, entries, numentries, encrypt, compress, lib, version);
		SUCCEED;
	}

	void* GetOutput() override {
		return mResult;
	}

};

enum JobOp : int {
	eCreateObject,
	eDeleteObject,
	eSetHashDB,
	eArchiveInfo,
	eOpenMetaStream,
	eCloseMetaStream,
	eReadArchive,
	eWriteArchive,
	eMetaSerialize,
	ePropertyValue,
	ePropertySet,
	eDataStream
};

Job* DispatchJob(JobOp op, void* pInput, void *pJobMemory) {
	Job* jobptr = NULL;
	switch (op) {
	case JobOp::eCreateObject:
		jobptr = pJobMemory ? new (pJobMemory) Job_CreateObject : new Job_CreateObject;
		break;
	case JobOp::ePropertyValue:
		jobptr = pJobMemory ? new (pJobMemory)  Job_PropertyValue : new Job_PropertyValue;
		break;
	case JobOp::ePropertySet:
		jobptr = pJobMemory ? new (pJobMemory) Job_PropertySet : new Job_PropertySet;
		break;
	case JobOp::eMetaSerialize:
		jobptr = pJobMemory ? new (pJobMemory)  Job_MetaSerialize : new Job_MetaSerialize;
		break;
	case JobOp::eDeleteObject:
		jobptr = pJobMemory ? new (pJobMemory) Job_DeleteObject : new Job_DeleteObject;
		break;
	case JobOp::eSetHashDB:
		jobptr = pJobMemory ? new (pJobMemory) Job_SetHashDB : new Job_SetHashDB;
		break;
	case JobOp::eArchiveInfo:
		jobptr = pJobMemory ? new (pJobMemory) Job_GetArchiveInfo : new Job_GetArchiveInfo;
		break;
	case JobOp::eOpenMetaStream:
		jobptr = pJobMemory ? new (pJobMemory) Job_MetaStreamOpen : new Job_MetaStreamOpen;
		break;
	case JobOp::eCloseMetaStream:
		jobptr = pJobMemory ? new (pJobMemory) Job_MetaStreamClose: new Job_MetaStreamClose;
		break;
	case JobOp::eReadArchive:
		jobptr = pJobMemory ? new (pJobMemory) Job_ReadTTArchive2 : new Job_ReadTTArchive2;
		break;
	case JobOp::eWriteArchive:
		jobptr = pJobMemory ? new (pJobMemory) Job_WriteTTArchive2 : new Job_WriteTTArchive2;
		break;
	case JobOp::eDataStream:
		jobptr = pJobMemory ? new (pJobMemory) Job_DataStream : new Job_DataStream ;
		break;
	}
	if (!jobptr)return NULL;
	jobptr->mbExternMem = pJobMemory != NULL;
	if (jobptr->mbProgressStatAvailable) {
		std::thread t(_DispatchJobAsync, jobptr, pInput);
		t.detach();
	}
	else {
		jobptr->PerformOperation(pInput);
		jobptr->mAccessLock.SetState(jobptr->mAccessLock.GetResult() == JobResult::eJobResult_Succeeded ?
			TryWaitResult::eResult_DoneSuccess : TryWaitResult::eResult_DoneError);
	}
	return jobptr;
}

inline extern std::vector<int> giJobPoolObjectIndices = {};

void FinishJob(Job* pJob) {//call to destroy a job
	if (!pJob)return;
	if (pJob->mbExternMem)
		delete pJob;
}

_TTToolLib_Exp void TEditor_RemoveJobMemoryFromInternalPool(int pPoolIndex) {
	giJobPoolObjectIndices.erase(std::remove(giJobPoolObjectIndices.begin(), giJobPoolObjectIndices.end(), pPoolIndex),
		giJobPoolObjectIndices.end());
}

_TTToolLib_Exp Job* TEditor_TryGetJobMemoryFromInternalPool(int* pDestPoolIndex) {
	static char sJobMemoryPool[JOB_POOL_SIZE * JOB_MAX_SIZE];
	if (giJobPoolObjectIndices.capacity() != JOB_POOL_SIZE)
		giJobPoolObjectIndices.reserve(JOB_POOL_SIZE);
	if (giJobPoolObjectIndices.size() >= JOB_POOL_SIZE)
		return NULL;
	int index;
	for (index = 0; index < JOB_POOL_SIZE; index++) {
		if (std::find(giJobPoolObjectIndices.cbegin(), giJobPoolObjectIndices.cend(), index) != giJobPoolObjectIndices.cend())
			continue;
	}
	giJobPoolObjectIndices.push_back(index);
	*pDestPoolIndex = index;
	return (Job*)(sJobMemoryPool + index * JOB_MAX_SIZE);
}

_TTToolLib_Exp Job* TEditor_CreateDispatchJob2(JobOp _Operation, void* _Input, void* mem) {
	return DispatchJob(_Operation, _Input,mem);
}

_TTToolLib_Exp Job* TEditor_CreateDispatchJob(JobOp _Operation, void* _Input) {
	return DispatchJob(_Operation, _Input,NULL); 
}

_TTToolLib_Exp TryWaitResult TEditor_PollJobState(Job* _JobPtr) {
	if (!_JobPtr)return TryWaitResult::eResult_NotDone;
	return _JobPtr->mAccessLock.PollState();
}

_TTToolLib_Exp void* TEditor_GetOutput(Job* _JobPtr) {
	return _JobPtr ? _JobPtr->GetOutput() : NULL;
}

_TTToolLib_Exp float TEditor_PollJobProgress(Job* _JobPtr) {
	if (!_JobPtr)return -1.0f;
	return _JobPtr->mAccessLock.GetProgress();
}

_TTToolLib_Exp JobResult TEditor_PollJobResult(Job* _JobPtr) {
	if (!_JobPtr)return JobResult::eJobResult_Uninitialized;
	return _JobPtr->mAccessLock.GetResult();
}

_TTToolLib_Exp void TEditor_FinishJob(Job* _JobPtr) {
	FinishJob(_JobPtr);
}

_TTToolLib_Exp void TEditor_Free(char* _BytePtr) {
	if (!_BytePtr)return;
	free(_BytePtr);
}

_TTToolLib_Exp char* TEditor_Malloc(int _Size) {
	return (char*)malloc(_Size);
}

_TTToolLib_Exp void* TEditor_GetArgumentBuffer() {
	static char* _ArgBuf[512];
	return _ArgBuf;
}

_TTToolLib_Exp void TEditor_SetStdStringData(String* _Str, const char* _Data) {
	if (_Str) {
		if (_Data) {
			_Str->operator=(_Data);
		}
		else {
			_Str->clear();
		}
	}
}

_TTToolLib_Exp const char* TEditor_GetStdStringData(String* _Str) {
	return _Str ? _Str->c_str() : NULL;
}

#endif