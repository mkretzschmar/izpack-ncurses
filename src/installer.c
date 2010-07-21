/*
 ============================================================================
 Name        : installer.c
 Author      : Marek Kretzschmar
 Version     :
 Copyright   : (C) 2010
 Description : Project SS2010 PTI997 SPL
 ============================================================================

 	// PSEUDOCODE für bidirektionale Prozesskommunikation mittels Pipes:
	//	pipe(rpipes);
	//	pipe(wpipes);
	//	fork()
	//	child:
	//	   close(wpipes[1]);
	//	   close(rpipes[0]);
	//	   dup2(wpipes[0], STDIN);
	//	   dup2(rpipes[1], STDOUT);
	//	   close(wpipes[0]);
	//	   close(rpipes[1]);
	//	   exec(..)
	//	parent:
	//	   close(wpipes[0]);
	//	   close(rpipes[1]);
	//	   //write to stdin
	//	   write(wpipes[1], ...);
	//	   //read from stdout
	//	   read(rpipes[0], ...);
	 *
 */

#include <sys/utsname.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "installer.h"

#define EXIT(s) {perror(s); exit(0);}
#define USAGE(s) {fprintf(stderr, "usage: %s (installer.jar)\n",s); exit(0);}
#define MAX 256

enum {
	READ, WRITE
};

/*
 *
 */
int main(int argc, char *argv[]) {
	struct utsname uts;
	int toJava[2], fromJava[2], n;

	char puffer[MAX];
	char *jar;

	pid_t child_pid;


	/* Prüfen, ob OS unterstuetzt wird */
	uname(&uts);
	/* @TODO: Check for all Linux/Unix OS types */
	if (strcmp(uts.sysname, "Linux") == 0) {
		printf("OS supported: %s \n", uts.sysname);
	} else {
		fprintf(stderr, "OS not supported: %s \n", uts.sysname);
	}

	// Kommandozeile generieren
	if (argc > 1) {
		jar = argv[1];
	} else {
		USAGE(argv[0]);
	}


	/* Zwei Pipes erzeugen (Multiplex) */
	if (pipe(toJava) < 0 || pipe(fromJava) < 0) {
		EXIT("Error creating pipes.");
	}

	/*Nun erzeugen wir einen neuen Prozeß*/
	switch (child_pid = fork()) {
	case -1:
		EXIT("FORK failed.")

	case 0: /* -------------------- Kindprozess ------------------------------*/
		printf("Elternprozess hat PID %i\n", getppid());
		printf("Kindprozess: (ich) habe PID %i\n", child_pid); // = 0

//		close(0); /* stdin schließen */
		dup(toJava[READ]); /* Lese-Deskriptor duplizieren */
//		close(1); /* stdout schließen */
		dup(fromJava[WRITE]); /* Schreib-Deskriptor duplizieren */

		/* Kindprozess wird von java überlagert,
		 * daher werden diese Deskriptoren nicht mehr benötigt.*/
//		close(fdout[READ]);
//		close(fdout[WRITE]);
//		close(fdin[READ]);
//		close(fdin[WRITE]);

		/* Nach dieser Zeile wird der Kindprozess von Java überlagert....*/
		//		execlp("grep", "grep", "^#include", NULL);
		printf("Lauching installer...");
		execlp("java", "java", "-jar", jar, "-console", NULL);

	default: /* ------------------- Elternprozess ----------------------------*/
		printf("Elternprozess: (ich) PID %i\n", getpid());
		printf("Kindprozess hat PID %i\n\n", child_pid);

		close(toJava[READ]); /* Elternprozess schreibt nur in die Pipe fdout */
		close(fromJava[WRITE]); /* Elternprozess liest nur von Pipe fdin aus java*/

		/* lesen der pipe von Java */
		/* read blockiert solange bis java fertig ist */
		while ((n = read(fromJava[READ], puffer, MAX)) != 0) {
			if (n == -1)
				EXIT("Error reading pipe.");
			/* Puffer ausgeben */
			if (write(STDOUT_FILENO, puffer, n) == -1) {
				EXIT("WRITE failed.");
			}
		}

		fgets(puffer, 1, stdin);

		n = sizeof(puffer);
		printf("%i Zeichen im Puffer: %s\n", n, puffer);

		/* Etwas in die Pipe fdout schreiben --> java */
		if (write(toJava[WRITE], "2", 1) == -1) {
		//if (write(fdout[WRITE], puffer, n) == -1) {
			EXIT("WRITE failed.");
		}

		write(toJava[WRITE], NULL, 1);

		/* Schreibseite schließen */
		close(toJava[WRITE]);
		/* Das ist das Zeichen für java zum lesen aus der Pipe fdout */

		/*Jetzt noch die Leseseite schließen wir sind fertig*/
		close(fromJava[READ]);

	}

	printf("*** IZPACK END ***\n");

	return EXIT_SUCCESS;
}

/*
 * Returns 1 if user has admin rights
 */
char *checkJRE(void) {
	char *t = "";
	// 1.) Prüfen, ob JAVA_HOME gesetzt ist

	// 2.) Prüfen, ob $JAVA_HOME/bin/java existiert

	// 3.) Java-Version herausfinden


	//	t = malloc(strlen(s2) + 1);

	return t;
}

void parent(pid_t handle) {

}

void child(pid_t handle) {

}
