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
        char rcarray[512];
        sprintf(rcarray, "%s", s_recv(sub));
        //first start server then start client
        if (strncmp(rcarray, "TAROT?>Ozi>ConnReq", strlen(pushtask) + 7) == 0)
        {
            s_send(pusher, "TAROT!>Ozi>ConnSucc");
            step = 1;
            loop = 1;
        }
        else
        {
            //step = 0;
            loop = 0;
        }

        if (strncmp( rcarray, "TAROT?>Ozi>ReqCards", strlen( subtask) + 8) == 0)
        {
            printf("Received a card request! Sending...\r\n");
            s_send(pusher, "TAROT!>Ozi>ReqCardsGo");
        }

        if (strncmp( rcarray, "TAROT?>Ozi>Card", strlen( subtask) + 4) == 0)
        {
            printf("Sending a card...\r\n");
            char *rcvstr = strrchr(rcarray, '>');
            memmove(rcvstr, rcvstr + 1, strlen(rcvstr));
            printf("%s\r\n", rcvstr);
            int caseconvert = atoi(rcvstr);
            char *sendString = "";
            switch (caseconvert)
            {
                case 0:
                    sendString = "Arcana: The Fool, The roots of one's abilities, talents and potential. Reverse: Foolish amidst excitement and fear, blind.";
                    break;
                case 1:
                    sendString = "Arcana: The Magician, A trickster, guide of souls into the underworld, earthly passions and spiritual clarity are part of his nature. Reverse: Negative: tricky, treacherous, leads us often to dangerous and frustrating places, to follow the inner guide does not always mean making the choices which are secure and guarantee results.";
                    break;
                case 2:
                    sendString = "Arcana: The High Priestess, The queen of the underworld, that part of us which knows the secrets of the inner world, glimpses through dreams, fantasies and intuition. Reverse: Platonic love, encounter of some kind with the hidden inner world, confusion.";
                    break;
                case 3:
                    sendString = "Arcana: The Empress, A matriarchal goddess, a beautiful earthy woman - experience of being part of nature and rooted in natural life, the appreciation of the senses and the simple pleasures of daily existence. Reverse: Stagnation of the spirit, apathy, mourning mother, which crush all possibility of change, full of bitterness and resentment because life requires change and separation and endings must occur.";
                    break;
                case 4:
                    sendString = "Arcana: The Emperor, Father who embodies our spiritual ideals, our ethical codes, a god of inspiration and sudden creative vision, a revelation of the truth. Reverse: Rigidity, implacable self-righteousness, inflexibility, arrogance, tyrants, dogma against human feelings.";
                    break;
                case 5:
                    sendString = "Arcana: The Hierophant, A priest or a teacher, a 'maker of bridges' who represents a wounded part of ourselves and therefore he is open to the worlds pain and longing. Reverse: Mistrust, knowledge out of books instead out of life, hypocritical, credulous.";
                    break;
                case 6:
                    sendString = "Arcana: The Lovers, Meeting between the cutting power of mind - the sword - and the cup of love, which is a symbol of relationship. Reverse: Driven blindly by desires, doubtful.";
                    break;
                case 7:
                    sendString = "Arcana: The Chariot, A new beginning with ambition, will, aggression and competitive instinct, with enthusiasm and vitality moves beyond boredom and daily routine. Reverse: Lust for destruction, brute strength and lack of refinement.";
                    break;
                case 8:
                    sendString = "Arcana: Strength, Associated with the lion we have an image of courage, strength and self-discipline of totally egocentric beginnings of a unique individuality. Reverse: Repressed instinct without any transformation, leaving behind a strong shell within a soul without passion and without a true identity.";
                    break;
                case 9:
                    sendString = "Arcana: The Hermit, Time for meditation, for looking inward bringing light into darkness, the unconscious, watching the dreams. Reverse: Estrangement, abandonment.";
                    break;
                case 10:
                    sendString = "Arcana: Wheel of Fortune, Here comes a big breakthrough, the unexpected luck - a new self-realization, the personal destiny. Reverse: Being out of control, burden of fate.";
                   break;
                case 11:
                    sendString = "Arcana: Justice, The weapons - which have to be used with intelligence, foresight and planning. Here we have the mind capacity to integrate both dark and light with logic, diplomacy and cleverness. Reverse: Cool, too idealistic and too destructive to the warmth of personal relationship.";
                    break;
                case 12:
                    sendString = "Arcana: The Hanged Man, The reversal of a situation brings powerlessness, doubt, reunification, brings stillness and reflection. Reverse: Suffering in order to manipulate others, staying passive about a situation from which one could extricate oneself quite easily.";
                    break;
                case 13:
                    sendString = "Arcana: Death, Image of the permanent and final end of a cycle of life. Reverse: Depression, because there has not yet been an acknowledgment that a phase of life has died at the same time that a new thing has been born.";
                    break;
                case 14:
                    sendString = "Arcana: Temperance, A quantum leap, a bridge between heaven and earth inviting the polar opposites, the creative forces of masculine and feminine, conscious and unconscious, the sun and the moon. Reverse: Too much harmony oriented, where no change is allowed to occur and the mind suffocates from sheer boredom.";
                    break;
                case 15:
                    sendString = "Arcana: The Devil, The card of the devil implies the necessity of a confrontation with all that is shadowy, shameful and base in the personality like sexuality, antagonism, obscure unrestrained energy, obsession, superstition. Reverse: Careless of result.";
                    break;
                case 16:
                    sendString = "Arcana: The Tower, This is a powerful instinctual force emerging from the unconscious, stronger than the will's efforts to repress it, collapse of old forms, inner and outer structure, of false or outgrown values, masks, costumes in a play to impress the audience, structures we build in the outside world to embody our incomplete selves. Reverse: Without looking inward it can be a stagnant state.";
                    break;
                case 17:
                    sendString = "Arcana: The Star, The star, the guiding vision of hope arises out of the ashes of the tower which has been destroyed. Here comes the harmonious uniting of heaven and earth, the divine protection and power to transcend the mundane world. Reverse: Blind hope.";
                    break;
                case 18:
                    sendString = "Arcana: The Moon, There is a final trial, a threshold of the new level of awareness - illusion - examination of the subconscious. The moon is seen full only when it is in exact opposition to the sun. Reverse: Resistant activity and mind-energy.";
                    break;
                case 19:
                    sendString = "Arcana: The Sun, Here the sun comes with vital energy, health, generosity, clarity, self-confidence, success and material wealth. Reverse: The burning heat of the sun can destroy, for one who does not respect nature's law.";
                    break;
                case 20:
                    sendString = "Arcana: Judgment, Time when the rewards of past efforts appear. Reverse: Self-made-prison.";
                    break;
                case 21:
                    sendString = "Arcana: The World, The great cycle of the Major Arcana ends where it begins, the unborn potential of the personality ultimately leads to the birth of freedom, open to all possibilities, crazy enough to start again. Reverse: N/A";
                    break;
            }
            char sendarr[1024];
            sprintf(sendarr, "TAROT!>Ozi>Card>%s", sendString);
            s_send(pusher, sendarr);

        }
        //while(loop)
        //{
            switch (step)
            {
                case 1: //This is actually redundant af, totally should remove or alter it, connreq and connsucc do the same exact thing
                    printf("Currently in card selection phase.\r\n");
                    s_send(pusher, "TAROT?>Ozi>SelCard");
                    step = 2;
                    //sleep(1);
                    break;
                case 2:
                    /*printf("Waiting for cards...\r\n");
                    char rstring[64];
                    sprintf(rstring, "%s", s_recv(sub));
                    if (strncmp( rstring, "TAROT?>Ozi>ReqCards", strlen( subtask) + (8*8)) == 0)
                    {
                        s_send(pusher, "TAROT!>Ozi>ReqCardsGo");
                        step = 3;

                    }*/
                    break;
                case 3:
                    //s_send(pusher, "TAROT!>Ozi>Card>Test"); //Can start sending card information here
                case 0: //exit loop for next connection
                    loop=0;
                    break;
            }
        //}

    }
    return 0;
}
