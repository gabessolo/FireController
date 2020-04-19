# FireController
controleur de feu (Windows, Raspbian, FreeRTOS, ucos-2)
A mini server dialogs with lights on a TCP / IP network.
The server listens for lights on port 50.
The fire dialogue protocol is as follows:
1)TCP / IP connection
2)subscription request
3)waiting for acknowledgment
4)listen in a loop for commands from the server
5)order fulfillment

4 fire are threads the 5th  is a java applet 
the java fire listen on port 88
the structure of the command is define into common.h file and also in FireControl.h file

The Fire controler is able to control unlimited number of file
the fire just have to follow the protocol and communicate on TCP IP
