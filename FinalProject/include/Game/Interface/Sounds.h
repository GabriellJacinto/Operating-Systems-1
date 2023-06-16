#ifndef SOUNDS_H
#define SOUNDS_H

#include "SFML/Audio.hpp"
#include "Concurrency/traits.h"

__BEGIN_API

class Sounds
{
public:
    static void loadSounds();

    static void playBackgroundMusic();
    static void playShotSound();
    static void playHitSound();
    static void playGameOverSound();
    static void playLevelUpSound();
    static void playRestartSound();

private:
    static sf::Music backgroundMusic;

    static sf::SoundBuffer shotSoundBuffer;
    static sf::SoundBuffer hitSoundBuffer;
    static sf::SoundBuffer gameOverSoundBuffer;
    static sf::SoundBuffer levelUpSoundBuffer;
    static sf::SoundBuffer restartSoundBuffer;

    static sf::Sound shotSound;
    static sf::Sound hitSound;
    static sf::Sound gameOverSound;
    static sf::Sound levelUpSound;
    static sf::Sound restartSound;
};

__END_API

#endif
