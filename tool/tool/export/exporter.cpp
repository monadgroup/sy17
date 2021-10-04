#include "exporter.h"
#include "dependency/shadermanager.h"
#include "dependency/shaderdependency.h"
#include "time/timemanager.h"
#include <regex>
#include <set>
#include <iostream>
#include "time/sync.h"
#include "time/track.h"

using namespace std;
using namespace exporter;
using namespace monad;

static regex stripSingleCommentRegex(R"(\/\/.*)", regex::ECMAScript);
static regex stripMultiCommentRegex(R"(\/\*[.\n]*?\*\/)", regex::ECMAScript);
static regex stripWhitespaceRegex(R"([^\S\n]+)", regex::ECMAScript);
static regex stripNewlineRegex(R"(\s*\n\s*)", regex::ECMAScript);

string exporter::minifyGlsl(string data) {
    data = regex_replace(data, stripSingleCommentRegex, "");
    data = regex_replace(data, stripMultiCommentRegex, "");
    data = regex_replace(data, stripWhitespaceRegex, " ");
    data = regex_replace(data, stripNewlineRegex, "\n");
    return data;
}

struct ShaderData {
    GLenum type;
    shared_ptr<ShaderDependency> shader;
};

bool operator<(const ShaderData &a, const ShaderData &b) {
    return a.type < b.type || a.shader < b.shader;
}

void exporter::exportShaders(stringstream &ss, const vector<ShaderKey> &shaderJournal) {
    // make deduplicated shader list
    set<ShaderData> shaderSet;
    for (const ShaderKey &key : shaderJournal) {
        auto itm = globalManager.get(key.name);
        if (itm->isDependency) itm->prependInfo();
        shaderSet.insert({ key.type, itm });
    }

    // make deduplicated fragment lists
    set<string> fragmentSet;
    for (const ShaderData &key : shaderSet) {
        for (const ShaderFragment &fragment : key.shader->fragments) {
            fragmentSet.insert(fragment.content);
        }
    }

    // build fragment list
    vector<unsigned char> fragmentData;
    map<string, UINT> fragmentPositions;
    int dbgi = 0;
    for (const string &fragmentString : fragmentSet) {
        fragmentPositions.insert(pair<string, UINT>(fragmentString, fragmentData.size()));
        auto minifiedShader = minifyGlsl(fragmentString);

        fragmentData.insert(fragmentData.end(), minifiedShader.begin(), minifiedShader.end());
        fragmentData.push_back(0);
    }

    // build shader list
    vector<UINT> shaderData;
    vector<unsigned char> shaderLengths;
    vector<GLenum> shaderTypes;
    map<string, unsigned char> shaderPositions;

    unsigned char i = 0;
    for (const ShaderData &key : shaderSet) {
        shaderPositions.insert(pair<string, unsigned char>(key.shader->path, i++));

        shaderTypes.push_back(key.type);
        shaderLengths.push_back((unsigned char)key.shader->fragments.size());
        for (const ShaderFragment &fragment : key.shader->fragments) {
            shaderData.push_back(fragmentPositions.find(fragment.content)->second);
        }
    }

    // build shader reference list
    vector<unsigned char> shaderRefs;
    for (const ShaderKey &key : shaderJournal) {
        shaderRefs.push_back(shaderPositions.find(key.name)->second);
    }

    // build the export string
    ss << "const char exports::shaderFragments[] = { ";
    for (const unsigned char &c : fragmentData) {
        ss << (int)c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned int exports::shaderOffsets[] = { ";
    for (const UINT &c : shaderData) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned char exports::shaderCount = " << shaderSet.size() << ";\n";
    ss << "const unsigned char exports::shaderLengths[] = { ";
    for (const unsigned char &c : shaderLengths) {
        ss << (int)c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const GLenum exports::shaderTypes[] = { ";
    for (const GLenum &c : shaderTypes) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned char exports::shaderMaps[] = { ";
    for (const unsigned char &c : shaderRefs) {
        ss << (int)c;
        ss << ", ";
    }
    ss << "};\n";
}

void exporter::exportTracks(stringstream &ss, const vector<string> &trackJournal, const TimeManager *manager) {
    // make deduplicated track list
    set<string> trackSet;
    for (const string &trackName : trackJournal) {
        trackSet.insert(trackName);
    }

    // put track data into arrays
    vector<UINT> trackRows;
    vector<float> trackValues;
    vector<enum key_type> trackTypes;
    vector<UINT> trackOffsets;
    vector<UINT> trackSizes;
    map<string, unsigned char> offsetMap;

    unsigned char i = 0;
    for (const string &trackName : trackSet) {
        auto track = sync_get_track(manager->device, trackName.c_str());

        offsetMap.insert(pair<string, unsigned char>(trackName, i++));
        trackOffsets.push_back(trackRows.size());
        trackSizes.push_back((UINT)track->num_keys);

        for (auto x = 0; x < track->num_keys; x++) {
            auto key = track->keys[x];
            trackRows.push_back((UINT)key.row);
            trackValues.push_back(key.value);
            trackTypes.push_back(key.type);
        }
    }

    // get track ref map
    vector<unsigned char> trackRefs;
    for (const string &trackName : trackJournal) {
        trackRefs.push_back(offsetMap.find(trackName)->second);
    }

    // build the export string
    ss << "const unsigned int exports::trackRows[] = { ";
    for (const UINT &c : trackRows) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const float exports::trackValues[] = { ";
    for (const float &c : trackValues) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const enum key_type exports::trackTypes[] = { ";
    for (const enum key_type &c : trackTypes) {
        switch (c) {
            case KEY_STEP: ss << "KEY_STEP"; break;
            case KEY_LINEAR: ss << "KEY_LINEAR"; break;
            case KEY_SMOOTH: ss << "KEY_SMOOTH"; break;
            case KEY_RAMP: ss << "KEY_RAMP"; break;
            default: break;
        }
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned char exports::trackCount = " << trackSet.size() << ";\n";
    ss << "const unsigned int exports::trackOffsets[] = { ";
    for (const UINT &c : trackOffsets) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned int exports::trackSizes[] = { ";
    for (const UINT &c : trackSizes) {
        ss << c;
        ss << ", ";
    }
    ss << "};\n";
    ss << "const unsigned char exports::trackMaps[] = { ";
    for (const unsigned char &c : trackRefs) {
        ss << (int)c;
        ss << ", ";
    }
    ss << "};\n";
}

stringstream exporter::exportData(const vector<ShaderKey> &shaderJournal, const vector<string> &trackJournal, const TimeManager *manager) {
    stringstream ss;
    ss << "/* This is an automatically generated file. Do not edit. */\n";
    ss << "#include \"export.h\"\n\n";

    exportShaders(ss, shaderJournal);
    exportTracks(ss, trackJournal, manager);

    return ss;
}
