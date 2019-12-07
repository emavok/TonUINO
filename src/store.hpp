

    // RESET --- ALLE DREI KNÖPFE BEIM STARTEN GEDRÜCKT HALTEN -> alle bekannten
    // Karten werden gelöscht
    if (digitalRead(buttonPause) == LOW && digitalRead(buttonUp) == LOW &&
        digitalRead(buttonDown) == LOW)
    {
        Serial.println(F("Reset -> EEPROM wird gelöscht"));
        for (int i = 0; i < EEPROM.length(); i++)
        {
            EEPROM.write(i, 0);
        }
    }