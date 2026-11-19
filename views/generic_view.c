#include "generic_view.h"
#include <stdlib.h>

View* xremote_view_get(XRemoteViewInstance* instance) {
    furi_assert(instance);
    return instance->view;
}

void xremote_view_free(XRemoteViewInstance* instance) {
    furi_assert(instance);
    if(instance->view) {
        view_free(instance->view);
    }
    free(instance);
}

void xremote_view_set_callback(
    XRemoteViewInstance* instance,
    XRemoteViewCallback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}
