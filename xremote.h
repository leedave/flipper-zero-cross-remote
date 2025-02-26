#pragma once

#include "scenes/xremote_scene.h"
#include "views/xremote_infoscreen.h"
#include "views/xremote_transmit.h"
#include "views/xremote_pause_set.h"
#include "helpers/xremote_storage.h"
#include "models/infrared/xremote_ir_remote.h"
#include "models/cross/xremote_cross_remote.h"
#include "helpers/subghz/subghz_types.h"
#include "helpers/subghz/subghz.h"
#include "xremote_i.h"

typedef struct SubGhz SubGhz;

typedef struct {
    Gui* gui;
    DialogsApp* dialogs;
    FuriString* file_path;
    NotificationApp* notification;
    SubGhzNotificationState state_notifications;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    Submenu* editmenu;
    ButtonMenu* button_menu_create;
    ButtonMenu* button_menu_create_add;
    ButtonMenu* button_menu_ir;
    TextInput* text_input;
    Popup* popup;
    Loading* loading;
    ViewStack* view_stack;
    SceneManager* scene_manager;
    VariableItemList* variable_item_list;
    XRemoteInfoscreen* xremote_infoscreen;
    XRemoteTransmit* xremote_transmit;
    XRemotePauseSet* xremote_pause_set;
    InfraredRemote* ir_remote_buffer;
    InfraredWorker* ir_worker;
    bool ir_is_otg_enabled; /**< Whether OTG power (external 5V) is enabled for IR. */
    uint32_t ir_tx_pin; 
    SubGhzRemote* sg_remote_buffer;
    CrossRemote* cross_remote;
    uint32_t haptic;
    uint32_t speaker;
    uint32_t led;
    uint32_t save_settings;
    uint32_t loop_transmit;
    uint32_t edit_item;
    uint32_t ir_timing;
    char* ir_timing_char;
    uint32_t sg_timing;
    char* sg_timing_char;
    bool transmitting;
    bool stop_transmit;
    size_t transmit_item;
    char text_store[XREMOTE_TEXT_STORE_NUM][XREMOTE_TEXT_STORE_SIZE + 1];
    SubGhz* subghz;
    NumberInput* number_input;
    bool loadFavorite;
} XRemote;

typedef enum {
    XRemoteViewIdInfoscreen,
    XRemoteViewIdMenu,
    XRemoteViewIdEditItem,
    XRemoteViewIdCreate,
    XRemoteViewIdCreateAdd,
    XRemoteViewIdSettings,
    XRemoteViewIdWip,
    XRemoteViewIdIrRemote,
    XRemoteViewIdStack,
    XRemoteViewIdTextInput,
    XRemoteViewIdNumberInput,
    XRemoteViewIdTransmit,
    XRemoteViewIdPauseSet,
} XRemoteViewId;

typedef enum {
    XRemoteHapticOff,
    XRemoteHapticOn,
} XRemoteHapticState;

typedef enum {
    XRemoteSpeakerOff,
    XRemoteSpeakerOn,
} XRemoteSpeakerState;

typedef enum {
    XRemoteLedOff,
    XRemoteLedOn,
} XRemoteLedState;

typedef enum {
    XRemoteLoopOff,
    XRemoteLoopOn,
} XRemoteLoopState;

typedef enum {
    XRemoteSettingsOff,
    XRemoteSettingsOn,
} XRemoteSettingsStoreState;

void xremote_popup_closed_callback(void* context);
void xremote_text_input_callback(void* context);
void xremote_ir_enable_otg(XRemote* app, bool enable);
void xremote_ir_set_tx_pin(XRemote* app);