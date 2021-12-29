#include <definitions.h>

void send_midi(uint8_t note)
{
    MIDI.sendNoteOn(note, NOTE_VELOCITY, MIDI_CHANNEL);
    delay(100);
    MIDI.sendNoteOff(note, NOTE_VELOCITY, MIDI_CHANNEL);
}

void stream_callback(FirebaseStream data)
{
    if (strcmp(data.dataPath().c_str(), AMP_TYPE) == 0)
        send_midi(AMP_TYPE_BASE_VALUE + data.to<int>());

    else if (strcmp(data.dataPath().c_str(), OVERDRIVE_ACTIVE) == 0)
        send_midi(OVERDRIVE_BASE_VALUE + data.to<int>());

    else if (strcmp(data.dataPath().c_str(), DELAY_ACTIVE) == 0)
        send_midi(DELAY_BASE_VALUE + data.to<int>());

    else if (strcmp(data.dataPath().c_str(), REVERB_ACTIVE) == 0)
        send_midi(REVERB_BASE_VALUE + data.to<int>());
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

void setup()
{
    initialize_serial();
    initialize_wifi();
    initialize_firebase();
}

void loop() {}