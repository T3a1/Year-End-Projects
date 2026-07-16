/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** MusicManager
*/

#ifndef MUSICMANAGER_HPP_
#define MUSICMANAGER_HPP_

#include <MusicSound.hpp>
#include <map>
#include <optional>
#include <vector>
#include "Constants.hpp"
#include <functional>

namespace Zappy {
    class MusicManager {
        public:
            MusicManager();
            ~MusicManager();

            void playMusic(const std::string& musicName);
            void playRandomMusic();
            void updateVolume(float volume);
            void stopAllMusics();
            void update();

        private:
            std::map<std::string, Raylib::MusicSound> _musicMap;
            std::vector<std::string> _playlist;
            std::optional<std::reference_wrapper<Raylib::MusicSound>> _currentMusic;
            std::string _currentMusicName;
    };
}

#endif /* !MUSICMANAGER_HPP_ */
