/* Modulo de mensagens do IPC */

/* Um processo se comunica com outro por meio de mensagens */
/* Ambos processos decidem em uma lista de mensagem para se comunicar */
/* Existe uma tabela de listas de mensagens */
/* Quando processos querem se comunicar precisam criar uma lista de mensagem */

#include "messages.h"
#include <kstdio.h>

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

int _msg_error_cases(unsigned size, const void *buf) {
  if (size == 0) {
    kprintf("ERRO NA MENSAGEM: mensagem sem conteudo.\n", (int) MAX_MSG_SIZE);
    return 1;
  }

  if (size > MAX_MSG_SIZE) {
    kprintf("ERRO NA MENSAGEM: mensagem maior que %d bytes.\n", (int) MAX_MSG_SIZE);
    return 1;
  }
  
  if (!buf) {
    kprintf("ERRO NA MENSAGEM: buffer com conteudo ausente.\n", (int) MAX_MSG_SIZE);
    return 1;
  }

  return 0;
}

int _send_error_cases(int qid) {
  if (qid == -1) {
    kprintf("ERRO NO ENVIO DE MENSAGEM: tabela cheia.\n");
    return 1;
  }

  if (msg_queues[qid].count == MAX_MSG) {
    kprintf("ERRO NO ENVIO DE MENSAGEM: fila %d atingiu o maximo de mensagens.\n", qid);
    return 1;
  }

  return 0;
}

int _get_msg_index(int qid) {
  int msg_index = msg_queues[qid].tail;
  if (msg_index == MAX_MSG) { /* vai para o 'inicio' da fila circular */
    msg_index = 0;
  }

  return msg_index;
}

int _insert_msg(int qid, struct msg temp_msg) {
  if (_send_error_cases(qid)) return -1;

  /* tenta inserir a mensagem na fila */
  int msg_index = _get_msg_index(qid);
  msg_queues[qid].msgs[msg_index] = temp_msg;

  msg_queues[qid].count++;
  msg_queues[qid].tail = msg_index + 1;

  return 0;
}

int sys_msgsnd(int qid, const void *buf, unsigned size, int type) {
  if (_msg_error_cases(size, buf)) {
    kprintf("Mensagem %d nao enviada", qid);
    return -1;
  }

  /* salvamos as informacoes em um endereco temporario */
  struct msg temp_msg;
  temp_msg.size = size;
  temp_msg.type = type;
  const unsigned char *bytes = buf;
  for (unsigned i = 0; i < size; i++) {
    temp_msg.buf[i] = bytes[i];
  }

  #pragma region SECAO_CRITICA
  /* tenta inserir a mensagem na fila */
  int out = _insert_msg(qid, temp_msg);
  #pragma endregion

  if (out == -1)
    kprintf("Mensagem %d nao enviada", qid);

  return out;
}
