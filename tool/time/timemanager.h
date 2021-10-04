#pragma once
#include "list.h"

#ifdef DEBUG
#include "lib/bass.h"
#else
#include "../synth/DirectSoundPlayer.h"
#endif

struct sync_device;
struct sync_track;

namespace monad {

    template<class T> class Track;

    class Scene;

    class TimeManager {
        List<Scene*> scenes;

        Track<float> *sceneTrack;

        double rowRate;
        double row = 0;

        void updateTime();

    public:
#ifdef DEBUG
        HSTREAM bass;
        sync_device *device;
#else
        OmedaPlayer::DirectSoundPlayer *directPlayer;
#endif

        explicit TimeManager(double rowRate, const List<Scene*> &scenes);

        void update();
        void play();
        void pause();

        double getRow() { return row; }
        Scene* getScene();

        void skipTo(double row);
        bool isPlaying();

        const sync_track *getTrack(const char *name);
    };

}
