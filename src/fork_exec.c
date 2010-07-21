/*	fork_exec.c	*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn (char*, char**);

int main2() {
	/* Die zu uebergebende Argumentliste */
	char* arg_list[] = {
		"ls",
		"-al",
		".",
		NULL
	};

	spawn ("ls", arg_list);
	printf ("Erledigt\n");
	return 0;
}

int spawn (char* program, char** arg_list){
	pid_t child_pid;

	/* neuen Prozess (Duplikat) erzeugen */
	child_pid = fork ();

	if (child_pid != 0) {	/*	Im Elternprozess*/
		return child_pid;
	}

	else {
		execvp (program, arg_list);
	 	fprintf (stderr, "Fehler bei execvp\n");
		abort();
	}
}
