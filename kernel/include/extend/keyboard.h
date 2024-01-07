#ifndef __EXTEND_KEYBOARD__
#define __EXTEND_KEYBOARD__

#include <types.h>

#define SHIFT_RELEASE1      0xAA
#define SHIFT_RELEASE2      0xB6
#define SHIFT_PRESSED1      0x36
#define SHIFT_PRESSED2      0x2A

#define COMMAND_BUFFER_SIZE 256

#define HELP_COMMAND        "help"
#define COUNT_COMMAND       "count"
#define EXIT_COMMAND        "exit"
#define SALAMI_COMMAND      "salami"
#define SHUTDOWN_COMMAND    "shutdown"
// Processed pressed key
void process_key(uint8_t key);

// Returns if console mode is enabled
int is_console_enabled();

#endif