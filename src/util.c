#include <string.h>

/* @see http://stackoverflow.com/questions/419604/using-strcat-in-c */
char *strconcat(char *s1, char *s2) {
	size_t old_size;
	char *t;
	old_size = strlen(s1);
	/* cannot use realloc() on initial const char* */
	t = malloc(old_size + strlen(s2) + 1);
	strcpy(t, s1);
	strcpy(t + old_size, s2);
	return t;
}
