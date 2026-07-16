/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Errors
*/

#pragma once

#include <exception>
#include <string>

namespace Zappy {
    class GuiError : public std::exception {
        public:
            GuiError(std::string message) : _message(std::move(message)) {}
            const char *what() const noexcept override { return _message.c_str(); }
        protected:
            std::string _message;
    };

    class LoadingError : public GuiError {
        public:
            LoadingError(std::string message) : GuiError(std::move(message)) {}
    };

    class ParsingError : public GuiError {
        public:
            ParsingError(std::string message) : GuiError(std::move(message)) {}
    };

    class ValidationError : public GuiError {
        public:
            ValidationError(std::string message) : GuiError(std::move(message)) {}
    };

    class NotFoundError : public GuiError {
        public:
            NotFoundError(std::string message) : GuiError(std::move(message)) {}
    };

    class NetworkError : public GuiError {
        public:
            NetworkError(std::string message) : GuiError(std::move(message)) {}
    };
}
