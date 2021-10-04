#include "time/timemanager.h"
#include "time/track_.h"
#include "../synth/Export.h"
#include "../synth/SongData.h"

using namespace monad;

TimeManager::TimeManager(double rowRate, const List<Scene *> &scenes) : rowRate(rowRate), scenes(scenes) {
    sceneTrack = new Track<float>(this, "scene");

    OmedaCommon::Util::CurrentSampleRate = sampleRate;
    OmedaCommon::Util::CurrentTempo = tempo;
    OmedaCommon::Util::CurrentPpq = ppq;

    auto data = new OmedaPlayer::SongData(staticBuffer, trackCount, tracks);
    auto player = new OmedaPlayer::SongPlayer(data);
    directPlayer = new OmedaPlayer::DirectSoundPlayer(player);
}

void TimeManager::updateTime() {
    row = directPlayer->GetPlayTime() * rowRate;
}

void TimeManager::update() {
    updateTime();
}

void TimeManager::play() {
    directPlayer->StartPlaying();
}
