#include "header/zhelpers.h"
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

    /*Connection initialization to Benthernet Broker*/
    void *context = zmq_ctx_new ();
    void *pusher = zmq_socket (context, ZMQ_PUSH);
    void *sub = zmq_socket(context, ZMQ_SUB);
    time_t t;
    srand((unsigned) time(&t)); //Initialize unique identifier number (possibly link to IP adress for one connection per network)

    zmq_connect(pusher, "tcp://benternet.pxl-ea-ict.be:24041");
    zmq_connect(sub, "tcp://benternet.pxl-ea-ict.be:24042");

    char subTopic[] = "INTERFACE!>Ozi>";
    char pubTopic[] = "INTERFACE?>Ozi>";
    char buffer[256];
    char scanVar[256];

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, subTopic, strlen(buffer));

    //inject client connected message
    printf("CLIENT MESSAGE: Connecting to server...\r\n");
    strncpy(buffer, pubTopic, 22);
    itoa(rand(), scanVar, 8);
    strncat(buffer, scanVar, 250);
    s_send(pusher, buffer);

    while(1)
    {
        char buffer[256] = "";
        zmq_recv(sub, buffer, 255, 0);
        printf("%s\r\n", buffer);

        //zmq_send(pusher,)
        strncpy(buffer, pubTopic, 22);
        scanf("%s", scanVar);
        strncat(buffer, scanVar , 250);
        s_send(pusher, buffer);
    }
    return 0;
}
