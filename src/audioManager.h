#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <iostream>

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>
#include <queue>


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class AudioManager
{
public:
    
    SoLoud::Soloud audio; // Audio engine

    // Sound effects
    SoLoud::Wav pop1; // Pop sound effect
    SoLoud::Wav pop2; // Pop sound effect
    SoLoud::Wav pop3; // Pop sound effect
    SoLoud::Wav pop4; // Pop sound effect

    SoLoud::Wav button; // button sound effect

    // constructor(s)
    AudioManager();

    // Functions
    void PlayPopSound();
    void StopOldestSound();
    void PlayButtonSound();

    void deInit();
private:
    std::queue<SoLoud::handle> activeSounds;
    int MaxPopSounds = 8;
};



#endif