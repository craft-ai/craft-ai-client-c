#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum dot_env_status {
    DOT_ENV_LOADED = 0,
    DOT_ENV_FILE_NOT_FOUND,
    DOT_ENV_ERROR
} dot_env_status_t;

static char read_char(char* line, size_t len, size_t* offset) {
    char c = '\0';
    if (*offset < len) {
        c = line[*offset];
    }
    ++(*offset);
    return c;
}

static void skip_spaces(char* line, size_t len, size_t* offset) {
    char c = '\0';
    while ((c = read_char(line, len, offset)) && isspace(c))
    {
      /* NOTHING */
    }
    --(*offset);
}

static dot_env_status_t dot_env_load() {
    FILE* fp;

    fp = fopen(".env", "r");
    if (fp == NULL) {
        return DOT_ENV_FILE_NOT_FOUND;
    }

    char* line = NULL;
    size_t lineSize = 0;
    size_t lineLen = 0;

    while ((lineLen = getline(&line, &lineSize, fp)) != -1) {
        char c = '\0';
        size_t offset = 0;
        char* key = 0;
        size_t keyLen = 0;
        char* value = 0;
        size_t valueLen = 0;

        /* Skip spaces */
        skip_spaces(line, lineLen, &offset);

        /* The key is everything before the next '=' */
        key = &line[offset];
        c = read_char(line, lineLen, &offset);

        /* It's maybe a comment */
        if (c == '#') {
            continue;
        }

        /* It's maybe an empty line */
        if (c == '\0') {
            continue;
        }

        while(!isspace(c) && c != '=' && c != '\0') {
            c = read_char(line, lineLen, &offset);
            ++keyLen;
        }

        /* Skip spaces */
        skip_spaces(line, lineLen, &offset);

        /* We should have a '=' here */
        if (c != '=') {
            fclose(fp);
            return DOT_ENV_ERROR;
        }

        /* Adding an inline string termination */
        key[keyLen] = '\0';

        /* Skip spaces */
        skip_spaces(line, lineLen, &offset);

        /* The value is everything until the end of line */
        value = &line[offset];
        /* Read until we reach the end */
        while((c = read_char(line, lineLen, &offset)) && !isspace(c) && c != '\0') {
            ++valueLen;
        }

        /* Adding an inline string termination */
        value[valueLen] = '\0';

        /* Setting the environment value */
        if (setenv(key, value, 1) != 0) {
            fclose(fp);
            return DOT_ENV_ERROR;
        }
    }

    fclose(fp);
    if (line) {
        free(line);
    }
    return DOT_ENV_LOADED;
}
