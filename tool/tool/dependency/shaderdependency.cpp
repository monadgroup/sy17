#include "shaderdependency.h"
#include "shadermanager.h"
#include "util.h"
#include <regex>
#include <fstream>
#include <iostream>
#include "debug.h"

using namespace std;
using namespace monad;

static regex pathRegex("#include\\s+\"(.*)\"", regex::ECMAScript);

ShaderDependency::ShaderDependency(string p) : path(move(p)) {
}

ShaderDependency::~ShaderDependency() {
    log((path + " is being removed").c_str());
}

void ShaderDependency::build() {
    fragments.clear();

    // read file
    ifstream t(path);
    if (t.fail()) {
        log((path + " does not exist. Aborting build.").c_str());
        return;
    }

    stringstream buffer;
    buffer << t.rdbuf();
    auto bufferStr = buffer.str();
    bufferStr.erase(std::remove(bufferStr.begin(), bufferStr.end(), '\r'), bufferStr.end());

    auto ownPointer = shared_from_this();

    // iterate over current dependencies and remove them
    for (const auto &dependency : dependencies) {
        dependency.second->dependents.erase(ownPointer);
    }

    DependencyType newDependencies;
    int startLine = 0;

    // split text into fragments
    smatch m;
    while (regex_search(bufferStr, m, pathRegex)) {
        // add content before regex to fragment
        auto pos = (unsigned int)m.position(0);
        fragments.emplace_back(path, bufferStr.substr(0, pos), startLine);

        // get contents of destination
        // todo: only include each shader once
        auto destRelPath = m[1].str();
        std::replace(destRelPath.begin(), destRelPath.end(), '/', '\\');
        auto destFullPath = concatPaths(dirName(path), destRelPath);
        auto dependency = globalManager.get(destFullPath);
        newDependencies.insert(pair<DependencyItem, DependencyItem>(dependency, dependency));
        dependency->dependents.insert(pair<DependentItem, DependentItem>(ownPointer, ownPointer));

        // insert all fragments from destination
        fragments.insert(fragments.end(), dependency->fragments.begin(), dependency->fragments.end());

        startLine += std::count(bufferStr.begin(), bufferStr.begin() + pos, '\n');
        bufferStr = m.suffix().str();
    }

    // only clear now to avoid deleting re-used dependencies
    dependencies = newDependencies;

    // add a fragment for the remaining content
    fragments.emplace_back(path, bufferStr, startLine);

    vector<std::shared_ptr<ShaderDependency>> updateDependents;

    // find all dependents we need to update, delete ones that have been freed
    for (auto depends = dependents.begin(); depends != dependents.end();) {
        if (depends->second.expired()) depends = dependents.erase(depends);
        else {
            updateDependents.push_back(depends->second.lock());
            depends++;
        }
    }

    for (const auto &depends : updateDependents) {
        depends->build();
    }
}

void ShaderDependency::prependInfo() {
    fragments.insert(fragments.begin(), {"Internal", "#version 450\n/* the cake is a lie. */\n", 0});
}
