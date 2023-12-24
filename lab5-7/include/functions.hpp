#pragma once

#include <map>
#include <string>
#include <zmq_addon.hpp>
#include <zmq.hpp>
#include <cerrno>
#include <sys/wait.h>

#include "definitions.hpp"

std::string getAddres(size_t);
void validateOperationType(Request&, std::string&);
void validateId(Request&, ssize_t);
void validateSubrequest(Request&, std::string&);
Request readRequest();

void pushRequest(zmq::socket_t& socket, Request& request);
void pushReply(zmq::socket_t& socket, Reply& request);
Reply pullReply(zmq::socket_t& socket);
Request pullRequest(zmq::socket_t& socket);

std::pair<pid_t, bool> createNewNode(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&, ssize_t, size_t&);
void updateNodeMap(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&, size_t&, Request&);

void killWorkers(std::unordered_map<ssize_t, std::pair<pid_t, size_t>>&);

bool isProcessExists(pid_t);