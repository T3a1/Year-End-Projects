/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** MusicManager
*/

#include "MusicManager.hpp"
#include <iostream>

namespace Zappy {
    MusicManager::MusicManager()
    {
        _musicMap.insert({"Barbie", Raylib::MusicSound(BARBIE_MUSIC_PATH)});
        _musicMap.insert({"Macarena", Raylib::MusicSound(MACARENA_MUSIC_PATH)});
        _musicMap.insert({"Blue", Raylib::MusicSound(BLUE_MUSIC_PATH)});
        _musicMap.insert({"Mii", Raylib::MusicSound(MII_MUSIC_PATH)});
        _musicMap.insert({"Chevaux", Raylib::MusicSound(CHEVAUX_MUSIC_PATH)});
        _musicMap.insert({"Souris", Raylib::MusicSound(SOURIS_MUSIC_PATH)});

        for (const auto& [name, musicSound] : _musicMap)
            if (name != "Macarena")
                _playlist.push_back(name);

        updateVolume(0.6f);
    }

    MusicManager::~MusicManager()
    {
        _musicMap.clear();
    }

    void MusicManager::playMusic(const std::string& musicName)
    {
        if (_musicMap.find(musicName) == _musicMap.end())
            throw Zappy::LoadingError("Music not found: " + musicName);

        if (musicName == _currentMusicName)
            return;

        if (_currentMusic)
            _currentMusic->get().stop();

        _currentMusicName = musicName;
        _currentMusic = std::ref(_musicMap.at(musicName));

        if (musicName == "Macarena") 
            _currentMusic->get().play(true);
        else
            _currentMusic->get().play(false);
    }

    void MusicManager::playRandomMusic()
    {
        if (_playlist.empty())
            return;

        int randomIndex = rand() % _playlist.size();
        playMusic(_playlist.at(randomIndex));
    }

    void MusicManager::updateVolume(float volume)
    {
        for (auto& [name, musicSound] : _musicMap)
            musicSound.updateVolume(volume);
    }

    void MusicManager::stopAllMusics()
    {
        for (auto& [name, musicSound] : _musicMap)
            musicSound.stop();
    }
    
    void MusicManager::update()
    {

        if (!_currentMusic)
            return;

        _currentMusic->get().updateStream();

        if (_currentMusicName == "Macarena" || _currentMusic->get().isPlaying())
            return;

        playRandomMusic();
    }
}
