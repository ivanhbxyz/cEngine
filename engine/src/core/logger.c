#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"
// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>






b8 initialze_logging() {
    // TODO: create log file
    return TRUE;
}

void shutdown_logging() {
    // TODO: cleanup logging/write queued entries
}

void log_output(log_level level, const char* message, ...) {
    // Array of level strings
    const char* level_strings[6] = {"[FATA]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    b8 is_error = level < LOG_LEVEL_WARN;

    // Techically imposes a 32k character limit on a single log entry , but..
    // Don't do that
    const i32 msg_length = 32000;
    char out_message[msg_length]; // array of chars

    // zeros out the memory. this is faster than using malloc. done on the stack
    memset(out_message, 0, sizeof(out_message));

    /*
        Format original message
        Note: oddly enough , MS's headers override the gc/Clang va_list type with a typedef char* va_list
        in some cases, and as a result throws a strange error her. The workaround for now is to just use __builtin_va_list
        which is the type GCC/Clang's va_start expects 
     */

    // What is va_list?
    __builtin_va_list arg_ptr; // this allows us to point to the argument list in the methods were ... is passed
    va_start(arg_ptr, message); // starts AFTER the message argument
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);
    char out_message2[msg_length]; // array of chars
    sprintf(out_message2, "%s%s\n", level_strings[level], out_message); // What is sprintf?

    // TODO: Platform specific output.
    if(is_error) {
        platform_console_write_error(out_message2, level);
    } else {
        platform_console_write(out_message2, level);
    }
}

void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    /*
        Note: You DO NOT have to write the implementation of a method declared in a header(.h) file in a .c file.
        It is possible to do it in a .h file. Like in this case.
    */
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}