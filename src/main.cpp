#include <definitions.h>

void send_midi(uint8_t note)
{
    MIDI.sendNoteOn(note, NOTE_VELOCITY, MIDI_CHANNEL);
    delay(100);
    MIDI.sendNoteOff(note, NOTE_VELOCITY, MIDI_CHANNEL);
}

void stream_callback(FirebaseStream data)
{
    FirebaseJson *json = data.to<FirebaseJson *>();
    uint8_t len = json->iteratorBegin();

    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t value = json->valueAt(i).value.toInt();

        if (strcmp(json->valueAt(i).key.c_str(), AMP_TYPE_KEY) == 0)
            send_midi(AMP_TYPE_MIDI_VALUE + value);

        else if (strcmp(json->valueAt(i).key.c_str(), OVERDRIVE_KEY) == 0)
            send_midi(OVERDRIVE_MIDI_VALUE + value);

        else if (strcmp(json->valueAt(i).key.c_str(), DELAY_KEY) == 0)
            send_midi(DELAY_MIDI_VALUE + value);

        else if (strcmp(json->valueAt(i).key.c_str(), REVERB_KEY) == 0)
            send_midi(REVERB_MIDI_VALUE + value);
    }

    json->iteratorEnd();
    json->clear();
}

void initialize_pins()
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
}

void initialize_serial()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void initialize_wifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
        delay(300);
}

void initialize_firebase()
{
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);

    Firebase.signUp(&config, &auth, "", "");

    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);

    Firebase.RTDB.beginStream(&stream, API_ROOT);

    Firebase.RTDB.setStreamCallback(&stream, stream_callback, [](bool) {});
}

void set_default_amp_preset() {
    FirebaseJson json;

    json.add(AMP_TYPE_KEY, 0);
    json.add(OVERDRIVE_KEY, 0);
    json.add(DELAY_KEY, 0);
    json.add(REVERB_KEY, 0);
    json.add(SELECTED_PRESET_KEY, "None");

    Firebase.RTDB.setJSONAsync(&fbdo, API_ROOT, &json);
}

void setup()
{
    initialize_serial();
    initialize_wifi();
    initialize_firebase();
    set_default_amp_preset();
    initialize_pins();
}

void loop() {}