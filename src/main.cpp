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

    for (size_t i = 0; i < len; i++)
    {
        if (strcmp(json->valueAt(i).key.c_str(), AMP_TYPE) == 0)
            send_midi(AMP_TYPE_BASE_VALUE + json->valueAt(i).value.toInt());

        else if (strcmp(json->valueAt(i).key.c_str(), OVERDRIVE_ACTIVE) == 0)
            send_midi(OVERDRIVE_BASE_VALUE + json->valueAt(i).value.toInt());

        else if (strcmp(json->valueAt(i).key.c_str(), DELAY_ACTIVE) == 0)
            send_midi(DELAY_BASE_VALUE + json->valueAt(i).value.toInt());

        else if (strcmp(json->valueAt(i).key.c_str(), REVERB_ACTIVE) == 0)
            send_midi(REVERB_BASE_VALUE + json->valueAt(i).value.toInt());
    }

    json->iteratorEnd();
    json->clear();
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
    pinMode(LED, OUTPUT);
    initialize_serial();
    initialize_wifi();
    initialize_firebase();
    digitalWrite(LED, HIGH);
}

void loop() {}