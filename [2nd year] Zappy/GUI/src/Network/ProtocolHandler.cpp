/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-13
** File description:
** ProtocolHandler
*/

#include "ProtocolHandler.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include "Errors.hpp"
#include <iostream>

namespace Zappy {

ProtocolHandler::ProtocolHandler(Client &client, Core &core)
    : _client(client), _core(core), _mapInitialized(false),
        _mapFullyLoaded(false),  _expectedTiles(0), _receivedTiles(0)
{
    _client.registerHandler("WELCOME", [](const auto&) {});
    _client.registerHandler("msz", [this](const auto& args) { handleMsz(args); });
    _client.registerHandler("sgt", [this](const auto& args) { handleSgt(args); });
    _client.registerHandler("bct", [this](const auto& args) { handleBct(args); });
    _client.registerHandler("tna", [this](const auto& args) { handleTna(args); });
    _client.registerHandler("pnw", [this](const auto& args) { handlePnw(args); });
    _client.registerHandler("ppo", [this](const auto& args) { handlePpo(args); });
    _client.registerHandler("plv", [this](const auto& args) { handlePlv(args); });
    _client.registerHandler("pin", [this](const auto& args) { handlePin(args); });
    _client.registerHandler("pdi", [this](const auto& args) { handlePdi(args); });
    _client.registerHandler("enw", [this](const auto& args) { handleEnw(args); });
    _client.registerHandler("ebo", [this](const auto& args) { handleEbo(args); });
    _client.registerHandler("edi", [this](const auto& args) { handleEdi(args); });
    _client.registerHandler("pex", [this](const auto& args) { handlePex(args); });
    _client.registerHandler("pbc", [this](const auto& args) { handlePbc(args); });
    _client.registerHandler("pic", [this](const auto& args) { handlePic(args); });
    _client.registerHandler("pie", [this](const auto& args) { handlePie(args); });
    _client.registerHandler("pfk", [this](const auto& args) { handlePfk(args); });
    _client.registerHandler("pdr", [this](const auto& args) { handlePdr(args); });
    _client.registerHandler("pgt", [this](const auto& args) { handlePgt(args); });
    _client.registerHandler("seg", [this](const auto& args) { handleSeg(args); });
    _client.registerHandler("sst", [this](const auto& args) { handleSst(args); });
    _client.registerHandler("smg", [this](const auto& args) { handleSmg(args); });
    _client.registerHandler("suc", [this](const auto& args) { handleSuc(args); });
    _client.registerHandler("sbp", [this](const auto& args) { handleSbp(args); });
    
}

bool ProtocolHandler::safe_stoi(const std::string& str, int& value) const {
    try {
        size_t pos;
        value = std::stoi(str, &pos);
        return pos == str.size();
    } catch (const std::exception&) {
        return false;
    }
}

bool ProtocolHandler::parseId(const std::string& str, int& out) const {
    std::string cleaned = str;

    if (!cleaned.empty() && cleaned[0] == '#')
        cleaned.erase(0, 1);

    return safe_stoi(cleaned, out);
} 

void ProtocolHandler::handleMsz(const std::vector<std::string>& args)  {
    if (args.size() != 2) {
        std::cerr << "msz: expected 2 arguments, got " << args.size() << std::endl;
        return;
    }
    if (_mapInitialized) {
        std::cerr << "msz: map already initialized, ignoring duplicate" << std::endl;
        return;
    }

    int width;
    int height;
    if (!safe_stoi(args[0], width) || !safe_stoi(args[1], height)) {
        std::cerr << "msz: invalid integer argument(s)" << std::endl;
        return;
    }
    if (width <= 0 || height <= 0) {
        std::cerr << "msz: width and height must be positive (got " << width << "x" << height << ")" << std::endl;
        return;
    }

    _core.onMsz(width, height);
    _mapInitialized = true;
    _expectedTiles = width * height;
    _receivedTiles = 0;
    _mapFullyLoaded = false;
}

void ProtocolHandler::handleSgt(const std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cerr << "sgt: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int timeUnit;
    if (!safe_stoi(args[0], timeUnit)) {
        std::cerr << "sgt: invalid integer argument" << std::endl;
        return;
    }
    if (timeUnit <= 0) {
        std::cerr << "sgt: time unit must be positive (got " << timeUnit << ")" << std::endl;
        return;
    }

    _core.onSgt(timeUnit);
}

void ProtocolHandler::handleBct(const std::vector<std::string>& args) {
    if (args.size() < 9) {
        std::cerr << "bct: expected 9 arguments, got " << args.size() << std::endl;
        return;
    }

    int x;
    int y;
    if (!safe_stoi(args[0], x) || !safe_stoi(args[1], y)) {
        std::cerr << "bct: invalid coordinate(s)" << std::endl;
        return;
    }
    if (x < 0 || y < 0) {
        std::cerr << "bct: coordinates must be non-negative (got " << x << "," << y << ")" << std::endl;
        return;
    }
    if (!_mapInitialized) {
        std::cerr << "bct: map not initialized yet, ignoring" << std::endl;
        return;
    }

    std::array<int, RESOURCE_COUNT> resources;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (!safe_stoi(args[2 + i], resources[i])) {
            std::cerr << "bct: invalid resource value at index " << i << std::endl;
            return;
        }
        if (resources[i] < 0) {
            std::cerr << "bct: resource quantity must be non-negative (got " << resources[i] << ")" << std::endl;
            return;
        }
    }

    _core.onBct(x, y, resources);

    if (_mapFullyLoaded)
        return;

    _receivedTiles++;
    if (_receivedTiles >= _expectedTiles)
        _mapFullyLoaded = true;
}

void ProtocolHandler::handleTna(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "tna: expected exactly 1 argument, got " << args.size() << std::endl;
        return;
    }
    _core.onTna(args[0]);
}

void ProtocolHandler::handlePnw(const std::vector<std::string>& args) {
    if (args.size() != 6) {
        std::cerr << "pnw: expected exactly 6 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    int x;
    int y;
    int orientation;
    int level;
    if (!parseId(args[0], id)) {
        std::cerr << "pnw: invalid player ID" << std::endl;
        return;
    }
    if (!safe_stoi(args[1], x)) {
        std::cerr << "pnw: invalid x coordinate" << std::endl;
        return;
    }
    if (!safe_stoi(args[2], y)) {
        std::cerr << "pnw: invalid y coordinate" << std::endl;
        return;
    }
    if (!safe_stoi(args[3], orientation)) {
        std::cerr << "pnw: invalid orientation" << std::endl;
        return;
    }
    if (!safe_stoi(args[4], level)) {
        std::cerr << "pnw: invalid level" << std::endl;
        return;
    }
    if (id < 0) {
        std::cerr << "pnw: player ID must be positive (got " << id << ")" << std::endl;
        return;
    }
    std::string team = args[5];

    _core.onPnw(id, x, y, orientation, level, team);
}

void ProtocolHandler::handlePpo(const std::vector<std::string>& args) {
    if (args.size() != 4) {
        std::cerr << "ppo: expected exactly 4 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    int x;
    int y;
    int orientation;
    if (!parseId(args[0], id) || !safe_stoi(args[1], x) || !safe_stoi(args[2], y) ||
        !safe_stoi(args[3], orientation)) {
        std::cerr << "ppo: invalid integer argument" << std::endl;
        return;
    }
    _core.onPpo(id, x, y, orientation);
}

void ProtocolHandler::handlePlv(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cerr << "plv: expected exactly 2 arguments, got " << args.size() << std::endl;
        return;
    }
    int id;
    int level;
    if (!parseId(args[0], id) || !safe_stoi(args[1], level)) {
        std::cerr << "plv: invalid integer argument" << std::endl;
        return;
    }
    if (level <= 0) {
        std::cerr << "plv: level must be positive (got " << level << ")" << std::endl;
        return;
    }
    if (level > 8) {
        std::cerr << "plv: level must be between 1 and 8 (got " << level << ")" << std::endl;
        return;
    }

    _core.onPlv(id, level);
}

void ProtocolHandler::handlePin(const std::vector<std::string>& args) {
    if (args.size() != 10) {
        std::cerr << "pin: expected 10 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    int x;
    int y;
    std::array<int, RESOURCE_COUNT> resources;
    if (!parseId(args[0], id) || !safe_stoi(args[1], x) || !safe_stoi(args[2], y))
        return;

    for (int i = 0; i < RESOURCE_COUNT; i++)
        if (!safe_stoi(args[3 + i], resources[i])) {
            std::cerr << "pin: invalid resource count" << std::endl;
            return;
        }

    _core.onPin(id, resources);
}

void ProtocolHandler::handlePdi(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "pdi: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int id;
    if (!parseId(args[0], id))
        return;

    _core.onPdi(id);
}

void ProtocolHandler::handleEnw(const std::vector<std::string>& args) {
    if (args.size() != 4) {
        std::cerr << "enw: expected 4 arguments, got " << args.size() << std::endl;
        return;
    }

    int eggId;
    int playerId;
    int x;
    int y;
    if (!parseId(args[0], eggId)) {
        std::cerr << "enw: invalid egg ID" << std::endl;
        return;
    }
    if (!parseId(args[1], playerId) || playerId < -1) {
        std::cerr << "enw: invalid player ID" << std::endl;
        return;
    }
    if (!safe_stoi(args[2], x) || !safe_stoi(args[3], y)) {
        std::cerr << "enw: invalid coordinate(s)" << std::endl;
        return;
    }
    _core.onEnw(eggId, playerId, x, y);
}

void ProtocolHandler::handleEbo(const std::vector<std::string>& args) {

    if (args.size() != 1) {
        std::cerr << "ebo: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int eggId;
    if (!parseId(args[0], eggId))
        return;

    _core.onEbo(eggId);
} 

void ProtocolHandler::handleEdi(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "edi: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int eggId;
    if (!parseId(args[0], eggId))
        return;

    _core.onEdi(eggId);
}

void ProtocolHandler::handlePex(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "pex: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int id;
    if (!parseId(args[0], id)) {
        std::cerr << "pex: invalid player ID" << std::endl;
        return;
    }

    _core.onPex(id);
}

void ProtocolHandler::handlePbc(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "pbc: expected at least 2 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    if (!parseId(args[0], id)) {
        std::cerr << "pbc: invalid player ID" << std::endl;
        return;
    }

    std::string message;
    for (size_t i = 1; i < args.size(); i++) {
        if (i > 1)
            message += " ";
        message += args[i];
    }

    _core.onPbc(id, message);
}

void ProtocolHandler::handlePic(const std::vector<std::string>& args) {
    if (args.size() < 4) {
        std::cerr << "pic: expected at least 4 arguments, got " << args.size() << std::endl;
        return;
    }

    int x;
    int y;
    int level;
    if (!safe_stoi(args[0], x) || !safe_stoi(args[1], y) || !safe_stoi(args[2], level)) {
        std::cerr << "pic: invalid integer argument" << std::endl;
        return;
    }

    std::vector<int> playerIds;
    for (size_t i = 3; i < args.size(); i++) {
        int id;
        if (parseId(args[i], id))
            playerIds.push_back(id);
    }

    _core.onPic(playerIds);
}

void ProtocolHandler::handlePie(const std::vector<std::string>& args) {

    if (args.size() != 3) {
        std::cerr << "pie: expected 3 arguments, got " << args.size() << std::endl;
        return;
    }

    int x;
    int y;
    int result;
    if (!safe_stoi(args[0], x) || !safe_stoi(args[1], y) || !safe_stoi(args[2], result)) {
        std::cerr << "pie: invalid integer argument" << std::endl;
        return;
    }

    _core.onPie(x, y, result);
}

void ProtocolHandler::handlePfk(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "pfk: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int id;
    if (!parseId(args[0], id)) {
        std::cerr << "pfk: invalid player ID" << std::endl;
        return;
    }

    _core.onPfk(id);
}

void ProtocolHandler::handlePdr(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cerr << "pdr: expected 2 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    int resourceNumber;
    if (!parseId(args[0], id) || !safe_stoi(args[1], resourceNumber)) {
        std::cerr << "pdr: invalid integer argument" << std::endl;
        return;
    }
    if (resourceNumber < 0 || resourceNumber >= RESOURCE_COUNT) {
        std::cerr << "pdr: resource index out of range: " << resourceNumber << std::endl;
        return;
    }

    _core.onPdr(id, resourceNumber);
}

void ProtocolHandler::handlePgt(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cerr << "pgt: expected 2 arguments, got " << args.size() << std::endl;
        return;
    }

    int id;
    int resourceNumber;
    if (!parseId(args[0], id) || !safe_stoi(args[1], resourceNumber)) {
        std::cerr << "pgt: invalid integer argument" << std::endl;
        return;
    }
    if (resourceNumber < 0 || resourceNumber >= RESOURCE_COUNT) {
        std::cerr << "pgt: resource index out of range: " << resourceNumber << std::endl;
        return;
    }

    _core.onPgt(id, resourceNumber);
}

void ProtocolHandler::handleSeg(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "seg: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    _core.onSeg(args[0]);
}

void ProtocolHandler::handleSst(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "sst: expected 1 argument, got " << args.size() << std::endl;
        return;
    }

    int timeUnit;
    if (!safe_stoi(args[0], timeUnit) || timeUnit <= 0) {
        std::cerr << "sst: invalid time unit" << std::endl;
        return;
    }

    _core.onSst(timeUnit);
}

void ProtocolHandler::handleSmg(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cerr << "smg: expected at least 1 argument" << std::endl;
        return;
    }

    std::string message;
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0)
            message += " ";
        message += args[i];
    }

    _core.onSmg(message);
}

void ProtocolHandler::handleSuc(const std::vector<std::string>& args) {

    if (!args.empty()) {
        std::cerr << "suc: expected no arguments, got " << args.size() << std::endl;
        return;
    }
    std::cerr << "GUI sent an unknown command to server" << std::endl;
}

void ProtocolHandler::handleSbp(const std::vector<std::string>& args) {
    if (!args.empty()) {
        std::cerr << "sbp: expected no arguments, got " << args.size() << std::endl;
        return;
    }
    std::cerr << "GUI sent a command to server with bad parameters" << std::endl;
}
}
