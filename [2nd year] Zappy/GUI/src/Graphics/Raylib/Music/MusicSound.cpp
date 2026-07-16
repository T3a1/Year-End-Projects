/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** Music
*/

#include "MusicSound.hpp"

namespace Raylib {
    MusicSound::MusicSound(std::string path)
    { 
        _music = LoadMusicStream(path.c_str());
        if (_music.ctxType == 0)
            throw Zappy::LoadingError("Failed to load music: " + path);
    }

    MusicSound::~MusicSound()
    {
        UnloadMusicStream(_music);
    }

    void MusicSound::play(bool loop)
    {
        _music.looping = loop;
        PlayMusicStream(_music);
        updateStream();
    }

    void MusicSound::updateVolume(float volume)
    {
        SetMusicVolume(_music, volume);
    }
    
    void MusicSound::updateStream()
    {
        UpdateMusicStream(_music);
    }

    Music& MusicSound::getMusic()
    {
        return _music;
    }

    void MusicSound::stop()
    {
        StopMusicStream(_music);
    }

    bool MusicSound::isPlaying()
    {
        return IsMusicStreamPlaying(_music);
    }

    MusicSound& MusicSound::operator=(MusicSound&& other)
    {
        if (this != &other) {
            UnloadMusicStream(_music);
            _music = other._music;
            other._music = {};
        }
        return *this;
    }
}
