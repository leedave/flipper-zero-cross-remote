#pragma once

#include <gui/view.h>
#include <gui/elements.h>
#include <gui/view_dispatcher.h>
#include "../helpers/xremote_custom_event.h"

typedef void (*XRemoteViewCallback)(XRemoteCustomEvent event, void* context);

typedef struct {
    View* view;
    XRemoteViewCallback callback;
    void* context;
} XRemoteViewInstance;

View* xremote_view_get(XRemoteViewInstance* instance);

void xremote_view_free(XRemoteViewInstance* instance);

void xremote_view_set_callback(
    XRemoteViewInstance* instance,
    XRemoteViewCallback callback,
    void* context);
