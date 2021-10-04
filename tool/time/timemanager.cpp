#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "timemanager.h"
#include "track_.h"

using namespace monad;

/*void TimeManager::updateTime() {
    if (playing) {
        auto passedSeconds = (timeGetTime() - playStartTime) / 1000.0;
        row = playStartRow + passedSeconds * rowRate;
        playTime = row / rowRate;
    }
}

void TimeManager::play() {
    playing = true;
    playStartRow = row;
    playStartTime = (int)(timeGetTime());
}

Scene* TimeManager::getScene() {
    auto trackVal = sceneTrack->getValue(getRow());
    auto sceneSize = scenes.size();

    auto scene = (int)fmodf(trackVal, sceneSize);
    return scenes[scene];
}*/

Scene* TimeManager::getScene() {
    auto trackVal = sceneTrack->getValue(getRow());
    auto sceneSize = scenes.size();

    auto scene = (int)fmodf(trackVal, sceneSize);
    return scenes[scene];
}
