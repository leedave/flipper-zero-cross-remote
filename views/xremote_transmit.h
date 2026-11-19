#pragma once

#include "generic_view.h"

void xremote_transmit_model_set_name(XRemoteViewInstance* instance, const char* name);
void xremote_transmit_model_set_type(XRemoteViewInstance* instance, int type);
// Seconds remaining for the active pause; shown as an MM:SS countdown.
void xremote_transmit_model_set_remaining(XRemoteViewInstance* instance, int remaining);

XRemoteViewInstance* xremote_transmit_alloc();

void xremote_transmit_enter(void* context);

bool xremote_transmit_input(InputEvent* event, void* context);