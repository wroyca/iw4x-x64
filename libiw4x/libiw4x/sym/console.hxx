#pragma once

using  console_show_t = void (*) ();
extern console_show_t console_show;

using  console_output_t = void (*) (const char*);
extern console_output_t console_output;

