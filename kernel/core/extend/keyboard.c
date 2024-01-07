#include <types.h>
#include <debug.h>
#include <string.h>

#include <extend/keyboard.h>
#include <extend/syscall.h>

unsigned char kbdus[128] = {
  0,27,'1','2','3','4','5','6','7','8', /* 9 */
  '9','0','-','=','\b',/* Backspace */
  '\t', /* Tab */
  'q','w','e','r',/* 19 */
  't','y','u','i','o','p','[',']','\n',/* Enter key */
  0,/* 29   - Control */
  'a','s','d','f','g','h','j','k','l',';',/* 39 */
 '\'','`',90,/* Left shift */
 '\\','z','x','c','v','b','n',/* 49 */
  'm',',','.','/',91,/* Right shift */
  '*',
   0, /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

unsigned char upperKbdus[128] = {
  0,27,'!','@','#','$','%','^','&','&', /* 9 */
  '(',')','_','+','\b',/* Backspace */
  '\t', /* Tab */
  'Q','W','E','R',/* 19 */
  'T','Y','U','I','O','P','{','}','\n',/* Enter key */
  0,/* 29   - Control */
  'A','S','D','F','G','H','J','K','L',':',/* 39 */
 '\"','~',90,/* Left shift */
 '|','Z','X','C','V','B','N',/* 49 */
  'M','<','>','?',91,/* Right shift */
  '*',
   0, /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

int cursor_position = 0;
char command_buffer[COMMAND_BUFFER_SIZE];
int console_enabled = 0;

void analyse_command() {
    if (strncmp(HELP_COMMAND, command_buffer, cursor_position) == 0) {
        debug("%s\n", "> This is the help page of the command mode");
        debug("%s\n", "> There are only a few commands, it's 10pm, we are the 07/01, we don't have time...");
        debug("%s\n", "> - 'count' -> See the counter value (yea useful)");
        debug("%s\n", "> - 'exit' -> Return to previous mode");
        debug("%s\n", "> - 'salami' -> Private joke");
        debug("%s\n", "> - 'shutdown' -> It gives you a bonnet!");
    } else if (strncmp(COUNT_COMMAND, command_buffer, cursor_position) == 0) {
        debug("%s%d\n", "> Current count is ", read_shared_memory_syscall((int*) 0x414000));
    } else if (strncmp(EXIT_COMMAND, command_buffer, cursor_position) == 0) {
        debug("%s\n", "> Returning to the boring count...");
        console_enabled = 0;
    } else if (strncmp(SALAMI_COMMAND, command_buffer, cursor_position) == 0) {
        debug("%s\n", "> lol");
    } else if (strncmp(SHUTDOWN_COMMAND, command_buffer, cursor_position) == 0) {
        debug("%s\n", "> I admire that move :clap:");
        shutdown_syscall();
    } else {
        debug("%s\n", "> Fais un effort, je comprends pas ce que tu me dis...");
    }
}

void print_key(char key) {
    if (key == '\b') {
        // We remove what we wrote if the cursor is not at the start
        if (cursor_position != 0) {
            debug("%s", "\b \b");
            cursor_position--;
        }
    } else if (key == '\n') {
        if (!console_enabled) {
            console_enabled = 1;
            debug("%s\n", "> Welcome to command mode!\n> Type 'help' to see all commands!");
        } else {
            // Command execution
            if (cursor_position != 0) {
                debug("\n");
                analyse_command();
                cursor_position = 0;
            }
        }
    } else {
        // Writing key if buffer not full
        if (cursor_position < COMMAND_BUFFER_SIZE) {
            debug("%c", key);
            command_buffer[cursor_position++] = key;
        }
    }
}

int shift = 0;
void process_key(uint8_t key) {
    uint8_t keyBuf;
    
    // If shift press then shift
    if (key == SHIFT_PRESSED1 || key == SHIFT_PRESSED2) {
        shift = 1;
        return;
    }
    
    // If shift release, then no shift
    if (key == SHIFT_RELEASE1 || key == SHIFT_RELEASE2) {
        shift = 0; 
        return;
    }
    
    // MSB set => key released, so ignore
    if ((key & 0x80) != 0) {    
        return;
    }

    // If the associated key is 0 then we were fooled
    if (kbdus[key] == 0) {
        return;
    }

    //load the key into the buffer
    if (shift) {
        keyBuf = upperKbdus[key];
    } else {
        keyBuf = kbdus[key];
    }
   
    print_key(keyBuf);
}

int is_console_enabled() {
    return console_enabled;
}