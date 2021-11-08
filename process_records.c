#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "report_record_formats.h"
#include "queue_ids.h"
#include <pthread.h>

int main(int argc, char**argv)
{
    int msqid;
    int ret;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    report_request_buf rbuf;
    report_record_buf sbuf;
    size_t buf_length;

    pthread_t statusReportThread;


    

    int threadIndex;
    int threadCount = 0;
    char searchString[SEARCH_STRING_FIELD_LENGTH];


    do {

        key = ftok(FILE_IN_HOME_DIR,QUEUE_NUMBER);
        if (key == 0xffffffff) {
            fprintf(stderr,"Key cannot be 0xffffffff..fix queue_ids.h to link to existing file\n");
            return 1;
        }
        //  start of refactored code
        if ((msqid = msgget(key, msgflg)) < 0) {
            int errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("(msgget)");
            fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
        }
        else
            fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);


        do {
            ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);    //   receive type 1 message

            int errnum = errno;
            if (ret < 0 && errno !=EINTR){
                fprintf(stderr, "Value of errno: %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr, "Error receiving msg: %s\n", strerror( errnum ));
            }
        } while ((ret < 0 ) && (errno == 4));

        if(threadCount == 0) {
            threadCount = rbuf.report_count;
        }

        threadIndex = rbuf.report_idx;

        strcpy(searchString, rbuf.search_string);

        key = ftok(FILE_IN_HOME_DIR, threadIndex);
        if (key == 0xffffffff) {
            fprintf(stderr,"Key cannot be 0xffffffff..fix queue_ids.h to link to existing file\n");
            return 1;
        }

        if ((msqid = msgget(key, msgflg)) < 0) {
            int errnum = errno;
            fprintf(stderr, "Value of errno: %d\n", errno);
            perror("(msgget)");
            fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
        }
        else
            fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);




        FILE *recordFile = fopen("records.mini", "r");   //  change to be the value from stdin, was f
        sbuf.mtype = 2; //  used to exist in do while
        char record[RECORD_FIELD_LENGTH]; // was str

        do {
            fgets(record, RECORD_FIELD_LENGTH, recordFile);
            if(strstr(record, searchString) != 0) {
                strcpy(sbuf.record, record);
                // printf("Entered if for thread%i\n", threadCount);
                buf_length = strlen(sbuf.record) + sizeof(int) + 1;
                // sbuf.mtype = 2;
                //  send message
                if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
                    int errnum = errno;
                    fprintf(stderr,"%d, %ld, %s %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
                    perror("(msgsnd)");
                    fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
                    exit(1);
                }
                else
                    fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);
                    // printf("Sent %s to thread %i\n", record, threadIndex);
            }
        } while(strcmp(record, "\n") != 0);

        
        sbuf.mtype = 2;
        sbuf.record[0]=0;
        // strcpy(sbuf.record, "");
        buf_length = strlen(sbuf.record) + sizeof(int)+1;//struct size without
        // Send a message.
        if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
            int errnum = errno;
            fprintf(stderr,"%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
            perror("(msgsnd)");
            fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
            exit(1);
        }
        else
            fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);

        threadCount--;


    } while(threadCount > 0);

    exit(0);
}
