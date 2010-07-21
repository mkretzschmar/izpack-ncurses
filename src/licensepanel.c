/*
 * @see http://de.wikibooks.org/wiki/Ncurses
 */

#include <menu.h>
#include <panel.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
#define MENU_ITEMS 8

ITEM **it;
MENU *me;
WINDOW *win;

/**
 * Callback-Function
 */
void exit_license(void) {
	int i;

	unpost_menu(me);
	free_menu(me);

	for (i = 0; i <= MENU_ITEMS; i++) {
		free_item(it[i]);
	}

	free(it);
	delwin(win);

	endwin();
}

void init_license(void) {
	int ch, cols, rows;

	// ncurses screen initialisieren
	initscr();
	// Callback-Funktion
	atexit(exit_license);
	clear();

	noecho(); // Anzeige der EIngabedaten unterdrücken
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

	/* Get all the mouse events */
	mousemask(ALL_MOUSE_EVENTS, NULL);

	it = (ITEM **) calloc(MENU_ITEMS + 1, sizeof(ITEM *));
	it[0] = new_item("[  A1  ]", "");
	it[1] = new_item("[  B1  ]", "");
	it[2] = new_item("[  A2  ]", "");
	it[3] = new_item("[  B2  ]", "");
	it[4] = new_item("[  A3  ]", "");
	it[5] = new_item("[  B3  ]", "");
	it[6] = new_item("[  A4  ]", "");
	it[7] = new_item("[ Ende ]", "");
	it[8] = 0;

	set_menu_format(me, 4, 2);

	//
	me = new_menu(it);

	// Neues Fenster erzeugen
	win = newwin(25, 80, 0, 0);
	box(win, 0, 0);
	mvwaddstr(win, 2, 2, "Paketauswahl");

	set_menu_win(me, win);
	set_menu_sub(me, derwin(win, 5, 30, 3, 2));

	set_menu_fore(me, COLOR_PAIR(2) | A_REVERSE);
	set_menu_back(me, COLOR_PAIR(2));

	wbkgd(win, COLOR_PAIR(2));

	post_menu(me);

	mvaddstr(27, 3, "Programm mittels Menü oder F10-Funktionstaste beenden");

	refresh();
	wrefresh(win);

	while ((ch = getch()) != KEY_F(10)) {
		switch (ch) {
		case KEY_DOWN:
			menu_driver(me, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(me, REQ_UP_ITEM);
			break;
		case KEY_LEFT:
			menu_driver(me, REQ_LEFT_ITEM);
			break;
		case KEY_RIGHT:
			menu_driver(me, REQ_RIGHT_ITEM);
			break;
		case KEY_STAB:
			menu_driver(me, REQ_UP_ITEM);
			break;
		case 0xA: /* Return- bzw. Enter-Taste -> ASCII-Code */
			if (item_index(current_item(me)) == 7)
				exit(0);
		}

		wrefresh(win);
	}
}

