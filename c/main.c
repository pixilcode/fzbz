#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
 * NOTE: This program doesn't clean up memory since it is short-lived,
 *       so it won't consume memory for an extended period of time
 */


// === ERROR HANDLING ===

/**
 * Indicates an error when used as a return value
 */
const int PROCESS_ERROR = -1;

#define ASSERT(condition) if (!(condition)) return PROCESS_ERROR;


// === DECLARATIONS ===

char* get_fizzbuzz_path();
char* get_user_home();
int read_fizzbuzz_file(char* fizzbuzz_path);
int read_int_from_file(FILE* file);
void run_fizzbuzz(int counter);
int write_fizzbuzz_file(char* fizzbuzz_path, int counter);
void print_error(char* msg);

/**
 * The default value when errors occur getting the current fizzbuzz value
 */
const int DEFAULT_VALUE = 1;


// === MAIN ===

int main() {
    char* fizzbuzz_path = get_fizzbuzz_path();

    // if the $HOME env variable isn't set
    // print error and quit early
    if (fizzbuzz_path == NULL) {
        print_error("error: $HOME environment variable isn't set");
        return EXIT_FAILURE;
    }

    // read the integer from the fizzbuzz file
    // or use the default if it failed
    int fizzbuzz_counter = read_fizzbuzz_file(fizzbuzz_path);
    if (fizzbuzz_counter < 0) fizzbuzz_counter = DEFAULT_VALUE;

    run_fizzbuzz(fizzbuzz_counter);

    // reset the counter if it's about to overflow
    if (fizzbuzz_counter == INT_MAX) fizzbuzz_counter = 0;

    write_fizzbuzz_file(fizzbuzz_path, fizzbuzz_counter + 1);

    return EXIT_SUCCESS;
}


// === HELPER FUNCTIONS ===

/**
 * The name of the fizzbuzz file
 */
const char* FIZZBUZZ_FILE_NAME = ".fizzbuzz";

/**
 * Get the location of the fizzbuzz file
 *
 * Returns NULL if the $HOME environment variable is unset
 */
char* get_fizzbuzz_path() {
    char* user_home = get_user_home();
    if (user_home == NULL) return NULL;

    // path length = length of directory + '/' + length of file name + '\0'
    size_t path_length = strlen(user_home) + 1 + strlen(FIZZBUZZ_FILE_NAME) + 1;
    char* fizzbuzz_path = malloc(path_length * sizeof(char));
    sprintf(fizzbuzz_path, "%s/%s", user_home, FIZZBUZZ_FILE_NAME);

    return fizzbuzz_path;
}

/**
 * Get the user's home directory from the $HOME environment variable
 */
char* get_user_home() {
    return getenv("HOME");
}

/**
 * Read the integer from the user's fizzbuzz file
 *
 * Returns a negative value if the read failed
 */
int read_fizzbuzz_file(char* fizzbuzz_path) {
    // open the file or return the default value
    FILE* fizzbuzz_file = fopen(fizzbuzz_path, "r");
    ASSERT(fizzbuzz_file != NULL);

    // read in the value from the file
    int current_value = read_int_from_file(fizzbuzz_file);
    ASSERT(current_value >= 0);

    // close the file
    fclose(fizzbuzz_file);

    return current_value;
}

/**
 * Read an integer from the given file (must be positive or 0)
 *
 * Returns a negative value if the integer is invalid 
 */
int read_int_from_file(FILE* file) {
    // get file size
    rewind(file);
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // read from the file
    char* buffer = (char*) malloc(sizeof(char) * (file_size + 1)); // +1 for '\0'
    size_t num_read = fread(buffer, sizeof(char), file_size, file);
    buffer[num_read] = '\0';
    ASSERT(num_read == file_size);

    // parse int from input buffer
    int value;
    int rc = sscanf(buffer, "%d", &value);
    ASSERT(rc == 1); // 1 indicates success

    return value;
}

/**
 * Do the fizzbuzz things
 */
void run_fizzbuzz(int counter) {
    int modulo_five = counter % 5;
    int modulo_three = counter % 3;

    // if not divisible by 5 or 3, don't do anything
    if (modulo_five != 0 && modulo_three != 0) return;

    // print corresponding fizzbuzz value based on the modulos
    if (modulo_three == 0) printf("fizz");
    if (modulo_five == 0) printf("buzz");
    fflush(stdout);

    // wait for 300 milliseconds
    usleep(300000);

    // remove the text from the screen
    printf("\033[2K\r");
    fflush(stdout);
}

/**
 * Write the counter value to the fizzbuzz file
 *
 * Returns 0 on success or a negative value on error
 */
int write_fizzbuzz_file(char* fizzbuzz_path, int counter) {
    // open the file
    FILE* file = fopen(fizzbuzz_path, "w");
    ASSERT(file != NULL);

    // write to the file
    fprintf(file, "%d", counter);

    // close the file
    fclose(file);

    // 0 indicates success
    return 0;
}

/**
 * Print an error to stderr
 */
void print_error(char* msg) {
    fprintf(stderr, "error: %s", msg);
}
