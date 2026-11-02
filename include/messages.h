#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MSG_QUEUES	8
#define MAX_MSG			16
#define MAX_MSG_SIZE	64

struct msg {
	int type;
	unsigned size;
	unsigned char buf[MAX_MSG_SIZE];
};

struct msg_queue {
	int key;
	int used;
	unsigned count;
	struct msg msgs[MAX_MSG];
	int head;
	int tail;
};

int sys_msgget(int key);
int sys_msgsnd(int qid, const void *buf, unsigned size, int type);
int sys_msgrcv(int qid, void *buf, unsigned size, int type);

#endif //MESSAGE_H
