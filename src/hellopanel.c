/*
 * @see http://de.wikibooks.org/wiki/Ncurses
 */

#include <menu.h>
#include <panel.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

WINDOW *win;

/**
 * Callback-Function
 */
void exit_hello(void) {
	delwin(win);
	endwin();
	exit(0);
}

void init_hello(FILE *fpipe) {
	int ch;
	char line[256];

	// ncurses screen initialisieren
	initscr();
	// Callback-Funktion
	//	atexit(exit_hello);
	clear();

	echo(); // Anzeige der Eingabedaten unterdrücken
	curs_set(0); // Curser auf Pos. 0 setzen
	cbreak(); // Aktivierung des cbreak-Modus, damit Ctrl+C und F1 wirken
	// MODI (siehe [1] S. 30/31
	// * RAW		: Zeilenpufferung und Einzelzeichenspeicher vollst. deaktiviert
	// * CBREAK	: ZP vollst. u. EZS teilweise deaktiviert
	// * COOKED	: ZP und EZS aktiviert
	// Hinweis: im RAW-Modus erfolgt keine Interrupt-Behandlung (z.B. Ctrl+C)

	// Return-Taste wird nur als Line-Feed (0xA) interpretiert
	nl();

	// Aktivierung der ncurses-Escape-Sequenzen für Spezialtasten
	keypad(stdscr, TRUE);

	// Aktivierung der Farbausgabe
	start_color();

	// Farbdefinition ändern
	init_color(COLOR_CYAN, 800, 800, 800);
	/* param 1     : color name
	 * param 2, 3, 4 : rgb content min = 0, max = 1000 */

	// Farb-Paar 1: Weiße Schrift auf blauem Grund
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	// Farb-Paar 2: Blaue Schrift auf grauem Grund
	init_pair(2, COLOR_BLUE, COLOR_WHITE);

	// Farb-Paar 1 für Standard-Fenster
	bkgd(COLOR_PAIR(1));

	// Neues Fenster erzeugen
	win = newwin(25, 80, 0, 0);
	box(win, 0, 0);
	mvwaddstr(win, 2, 2, "Willkommen zur Installation");

	wbkgd(win, COLOR_PAIR(2));

	mvaddstr(27, 3, "Installer F10-Funktionstaste beenden");

	refresh();
	wrefresh(win);

	while ((ch = getch()) != KEY_F(10)) {
		switch (ch) {
		case KEY_DOWN:
			break;
		case KEY_UP:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case KEY_STAB:
			break;
		case 0xA: /* Return- bzw. Enter-Taste -> ASCII-Code */

			while (fgets(line, sizeof line, fpipe)) {
				mvaddstr(28,3, "Stream:");
				mvaddstr(29,3, line);
				/* Parsen des IzPack-Outputs */
			}

		}

		wrefresh(win);
	}
}

