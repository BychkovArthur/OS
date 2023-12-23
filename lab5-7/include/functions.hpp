#pragma once

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