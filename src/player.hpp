// ================================================================================================
// Tonuino Player class
// ================================================================================================

#pragma once

#include <Arduino.h>

// ------------------------------------------------------------------------------------------------
// Playback modes
// ------------------------------------------------------------------------------------------------
enum EPlaybackMode
{
    // Hörspielmodus: eine zufällige Datei aus dem Ordner
    RANDOM_TRACK = 1,
    // Album Modus: kompletten Ordner spielen
    FOLDER_PROGRESSIVE,
    // Party Modus: Ordner in zufälliger Reihenfolge
    FOLDER_RANDOMIZED,
    // Einzel Modus: eine Datei aus dem Ordner abspielen
    SINGLE_TRACK,
    // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken
    FOLDER_PROGRESSIVE_STORED
};

// ------------------------------------------------------------------------------------------------
// Tonuino player class
// ------------------------------------------------------------------------------------------------
template<class MP3_PLAYER> class TonuinoPlayer {
public:
    // --------------------------------------------------------------------------------------------
    // Constructor
    // --------------------------------------------------------------------------------------------
    TonuinoPlayer(MP3_PLAYER& mp3Player, int busyPin, uint8_t volume = 15) :
        _mp3Player(mp3Player),
        _busyPin(busyPin),
        _volume(volume)
    {}

    // --------------------------------------------------------------------------------------------
    // Call in Arduino setup function
    // --------------------------------------------------------------------------------------------
    void setup() {
        Serial.println(F("TonuinoPlayer::setup"));

        // initialize busy pin for input
        pinMode(_busyPin, INPUT);

        // start mp3 player
        _mp3Player.begin();

        // set volume
        _mp3Player.setVolume(_volume);
    }

    // --------------------------------------------------------------------------------------------
    // Call in Arduino loop function
    // --------------------------------------------------------------------------------------------
    void loop() {
        _mp3Player.loop();
    }

    // --------------------------------------------------------------------------------------------
    // Play next track
    // --------------------------------------------------------------------------------------------
    void nextTrack() {
        Serial.println(F("TonuinoPlayer::nextTrack"));
        if (_track == 0 || _folder == 0) {
            Serial.println(F("ERROR: not running"));
            return;
        }

        if (_track >= _folderTrackCount) {
            Serial.println(F("ERROR: no next track"));
            return;
        }
        _track++;

        // start playing
        _mp3Player.playFolderTrack(_folder, _track);
        delay(100);
    }

    // --------------------------------------------------------------------------------------------
    // Play previous track
    // --------------------------------------------------------------------------------------------
    void prevTrack() {
        Serial.println(F("TonuinoPlayer::prevTrack"));
        if (_track == 0 || _folder == 0) {
            Serial.println(F("ERROR: not running"));
            return;
        }

        if (_track <= 1) {
            Serial.println(F("ERROR: no previous track"));
            return;
        }
        _track--;

        // start playing
        _mp3Player.playFolderTrack(_folder, _track);
        delay(100);
    }

    // --------------------------------------------------------------------------------------------
    // Start playing a specific track of a specific folder with a specific mode
    // --------------------------------------------------------------------------------------------
    void start(uint8_t folder, uint8_t track, int mode = 0) {
        Serial.print(F("TonuinoPlayer::start: "));
        Serial.print(folder);
        Serial.print(F("/"));
        Serial.println(track);

        // copy values
        _folder = folder;
        _track = track;
        _playbackMode = mode;

/*
        // get number of folders
        uint16_t folderCount = _mp3Player.getTotalFolderCount();
        Serial.print(F("Total folder count: "));
        Serial.print(folderCount);
        if (_folder > folderCount) {
            Serial.println(F("ERROR: folder index out of bounds"));
            return;
        }

        // get number of tracks in folder
        _folderTrackCount = _mp3Player.getFolderTrackCount(_folder);
        Serial.print(F("Total tracks in folder "));
        Serial.print(_folder);
        Serial.print(F(": "));
        Serial.print(_folderTrackCount);
        if (_track > _folderTrackCount) {
            Serial.println(F("ERROR: track index out of bounds"));
            return;
        }
*/
        // start playing
        _mp3Player.playFolderTrack(_folder, _track);
        delay(100);
    }

    // --------------------------------------------------------------------------------------------
    // Plays a single track from the mp3 folder
    // --------------------------------------------------------------------------------------------
    void startMp3Track(uint16_t track) {
        Serial.print(F("TonuinoPlayer::startMp3Track: "));
        Serial.println(track);
        _track = 0;
        _folder = 0;
        _folderTrackCount = 0;
        // start playing from mp3 folder
        _mp3Player.playMp3FolderTrack(track);
        delay(100);
    }

    // --------------------------------------------------------------------------------------------
    // Plays a single track from the advert folder
    // --------------------------------------------------------------------------------------------
    void startAdvert(uint16_t track) {
        Serial.print(F("TonuinoPlayer::startAdvert: "));
        Serial.println(track);
        // start playing from mp3 folder
        _mp3Player.playAdvertisement(track);
        delay(100);
    }

    // --------------------------------------------------------------------------------------------
    // Pauses playing
    // --------------------------------------------------------------------------------------------
    void pause() {
        Serial.println(F("TonuinoPlayer::pause"));
        if (!isPlaying()) {
            Serial.println(F("ERROR: not playing"));
        } else if (_folder > 0 && _track > 0) {
            _mp3Player.pause();
        } else {
            Serial.println(F("ERROR: playing an mp3 or advert track..."));
        }
    }

    // --------------------------------------------------------------------------------------------
    // Continues playing
    // --------------------------------------------------------------------------------------------
    void resume() {
        Serial.println(F("TonuinoPlayer::resume"));
        if (isPlaying()) {
            Serial.println(F("ERROR: already playing"));
        } else if (_folder > 0 && _track > 0) {
            _mp3Player.start();
        } else {
            Serial.println(F("ERROR: no track/folder..."));
        }
    }

    // --------------------------------------------------------------------------------------------
    // Returns true if a track is currently being player
    // --------------------------------------------------------------------------------------------
    bool isPlaying() {
        return !digitalRead(_busyPin);
    }

    // --------------------------------------------------------------------------------------------
    // Sets the current volume 0 .. x .. 20
    // --------------------------------------------------------------------------------------------
    void setVolume(uint8_t volume) {
        Serial.println(F("TonuinoPlayer::setVolume"));
        if  (volume < 0 || volume > _maxVolume) {
            Serial.println(F("ERROR: volume out of bounds"));
            return;
        }
        _volume = volume;
        _mp3Player.setVolume(volume);
    }

    // --------------------------------------------------------------------------------------------
    // Returns current volume
    // --------------------------------------------------------------------------------------------
    uint8_t getVolume() {
        return _volume;
    }

    // --------------------------------------------------------------------------------------------
    // Increses current volume
    // --------------------------------------------------------------------------------------------
    uint8_t incVolume() {
        Serial.println(F("TonuinoPlayer::incVolume"));
        if (_volume < _maxVolume) {
            _volume++;
        } else {
            Serial.println(F("ERROR: max volume reached"));
        }
        _mp3Player.setVolume(_volume);
        return _volume;
    }

    // --------------------------------------------------------------------------------------------
    // Decreases current volume
    // --------------------------------------------------------------------------------------------
    uint8_t decVolume() {
        Serial.println(F("TonuinoPlayer::decVolume"));
        if (_volume > 0) {
            _volume--;
        } else {
            Serial.println(F("ERROR: min volume reached"));
        }
        _mp3Player.setVolume(_volume);
        return _volume;
    }

    // --------------------------------------------------------------------------------------------
    // Returns current track number
    // --------------------------------------------------------------------------------------------
    uint16_t getTrack() {
        return _track;
    }

    // --------------------------------------------------------------------------------------------
    // Returns current folder number
    // --------------------------------------------------------------------------------------------
    uint16_t getFolder() {
        return _folder;
    }

private:

    // reference to the mp3 player class
    MP3_PLAYER& _mp3Player;

    // arduino pin to read from to check whether mp3 player is busy
    int _busyPin;

    // current playback mode
    uint8_t _playbackMode = 0;
    // current folder
    uint8_t _folder = 0;
    // current track
    uint8_t _track = 0;
    // track count in folder
    uint16_t _folderTrackCount = 0;
    // last track
    uint16_t _lastTrackFinished = 0;
    // current volume
    uint8_t _volume = 0;
    // maximum volume
    uint8_t _maxVolume = 20;
};

/*
void nextTrack(uint16_t track) {
  if (track == _lastTrackFinished) {
    return;
   }
   _lastTrackFinished = track;

   if (knownCard == false)
    // Wenn eine neue Karte angelernt wird soll das Ende eines Tracks nicht
    // verarbeitet werden
    return;

  if (myCard.mode == 1) {
    Serial.println(F("Hörspielmodus ist aktiv -> keinen neuen Track spielen"));
//    mp3.sleep(); // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
  }
  if (myCard.mode == 2) {
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      mp3.playFolderTrack(myCard.folder, currentTrack);
      Serial.print(F("Albummodus ist aktiv -> nächster Track: "));
      Serial.print(currentTrack);
    } else
//      mp3.sleep();   // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
    { }
  }
  if (myCard.mode == 3) {
    uint16_t oldTrack = currentTrack;
    currentTrack = random(1, numTracksInFolder + 1);
    if (currentTrack == oldTrack)
      currentTrack = currentTrack == numTracksInFolder ? 1 : currentTrack+1;
    Serial.print(F("Party Modus ist aktiv -> zufälligen Track spielen: "));
    Serial.println(currentTrack);
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 4) {
    Serial.println(F("Einzel Modus aktiv -> Strom sparen"));
//    mp3.sleep();      // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
  }
  if (myCard.mode == 5) {
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      Serial.print(F("Hörbuch Modus ist aktiv -> nächster Track und "
                     "Fortschritt speichern"));
      Serial.println(currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
      // Fortschritt im EEPROM abspeichern
      EEPROM.write(myCard.folder, currentTrack);
    } else {
//      mp3.sleep();  // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
      // Fortschritt zurück setzen
      EEPROM.write(myCard.folder, 1);
    }
  }
}

static void previousTrack() {
  if (myCard.mode == 1) {
    Serial.println(F("Hörspielmodus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 2) {
    Serial.println(F("Albummodus ist aktiv -> vorheriger Track"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 3) {
    Serial.println(F("Party Modus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 4) {
    Serial.println(F("Einzel Modus aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 5) {
    Serial.println(F("Hörbuch Modus ist aktiv -> vorheriger Track und "
                     "Fortschritt speichern"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
    // Fortschritt im EEPROM abspeichern
    EEPROM.write(myCard.folder, currentTrack);
  }
}


void start(myCard) {
                _lastTrackFinished = 0;
            numTracksInFolder = mp3.getFolderTrackCount(myCard.folder);
            Serial.print(numTracksInFolder);
            Serial.print(F(" Dateien in Ordner "));
            Serial.println(myCard.folder);

            // Hörspielmodus: eine zufällige Datei aus dem Ordner
            if (myCard.mode == 1)
            {
                Serial.println(F("Hörspielmodus -> zufälligen Track wiedergeben"));
                currentTrack = random(1, numTracksInFolder + 1);
                Serial.println(currentTrack);
                mp3.playFolderTrack(myCard.folder, currentTrack);
            }
            // Album Modus: kompletten Ordner spielen
            if (myCard.mode == 2)
            {
                Serial.println(F("Album Modus -> kompletten Ordner wiedergeben"));
                currentTrack = 1;
                mp3.playFolderTrack(myCard.folder, currentTrack);
            }
            // Party Modus: Ordner in zufälliger Reihenfolge
            if (myCard.mode == 3)
            {
                Serial.println(
                    F("Party Modus -> Ordner in zufälliger Reihenfolge wiedergeben"));
                currentTrack = random(1, numTracksInFolder + 1);
                mp3.playFolderTrack(myCard.folder, currentTrack);
            }
            // Einzel Modus: eine Datei aus dem Ordner abspielen
            if (myCard.mode == 4)
            {
                Serial.println(
                    F("Einzel Modus -> eine Datei aus dem Odrdner abspielen"));
                currentTrack = myCard.special;
                mp3.playFolderTrack(myCard.folder, currentTrack);
            }
            // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken
            if (myCard.mode == 5)
            {
                Serial.println(F("Hörbuch Modus -> kompletten Ordner spielen und "
                                 "Fortschritt merken"));
                currentTrack = EEPROM.read(myCard.folder);
                mp3.playFolderTrack(myCard.folder, currentTrack);
            }
        }
}
*/
