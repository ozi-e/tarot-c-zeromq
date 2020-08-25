

## Tarot description program in C.


*Gives description based on cards drawn, deck shuffles dynamically.*

Server-client based Tarot game.

### How to run:

 - Launch the 0MQ broker at "tarot-c-zeromq\broker\windist_localhost" to run locally.

- Edit both client and server programs "tarotclientmq" and "tarotservermq" to connect to the right IP adress. This can be left default if decided to run locally with the localhost broker.

- If building the program is not possible, compile a QT library for 0MQ using https://github.com/Bart-PXL/libzmq/tree/qmake first.

- Launch the server at "tarot-c-zeromq\build-tarotservermq-Desktop_Qt_5_14_2_MinGW_64_bit-Debug" using PowerShell or CMD.

- Launch the client at "tarot-c-zeromq\build-tarotclientmq-Desktop_Qt_5_14_2_MinGW_64_bit-Debug".

### How to play:
The main goal of this game is to give general description of a card.
To play, select 6 cards by typing the corresponding number and pressing enter for each card.
The cards will appear on the command prompt with minimal ascii, so you know what card you chose already.
The cards can be deselected by typing the right number again.
When 6 cards have been selected the program will return the descriptions for those cards.
Source:
 - Deck from http://www.m31.de/colman-smith/index.html

