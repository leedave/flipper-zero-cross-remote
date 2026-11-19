#include "xremote_infoscreen.h"
#include "../xremote.h"

typedef struct {
    int some_value;
} XRemoteInfoScreenModel;

void xremote_infoscreen_draw(Canvas* canvas, XRemoteInfoScreenModel* model) {
    UNUSED(model);
    char buffer[64];
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignTop, "Cross Remote");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 22, AlignCenter, AlignTop, "Chain IR and SubGhz");
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "Commands");

    snprintf(buffer, sizeof(buffer), "Version: %s", XREMOTE_VERSION);
    canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignTop, buffer);
    elements_button_center(canvas, "Back");
}

bool xremote_infoscreen_input(InputEvent* event, void* context) {
    furi_assert(context);
    XRemoteViewInstance* instance = context;
    if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyOk:
            with_view_model(
                instance->view,
                XRemoteInfoScreenModel * model,
                {
                    UNUSED(model);
                    instance->callback(XRemoteCustomEventInfoScreenOk, instance->context);
                },
                true);
            break;
        default:
            break;
        }
    }
    return true;
}

void xremote_infoscreen_exit(void* context) {
    furi_assert(context);
}

void xremote_infoscreen_enter(void* context) {
    furi_assert(context);
    XRemoteViewInstance* instance = (XRemoteViewInstance*)context;
    with_view_model(instance->view, XRemoteInfoScreenModel * model, { UNUSED(model); }, true);
}

XRemoteViewInstance* xremote_infoscreen_alloc() {
    XRemoteViewInstance* instance = malloc(sizeof(XRemoteViewInstance));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(XRemoteInfoScreenModel));
    view_set_context(instance->view, instance); // furi_assert crashes in events without this
    view_set_draw_callback(instance->view, (ViewDrawCallback)xremote_infoscreen_draw);
    view_set_input_callback(instance->view, xremote_infoscreen_input);
    //view_set_enter_callback(instance->view, xremote_infoscreen_enter);
    //view_set_exit_callback(instance->view, xremote_infoscreen_exit);
    with_view_model(instance->view, XRemoteInfoScreenModel * model, { UNUSED(model); }, true);

    return instance;
}
