#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <map>

namespace monad {
    class ShaderDependency;

    typedef std::shared_ptr<ShaderDependency> DependencyItem;
    typedef std::weak_ptr<ShaderDependency> DependentItem;
}

namespace monad {

    struct ShaderFragment {
        std::string source;
        std::string content;
        int line;

        ShaderFragment(std::string s, std::string c, int l) : source(std::move(s)), content(std::move(c)), line(l) {}
    };

    class ShaderDependency : public std::enable_shared_from_this<ShaderDependency> {
    public:
    private:
        typedef std::map<DependencyItem, DependencyItem> DependencyType;
        typedef std::map<DependentItem, DependentItem, std::owner_less<DependentItem>> DependentType;

        DependencyType dependencies;
        DependentType dependents;

    public:
        bool isDependency = true;
        std::string path;

        std::vector<ShaderFragment> fragments;

        explicit ShaderDependency(std::string p);
        virtual ~ShaderDependency();

        virtual void build();

        void prependInfo();
    };

}

