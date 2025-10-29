#include "../include/srec_parser.h"

int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");

    bool status = parse_file(fp);
    fclose(fp);
    return status;
}

bool parse_file(FILE* fp){
    bool ret = true;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if(!parse_line(line))
        {
            fprintf(stderr, "ERROR: parse line error");
            ret = false;
            break;
        }
    }
    return ret;
}

bool parse_line(char* line){
    bool ret = true;
    int addr_len = 0; //address length 
    char addr[100]; //adress field 
    char data[100]; //data field 
    int byte_count = 0;
    int checksum = 0;
    /* If the first character is S, it's the start of Motorola S-rec format */
    if(line[0] == 'S') 
    {
        /* Check if the second character is a digit */
        if(isdigit(line[1])) 
        {
        /* Check if the length of a packet having minimum length >= 0x03*/
            if((hex_to_val(line[2]) >= 0) && (hex_to_val(line[3]) >=3))
            {
                /* Calculate the length of address + data + checksum */
                byte_count = (hex_to_val(line[2]))*16 + (hex_to_val(line[3]));
                /* Check if the checksum has an error or something */
                checksum = ascii_hex_to_byte(line[strlen(line)-2]);
                if(checksum == (0xFF - ((sum_of_bytes(line[4], strlen(byte_count - 2))) & 0xFF))) 
                {
                    switch(line[1])
                    {
                        case '0':
                            //Do something
                        case '1':
                            addr_len = 2;
                            strcnp(addr, line[4],2); 
            
                        case '2':
                            addr_len = 3;
                            strcnp(addr, line[4], 3);
                        case '3':
                            addr_len = 4;
                            strcnp(addr, line[4], 4);
                        case '7':
                        case '8':
                        case '9':
                        default:
                            printf("Doesn't support this kind of format!"); 
                            ret = false;
                    }
                }
                else 
                {
                    fprintf(stderr, "ERROR: Checksum not matched, error happens!");
                    ret = false;
                }
            } 
            else 
            {
                fprintf(stderr, "ERROR: The length of a packer should be >= 0x03");   
                ret = false;
            }
        }
        else 
        {
            fprintf(stderr, "ERROR: The second character should be a digit");
            ret = false; 
        }
    } 
    else 
    {
        fprintf(stderr, "ERROR: The first character is not S");
        ret = false;
    }
    return ret;
}

// Convert one ASCII hex digit to its integer value
static uint8_t hex_to_val(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    return 0; // Invalid, handle error in real code
}

uint8_t ascii_hex_to_byte(const char *s)
{
    return (ascii_to_nibble(s[0]) << 4) | ascii_to_nibble(s[1]);
}

// Convert two ASCII hex digits into one byte
static uint8_t hex_to_byte(const char *s) {
    return (hex_to_val(s[0]) << 4) | hex_to_val(s[1]);
}