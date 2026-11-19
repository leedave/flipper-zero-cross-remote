#include "../xremote.h"
#include "../views/xremote_pause_set.h"

void xremote_scene_pause_set_on_enter(void* context) {
    furi_assert(context);
    XRemote* app = context;
    xremote_view_set_callback(app->xremote_pause_set, xremote_generic_scene_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, XRemoteViewIdPauseSet);
}

bool xremote_scene_pause_set_on_event(void* context, SceneManagerEvent event) {
    XRemote* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case XRemoteCustomEventInfoScreenOk:
            scene_manager_next_scene(app->scene_manager, XRemoteSceneMenu);
            consumed = true;
            break;
        case XRemoteCustomEventPauseSetBack:
            if(app->pause_edit_mode) {
                // Editing an existing Pause item's timing: return to the Edit/Delete menu.
                scene_manager_previous_scene(app->scene_manager);
            } else if(!scene_manager_search_and_switch_to_previous_scene(
                          app->scene_manager, XRemoteSceneCreateAdd)) {
                scene_manager_stop(app->scene_manager);
                view_dispatcher_stop(app->view_dispatcher);
            }
            consumed = true;
            break;
        case XRemoteCustomEventPauseSetOk:
            if(app->pause_edit_mode) {
                // Editing an existing Pause item's timing: return to the Edit/Delete menu.
                scene_manager_previous_scene(app->scene_manager);
            } else {
                scene_manager_search_and_switch_to_previous_scene(
                    app->scene_manager, XRemoteSceneCreate);
            }
            consumed = true;
            break;
        }
    default:
        break;
    }
    return consumed;
}

void xremote_scene_pause_set_on_exit(void* context) {
    XRemote* app = context;
    app->pause_edit_mode = false;
}
