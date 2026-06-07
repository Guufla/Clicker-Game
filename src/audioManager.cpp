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

    audio.setGlobalVolume(0.35f);

    // if (button.load(FileSystem::getPath("resources/audio/button.wav").c_str()) != 0)
    // {
    //     std::cout << "Failed to load button sound\n";
    // }

}

// Functions
void AudioManager::PlayPopSound()
{  
    if(BubblePopThisFrame == true)
    {
        return;
    }

    BubblePopThisFrame = true;
    
    if (activeSounds.size() >= MaxPopSounds)
    {
        audio.stop(activeSounds.front());
        activeSounds.erase(activeSounds.begin());
    }

    int randomPop = rand() % 4; // Random number between 0 and 3
    SoLoud::handle handle;

    // audio.setRelativePlaySpeed(handle, 2.0f);

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

    activeSounds.push_back(handle);
}

void AudioManager::CleanUpSounds()
{
    BubblePopThisFrame = false;
    for(int i = activeSounds.size() - 1; i >= 0; i--)
    {
        SoLoud::handle handle = activeSounds.front();
        if (!audio.isValidVoiceHandle(activeSounds[i]))
        {
            activeSounds.erase(activeSounds.begin() + i);
        }
    }
}

void AudioManager::KillAllPopSounds()
{
    for(SoLoud::handle handle : activeSounds)
    {
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