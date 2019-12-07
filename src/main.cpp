// ================================================================================================
// Main function
// ================================================================================================

#include <Arduino.h>
#include <DFMiniMp3.h>
// #include <EEPROM.h>
#include <JC_Button.h>
// #include <MFRC522.h>
// #include <SPI.h>
#include <SoftwareSerial.h>

#include "mp3notify.hpp"
#include "controller.hpp"
#include "player.hpp"

// ------------------------------------------------------------------------------------------------
// PIN definitions
// ------------------------------------------------------------------------------------------------

// Software Serial read pin
#define PIN_SS_RX 2
// Software Serial write pin
#define PIN_SS_TX 3

// Button Pause pin
#define PIN_BTN_PLAY_PAUSE A0
#define PIN_BTN_PREV_TRACK A1
#define PIN_BTN_NEXT_TRACK A2
#define PIN_BTN_VOLUME_DEC A3
#define PIN_BTN_VOLUME_INC A4

// mp3 player is busy pin
#define PIN_MP3_BUSY 4

// ------------------------------------------------------------------------------------------------
// Global variables
// ------------------------------------------------------------------------------------------------

// Software Serial for communication with DFMiniMp3 player
SoftwareSerial softwareSerial(PIN_SS_RX, PIN_SS_TX);

// DFPlayer Mini
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> MP3_Player;
MP3_Player mp3Player(softwareSerial);

// create tonuino player
TonuinoPlayer<MP3_Player> tonuinoPlayer(mp3Player, PIN_MP3_BUSY);

// create button controller
TonuinoButtonController controller(
    PIN_BTN_PLAY_PAUSE,
    PIN_BTN_PREV_TRACK,
    PIN_BTN_NEXT_TRACK,
    PIN_BTN_VOLUME_DEC,
    PIN_BTN_VOLUME_INC
);

// ------------------------------------------------------------------------------------------------
// Main setup function
// ------------------------------------------------------------------------------------------------
void setup()
{
    // initialize serial communication
    Serial.begin(115200);

    // setup random seed by reeding a random value from analog pin
    randomSeed(analogRead(A0));

    // startup message
    Serial.println(F("TonUINO Version 2.0"));
    Serial.println(F("(c) Thorsten Voß"));
    Serial.println(F("Adapted to PlatformIO by JVM"));

    // setup card reader

    // setup void menu

    // setup button controller
    controller.setup();

    // setup tonuino player
    tonuinoPlayer.setup();

    // setup eeprom
}

uint8_t volume;
uint16_t currentTrack;

// ------------------------------------------------------------------------------------------------
// Main loop function
// ------------------------------------------------------------------------------------------------
void loop()
{
    // do
    {
        // continue player
        tonuinoPlayer.loop();

        // update button state
        controller.loop();

        // evaluate button controller states
        if (controller.prevTrackBtn().wasLongPressed() &&
            controller.playPauseBtn().wasLongPressed() &&
            controller.nextTrackBtn().wasLongPressed() &&
            !tonuinoPlayer.isPlaying()) {
            Serial.println(F("Prev+Play+Next buttons pressed."));
            Serial.println(F("Reset card..."));
            tonuinoPlayer.startMp3Track(800);
            do {
                delay(10);
            } while (tonuinoPlayer.isPlaying());
        } else if (controller.playPauseBtn().wasLongPressed()) {
            Serial.println(F("Play button pressed long."));
            if (tonuinoPlayer.isPlaying()) {
                currentTrack = tonuinoPlayer.getTrack();
                Serial.print(F("Current track: "));
                Serial.println(currentTrack);
                tonuinoPlayer.startAdvert(currentTrack);
            }
        }

        if (controller.playPauseBtn().wasPressed()) {
            Serial.println(F("Play button pressed."));
            if (tonuinoPlayer.isPlaying()) {
                Serial.println(F("is playing"));
                tonuinoPlayer.pause();
            } else {
                Serial.println(F("is NOT playing"));
                tonuinoPlayer.resume();
            }

        }
        if (controller.prevTrackBtn().wasPressed()) {
            Serial.println(F("Track prev button pressed."));
            tonuinoPlayer.start(1,1);
        }
        if (controller.nextTrackBtn().wasPressed()) {
            Serial.println(F("Track next button pressed."));
            tonuinoPlayer.start(3,1);
        }
        if (controller.decVolumeBtn().wasPressed()) {
            Serial.println(F("Volume decrease button pressed."));
            volume = tonuinoPlayer.decVolume();
            Serial.print(F("New volume: "));
            Serial.println(volume);
            if (tonuinoPlayer.isPlaying()) {
                tonuinoPlayer.startAdvert(volume);
            } else {
                tonuinoPlayer.startMp3Track(volume);
                delay(1000);
            }
        }
        if (controller.incVolumeBtn().wasPressed()) {
            Serial.println(F("Volume increase button pressed."));
            volume = tonuinoPlayer.incVolume();
            Serial.print(F("New volume: "));
            Serial.println(volume);
            if (tonuinoPlayer.isPlaying()) {
                tonuinoPlayer.startAdvert(volume);
            } else {
                tonuinoPlayer.startMp3Track(volume);
                delay(1000);
            }
        }

        // delay(10);

        // Options
        // pause / resume playing / advertize track number (long press)

        // next / prev track

        // volumen inc/dec

        // reset card (prev + play + next)
    }
    // while (!mfrc522.PICC_IsNewCardPresent());
/*
    // RFID Karte wurde aufgelegt
    // try to read uid
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    if (readCard(&myCard) == true)
    {
        if (myCard.cookie == 322417479 && myCard.folder != 0 && myCard.mode != 0)
        {

            knownCard = true;


        // Neue Karte konfigurieren
        else
        {
            knownCard = false;
            setupCard();
        }
    }

    // PICC in state ACTIVE(*) to go to state HALT
    // TODO: check error code
    mfrc522.PICC_HaltA();
    // exit the PCD from its authenticated state
    mfrc522.PCD_StopCrypto1();
*/
}
