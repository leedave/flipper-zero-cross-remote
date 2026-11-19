#include "../helpers/subghz/subghz.h"
#include "../xremote.h"

enum SubmenuIndex {
    SubmenuIndexCreate = 10,
    SubmenuIndexLoad,
    SubmenuIndexEdit,
    SubmenuIndexSettings,
    SubmenuIndexInfoScreen,
};

void xremote_scene_menu_on_enter(void* context) {
    XRemote* app = context;

    xremote_cross_remote_clear_items(app->cross_remote);
    xremote_ir_remote_clear_buttons(app->ir_remote_buffer);

#define ADD_SUBMENU(text, id) \
    submenu_add_item(app->submenu, text, id, xremote_generic_submenu_callback, app);

    ADD_SUBMENU("New Command Chain", SubmenuIndexCreate);
    ADD_SUBMENU("Run Saved Command", SubmenuIndexLoad);
    ADD_SUBMENU("Edit / Delete Command", SubmenuIndexEdit);
    ADD_SUBMENU("Settings", SubmenuIndexSettings);
    ADD_SUBMENU("App Info", SubmenuIndexInfoScreen);

#undef ADD_SUBMENU

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, XRemoteSceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, XRemoteViewIdMenu);
}

bool xremote_scene_menu_on_event(void* context, SceneManagerEvent event) {
    XRemote* app = context;
    if(event.type == SceneManagerEventTypeBack) {
        //exit app
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexCreate) {
            scene_manager_set_scene_state(
                app->scene_manager, XRemoteSceneMenu, SubmenuIndexCreate);
            scene_manager_next_scene(app->scene_manager, XRemoteSceneCreate);
            return true;
        } else if(event.event == SubmenuIndexLoad) {
            scene_manager_set_scene_state(app->scene_manager, XRemoteSceneMenu, SubmenuIndexLoad);
            scene_manager_next_scene(app->scene_manager, XRemoteSceneXrList);
            return true;
        } else if(event.event == SubmenuIndexSettings) {
            scene_manager_set_scene_state(
                app->scene_manager, XRemoteSceneMenu, SubmenuIndexSettings);
            scene_manager_next_scene(app->scene_manager, XRemoteSceneSettings);
            return true;
        } else if(event.event == SubmenuIndexInfoScreen) {
            scene_manager_set_scene_state(
                app->scene_manager, XRemoteSceneMenu, SubmenuIndexInfoScreen);
            scene_manager_next_scene(app->scene_manager, XRemoteSceneInfoScreen);
            return true;
        } else if(event.event == SubmenuIndexEdit) {
            scene_manager_set_scene_state(app->scene_manager, XRemoteSceneMenu, SubmenuIndexEdit);
            scene_manager_next_scene(app->scene_manager, XRemoteSceneXrListEdit);
        }
    }
    return false;
}

void xremote_scene_menu_on_exit(void* context) {
    XRemote* app = context;
    submenu_reset(app->submenu);
}