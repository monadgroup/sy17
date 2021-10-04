#include "time/timemanager.h"
#include "time/sync.h"
#include "debug.h"
#include "time/track_.h"
#include "export/journals.h"

using namespace monad;

static void sync_pause(void *d, int flag) {
    auto manager = (TimeManager*)d;
    if (flag) manager->play();
    else manager->pause();
}

static void sync_getrow(void *d, int row) {
    ((TimeManager*)d)->skipTo(row);
}

static int sync_isplaying(void *d) {
    return ((TimeManager*)d)->isPlaying();
}

static struct sync_cb sync_cb = {
        sync_pause, sync_getrow, sync_isplaying
};


TimeManager::TimeManager(double rowRate, const List<Scene*> &scenes) : device(sync_create_device("sync")), scenes(scenes), rowRate(rowRate) {
    // initialize BASS
    if (!BASS_Init(-1, 44100, 0, nullptr, nullptr)) log("Failed to init BASS");
    bass = BASS_StreamCreateFile(false, "../../audio.ogg", 0, 0, BASS_STREAM_PRESCAN);

    if (!bass) log("Failed to open audio");

    // initialize tracker
    if (sync_tcp_connect(device, "localhost", SYNC_DEFAULT_PORT)) {
        log("Failed to connect to tracker!");
    } else {
        log("Connected to tracker successfully.");
    }

    sceneTrack = new Track<float>(this, "scene");
}

void TimeManager::updateTime() {
    QWORD pos = BASS_ChannelGetPosition(bass, BASS_POS_BYTE);
    double time = BASS_ChannelBytes2Seconds(bass, pos);
    row = time * rowRate;
}

void TimeManager::update() {
    updateTime();

    if (sync_update(device, (int)row, &sync_cb, this) && sync_tcp_connect(device, "localhost", SYNC_DEFAULT_PORT)) {
        log("Lost connection to tracker :(");
    }
}

void TimeManager::play() {
    BASS_ChannelPlay(bass, false);
}

void TimeManager::pause() {
    BASS_ChannelPause(bass);
}

void TimeManager::skipTo(double row) {
    auto pos = BASS_ChannelSeconds2Bytes(bass, row / rowRate);
    BASS_ChannelSetPosition(bass, pos, BASS_POS_BYTE);
}

bool TimeManager::isPlaying() {
    return BASS_ChannelIsActive(bass) == BASS_ACTIVE_PLAYING;
}

std::vector<std::string> monad::trackJournal;

const sync_track *TimeManager::getTrack(const char *name) {
    trackJournal.emplace_back(name);
    return sync_get_track(device, name);
}
