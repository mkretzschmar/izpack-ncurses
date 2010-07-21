/*
 * installer.h
 *
 *  Created on: 20.07.2010
 *      Author: mk
 */

#ifndef INSTALLER_H_
#define INSTALLER_H_

extern char **environ;

char *checkJRE(void);
void test_ncurses(FILE *fpipe1, FILE *fpipe2);

char *strconcat(char *s1, char *s2);
void init_hello(FILE *fpipe);

#endif /* INSTALLER_H_ */
