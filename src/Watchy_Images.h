#ifndef WATCHY_IMAGES_H
#define WATCHY_IMAGES_H

#include <Watchy.h>
#include "images.h"

class WatchyImages : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void syncTime();
        void drawBackground();
        void drawSpiral(int strokeSize, int maxRadius, int maxRatio);
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
};

#endif