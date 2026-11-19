#include "xremote.h"

#ifdef DEBUG_LOG
void xremote_debug_heap() {
    FURI_LOG_D(TAG, "Heap Free: %zu bytes", memmgr_get_free_heap());
    FURI_LOG_D(TAG, "Heap Total: %zu bytes", memmgr_get_total_heap());
    FURI_LOG_D(TAG, "Heap Minimum: %zu bytes", memmgr_get_minimum_free_heap());
}
#endif

bool xremote_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    XRemote* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

void xremote_tick_event_callback(void* context) {
    furi_assert(context);
    XRemote* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

//leave app if back button pressed
bool xremote_navigation_event_callback(void* context) {
    furi_assert(context);
    XRemote* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

void xremote_app_alloc(XRemote* app) {
    furi_assert(app);
    app->gui = furi_record_open(RECORD_GUI);
    app->notification = furi_record_open(RECORD_NOTIFICATION);

    //Turn backlight on, believe me this makes testing your app easier
    notification_message(app->notification, &sequence_display_backlight_on);

    //Scene additions
    app->view_dispatcher = view_dispatcher_alloc();

    app->scene_manager = scene_manager_alloc(&xremote_scene_handlers, app);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, xremote_navigation_event_callback);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, xremote_tick_event_callback, 100);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, xremote_custom_event_callback);

    // Set defaults, in case no config loaded
    app->haptic = 1;
    app->speaker = 1;
    app->led = 1;
    app->save_settings = 1;
    app->transmitting = 0;
    app->ir_timing = 1000;
    app->ir_timing_char = "1000";
    app->sg_timing = 500;
    app->sg_timing_char = "500";
    app->stop_transmit = false;
    app->loop_transmit = 0;
    app->transmit_item = 0;
    app->loadFavorite = false;
    app->pause_edit_mode = false;

    // Load configs
    xremote_read_settings(app);

    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->file_path = furi_string_alloc();

    app->sg_remote_buffer = xremote_sg_remote_alloc();
    app->ir_remote_buffer = xremote_ir_remote_alloc();

    app->ir_worker = infrared_worker_alloc();
    app->cross_remote = xremote_cross_remote_alloc();

    app->loading = loading_alloc();

    app->subghz = subghz_alloc();

#define ADD_TO_DISPATCHER(id, obj) view_dispatcher_add_view(app->view_dispatcher, id, obj)

    app->submenu = submenu_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdMenu, submenu_get_view(app->submenu));

    app->editmenu = submenu_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdEditItem, submenu_get_view(app->editmenu));

    app->button_menu_create = button_menu_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdCreate, button_menu_get_view(app->button_menu_create));

    app->button_menu_create_add = button_menu_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdCreateAdd, button_menu_get_view(app->button_menu_create_add));

    app->variable_item_list = variable_item_list_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdSettings, variable_item_list_get_view(app->variable_item_list));

#ifdef WIP_FORM
    app->popup = popup_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdWip, popup_get_view(app->popup));
#endif

    app->view_stack = view_stack_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdStack, view_stack_get_view(app->view_stack));

    app->text_input = text_input_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdTextInput, text_input_get_view(app->text_input));

    app->number_input = number_input_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdNumberInput, number_input_get_view(app->number_input));

    app->xremote_transmit = xremote_transmit_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdTransmit, xremote_view_get(app->xremote_transmit));

    app->xremote_pause_set = xremote_pause_set_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdPauseSet, xremote_view_get(app->xremote_pause_set));

    app->button_menu_ir = button_menu_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdIrRemote, button_menu_get_view(app->button_menu_ir));

    app->xremote_infoscreen = xremote_infoscreen_alloc();
    ADD_TO_DISPATCHER(XRemoteViewIdInfoScreen, xremote_view_get(app->xremote_infoscreen));

#undef ADD_TO_DISPATCHER

    //End Scene Additions
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
}

void xremote_app_free(XRemote* app) {
    furi_assert(app);

#define DEL_FROM_DISPATCHER(id) view_dispatcher_remove_view(app->view_dispatcher, id);

    DEL_FROM_DISPATCHER(XRemoteViewIdMenu);
    DEL_FROM_DISPATCHER(XRemoteViewIdEditItem);
    DEL_FROM_DISPATCHER(XRemoteViewIdCreate);
    DEL_FROM_DISPATCHER(XRemoteViewIdCreateAdd);
    DEL_FROM_DISPATCHER(XRemoteViewIdSettings);
#ifdef WIP_FORM
    DEL_FROM_DISPATCHER(XRemoteViewIdWip);
#endif
    DEL_FROM_DISPATCHER(XRemoteViewIdStack);
    DEL_FROM_DISPATCHER(XRemoteViewIdTextInput);
    DEL_FROM_DISPATCHER(XRemoteViewIdNumberInput);
    DEL_FROM_DISPATCHER(XRemoteViewIdTransmit);
    DEL_FROM_DISPATCHER(XRemoteViewIdPauseSet);
    DEL_FROM_DISPATCHER(XRemoteViewIdIrRemote);
    DEL_FROM_DISPATCHER(XRemoteViewIdInfoScreen);
#undef DEL_FROM_DISPATCHER

    // free resources reverse order
    view_stack_free(app->view_stack);
#ifdef WIP_FORM
    popup_free(app->popup);
#endif
    variable_item_list_free(app->variable_item_list);
    button_menu_free(app->button_menu_ir);
    button_menu_free(app->button_menu_create_add);
    button_menu_free(app->button_menu_create);
    xremote_view_free(app->xremote_pause_set);
    xremote_view_free(app->xremote_transmit);
    xremote_view_free(app->xremote_infoscreen);
    submenu_free(app->editmenu);
    submenu_free(app->submenu);
    text_input_free(app->text_input);
    number_input_free(app->number_input);
    subghz_free(app->subghz);
    loading_free(app->loading);
    xremote_cross_remote_free(app->cross_remote);
    infrared_worker_free(app->ir_worker);
    xremote_ir_remote_free(app->ir_remote_buffer);
    xremote_sg_remote_free(app->sg_remote_buffer);
    furi_string_free(app->file_path);
    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
}

void xremote_popup_closed_callback(void* context) {
    furi_assert(context);
    XRemote* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, XRemoteCustomEventTypePopupClosed);
}

void xremote_text_input_callback(void* context) {
    furi_assert(context);
    XRemote* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, XRemoteCustomEventTextInput);
}

void xremote_generic_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    XRemote* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void xremote_generic_scene_callback(XRemoteCustomEvent event, void* context) {
    furi_assert(context);
    XRemote* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void xremote_ir_enable_otg(XRemote* app, bool enable) {
    if(enable) {
        furi_hal_power_enable_otg();
    } else {
        furi_hal_power_disable_otg();
    }
    app->ir_is_otg_enabled = enable;
}

void xremote_ir_set_tx_pin(XRemote* app) {
    if(app->ir_tx_pin < FuriHalInfraredTxPinMax) {
        furi_hal_infrared_set_tx_output(app->ir_tx_pin);
    } else {
        FuriHalInfraredTxPin tx_pin_detected = furi_hal_infrared_detect_tx_output();
        furi_hal_infrared_set_tx_output(tx_pin_detected);
        if(tx_pin_detected != FuriHalInfraredTxPinInternal) {
            xremote_ir_enable_otg(app, true);
        }
    }
}

static void xremote_ir_load_settings(XRemote* app) {
    xremote_ir_set_tx_pin(app);
    if(app->ir_tx_pin < FuriHalInfraredTxPinMax) {
        xremote_ir_enable_otg(app, app->ir_is_otg_enabled);
    }
}

int32_t xremote_app(void* p) {
    XRemote app = {0};
    xremote_app_alloc(&app);

    furi_hal_power_suppress_charge_enter();
    xremote_ir_load_settings(&app);

    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_common_mkdir(storage, XREMOTE_APP_FOLDER);
    furi_record_close(RECORD_STORAGE);

    //bool loadFavorite = false;
    if(p && strlen(p)) {
        furi_string_set_str(app.file_path, p);
        app.loadFavorite = xremote_cross_remote_load(app.cross_remote, app.file_path);
    }
    if(app.loadFavorite) {
        scene_manager_next_scene(
            app.scene_manager, XRemoteSceneTransmit); //if you loaded from Favorites
    } else {
        scene_manager_next_scene(
            app.scene_manager, XRemoteSceneMenu); //if you want to directly start with Menu
    }

#ifdef DEBUG_LOG
    xremote_debug_heap();
#endif

    view_dispatcher_run(app.view_dispatcher);

    xremote_save_settings(&app);
    furi_hal_power_suppress_charge_exit();
    xremote_ir_enable_otg(&app, false);
    xremote_app_free(&app);

    return 0;
}
