/* Modulo de mensagens do IPC */

/* Um processo se comunica com outro por meio de mensagens */
/* Ambos processos decidem em uma lista de mensagem para se comunicar */
/* Existe uma tabela de listas de mensagens */
/* Quando processos querem se comunicar precisam criar uma lista de mensagem */

#include "messages.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/* Cria 8 filas de mensagem */
struct msg_queue msg_queues[MAX_MSG_QUEUES] = {0};

int *sys_msgget(int key) {
	int qid = key % MAX_MSG_QUEUES;
	// retorna se ja existe
	if (msg_queues[qid].used == 1) {
		return &msg_queues[qid];
	}
	// se nao cria uma nova
	else {
		msg_queues[qid].key = key;
		msg_queues[qid].used = 1;
		msg_queues[qid].count = 0;
		msg_queues[qid].head = 0;
		msg_queues[qid].tail = 0;
		return &msg_queues[qid];
	}
	return NULL;
}
