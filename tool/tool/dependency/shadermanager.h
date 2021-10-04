#pragma once
#include <map>
#include <memory>
#include <string>
#include <set>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define SH_CHANGENOTIFY 2000

namespace monad {

    class ShaderDependency;

    class ShaderManager {
        std::map<const std::string, std::weak_ptr<ShaderDependency>> cache;
        std::set<std::string> changeList;

        bool isWatching = false;
        CRITICAL_SECTION watchCriticalSection;
        CRITICAL_SECTION changeCriticalSection;
        std::string watchRoot;
        HANDLE threadId;
        HANDLE watchFile;

        static DWORD WINAPI watchThread(void *param);

        void setIsWatching(bool val);
        bool getIsWatching();

    public:
        ShaderManager();
        ~ShaderManager();

        void startWatching(const std::string &root);
        void update();
        void stopWatching();

        std::shared_ptr<ShaderDependency> get(const std::string &path);
        void add(std::shared_ptr<ShaderDependency> dep);

        static std::string nicePath(const std::string &fullpath);
    };

    extern ShaderManager globalManager;

}
