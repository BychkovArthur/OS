#pragma once

#define MIN_DYNAMIC_PORT 49152
#define MAX_PORT_LENGTH 5
#define MAX_NODE_ID_LENGTH 5
#define PID_FOR_ALREADY_EXIST_NODE -1
#define ALREADY_REPLACED 0

enum class OperationType {
    CREATE,
    EXEC,
    PING,
    QUIT,
    NOTHING,
};

enum class TimerSubrequest {
    START,
    STOP,
    TIME,
    NOTHING,
};

enum class ErrorTypes {
    STOP_BEFORE_START,
    NO_ERRORS,
};

/*
Структура комманды
Сначала хранится тип команды
Далее, id, в какому узлу надо применить эту комманду
И, только для комманды exec будет хранится тип подкомманды, в остальных NOTHING
Для `create id -1` -1 хранить не будем
*/
struct Request {
    OperationType operationType;
    ssize_t id;
    TimerSubrequest subrequest;
};

struct Reply {
    OperationType operationType;
    ssize_t result;
    ErrorTypes error;
    ssize_t id;
    TimerSubrequest subrequest;
};