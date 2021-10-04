#define _WIN32_WINNT _WIN32_WINNT_WINBLUE
#include "shadermanager.h"
#include "shaderdependency.h"
#include <shlobj.h>
#include <iostream>
#include <shlwapi.h>
#include "debug.h"
#include "dependency/util.h"

#define READ_DIR_CHANGE_BUFFER_SIZE 4096

using namespace std;
using namespace monad;

ShaderManager monad::globalManager{};

ShaderManager::ShaderManager() {
    InitializeCriticalSection(&watchCriticalSection);
    InitializeCriticalSection(&changeCriticalSection);
}

ShaderManager::~ShaderManager() {
    stopWatching();
    DeleteCriticalSection(&watchCriticalSection);
    DeleteCriticalSection(&changeCriticalSection);
}

void ShaderManager::startWatching(const std::string &root) {
    if (isWatching) return;
    isWatching = true;

    watchRoot = root;
    watchFile = CreateFile(root.c_str(),
                           FILE_LIST_DIRECTORY,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           nullptr,
                           OPEN_EXISTING,
                           FILE_FLAG_BACKUP_SEMANTICS,
                           nullptr);
    threadId = CreateThread(nullptr, 0, &watchThread, this, 0, nullptr);
    log(("Watching " + root).c_str());
}

void ShaderManager::stopWatching() {
    std::cout << "Stopping watching..." << std::endl;
    if (!isWatching) return;
    setIsWatching(false);


    std::cout << "Waiting for thread to finish" << std::endl;
    CancelIoEx(watchFile, nullptr);
    WaitForSingleObject(threadId, INFINITE);

    log("Stopped watching");
}

DWORD WINAPI ShaderManager::watchThread(void *param) {
    auto manager = (ShaderManager*)param;

    auto buffer = new char[READ_DIR_CHANGE_BUFFER_SIZE];

    while (manager->getIsWatching()) {
        DWORD bytesReturned;

        ReadDirectoryChangesW(
            manager->watchFile,
            buffer,
            READ_DIR_CHANGE_BUFFER_SIZE,
            TRUE,
            FILE_NOTIFY_CHANGE_LAST_WRITE,
            &bytesReturned,
            nullptr,
            nullptr
        );

        if (!manager->getIsWatching()) break;

        EnterCriticalSection(&manager->changeCriticalSection);
        auto nextEntry = buffer;
        while (true) {
            auto readPtr = nextEntry;

            auto nextEntryOffset = *(DWORD*)readPtr;
            readPtr += sizeof(DWORD);
            auto action = *(DWORD*)readPtr;

            if (action == FILE_ACTION_MODIFIED) {
                readPtr += sizeof(DWORD);
                auto fileNameLength = *(DWORD *) readPtr;
                readPtr += sizeof(DWORD);

                auto filenameW = std::wstring((WCHAR*)readPtr, fileNameLength / sizeof(WCHAR));
                auto filename = std::string(filenameW.begin(), filenameW.end());

                manager->changeList.insert(filename);
            }

            if (nextEntryOffset == 0) break;
            nextEntry += nextEntryOffset;
        }
        LeaveCriticalSection(&manager->changeCriticalSection);
    }

    delete[] buffer;
    CloseHandle(manager->watchFile);
}

void ShaderManager::update() {
    EnterCriticalSection(&changeCriticalSection);
    for (const auto &filename : changeList) {
        auto fullPath = concatPaths(watchRoot, filename);
        auto entry = cache.find(fullPath);
        if (entry != cache.end() && !entry->second.expired()) {
            entry->second.lock()->build();
        } else {
            log(("File " + fullPath + " is not used. " + (entry == cache.end() ? "(not in cache at all)" : "(has been removed)")).c_str());
        }
    }
    changeList.clear();
    LeaveCriticalSection(&changeCriticalSection);
}

void ShaderManager::setIsWatching(bool val) {
    EnterCriticalSection(&watchCriticalSection);
    isWatching = val;
    LeaveCriticalSection(&watchCriticalSection);
}

bool ShaderManager::getIsWatching() {
    EnterCriticalSection(&watchCriticalSection);
    auto val = isWatching;
    LeaveCriticalSection(&watchCriticalSection);
    return val;
}

shared_ptr<ShaderDependency> ShaderManager::get(const std::string &path) {
    auto ref = cache.find(path);

    auto exists = ref != cache.end();

    // if ref has expired, remove it and treat as if it didn't exist
    if (exists && ref->second.expired()) {
        cache.erase(ref);
        ref = cache.end();
        exists = false;
    }

    shared_ptr<ShaderDependency> ptr;

    if (!exists) {
        ptr = make_shared<ShaderDependency>(path);
        ptr->build();
        cache.insert(pair<const std::string, std::weak_ptr<ShaderDependency>>(path, ptr));
    } else ptr = ref->second.lock();

    return ptr;
}

void ShaderManager::add(std::shared_ptr<ShaderDependency> dep) {
    cache.insert(pair<const std::string, std::weak_ptr<ShaderDependency>>(dep->path, dep));
}

std::string ShaderManager::nicePath(const std::string &fullpath) {
    // get full path of shader directory
    char shaderDirectoryPath[MAX_PATH];
    GetFullPathName("../shaders", MAX_PATH, shaderDirectoryPath, nullptr);

    // get fullpath relative to shader directory
    char relativePath[MAX_PATH];
    PathRelativePathTo(relativePath, shaderDirectoryPath, FILE_ATTRIBUTE_DIRECTORY, fullpath.c_str(), FILE_ATTRIBUTE_NORMAL);

    auto relativeStr = std::string(relativePath);
    if (relativeStr.rfind(".\\", 0) == 0) relativeStr.erase(0, 2);
    return relativeStr;
}
