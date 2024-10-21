#include <stdlib.h>
#include <stdio.h>

#define ERROR_LIST(X)          \
    X(ERROR_FILE_NOT_FOUND, "File not found")    \
    X(ERROR_INVALID_INPUT, "Invalid input")      \
    X(ERROR_OUT_OF_MEMORY, "Out of memory")      \
    X(ERROR_UNKNOWN, "Unknown error")

#define DEFINE_ERROR_ENUM(code, message) code,
enum ErrorCode {
    ERROR_LIST(DEFINE_ERROR_ENUM)
};

#define DEFINE_ERROR_STRING(code, message) case code: return message;
const char* get_error_message(enum ErrorCode error_code) {
    switch (error_code) {
        ERROR_LIST(DEFINE_ERROR_STRING)
        default: return "Unrecognized error";
    }
}

int main(int argc, char *argv[])
{
  enum ErrorCode error = ERROR_OUT_OF_MEMORY;
  printf("Error: %s\n", get_error_message(error));
  return EXIT_SUCCESS;
}
