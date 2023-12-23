#pragma once

#include <map>
#include <string>
#include <zmq_addon.hpp>
#include <zmq.hpp>

#include "definitions.hpp"

std::string getAddres(size_t);
void validateOperationType(Command&, std::string&);
void validateId(Command&, ssize_t);
void validateSubcommand(Command&, std::string&);
Command readCommand();

void pushMessage(zmq::socket_t&, Command&);
Command pullMessage(zmq::socket_t&);

pid_t createNewNode(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&, ssize_t, size_t);

void updateNodeMap(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&, size_t&, Command&);

void killWorkers(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&);