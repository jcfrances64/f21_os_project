#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "report_record_formats.h"
#include "queue_ids.h"

int main(void) {


    FILE *f = fopen("records.mini", "r");

    char str[RECORD_FIELD_LENGTH];

    fgets(str, RECORD_FIELD_LENGTH, f);
    printf("%s", str);

    // while(strlen(str) != 0 || index > 20) {
    //     printf("%i:     %s", index, str);
    //     index++;
    //     fgets(str, RECORD_FIELD_LENGTH, f);

    // }
    int index = 0;

    while(strcmp(str, "\n") != 0) {
        if(strstr(str, "H Peck")) {
            printf("**MATCH**\t%s", str);
        } else {
            printf("%s", str);
        }
        fgets(str, RECORD_FIELD_LENGTH, f);
        index++;
        if(index > 20) {
            break;
        }
    }

    // int i;

    // for(i = 0; i < 20; i++) {
    //     if(strstr(str, "H Peck")) {
    //         printf("**MATCH**\t%s", str);
    //     } else {
    //         printf("**%s**", str);
    //         printf("%i", strlen(str));
    //     }
    //     fgets(str, RECORD_FIELD_LENGTH, f);

    // }



}