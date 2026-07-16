/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** MusicSound
*/

#ifndef MUSIC_SOUND_HPP_
#define MUSIC_SOUND_HPP_

#include "raylib.h"
#include <string>
#include <map>
#include "Errors.hpp"

namespace Raylib {

    class MusicSound {
        
        public:
            MusicSound(std::string path);
            ~MusicSound();

            MusicSound(const MusicSound&) = delete;
            MusicSound& operator=(const MusicSound&) = delete;

            MusicSound(MusicSound&& other) noexcept : _music(other._music) { other._music = {}; }

            MusicSound& operator=(MusicSound&& other);

            void play(bool loop = false);
            void updateVolume(float volume);
            void updateStream();
            Music& getMusic();
            void stop();
            bool isPlaying();

        private:
            Music _music;
    };
};
#endif /* !MUSIC_SOUND_HPP_ */
