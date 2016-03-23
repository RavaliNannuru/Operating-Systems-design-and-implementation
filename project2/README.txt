Project2 CS-551 IPC System call implementation

Team Members
Balaji A R (A20347964)
Paulo Vitor Becker Ayrosa Monterio de Andrade (A20341532)
Sai Ravali Nannuru (A20354346)

The folder contains the following files:
1. misc.c : code for the Message Queue implementation and its supported system calls.
2. table.c : contains table of all the system calls to PM Server.
3. proto.h : Header file containing the declaration of the system call and the function implementation 
4. callnr.h : contains the list of system calls and a number associated to them
5. sender.c: An user process that can send messages to multiple receivers. this process also executes mq_open() to create a queue.
6. receiver.c : An user process than can receive their messages from the specifc queue.
7. mq_close.c: A user process to close the queue.
8. mq_getattr.c : User process to get details about the queue.
9. mq_setattr.c : user process to set details about the queue. those details could be no of messages, whether system call is blocked/non blocking nature.
10. mqueue.h: header file containing mapping for the system call.
11. design_documet : A design document for our IPC system calls
12. receiver_notification.c: user process which waits to receive the notification.
13. mq_reqnotify.c: user process to register a receiver for notification.

For detailed information about the implementation, Please refer to the Design Document. 
Steps to build:
The folder contains the user programs that use the implemented IPC System calls. If you want to make changes and rebuild
again, use following commands:
1. make mq_close
2. make mq_getattr
3. make mq_setattr
4. make mq_reqnotify
5. make sender
6. make receiver
7. make receiver_notification

Steps to test:

	1. Steps to send message:
	Syntax:
	> /sender <queuename> <message> <list of receivers>...
	Sample:
	> ./sender q0 message0 1

	2. Steps to receive the message
	Syntax:
	>./receiver <queuename> <receiver_lis>
	Eg. 
	> ./receiver 1
	> Message recieved is Message1 
	
	3. Steps to close a queue
	Syntax:
	> ./mq_close <queuename>
	Sample:
	> ./mq_close q0
	
	4. Steps to get details about a queue
	Syntax:
	> ./mq_getattr <queuename>
	Sample
	> ./mq_getattr q0
	
	5. Steps to set details about a queue
	Syntax:
	> ./mq_setattr <queuename> <queue-size> <blocking(1)/nonblocking(0)>
	Sample:
	> ./mq_setattr q0 15 0
	
	6. Steps to receive notification
	Syntax:
	> ./mq_reqnotify <queuename> <reciever_pid>
	Syntax:
	> ./mq_reqnotify q0 121
	
