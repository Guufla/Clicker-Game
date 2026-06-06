#include "audioManager.h"

AudioManager::AudioManager()
{
    audio.init();
    if (pop1.load(FileSystem::getPath("resources/audio/pop1.wav").c_str()) != 0)
    {
        std::cout << "Failed to load pop1 sound\n";
    }
    if (pop2.load(FileSystem::getPath("resources/audio/pop2.wav").c_str()) != 0)
    {
        std::cout << "Failed to load pop2 sound\n";
    }
    if (pop3.load(FileSystem::getPath("resources/audio/pop3.wav").c_str()) != 0)
    {
        std::cout << "Failed to load pop3 sound\n";
    }
    if (pop4.load(FileSystem::getPath("resources/audio/pop4.wav").c_str()) != 0)
    {
        std::cout << "Failed to load pop4 sound\n";
    }

    audio.setGlobalVolume(1.0f);

    // if (button.load(FileSystem::getPath("resources/audio/button.wav").c_str()) != 0)
    // {
    //     std::cout << "Failed to load button sound\n";
    // }

}

// Functions
void AudioManager::PlayPopSound()
{
    if (activeSounds.size() >= MaxPopSounds)
    {
        StopOldestSound();
    }

    int randomPop = rand() % 4; // Random number between 0 and 3
    SoLoud::handle handle;

    if(randomPop == 0)
    {
        handle = audio.play(pop1);
    }
    else if(randomPop == 1)
    {
        handle = audio.play(pop2);
    }
    else if(randomPop == 2)
    {
        handle = audio.play(pop3);
    }
    else
    {
        handle = audio.play(pop4);
    }

    activeSounds.push(handle);
}

void AudioManager::StopOldestSound()
{
    if (!activeSounds.empty())
    {
        SoLoud::handle handle = activeSounds.front();
        audio.stop(handle);
    }
}

void AudioManager::PlayButtonSound()
{
    audio.play(button);
}

void AudioManager::deInit()
{
    audio.deinit();
}