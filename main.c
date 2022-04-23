#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

struct Timestamp {
    int hour;
    int minute;
    int second;
    int millisecond;
};

int is_time_mark(char *str);

void rewrite_time_mark(char *time_mark, int shift_ms);

void shift_timestamp(struct Timestamp *timestamp, int shift_ms);

int main() {
    char *srt_file = "./shrek_2.srt";
    char *out_file = "./shrek_2_out.srt";
    int shift_ms = 54321;

    FILE *fp_in = fopen(srt_file, "r");
    FILE *fp_out = fopen(out_file, "w");

    char line[BUFFER_SIZE];
    printf("Reading file %s\n", srt_file);
    while (fgets(line, BUFFER_SIZE, fp_in)) {
        if (is_time_mark(line)) {
            rewrite_time_mark(line, shift_ms);
        }
        fputs(line, fp_out);
    }
    printf("Written file %s\n", out_file);

    fclose(fp_in);
    fclose(fp_out);
    return 0;
}

int is_time_mark(char *str) {
    size_t len = strlen(str);
    if (len < 29) {
        return FALSE;
    }
    for (int i = 0; i < len; i++) {
        if (isalpha(str[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

void rewrite_time_mark(char *time_mark, int shift_ms) {
    struct Timestamp left;
    struct Timestamp right;

    sscanf(time_mark, "%d:%d:%d,%d --> %d:%d:%d,%d", &left.hour, &left.minute, &left.second, &left.millisecond,
           &right.hour, &right.minute, &right.second, &right.millisecond);

    left.millisecond *= 10;
    right.millisecond *= 10;
    shift_timestamp(&left, shift_ms);
    shift_timestamp(&right, shift_ms);
    left.millisecond /= 10;
    right.millisecond /= 10;

    sprintf(time_mark, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", left.hour, left.minute, left.second,
            left.millisecond, right.hour, right.minute, right.second, right.millisecond);
}

void shift_timestamp(struct Timestamp *timestamp, int shift_ms) {
    timestamp->millisecond += shift_ms;
    timestamp->second += timestamp->millisecond / 1000;
    timestamp->millisecond %= 1000;
    timestamp->minute += timestamp->second / 60;
    timestamp->second %= 60;
    timestamp->hour += timestamp->minute / 60;
    timestamp->minute %= 60;
}
