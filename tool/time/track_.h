#pragma once
#include "timemanager.h"

struct sync_track;

namespace monad {

    template<class T>
    class Track {
        TimeManager *manager;
        const int trackCount;
        const sync_track **tracks;

        void init() {
            tracks = new const sync_track*[trackCount];
        }

    public:
        Track(TimeManager *manager, const char* name);
        ~Track() { delete[] tracks; }

        T getValue() {
            auto row = manager->getRow();
            return getValue(row);
        }
        T getValue(double rows);
    };

}
