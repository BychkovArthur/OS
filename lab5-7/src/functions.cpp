#include "functions.hpp"

#include <iostream>
#include <sstream>

std::string getAddres(size_t port) {
    return std::string("tcp://127.0.0.1:") + std::to_string(port);
}

void validateOperationType(Command& command, std::string& operationType) {
    if (operationType == "quit") {
        command.operationType = OperationType::QUIT;
    } else if (operationType == "create") {
        command.operationType = OperationType::CREATE;
    } else if (operationType == "exec") {
        command.operationType = OperationType::EXEC;
    } else if (operationType == "ping") {
        command.operationType = OperationType::PING;
    } else {
        throw std::invalid_argument("Invalid operation");
    }
}
void validateId(Command& command, ssize_t id) {
    if (id < 0) {
        throw std::invalid_argument("Invalid worker node id");
    }
    command.id = id;
}
void validateSubcommand(Command& command, std::string& timerSubcommand) {
    using std::invalid_argument;
    if (command.operationType == OperationType::PING && !timerSubcommand.empty()) {
        throw invalid_argument("Ping can't accept third argument");
    } else if (command.operationType == OperationType::CREATE && timerSubcommand != "-1") {
        throw invalid_argument("Create can push only in end");
    } else if (command.operationType == OperationType::PING || command.operationType == OperationType::CREATE) {
        command.subcommand = TimerSubcommand::NOTHING;
    }

    if (command.operationType == OperationType::EXEC && timerSubcommand == "start") {
        command.subcommand = TimerSubcommand::START;
    } else if (command.operationType == OperationType::EXEC && timerSubcommand == "stop") {
        command.subcommand = TimerSubcommand::STOP;
    } else if (command.operationType == OperationType::EXEC && timerSubcommand == "time") {
        command.subcommand = TimerSubcommand::TIME;
    } else if (command.operationType == OperationType::EXEC) {
        throw invalid_argument("Invalid subcommand");
    }
}

Command readCommand() {
    using std::cout, std::cin, std::endl, std::string, std::stringstream, std::getline, std::invalid_argument;

    string operation;
    Command command;

    string operationType;
    size_t id = -1;
    string timerSubcommand;

    cout << ">>> ";
    getline(cin, operation);
    cout << endl;

    stringstream ss(operation);

    ss >> operationType;
    validateOperationType(command, operationType);

    if (command.operationType == OperationType::QUIT) {
        if (ss >> operationType) {
            throw invalid_argument("Invalid input");
        }
        command.id = -1;
        command.subcommand = TimerSubcommand::NOTHING;
        return command;
    }

    ss >> id;
    validateId(command, id);
    ss >> timerSubcommand;
    validateSubcommand(command, timerSubcommand);

    // Пришла еще одно строка
    if ((ss >> timerSubcommand)) {
        throw invalid_argument("Invalid input");
    }

    return command;
}

void pushMessage(zmq::socket_t& socket, Command& command) {
    zmq::message_t message(&command, sizeof(Command));

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    socket.send(message, zmq::send_flags::none);
    #pragma GCC diagnostic pop
}

Command pullMessage(zmq::socket_t& socket) {
    zmq::message_t message(sizeof(Command));

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
    socket.recv(message, zmq::recv_flags::none);
    #pragma GCC diagnostic pop

    return *message.data<Command>();
}