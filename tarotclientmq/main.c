#include "header/zhelpers.h"
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

    char subTopic[] = "TAROT!>Ozi>";
    char pubTopic[] = "TAROT?>Ozi>";
    char buffer[256];
    char *rqbuf = NULL;
    char *itoabuf = NULL;
    char *rcvbuf = "blablabla";

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, subTopic, strlen(buffer));

    //inject client connected message
    printf("CLIENT MESSAGE: Connecting to server...\r\n");

    bool connection = 0;
    int state = 0;
    bool cardsSelected = 0; //Cards are selected or not

    while (connection == 0)
    {
        s_send(pusher, "TAROT?>Ozi>ConnReq");
        if( strncmp( s_recv(sub), "TAROT!>Ozi>ConnSucc", strlen( subTopic) + (8*8)) == 0)
        {
            printf("CLIENT MESSAGE: Connection success!\r\n");
            connection = 1;
        }
        else
        {
            printf("CLIENT MESSAGE: Connection failure, retrying...\r\n");
            connection = 0;
        }
    }

    while(cardsSelected == 0)
    {
        if (strncmp( s_recv(sub), "TAROT?>Ozi>SelCard", strlen( subTopic) + (7*8)) == 0)
        {
            bool doneWhile = 0;
            int pickedFlags[22] = {0};
            int flagcounter = 0;
            int selFlag = 23;
            while(doneWhile == 0)
            {
                printf("Please select 6 cards, one by one. Example: 11 enter, 6 enter...\r\n");
                for(int i = 0; i < 22; i++)
                {
                    if (pickedFlags[i] > 0) //flagged round
                    {
                        printf("[]");
                    }
                    else
                    {
                        printf("  ");
                    }
                }
                printf("\r\n");
                for(int j = 0; j < 22; j++)
                {
                    if (pickedFlags[j] == 0) //unflagged round
                    {
                        printf("[]");
                    }
                    else
                    {
                        printf("  ");
                    }
                }
                printf("\r\n");

                selFlag = 23;
                scanf("%i", &selFlag);
                selFlag--;
                if (selFlag < 23 && selFlag >= 0)
                {
                    if (pickedFlags[selFlag] == 0)
                    {
                        pickedFlags[selFlag] = 1;
                        flagcounter++;
                    }
                    else
                    {
                        pickedFlags[selFlag] = 0;
                        flagcounter--;
                    }
                    printf("Thanks!\r\n");
                }
                else
                {
                    printf("Oop! Card index is invalid, please select a valid card numbering between 1 and 22\r\n");
                }

                if (flagcounter >= 6)
                {
                    doneWhile = 1;
                    cardsSelected = 1;
                }

                /*int selFlag = 23;
                bool doneSelecting = 0;
                while (doneSelecting == 0)
                {
                    scanf("Card: %i", &selFlag);
                }*/
            }

        }


    }

    if ( cardsSelected )
    {
        cardsSelected = 0; //Reset cards selected to restart game
        int selectotherrandomflag = 0;
        int randomCards[6] = { 50 };
        //Because tarots are complete utter bullcrap we pull out 6 randoms and stick them in an array to show LOL
        printf("You drew... ");
        for (int k = 0; k < 6;) //dont increment k
        {
            srand(time(0));
            int num = (rand() % (21 - 0 + 1)) + 0;

            for (int l = 0; l < k; l++)
            {
                if (randomCards[l] == num)
                {
                    selectotherrandomflag = 1;
                }
            }

            if (selectotherrandomflag)
            {
                selectotherrandomflag = 0;
            }
            else
            {
                randomCards[k] = num;
                printf("%d ", randomCards[k]);
                k++;
            }
        }
        printf("\r\nNow reading your cards...\r\n");

        s_send(pusher, "TAROT?>Ozi>ReqCards");
        //s_send(pusher, "sending...");
        /*while (strncmp( s_recv(sub), "TAROT!>Ozi>ReqCardsGo", strlen( subTopic) + 10) != 0) //Wait til confirmation
        {

        }*/
        for (int m = 0; m < 6; m++)
        {
            //s_send(pusher, "sending...");
            //One by one request cards from server
            //rqbuf = "TAROT?>Ozi>Card>";
            char buf[128];
            sprintf(buf, "TAROT?>Ozi>Card>%d", randomCards[m]);
            //itoa(3, itoabuf, 10); //randomCards[m]
            //strncat(rqbuf, "10", strlen(itoabuf));
            s_send(pusher, buf);
            printf("%s\r\n", buf);
            bool whileflag = 0;
            while (whileflag) //Wait til confirmation
            {
                char recbf[512];

                sprintf(recbf, "%s", s_recv(sub));  //Some trickery
                if( strncmp( recbf, "TAROT!>Ozi>Card>", strlen( subTopic) + 5*8) == 0 )
                {
                    whileflag = 0;
                }
                //s_send(pusher, "sending...");
            }
           // s_send(pusher, "sending...");
            //printf("%s card\r\n", buf);

        }
    }


    /*strncpy(buffer, pubTopic, 22);
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
    }*/
    return 0;
}
