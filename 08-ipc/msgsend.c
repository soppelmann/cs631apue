/* A simple program to illustrate the use of Message
 * Queues.
 *
 * Note that message queues continue to exist after
 * all processes have terminated; messages continue to
 * remain in the queues as well.  This is desired, but
 * requires that processes clean up after themselves
 * when they are done using the queues.
 *
 * Note that in this example we manually specify the
 * key instead of using ftok(2).
 *
 * Use this tool to create/send messages, then run
 * msgrecv to retrieve them (in order).  Note that
 * msgrecv will block if no messages are in the queue.
 *
 * Use ipcs(1) to inspect the usage.
 *
 * Derived from:
 * https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html
 */

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSGSZ     128

typedef struct msgbuf {
	long mtype;
	char mtext[MSGSZ];
} message_buf;

int
main(int argc, char **argv)
{
	int msqid;
	key_t key;
	message_buf sbuf;
	size_t len;

	if (argc != 3) {
		(void)fprintf(stderr, "Usage: msgsend key message\n");
		exit(EXIT_FAILURE);
	}

	if ((key = atoi(argv[1])) < 1) {
		(void)fprintf(stderr, "Invalid key: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((msqid = msgget(key, IPC_CREAT | 0644)) < 0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	sbuf.mtype = 1;

	(void)strncpy(sbuf.mtext, argv[2], MSGSZ);

	len = strlen(sbuf.mtext) + 1;

	if (msgsnd(msqid, &sbuf, len, IPC_NOWAIT) < 0) {
		(void)fprintf (stderr, "%d, %ld, %s, %d\n",
				msqid, sbuf.mtype, sbuf.mtext, (int)len);
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
