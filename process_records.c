#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "report_record_formats.h"
#include "queue_ids.h"
#include <signal.h>
#include <pthread.h>


pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct countStructure {
    int threadID;
    int recordCount;
    char searchString[SEARCH_STRING_FIELD_LENGTH];
} countStruct;

static void signalHandler(int sig) {
    pthread_cond_signal(&cv);
    // printf("Called sigint!\n");
    signal(SIGINT, signalHandler);

}

void *printStatusReport(void *reportStatusStruct) {
    
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cv, &mutex);

    printf("***Report***\n");




    pthread_mutex_unlock(&mutex);


}

// void printStatusReport(countStruct *reportCountStruct, int recordsRead, int threadCount) {
//     printf("***Report***\n");
//     printf("%i records read for %i reports\n", recordsRead, threadCount);
//     int i;
//     for(i = 0; i < threadCount; i++) {
//         printf("Records sent for report index %i: %i\n", reportCountStruct[i].threadID, reportCountStruct[i].recordCount);
//     }
// }







int main(int argc, char**argv) {
    int msqid;
    int ret;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    report_request_buf rbuf;
    report_record_buf sbuf;
    size_t buf_length;
    countStruct * reportCountStruct;
    int rc;
    
    



    int threadCount = 0;
    int recordsRead = 0;


    

    int threadIndex;
    // int threadCount = 0;
    int currentThread;
    // bool threadCountInitialized;
    // char searchString[SEARCH_STRING_FIELD_LENGTH];

    // int structIndex = 0;

    //  retrieve report requests, save into countstruct
    do {
        key = ftok(FILE_IN_HOME_DIR,QUEUE_NUMBER);
        if (key == 0xffffffff) {
            fprintf(stderr,"Key cannot be 0xffffffff..fix queue_ids.h to link to existing file\n");
            return 1;
        }
        //  gets message 
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
            threadIndex = threadCount;
            reportCountStruct = malloc(threadCount * sizeof(countStruct));
            int i;
            for(i = 0; i < threadCount; i++) {
                reportCountStruct[i].recordCount = 0;
                reportCountStruct[i].threadID = 0;
                strcpy(reportCountStruct[i].searchString, "");
            }
        } else {    // handle possible error where threadCount isnt -1??
            fprintf(stderr, "Error initializing count structure\n");
        }



        currentThread = rbuf.report_idx;
        // printf("Thread %i\n" + currentThread);

        reportCountStruct[currentThread - 1].threadID = rbuf.report_idx;
        strcpy(reportCountStruct[currentThread - 1].searchString, rbuf.search_string);
        // reportCountStruct;
        // structIndex++;

        // threadIndex = rbuf.report_idx;
        threadIndex--;


    } while(threadIndex > 0);


  


        // printf("Before thread initialization\n");
        // pthread_mutex_unlock(&mutex);
        // start status report thread
        pthread_t statusReportThread;
        signal(SIGINT, signalHandler);
        pthread_create(&statusReportThread, NULL, (void *) printStatusReport, NULL);
        // printf("After thread initialization\n");


        //   delete later



        // if ((msqid = msgget(key, msgflg)) < 0) {
        //     int errnum = errno;
        //     fprintf(stderr, "Value of errno: %d\n", errno);
        //     perror("(msgget)");
        //     fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
        // }
        // else
        //     fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);


        // do {
        //     ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);    //   receive type 1 message

        //     int errnum = errno;
        //     if (ret < 0 && errno !=EINTR){
        //         fprintf(stderr, "Value of errno: %d\n", errno);
        //         perror("Error printed by perror");
        //         fprintf(stderr, "Error receiving msg: %s\n", strerror( errnum ));
        //     }
        // } while ((ret < 0 ) && (errno == 4));

    //delete



    

    //  retrieve records

    FILE *recordFile = fopen("records.mini", "r");   //  change to be the value from stdin, was f
    sbuf.mtype = 2; //  used to exist in do while
    char record[RECORD_FIELD_LENGTH]; // was str

    do {
        fgets(record, RECORD_FIELD_LENGTH, recordFile);
        // if(strstr(record, searchString) != 0) {
        
        int i;
        for(i = 0; i < threadCount; i++) {
            if(strstr(record, reportCountStruct[i].searchString) != 0) {

                // printf("Record Matched on thread: %i %s\n", reportCountStruct[i].threadID, record);

                key = ftok(FILE_IN_HOME_DIR, reportCountStruct[i].threadID);
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

                strcpy(sbuf.record, record);
                buf_length = strlen(sbuf.record) + sizeof(int) + 1;
                // sbuf.mtype = 2;
                //  send message
                //or add lock here

                pthread_mutex_lock(&mutex);
                reportCountStruct[i].recordCount++;
                // recordsRead++;
                if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
                    int errnum = errno;
                    fprintf(stderr,"%d, %ld, %s %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
                    perror("(msgsnd)");
                    fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
                    exit(1);
                }
                else
                    fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);


                pthread_mutex_unlock(&mutex);
                //remove lock
                    // printf("Sent %s to thread %i\n", record, threadIndex);
            }
        }

        if(strcmp(record, "\n") != 0) {
            pthread_mutex_lock(&mutex);
            recordsRead++;
            pthread_mutex_unlock(&mutex);
        }

    } while(strcmp(record, "\n") != 0);

    int i;
    for(i = 0; i < threadCount; i++) {

        key = ftok(FILE_IN_HOME_DIR, reportCountStruct[i].threadID);
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

    }

    // printf("Thread count %s\n", threadCount);
    // printf("Records read: %s\n", recordsRead);
    // printStatusReport(reportCountStruct, recordsRead, threadCount);
    


 

    exit(0);
}
