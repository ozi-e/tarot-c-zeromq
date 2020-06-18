#include "header/zhelpers.h"
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int clientLog[128][2];

int main()
{
    void *context = zmq_ctx_new ();
    void *pusher = zmq_socket (context, ZMQ_PUSH);
    void *sub = zmq_socket(context, ZMQ_SUB);

    zmq_connect(pusher, "tcp://benternet.pxl-ea-ict.be:24041");
    zmq_connect(sub, "tcp://benternet.pxl-ea-ict.be:24042");

    char pushtask[] = "TAROT?>Ozi>";  //service send commando
    // char pushanswer[]= "TAROT>Ozi>";
    char pushanswer_h[]= "TAROT!>Ozi>"; //game push answer to service
    char subtask[] = "TAROT?>Ozi>";  //get sent command from game
    char subanswer[]= "TAROT!>Ozi>";   //Channel to get from
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, subtask, strlen(subtask));

    //Check identifier
    //skip for now

    int step = 0; //maybe state machine
    bool loop = 0;
    //bool cardsSelected = 0; //Cards are selected or not
    while(1)
    {
        //first start server then start client
        if (strncmp(s_recv(sub), "TAROT?>Ozi>ConnReq", strlen(pushtask) + 7*8) == 0)
        {
            s_send(pusher, "TAROT!>Ozi>ConnSucc");
            step = 1;
            loop = 1;
        }
        else
        {
            step = 0;
            loop = 0;
        }

        while(loop == 1)
        {
            //printf("step: %d", step);
            //printf("loop: %d", loop);
            switch (step)
            {
                case 1:
                    printf("Currently in card selection phase.\r\n");
                    s_send(pusher, "TAROT?>Ozi>SelCard");
                    step = 2;
                    sleep(1);
                    break;
                case 2:
                    printf("Waiting for cards...\r\n");
                    if (strncmp(s_recv(sub), "TAROT?>Ozi>ReqCards", strlen(pushtask) + 8*8) == 0)
                    {
                        s_send(pusher, "TAROT!>Ozi>ReqCardsGo");
                        step = 3;
                        s_send(pusher, "TAROT!>Ozi>Card>Test");
                    }
                    break;
                case 0: //exit loop for next connection
                    loop=0;
                    break;
            }

        }
    }
    return 0;
}
