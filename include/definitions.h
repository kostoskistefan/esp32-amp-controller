#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Arduino.h>
#include <MIDI.h>
#include <WiFi.h>

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define LED 2

#define MIDI_CHANNEL          1
#define NOTE_VELOCITY         127

#define AMP_TYPE_MIDI_VALUE   36
#define OVERDRIVE_MIDI_VALUE  39
#define DELAY_MIDI_VALUE      41
#define REVERB_MIDI_VALUE     43

#define SERIAL_BAUD_RATE      115200

#define WIFI_SSID             "WIFI_SSID"
#define WIFI_PASSWORD         "WIFI_PASSWORD"

#define API_KEY               "API_KEY"
#define DATABASE_URL          "DATABASE_URL"

#define API_ROOT              "/"
#define AMP_TYPE_KEY          "amp_type"
#define OVERDRIVE_KEY         "overdrive_active"
#define DELAY_KEY             "delay_active"
#define REVERB_KEY            "reverb_active"
#define SELECTED_PRESET_KEY   "selected_preset"

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

MIDI_CREATE_DEFAULT_INSTANCE();

#endif