#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct Process {
	pid_t pid;
	int fork_call;
	struct Process* parent;
	struct Process* children;
	int child_count;
} Process;

void init_process(Process* process, pid_t pid, int fork_call);

int proc_add_child(Process* process, Process* child);

int proc_count_grandchildren(Process* process);

void proc_free_descendant_mem(Process* process);

Process* proc_oldest_ancestor(const Process* process);

#endif // PROCESS_H
