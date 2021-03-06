#include <stddef.h>

#include "process.h"

void init_process(Process* process, pid_t pid, int fork_call) {
	process->pid = pid;
	process->fork_call = fork_call;
	process->parent = NULL;
	process->children = NULL;
	process->child_count = 0;
}

int proc_add_child(Process* process, Process* child) {
	child->parent = process;

	process->children = realloc(process->children,
			sizeof(Process)*(process->child_count+1));
	if(process->children == NULL) {
		return 1;
	}
	process->children[process->child_count] = *child;
	process->child_count++;

	return 0;
}

int proc_count_grandchildren(Process* process) {
	if(process->children == NULL) {
		return 0;
	}

	int grandchild_count = 0;
	int i;
	for(i=0; i<process->child_count; i++) {
		grandchild_count += process->children[i].child_count;
	}

	return grandchild_count;
}

void proc_free_descendant_mem(Process* process) {
	if(process == NULL) {
		return;
	}
	else if(process->children == NULL) {
		return;
	}
	else if(proc_count_grandchildren(process) == 0) {
		free(process->children);
		return;
	}

	int i;
	for(i=0; i<process->child_count; i++) {
		proc_free_descendant_mem(process->children+i);
	}
}

Process* proc_oldest_ancestor(const Process* process) {
	if(process->parent == NULL) {
		return NULL;
	}
	Process* proc = process;
	Process* parent;
	while((parent = proc->parent) != NULL) {
		proc = parent;
	}
	return proc;
}
