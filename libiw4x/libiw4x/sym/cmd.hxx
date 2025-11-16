#pragma once

using  cmd_execute_single_command_t = void (*) (int, int, const char*);
extern cmd_execute_single_command_t cmd_execute_single_command;
