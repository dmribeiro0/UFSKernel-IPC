/* Modulo de mensagens do IPC */

/* Um processo se comunica com outro por meio de mensagens */
/* Ambos processos decidem em uma lista de mensagem para se comunicar */
/* Existe uma tabela de listas de mensagens */
/* Quando processos querem se comunicar precisam criar uma lista de mensagem */

#include "messages.h"

/* Cria 8 filas de mensagem */
static struct msg_queue msg_queues[MAX_MSG_QUEUES] = {0};

int sys_msgget(int key) {
  int free_slot = -1;

  for (int i = 0; i < MAX_MSG_QUEUES; i++) {
    if (msg_queues[i].used && msg_queues[i].key == key) { /* hit */
      return i;
    }
    if (msg_queues[i].used == 0 && free_slot == -1) { /* primeiro espaco vazio */
      free_slot = i;
    }
  }

  if (free_slot != -1) { /* cria uma fila se houver espaco vazio */
    msg_queues[free_slot].key = key;
    msg_queues[free_slot].used = 1;
    msg_queues[free_slot].count = 0;
    msg_queues[free_slot].head = 0;
    msg_queues[free_slot].tail = 0;
    return free_slot;
  }

  /* nao ha espacos livre na tabela: retorna erro */
  return -1;
}
