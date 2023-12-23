#pragma once

#define MIN_DYNAMIC_PORT 49152

enum class OperationType {
    CREATE,
    EXEC,
    PING,
    QUIT,
};

enum class TimerSubcommand {
    START,
    STOP,
    TIME,
    NOTHING,
};

/*
Структура комманды
Сначала хранится тип команды
Далее, id, в какому узлу надо применить эту комманду
И, только для комманды exec будет хранится тип подкомманды, в остальных NOTHING
Для `create id -1` -1 хранить не будем
*/
struct Command {
    OperationType operationType;
    ssize_t id;
    TimerSubcommand subcommand;
};