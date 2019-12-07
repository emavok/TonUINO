// ================================================================================================
// Smart Button class
// ================================================================================================
#include <Arduino.h>
#include <JC_Button.h>

// ------------------------------------------------------------------------------------------------
// Smart button - auto init and simple short-press / long-press interface
// ------------------------------------------------------------------------------------------------
class SmartButton {
public:
    // ------------------------------------------------------------------------------------------------
    // init with pin and optional long press duration (defaults to 1000 ms)
    // ------------------------------------------------------------------------------------------------
    SmartButton( uint8_t pin, uint16_t longPressDuration = 1000) :
        _button(pin),
        _duration(longPressDuration)
    {}

    // ------------------------------------------------------------------------------------------------
    // Call to setup button pin mode
    // ------------------------------------------------------------------------------------------------
    void setup() {
        _button.begin();
    }

    // ------------------------------------------------------------------------------------------------
    // Call to update button state
    // ------------------------------------------------------------------------------------------------
    void loop() {
        _button.read();
    }

    // ------------------------------------------------------------------------------------------------
    // Returns true if button was pressed short
    // ------------------------------------------------------------------------------------------------
    bool wasPressed() {
        // if the button was released
        if (_button.wasReleased()) {
            // check if we need to ignore this due to a long press release
            if (!_ignore) {
                // no - it was a real short press
                return true;
            } else {
                // yes - reset ignore flag
                _ignore = false;
            }
        }
        // no - it was not pressed at all
        return false;
    }

    // ------------------------------------------------------------------------------------------------
    // Returns true if button was pressed long
    // ------------------------------------------------------------------------------------------------
    bool wasLongPressed() {
        // was button pressed for a long enough time?
        if (_button.pressedFor(_duration)) {
            // flag to ignore next button release to prevent triggering a
            // short press, too
            _ignore = true;
            // yes - it was a long press
            return true;
        }
        // no - it was no long press
        return false;
    }

private:
    // JC button
    Button      _button;
    // ignore release due to long press
    bool        _ignore;
    // duration of long press
    uint32_t    _duration;
};
