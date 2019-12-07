// ------------------------------------------------------------------------------------------------
// Mp3Notify class
// ------------------------------------------------------------------------------------------------
#include <Arduino.h>

/** Callback function type for handling track-finished events */
typedef void(*TrackFinishedCallback)(uint16_t);

void noopCallback(uint16_t) {};

// ------------------------------------------------------------------------------------------------
// Mp3Notify class
// ------------------------------------------------------------------------------------------------
class Mp3Notify {
private:
    /** Pointer to track-finished callback function */
    static TrackFinishedCallback pTrackFinishedCallback;

public:

    // setup notify static class
    static void setup(TrackFinishedCallback _pTrackFinishedCallback) {
        pTrackFinishedCallback = _pTrackFinishedCallback;
    }

    // --------------------------------------------------------------------------------------------
    // API implementation
    // --------------------------------------------------------------------------------------------
    static void OnError(uint16_t errorCode) {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("DFPlayer Com Error: ");
        Serial.println(errorCode);
    }

    static void OnPlayFinished(uint16_t track) {
        Serial.print("Track beendet: ");
        Serial.println(track);
        delay(100);
        noopCallback(track);
    }

    static void OnCardOnline(uint16_t code) {
        Serial.println(F("SD Karte online."));
    }

    static void OnCardInserted(uint16_t code) {
        Serial.println(F("SD Karte bereit."));
    }

    static void OnCardRemoved(uint16_t code) {
        Serial.println(F("SD Karte entfernt."));
    }

    static void OnUsbOnline(uint16_t code) {
        Serial.println(F("USB online."));
    }

    static void OnUsbInserted(uint16_t code) {
        Serial.println(F("USB bereit."));
    }

    static void OnUsbRemoved(uint16_t code) {
        Serial.println(F("USB entfernt."));
    }
};
