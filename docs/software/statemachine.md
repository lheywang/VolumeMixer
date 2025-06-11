# State Machine

## Steps

The whole device and host are based on a state machine, to track the different steps of connection.
There's different states :

- BOARD_INIT
- WAIT_FOR_CONNECT
- WAIT_FOR_COMMAND
- WAIT_FOR_RESPONSE

Theses firsts steps :

### BOARD_INIT

In this steps, triggered by electrical reset, or RESET command, the board reset itself to it's default state.
In this mode, the both the device and host aren't open to commands.

### WAIT_FOR_CONNECT

In this second step, the link between host and device is established. This is done by the host to open all of the COM ports on the device, and reading on it for 500 ms.
Meanwhile the device send a specific string such as : "START;CONNC;021;{"state":"available"};49934095;END" every 250 ms.

Once such a message is received by the host, the connection request is sent, and the device acknoweledge.
Both the host and device then go on a private state where they're connection is reserved.

### WAIT_FOR_COMMAND

For this third step, the device is waiting for a command. This doesn't mean it does nothing, it's only waiting for a command to arrive.
In background, the position of sliders are sampled regularly to ensure a quick response to command.

Thats the task of the host to request the values rather than filling input buffers with junk.

### WAIT_FOR_RESPONSE

This is the last step, it's directly followed by a command write. It does not last a lot.
