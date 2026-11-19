#include "../xremote.h"
#include "../helpers/xremote_custom_event.h"
#include "../views/xremote_infoscreen.h"

void xremote_scene_infoscreen_on_enter(void* context) {
    furi_assert(context);
    XRemote* app = context;
    xremote_view_set_callback(app->xremote_infoscreen, xremote_generic_scene_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, XRemoteViewIdInfoScreen);
}

bool xremote_scene_infoscreen_on_event(void* context, SceneManagerEvent event) {
    XRemote* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case XRemoteCustomEventInfoScreenOk:
            scene_manager_next_scene(app->scene_manager, XRemoteSceneMenu);
            consumed = true;
            break;
        case XRemoteCustomEventInfoScreenBack:
            notification_message(app->notification, &sequence_reset_red);
            notification_message(app->notification, &sequence_reset_green);
            notification_message(app->notification, &sequence_reset_blue);
            if(!scene_manager_search_and_switch_to_previous_scene(
                   app->scene_manager, XRemoteSceneInfoScreen)) {
                scene_manager_stop(app->scene_manager);
                view_dispatcher_stop(app->view_dispatcher);
            }
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

void xremote_scene_infoscreen_on_exit(void* context) {
    UNUSED(context);
}