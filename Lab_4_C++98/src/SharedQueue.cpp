#include "../include/SharedQueue.h"
#include <cstring>
#include <algorithm>

SharedQueue::SharedQueue()
: hFile_(INVALID_HANDLE_VALUE),
  hMap_(NULL),
  header_(NULL),
  slots_(NULL),
  hMutex_(NULL),
  hSemEmpty_(NULL),
  hSemFull_(NULL),
  hAllReadyEvent_(NULL) {}

SharedQueue::~SharedQueue() {
    CloseAll();
}

std::string MakeObjectBaseName(const std::string& fileName) {
    std::string base;
    base.reserve(fileName.size());
    size_t i;
    for (i = 0; i < fileName.size(); ++i) {
        char c = fileName[i];
        if (c == '\\' || c == '/' || c == ':')
            base.push_back('_');
        else
            base.push_back(c);
    }
    return base;
}

std::string MakeNamedObject(const std::string& base, const std::string& suffix) {
    // No Global\ prefix to avoid privilege issues
    return std::string("IPC_") + base + "_" + suffix;
}

void SharedQueue::CloseAll() {
    if (header_) {
        UnmapViewOfFile(header_);
        header_ = NULL;
        slots_ = NULL;
    }
    if (hMap_) { CloseHandle(hMap_); hMap_ = NULL; }
    if (hFile_ != INVALID_HANDLE_VALUE) { CloseHandle(hFile_); hFile_ = INVALID_HANDLE_VALUE; }

    if (hMutex_) { CloseHandle(hMutex_); hMutex_ = NULL; }
    if (hSemEmpty_) { CloseHandle(hSemEmpty_); hSemEmpty_ = NULL; }
    if (hSemFull_) { CloseHandle(hSemFull_); hSemFull_ = NULL; }
    if (hAllReadyEvent_) { CloseHandle(hAllReadyEvent_); hAllReadyEvent_ = NULL; }
}

bool SharedQueue::MapFile(const std::string& path, bool create, unsigned int capacity) {
    DWORD access = GENERIC_READ | GENERIC_WRITE;
    DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD disp = create ? CREATE_ALWAYS : OPEN_EXISTING;

    hFile_ = CreateFileA(path.c_str(), access, share, NULL, disp,
                         FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile_ == INVALID_HANDLE_VALUE) {
        PrintLastErrorA("Create/Open file failed");
        return false;
    }

    DWORD totalSize;
    if (create) {
        totalSize = (DWORD)(sizeof(QueueHeader) + capacity * sizeof(MessageSlot));
    } else {
        LARGE_INTEGER sz;
        if (!GetFileSizeEx(hFile_, &sz)) {
            PrintLastErrorA("GetFileSizeEx");
            return false;
        }
        totalSize = (DWORD)sz.QuadPart;
    }

    hMap_ = CreateFileMappingA(
        hFile_, NULL, PAGE_READWRITE,
        0, totalSize, NULL
    );
    if (!hMap_) {
        PrintLastErrorA("CreateFileMapping");
        return false;
    }

    header_ = (QueueHeader*)MapViewOfFile(hMap_, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!header_) {
        PrintLastErrorA("MapViewOfFile");
        return false;
    }
    slots_ = (MessageSlot*)((char*)header_ + sizeof(QueueHeader));
    return true;
}

bool SharedQueue::OpenSyncObjects(bool create, unsigned int capacity, unsigned int expectedSenders) {
    std::string mutexName = MakeNamedObject(baseName_, "mtx");
    std::string semEmptyName = MakeNamedObject(baseName_, "semEmpty");
    std::string semFullName  = MakeNamedObject(baseName_, "semFull");
    std::string readyEventName = MakeNamedObject(baseName_, "allReady");

    if (create) {
        hMutex_ = CreateMutexA(NULL, FALSE, mutexName.c_str());
        if (!hMutex_) { PrintLastErrorA("CreateMutex"); return false; }

        hSemEmpty_ = CreateSemaphoreA(NULL, capacity, capacity, semEmptyName.c_str());
        if (!hSemEmpty_) { PrintLastErrorA("CreateSemaphore empty"); return false; }

        hSemFull_ = CreateSemaphoreA(NULL, 0, capacity, semFullName.c_str());
        if (!hSemFull_) { PrintLastErrorA("CreateSemaphore full"); return false; }

        hAllReadyEvent_ = CreateEventA(NULL, TRUE, FALSE, readyEventName.c_str());
        if (!hAllReadyEvent_) { PrintLastErrorA("CreateEvent allReady"); return false; }

        header_->capacity = capacity;
        header_->msgLen = MAX_MESSAGE_LEN;
        header_->readIndex = 0;
        header_->writeIndex = 0;
        header_->senderReadyCount = 0;
        header_->expectedSenders = expectedSenders;
        header_->shuttingDown = 0;
        header_->reserved[0] = header_->reserved[1] = header_->reserved[2] = 0;
    } else {
        hMutex_ = OpenMutexA(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, mutexName.c_str());
        if (!hMutex_) { PrintLastErrorA("OpenMutex"); return false; }

        hSemEmpty_ = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, semEmptyName.c_str());
        if (!hSemEmpty_) { PrintLastErrorA("OpenSemaphore empty"); return false; }

        hSemFull_ = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, semFullName.c_str());
        if (!hSemFull_) { PrintLastErrorA("OpenSemaphore full"); return false; }

        hAllReadyEvent_ = OpenEventA(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, readyEventName.c_str());
        if (!hAllReadyEvent_) { PrintLastErrorA("OpenEvent allReady"); return false; }
    }
    return true;
}

bool SharedQueue::CreateAsReceiver(const std::string& fileName,
                                   unsigned int capacity,
                                   unsigned int expectedSenders) {
    baseName_ = MakeObjectBaseName(fileName);
    if (!MapFile(fileName, true, capacity))
        return false;
    if (!OpenSyncObjects(true, capacity, expectedSenders))
        return false;
    printf("[Receiver] Queue created: capacity=%u, senders=%u\n",
           capacity, expectedSenders);
    return true;
}

bool SharedQueue::OpenAsSender(const std::string& fileName) {
    baseName_ = MakeObjectBaseName(fileName);
    if (!MapFile(fileName, false, 0))
        return false;
    if (!OpenSyncObjects(false, 0, 0))
        return false;
    if (header_->msgLen != MAX_MESSAGE_LEN) {
        printf("[Sender] Incompatible message length\n");
        return false;
    }
    return true;
}

bool SharedQueue::IsValid() const {
    return header_ != NULL;
}

bool SharedQueue::WaitAllSendersReady(DWORD timeoutMs) {
    DWORD res = WaitForSingleObject(hAllReadyEvent_, timeoutMs);
    if (res == WAIT_OBJECT_0) return true;
    if (res == WAIT_TIMEOUT) printf("[Receiver] Sender readiness wait timeout\n");
    else PrintLastErrorA("WaitAllSendersReady");
    return false;
}

bool SharedQueue::SignalSenderReady() {
    DWORD waitRes = WaitForSingleObject(hMutex_, INFINITE);
    if (waitRes != WAIT_OBJECT_0) {
        PrintLastErrorA("Mutex wait SignalSenderReady");
        return false;
    }
    header_->senderReadyCount++;
    if (header_->senderReadyCount == header_->expectedSenders) {
        SetEvent(hAllReadyEvent_);
    }
    ReleaseMutex(hMutex_);
    return true;
}

void SharedQueue::SignalShutdown() {
    if (!header_) return;
    WaitForSingleObject(hMutex_, INFINITE);
    header_->shuttingDown = 1;
    ReleaseMutex(hMutex_);

    ReleaseSemaphore(hSemFull_, header_->capacity, NULL);
    ReleaseSemaphore(hSemEmpty_, header_->capacity, NULL);
    SetEvent(hAllReadyEvent_);
}

bool SharedQueue::IsShuttingDown() const {
    return header_ != NULL && header_->shuttingDown != 0;
}

bool SharedQueue::PopMessage(std::string &outMsg, bool verbose) {
    outMsg.clear();
    if (!IsValid()) return false;

    DWORD wr = WaitForSingleObject(hSemFull_, INFINITE);
    if (wr != WAIT_OBJECT_0) {
        PrintLastErrorA("Wait semFull");
        return false;
    }

    WaitForSingleObject(hMutex_, INFINITE);

    if (header_->shuttingDown &&
        header_->readIndex == header_->writeIndex) {
        ReleaseMutex(hMutex_);
        return false;
    }

    unsigned int idx = header_->readIndex % header_->capacity;
    outMsg = slots_[idx].data;
    header_->readIndex = (header_->readIndex + 1) % header_->capacity;

    ReleaseMutex(hMutex_);
    ReleaseSemaphore(hSemEmpty_, 1, NULL);

    if (verbose) {
        printf("[Receiver] Read message: '%s'\n", outMsg.c_str());
    }
    return true;
}

bool SharedQueue::PushMessage(const std::string& msg, bool verbose) {
    if (!IsValid()) return false;
    if (msg.size() >= MAX_MESSAGE_LEN) {
        printf("[Sender] Message too long (>= %u)\n", MAX_MESSAGE_LEN);
        return false;
    }

    DWORD wr = WaitForSingleObject(hSemEmpty_, INFINITE);
    if (wr != WAIT_OBJECT_0) {
        PrintLastErrorA("Wait semEmpty");
        return false;
    }

    DWORD mw = WaitForSingleObject(hMutex_, INFINITE);
    if (mw != WAIT_OBJECT_0) {
        PrintLastErrorA("Mutex wait push");
        return false;
    }
    if (header_->shuttingDown) {
        ReleaseMutex(hMutex_);
        return false;
    }

    unsigned int idx = header_->writeIndex % header_->capacity;
    memset(slots_[idx].data, 0, MAX_MESSAGE_LEN);
    memcpy(slots_[idx].data, msg.c_str(), msg.size());
    header_->writeIndex = (header_->writeIndex + 1) % header_->capacity;

    ReleaseMutex(hMutex_);
    ReleaseSemaphore(hSemFull_, 1, NULL);

    if (verbose) {
        printf("[Sender] Sent message: '%s'\n", msg.c_str());
    }
    return true;
}