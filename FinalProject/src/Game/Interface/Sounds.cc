#include "Game/Interface/Sounds.h"
#include "Concurrency/traits.h"
#include "SFML/Audio.hpp"

__BEGIN_API

sf::Music Sounds::backgroundMusic;

sf::SoundBuffer Sounds::shotSoundBuffer;

sf::SoundBuffer Sounds::hitSoundBuffer;

sf::SoundBuffer Sounds::gameOverSoundBuffer;

sf::SoundBuffer Sounds::levelUpSoundBuffer;

sf::SoundBuffer Sounds::restartSoundBuffer;

sf::Sound Sounds::shotSound;

sf::Sound Sounds::hitSound;

sf::Sound Sounds::gameOverSound;

sf::Sound Sounds::levelUpSound;

sf::Sound Sounds::restartSound;

void Sounds::loadSounds()
{
    backgroundMusic.openFromFile("assets/sounds/background.ogg");
    backgroundMusic.setLoop(true);

    shotSoundBuffer.loadFromFile("assets/sounds/shot.wav");
    hitSoundBuffer.loadFromFile("assets/sounds/hit.wav");
    gameOverSoundBuffer.loadFromFile("assets/sounds/gameOver.wav");
    levelUpSoundBuffer.loadFromFile("assets/sounds/levelUp.wav");
    restartSoundBuffer.loadFromFile("assets/sounds/restart.wav");

    shotSound.setBuffer(shotSoundBuffer);
    hitSound.setBuffer(hitSoundBuffer);
    gameOverSound.setBuffer(gameOverSoundBuffer);
    levelUpSound.setBuffer(levelUpSoundBuffer);
    restartSound.setBuffer(restartSoundBuffer);
}

void Sounds::playBackgroundMusic()
{
    backgroundMusic.play();
}

void Sounds::playShotSound()
{
    shotSound.play();
}

void Sounds::playHitSound()
{
    hitSound.play();
}

void Sounds::playGameOverSound()
{
    gameOverSound.play();
}

void Sounds::playLevelUpSound()
{
    levelUpSound.play();
}

void Sounds::playRestartSound()
{
    restartSound.play();
}

__END_API