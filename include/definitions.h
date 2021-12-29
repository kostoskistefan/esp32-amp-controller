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

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 2

#define NOTE_VELOCITY         0x7F
#define MIDI_CHANNEL          1

#define AMP_TYPE_BASE_VALUE   36
#define OVERDRIVE_BASE_VALUE  39
#define DELAY_BASE_VALUE      41
#define REVERB_BASE_VALUE     43

#define SERIAL_BAUD_RATE      115200

#define WIFI_SSID             "WIFI_SSID"
#define WIFI_PASSWORD         "WIFI_PASSWORD"

#define API_KEY               "API_KEY"
#define DATABASE_URL          "DATABASE_URL"

#define API_ROOT              "/"
#define AMP_TYPE              "amp_type"
#define OVERDRIVE_ACTIVE      "overdrive_active"
#define DELAY_ACTIVE          "delay_active"
#define REVERB_ACTIVE         "reverb_active"

FirebaseData stream;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#endif