#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "report_record_formats.h"
#include "queue_ids.h"

int main(int argc, char**argv)
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    report_request_buf rbuf;
    report_record_buf sbuf;
    size_t buf_length;


    key = ftok(FILE_IN_HOME_DIR,QUEUE_NUMBER);
    if (key == 0xffffffff) {
        fprintf(stderr,"Key cannot be 0xffffffff..fix queue_ids.h to link to existing file\n");
        return 1;
    }
    msqid = msgget(key, msgflg);
    int ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);
    
    char searchString[SEARCH_STRING_FIELD_LENGTH];
    strcpy(searchString, rbuf.search_string);
    


    int i;
    int threadCount = rbuf.report_count;
    // printf("Thread Count: %i", threadCount);

    
    for(i = 0; i < threadCount; i++) {

        int index = rbuf.report_idx;
        printf("index: %i\n", index);

        key = ftok(FILE_IN_HOME_DIR, index);
        msqid = msgget(key, msgflg);


        FILE *f = fopen("records.mini", "r");
        char str[RECORD_FIELD_LENGTH];
        fgets(str, RECORD_FIELD_LENGTH, f);


        sbuf.mtype = 2;

        while(strcmp(str, "\n") != 0) {
            if(strstr(str, searchString)) {
                strcpy(sbuf.record, str);
                printf("Entered if\n");
                buf_length = strlen(sbuf.record) + sizeof(int) + 1;
                msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT); //implement thread to send this message
            }
            fgets(str, RECORD_FIELD_LENGTH, f);

        }

        strcpy(sbuf.record, "");
        printf("Empty String\n");
        buf_length = strlen(sbuf.record) + sizeof(int) + 1;
        msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);

        if(i == (threadCount - 1)) {
            break;
        }


        key = ftok(FILE_IN_HOME_DIR,QUEUE_NUMBER);
        msqid = msgget(key, msgflg);

        int ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);
        strcpy(searchString, rbuf.search_string);

        int j;
        int k;
        for(j = 0; j < 1000; j++) {
            for(k = 0; k < 1000; k++) {

            }
        }

    }
    


    // while(strcmp(str, "\n") != 0) {
    //     if(strstr(str, searchString)) {
    //         strcpy(sbuf.record, str);
    //         printf("Entered if\n");
    //         buf_length = strlen(sbuf.record) + sizeof(int) + 1;
    //         msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    //         // printf("**MATCH**\t%s", str);
    //     }
    //     fgets(str, RECORD_FIELD_LENGTH, f);
    // }
    // strcpy(sbuf.record, "");
    // buf_length = strlen(sbuf.record) + sizeof(int) + 1;
    // msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);



    // printf("%s", str);

    // sbuf.mtype = 2;

    // strcpy(sbuf.record, str);
    // buf_length = strlen(sbuf.record) + sizeof(int) + 1;
    // msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    // while (1) {
        
    //     char temp[RECORD_FIELD_LENGTH];
    //     fgets(temp, RECORD_FIELD_LENGTH, f);
    //     // if(te)


    //     if(strstr(searchString, temp) != NULL) {
    //         printf("true\n");
    //         strcpy(sbuf.record, str);
    //         buf_length = strlen(sbuf.record) + sizeof(int) + 1;
    //         msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    //         break;
    //     }
    //     printf("1");

    // } 



 // // We'll send message type 2
    // sbuf.mtype = 2;
    // strcpy(sbuf.record,"09/22/21  Windshield   Repair  H Peck      JJDoe   $400  1.5   $60    $460  "); 
    // buf_length = strlen(sbuf.record) + sizeof(int)+1;//struct size without
    // // Send a message.
    // if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr,"%d, %ld, %s %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
    //     perror("(msgsnd)");
    //     fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
    //     exit(1);
    // }
    // else
    //     fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);





    // int i;
    // int ret;
    // for(i = 0; i < 2; i++) {
    //     msqid = msgget(key, msgflg);
    //     ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);
    //     // System.out.println(msqid);
    //     fprintf(stderr, "Msqid = %d\tmsgrcv = %i\t", msqid, ret);
    //     fprintf(stderr, "Type: %ld\tSearch String: %s\n", rbuf.mtype, rbuf.search_string);
    //     // fprintf(stderr, "Msg = ")
    // }









    // if ((msqid = msgget(key, msgflg)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr, "Value of errno: %d\n", errno);
    //     perror("(msgget)");
    //     fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
    // }
    // else
    //     fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);

    // // msgrcv to receive report request
    // int ret;
    // do {
    //     ret = msgrcv(msqid, &rbuf, sizeof(rbuf), 1, 0);//receive type 1 message

    //     int errnum = errno;
    //     if (ret < 0 && errno !=EINTR){
    //         fprintf(stderr, "Value of errno: %d\n", errno);
    //         perror("Error printed by perror");
    //         fprintf(stderr, "Error receiving msg: %s\n", strerror( errnum ));
    //     }
    // } while ((ret < 0 ) && (errno == 4));
    // //fprintf(stderr,"msgrcv error return code --%d:$d--",ret,errno);

    // fprintf(stderr,"process-msgrcv-request: msg type-%ld, Record %d of %d: %s ret/bytes rcv'd=%d\n", rbuf.mtype, rbuf.report_idx,rbuf.report_count,rbuf.search_string, ret);

    // key = ftok(FILE_IN_HOME_DIR,1);
    // if (key == 0xffffffff) {
    //     fprintf(stderr,"Key cannot be 0xffffffff..fix queue_ids.h to link to existing file\n");
    //     return 1;
    // }


    // if ((msqid = msgget(key, msgflg)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr, "Value of errno: %d\n", errno);
    //     perror("(msgget)");
    //     fprintf(stderr, "Error msgget: %s\n", strerror( errnum ));
    // }
    // else
    //     fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);

    // // We'll send message type 2
    // sbuf.mtype = 2;
    // strcpy(sbuf.record,"09/22/21  Windshield   Repair  H Peck      JJDoe   $400  1.5   $60    $460  "); 
    // buf_length = strlen(sbuf.record) + sizeof(int)+1;//struct size without
    // // Send a message.
    // if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr,"%d, %ld, %s %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
    //     perror("(msgsnd)");
    //     fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
    //     exit(1);
    // }
    // else
    //     fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);

    // // We'll send message type 2
    // sbuf.mtype = 2;
    // strcpy(sbuf.record,"Repair P123-23 $24.95 $175 12345698452");
    // buf_length = strlen(sbuf.record) + sizeof(int)+1;//struct size without
    // // Send a message.
    // if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr,"%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
    //     perror("(msgsnd)");
    //     fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
    //     exit(1);
    // }
    // else
    //     fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);

    // // We'll send message type 2
    // sbuf.mtype = 2;
    // sbuf.record[0]=0;
    // buf_length = strlen(sbuf.record) + sizeof(int)+1;//struct size without
    // // Send a message.
    // if((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0) {
    //     int errnum = errno;
    //     fprintf(stderr,"%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.record, (int)buf_length);
    //     perror("(msgsnd)");
    //     fprintf(stderr, "Error sending msg: %s\n", strerror( errnum ));
    //     exit(1);
    // }
    // else
    //     fprintf(stderr,"msgsnd-report_record: record\"%s\" Sent (%d bytes)\n", sbuf.record,(int)buf_length);


    exit(0);
}
