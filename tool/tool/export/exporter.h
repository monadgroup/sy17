#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "journals.h"

namespace monad {
    class TimeManager;
}

namespace exporter {

    std::string minifyGlsl(std::string data);
    void exportShaders(std::stringstream &ss, const std::vector<monad::ShaderKey> &shaderJournal);
    void exportTracks(std::stringstream &ss, const std::vector<std::string> &trackJournal, const monad::TimeManager *manager);
    std::stringstream exportData(const std::vector<monad::ShaderKey> &shaderJournal, const std::vector<std::string> &trackJournal,
                                 const monad::TimeManager *manager);

}

