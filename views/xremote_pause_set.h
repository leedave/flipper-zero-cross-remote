#pragma once

#include "generic_view.h"

// Pause duration is stored in SECONDS. Displayed as MM:SS.
#define PAUSE_TIME_LENGTH 6 // "MM:SS\0"
#define PAUSE_TIME_FORMAT "%02d:%02d"
#define XREMOTE_PAUSE_MAX_SECONDS 3600 // 60 minutes
#define XREMOTE_PAUSE_FAST_STEP 5 // seconds added per held (repeat) press
#define XREMOTE_PAUSE_FIELD_MINUTES 0
#define XREMOTE_PAUSE_FIELD_SECONDS 1

XRemoteViewInstance* xremote_pause_set_alloc();

void xremote_pause_set_enter(void* context);
