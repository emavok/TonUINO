// ================================================================================================
// Tonuino Button Controller class
// ================================================================================================
#pragma once
#include <Arduino.h>
#include "smartbutton.hpp"

// ------------------------------------------------------------------------------------------------
// Smart button - auto init and simple short-press / long-press interface
// ------------------------------------------------------------------------------------------------
class TonuinoButtonController {
public:
    TonuinoButtonController(
        uint8_t pinBtnPlayPause,
        uint8_t pinBtnTrackPrev,
        uint8_t pinBtnTrackNext,
        uint8_t pinBtnVolumeDec,
        uint8_t pinBtnVolumeInc
    ) :
        _playPause(pinBtnPlayPause),
        _trackPrev(pinBtnTrackPrev),
        _trackNext(pinBtnTrackNext),
        _volumeDec(pinBtnVolumeDec),
        _volumeInc(pinBtnVolumeInc)
    {}

    // ------------------------------------------------------------------------------------------------
    // Setup button pins
    // ------------------------------------------------------------------------------------------------
    void setup() {
        _playPause.setup();
        _trackPrev.setup();
        _trackNext.setup();
        _volumeDec.setup();
        _volumeInc.setup();
    }

    // ------------------------------------------------------------------------------------------------
    // Update button states
    // ------------------------------------------------------------------------------------------------
    void loop() {
        _playPause.loop();
        _trackPrev.loop();
        _trackNext.loop();
        _volumeDec.loop();
        _volumeInc.loop();
    }

    // ------------------------------------------------------------------------------------------------
    // Get access to buttons to query pressed states
    // ------------------------------------------------------------------------------------------------
    SmartButton& playPauseBtn() { return _playPause; }
    SmartButton& prevTrackBtn() { return _trackPrev; }
    SmartButton& nextTrackBtn() { return _trackNext; }
    SmartButton& decVolumeBtn() { return _volumeDec; }
    SmartButton& incVolumeBtn() { return _volumeInc; }

private:

    SmartButton _playPause;
    SmartButton _trackPrev;
    SmartButton _trackNext;
    SmartButton _volumeDec;
    SmartButton _volumeInc;
};
