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

TimeManager::TimeManager(double rowRate, const List<Scene *> &scenes) : device(sync_create_device("sync")), scenes(scenes), rowRate(rowRate) {
    if (sync_tcp_connect(device, "localhost", SYNC_DEFAULT_PORT)) {
        log("Failed to connect to tracker!");
    } else {
        log("Connected to tracker successfully.");
    }

    sceneTrack = new Track<float>(this, "scene");
}

void TimeManager::update() {
    updateTime();

    if (sync_update(device, (int)row, &sync_cb, this) && sync_tcp_connect(device, "localhost", SYNC_DEFAULT_PORT)) {
        log("Lost connection to tracker :(");
    }
}

TimeManager::~TimeManager() {
    delete sceneTrack;
    sync_destroy_device(device);
}

void TimeManager::pause() {
    playing = false;
}

void TimeManager::skipTo(double newRow) {
    row = newRow;
    if (playing) play();
}

std::vector<std::string> monad::trackJournal;

const sync_track* TimeManager::getTrack(const char *name) {
    trackJournal.emplace_back(name);
    return sync_get_track(device, name);
}
