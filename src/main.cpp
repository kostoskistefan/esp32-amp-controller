#include <definitions.h>

void clear_amp_type()
{
    for (uint8_t i = 0; i < 3; i++)
        selectedAmpNumber[i] = false;
}

void clear_preset()
{
    for (uint8_t i = 0; i < PRESET_COUNT; i++)
        selectedPresetNumber[i] = false;
}

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
        if (strcmp(json->valueAt(i).key.c_str(), AMP_TYPE_KEY) == 0)
        {
            uint8_t value = json->valueAt(i).value.toInt();
            send_midi(AMP_TYPE_MIDI_VALUE + value);
            clear_amp_type();
            selectedAmpNumber[value] = 1;
        }

        else if (strcmp(json->valueAt(i).key.c_str(), OVERDRIVE_KEY) == 0)
        {
            uint8_t value = json->valueAt(i).value.toInt();
            send_midi(OVERDRIVE_MIDI_VALUE + value);
            overdriveActive = value;
        }

        else if (strcmp(json->valueAt(i).key.c_str(), DELAY_KEY) == 0)
        {
            uint8_t value = json->valueAt(i).value.toInt();
            send_midi(DELAY_MIDI_VALUE + value);
            delayActive = value;
        }

        else if (strcmp(json->valueAt(i).key.c_str(), REVERB_KEY) == 0)
        {
            uint8_t value = json->valueAt(i).value.toInt();
            send_midi(REVERB_MIDI_VALUE + value);
            reverbActive = value;
        }

        else if (strcmp(json->valueAt(i).key.c_str(), SELECTED_PRESET_KEY) == 0)
        {
            clear_preset();
            
            for (uint8_t j = 0; j < PRESET_COUNT; j++)
            {
                String presetName = '"' + presetNames[j] + '"';

                if (json->valueAt(i).value == presetName)
                    selectedPresetNumber[j] = true;
            }
        }
    }

    json->iteratorEnd();
    json->clear();

    pageDrawn = false;
}

void initialize_pins()
{
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
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

void initialize_tft()
{
    tft.init();
    uint16_t calData[5] = {225, 3442, 302, 3497, 1};
    tft.setTouch(calData);
    tft.setRotation(1);
    tft.fillScreen(TFT_LIGHTGREY);
    tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
}

void initialize_single_button(TFT_eSPI_Button &button, uint16_t x, uint8_t y, uint16_t w, uint8_t h, char *text)
{
    button.initButton(&tft, x, y, w, h, TFT_BLACK, TFT_LIGHTGREY, TFT_DARKCYAN, text, 2);
    button.setLabelDatum(0, 5, MC_DATUM);
}

void initialize_buttons()
{
    initialize_single_button(nextPageButton, 285, 11, 68, 20, (char *)">");

    initialize_single_button(presetButtons[0], 80, 62, 154, 64, presetDefinitions[0].selected_preset);
    initialize_single_button(presetButtons[1], 80, 131, 154, 64, presetDefinitions[1].selected_preset);
    initialize_single_button(presetButtons[2], 80, 202, 154, 64, presetDefinitions[2].selected_preset);
    initialize_single_button(presetButtons[3], 240, 62, 154, 64, presetDefinitions[3].selected_preset);
    initialize_single_button(presetButtons[4], 240, 131, 154, 64, presetDefinitions[4].selected_preset);
    initialize_single_button(presetButtons[5], 240, 202, 154, 64, presetDefinitions[5].selected_preset);

    initialize_single_button(ampButtons[0], 80, 62, 154, 64, (char *)"Clean");
    initialize_single_button(ampButtons[1], 80, 131, 154, 64, (char *)"Crunch");
    initialize_single_button(ampButtons[2], 80, 202, 154, 64, (char *)"Lead");

    initialize_single_button(overdriveButton, 240, 62, 154, 64, (char *)"Overdrive");
    initialize_single_button(delayButton, 240, 131, 154, 64, (char *)"Delay");
    initialize_single_button(reverbButton, 240, 202, 154, 64, (char *)"Reverb");
}

void set_button_press(TFT_eSPI_Button &button, uint8_t &touched, uint16_t &t_x, uint16_t &t_y)
{
    button.press(touched && button.contains(t_x, t_y));
}

void send_amp_preset(Preset preset)
{
    FirebaseJson json;

    json.add(AMP_TYPE_KEY, preset.amp_type);
    json.add(OVERDRIVE_KEY, preset.overdrive_active);
    json.add(DELAY_KEY, preset.delay_active);
    json.add(REVERB_KEY, preset.reverb_active);
    json.add(SELECTED_PRESET_KEY, preset.selected_preset);

    Firebase.RTDB.setJSONAsync(&fbdo, API_ROOT, &json);
}

void set_amp_preset(uint8_t preset_number)
{
    clear_preset();
    selectedPresetNumber[preset_number] = true;

    send_amp_preset(presetDefinitions[preset_number]);

    pageDrawn = false;
}

void navbar_touch_handler(uint16_t &t_x, uint16_t &t_y, uint8_t &touched)
{
    set_button_press(nextPageButton, touched, t_x, t_y);

    if (nextPageButton.justPressed())
    {
        tft.fillScreen(TFT_LIGHTGREY);

        currentPage = 1 - currentPage;

        pageDrawn = false;
    }
}

void draw_title_bar(char *title)
{
    nextPageButton.drawButton(false);

    tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);

    tft.setCursor(160 - tft.textWidth(title) / 2, 4);
    tft.print(title);
    tft.drawFastHLine(0, 22, TFT_HEIGHT, TFT_BLACK);
    tft.drawFastHLine(0, 23, TFT_HEIGHT, TFT_BLACK);
}

void draw_amp_preset_page(uint8_t &touched, uint16_t &t_x, uint16_t &t_y)
{
    for (uint8_t i = 0; i < PRESET_COUNT; i++)
    {
        set_button_press(presetButtons[i], touched, t_x, t_y);

        if (presetButtons[i].justPressed())
        {
            set_amp_preset(i);
            presetButtons[i].drawButton(selectedPresetNumber[i]);
        }
    }

    if (!pageDrawn)
    {
        draw_title_bar((char *)PRESETS_PAGE_TITLE);

        for (uint8_t i = 0; i < PRESET_COUNT; i++)
            presetButtons[i].drawButton(selectedPresetNumber[i]);

        pageDrawn = true;
    }
}

void update_amp_setting(uint8_t selected_amp)
{
    if (ampButtons[selected_amp].justPressed())
    {
        send_midi(AMP_TYPE_MIDI_VALUE + selected_amp);

        clear_amp_type();

        selectedAmpNumber[selected_amp] = true;

        Firebase.RTDB.setIntAsync(&fbdo, (char *)AMP_TYPE_KEY, selected_amp);

        ampButtons[selected_amp].drawButton(selectedAmpNumber[selected_amp]);

        pageDrawn = false;
    }
}

void update_pedal_setting(TFT_eSPI_Button &button, char *key, uint8_t &value)
{
    if (button.justPressed())
    {
        value = 1 - value;

        if(strcmp(key, OVERDRIVE_KEY) == 0)
            send_midi(OVERDRIVE_MIDI_VALUE + value);

        else if(strcmp(key, DELAY_KEY) == 0)
            send_midi(DELAY_MIDI_VALUE + value);

        else if(strcmp(key, REVERB_KEY) == 0)
            send_midi(REVERB_MIDI_VALUE + value);

        Firebase.RTDB.setIntAsync(&fbdo, key, value);

        button.drawButton(value);

        pageDrawn = false;
    }
}

void draw_amp_settings_page(uint8_t &touched, uint16_t &t_x, uint16_t &t_y)
{
    set_button_press(ampButtons[0], touched, t_x, t_y);
    set_button_press(ampButtons[1], touched, t_x, t_y);
    set_button_press(ampButtons[2], touched, t_x, t_y);

    set_button_press(overdriveButton, touched, t_x, t_y);
    set_button_press(delayButton, touched, t_x, t_y);
    set_button_press(reverbButton, touched, t_x, t_y);

    for (uint8_t i = 0; i < 3; i++)
        update_amp_setting(i);

    update_pedal_setting(overdriveButton, (char *)OVERDRIVE_KEY, overdriveActive);
    update_pedal_setting(delayButton, (char *)DELAY_KEY, delayActive);
    update_pedal_setting(reverbButton, (char *)REVERB_KEY, reverbActive);

    if (!pageDrawn)
    {
        draw_title_bar((char *)SETTINGS_PAGE_TITLE);

        for (uint8_t i = 0; i < 3; i++)
            ampButtons[i].drawButton(selectedAmpNumber[i]);

        overdriveButton.drawButton(overdriveActive);
        delayButton.drawButton(delayActive);
        reverbButton.drawButton(reverbActive);

        pageDrawn = true;
    }
}

void draw_initialization_page()
{
    tft.setCursor((TFT_HEIGHT / 2) - tft.textWidth("Initializing") / 2, (TFT_WIDTH / 2) - tft.textsize - 10);
    tft.print("Initializing");
    tft.setCursor((TFT_HEIGHT / 2) - tft.textWidth("VAC") / 2, (TFT_WIDTH / 2) + tft.textsize + 10);
    tft.print("VAC");
}

void draw_pages()
{
    uint16_t t_x = 0, t_y = 0;
    uint8_t touched = tft.getTouch(&t_x, &t_y);

    navbar_touch_handler(t_x, t_y, touched);

    switch (currentPage)
    {
    case 0:
        draw_amp_preset_page(touched, t_x, t_y);
        break;
    case 1:
        draw_amp_settings_page(touched, t_x, t_y);
        break;
    default:
        draw_amp_preset_page(touched, t_x, t_y);
        break;
    }

    delay(250);
}

void setup()
{
    initialize_serial();

    initialize_tft();
    initialize_buttons();
    draw_initialization_page();

    initialize_wifi();
    initialize_firebase();

    send_amp_preset({0, 0, 0, 0, (char *)DEFAULT_PRESET_NAME});

    initialize_pins();

    tft.fillScreen(TFT_LIGHTGREY);
}

void loop()
{
    draw_pages();
}