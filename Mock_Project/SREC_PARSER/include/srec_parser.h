#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

bool parse_file(FILE*);
bool parse_line(char*);
static uint8_t hex_to_val(char );
static uint8_t hex_to_byte(const char *);

