#include "ini.h"
#include <cstdio>
#include <cstring>
#include <cctype>

// 최대 라인 길이
#define MAX_LINE 200

// 오른쪽 공백 제거
static char* rstrip(char* s) {
    char* p = s + strlen(s);
    while (p > s && isspace(static_cast<unsigned char>(*(p - 1)))) --p;
    *p = '\0';
    return s;
}

// 왼쪽 공백 건너뛰기
static char* lskip(const char* s) {
    while (*s && isspace(static_cast<unsigned char>(*s))) s++;
    return (char*)s;
}

// '=' 문자 또는 주석 시작 위치 찾기
static char* find_char_or_comment(const char* s, char c) {
    int was_space = 0;
    while (*s && *s != c && !(was_space && (*s == ';' || *s == '#'))) {
        was_space = isspace(static_cast<unsigned char>(*s));
        s++;
    }
    return (char*)s;
}

// 안전한 strncpy (널 종료 보장)
static char* strncpy0(char* dest, const char* src, size_t size) {
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

// ini_parse 구현
int ini_parse(const char* filename, ini_handler handler, void* user) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    char line[MAX_LINE];
    char section[MAX_LINE] = "";
    char prev_name[MAX_LINE] = "";

    int lineno = 0;
    int error = 0;

    while (fgets(line, sizeof(line), file)) {
        lineno++;
        char* start = lskip(rstrip(line));

        if (*start == ';' || *start == '#' || *start == '\0') {
            continue; // 주석 또는 빈 줄
        } else if (*start == '[') {
            // 섹션
            char* end = strchr(start + 1, ']');
            if (!end) {
                error = lineno;
                break;
            }
            *end = '\0';
            strncpy0(section, start + 1, sizeof(section));
            *prev_name = '\0';
        } else {
            // 키=값
            char* equal = find_char_or_comment(start, '=');
            if (*equal != '=') {
                error = lineno;
                break;
            }

            *equal = '\0';
            char* name = rstrip(start);
            char* value = lskip(equal + 1);

            // 주석 제거
            char* comment = find_char_or_comment(value, '\0');
            if (*comment == ';' || *comment == '#') *comment = '\0';
            rstrip(value);

            strncpy0(prev_name, name, sizeof(prev_name));

            if (!handler(user, section, name, value)) {
                error = lineno;
                break;
            }
        }
    }

    fclose(file);
    return error;
}
