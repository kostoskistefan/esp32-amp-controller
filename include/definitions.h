#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>
#include <MIDI.h>

#include <TFT_eSPI.h>

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define MIDI_CHANNEL 1
#define NOTE_VELOCITY 127

#define AMP_TYPE_MIDI_VALUE 36
#define OVERDRIVE_MIDI_VALUE 39
#define DELAY_MIDI_VALUE 41
#define REVERB_MIDI_VALUE 43

#define SERIAL_BAUD_RATE 115200

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define API_KEY "API_KEY"
#define DATABASE_URL "DATABASE_URL"

#define API_ROOT "/"
#define AMP_TYPE_KEY "amp_type"
#define OVERDRIVE_KEY "overdrive_active"
#define DELAY_KEY "delay_active"
#define REVERB_KEY "reverb_active"
#define SELECTED_PRESET_KEY "selected_preset"

#define PRESETS_PAGE_TITLE "Presets"
#define SETTINGS_PAGE_TITLE "Settings"

#define PRESET_COUNT 6

#define DEFAULT_PRESET_NAME "None"

String presetNames[PRESET_COUNT] ={
        "Large Ambient",
        "Rain Drops",
        "Jimmy HendRIFFs",
        "Johnny B Lead",
        "Architects Rhythm",
        "Memphis May Lead"
};

TFT_eSPI tft = TFT_eSPI();

TFT_eSPI_Button presetButtons[6];
TFT_eSPI_Button ampButtons[3];

TFT_eSPI_Button
    nextPageButton,
    overdriveButton,
    delayButton,
    reverbButton;

typedef struct
{
    uint8_t amp_type;
    uint8_t overdrive_active;
    uint8_t delay_active;
    uint8_t reverb_active;
    char *selected_preset;
} Preset;

Preset presetDefinitions[PRESET_COUNT] = {
    {0, 0, 0, 1, (char *)presetNames[0].c_str()},
    {0, 0, 1, 1, (char *)presetNames[1].c_str()},
    {1, 0, 0, 0, (char *)presetNames[2].c_str()},
    {1, 1, 0, 1, (char *)presetNames[3].c_str()},
    {2, 1, 0, 0, (char *)presetNames[4].c_str()},
    {2, 1, 1, 1, (char *)presetNames[5].c_str()}};

uint8_t selectedPresetNumber[PRESET_COUNT] = {false};

uint8_t selectedAmpNumber[3] = {false};
uint8_t overdriveActive = 0;
uint8_t delayActive = 0;
uint8_t reverbActive = 0;

uint8_t pageDrawn = false;
uint8_t currentPage = 0;

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

MIDI_CREATE_DEFAULT_INSTANCE();

#endif
