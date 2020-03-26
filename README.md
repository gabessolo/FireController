# FireController
controleur de feu (Windows, Raspbian, FreeRTOS, ucos-2)
A mini server dialogs with lights on a TCP / IP network.
The server listens for lights on port 13374.
The fire dialogue protocol is as follows:
TCP / IP connection
subscription request
waiting for acknowledgment
listen in a loop for commands from the server
order fulfillment
