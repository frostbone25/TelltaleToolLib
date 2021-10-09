#include "../TelltaleToolLibrary.h"

// Copyright Lucas Saragosa - Part of the Telltale Editor project. Job semantics and types from Telltale. Implementation is by me.

// This header should not be included in any files.

#ifndef FORCE_EXCLUDE_TEditor

#include <mutex>
#include <thread>
#include <vector>

enum class JobOp {
	eArchiveLoad = 0,
	eArchiveFlush = 1,
	eArchiveFilesNum = 2,
	eArchiveFileAtIndex = 3,
	eArchiveFindBySymbol = 4,
	eArchiveCreate = 5,
	eArchiveDestroy,
	eArchiveFileName,
	eArchiveSetFlags,
	eArchiveGetFlags,

};

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
	std::mutex mAccessLock;
	TryWaitResult mState;
	JobResult mResult;
	float mProgress;
public:

	CriticalJobAccess() : mState(TryWaitResult::eResult_NotDone), mResult(JobResult::eJobResult_Uninitialized) {}

	TryWaitResult PollState() {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		return mState;
	}

	void SetState(TryWaitResult pState) {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		mState = pState;
	}

	void SetProgress(float pProgress) {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		if (pProgress > 1 || pProgress < 0)return;
		mProgress = pProgress;
	}

	float GetProgress() {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		return mProgress;
	}

	JobResult GetResult() {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		return mResult;
	}

	void SetResult(JobResult pResult) {
		std::scoped_lock<std::mutex> _Lock(mAccessLock);
		mResult = pResult;
	}

	CriticalJobAccess(const CriticalJobAccess&) = delete;
	CriticalJobAccess(CriticalJobAccess&&) = delete;
	CriticalJobAccess& operator=(const CriticalJobAccess&) = delete;
	CriticalJobAccess& operator=(CriticalJobAccess&&) = delete;

};

class Job {
public:

	//if true this job should be async
	bool mbProgressStatAvailable;
	CriticalJobAccess mAccessLock;
	
	virtual void PerformOperation(void* pInput) = 0;
	virtual void* GetOutput() = 0;
	virtual ~Job() {}
	
};

void _DispatchJobAsync(Job* pJob,void* pInput) {
	pJob->PerformOperation(pInput);
}

Job* DispatchJob(JobOp op, void* pInput) {
	Job* jobptr = NULL;//todo set to class subclass
	if (jobptr->mbProgressStatAvailable) {
		std::thread t(_DispatchJobAsync, jobptr, pInput);
		t.detach();
	}
	else {
		jobptr->PerformOperation(pInput);
	}
	return jobptr;
}

void FinishJob(Job* pJob) {//call to destroy a job
	if (!pJob)return;
	delete pJob;
}

//TODO export functions should wrap it 

#endif

