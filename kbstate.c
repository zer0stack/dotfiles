#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#define MAXLEN 32
#define INTERVAL 1

/* Compile with:
 * gcc -I/usr/include kbstate.c -lX11 -lxkbfile -o kbstate */

int str_to_upper(char *str) {
  if (str != NULL) {
    for (char *p = str; *p; p++)
      *p = toupper((unsigned char) *p);
    return EXIT_SUCCESS;
  } 
  else
    return EXIT_FAILURE;
}

const char *get_layout(Display *dpy, bool up) {
  XkbStateRec state;
  XkbGetState(dpy, XkbUseCoreKbd, &state);

  XkbRF_VarDefsRec vd;
  XkbRF_GetNamesProp(dpy, NULL, &vd);
  
  char *tok = strtok(vd.layout, ",");

  for (int i = 0; i < state.group; i++) {
    tok = strtok(NULL, ",");
    if (tok == NULL)
      return NULL;
  }

  if (up)
    str_to_upper(tok);

  return tok;
}

int put_infos(char *pref, const char *layout) {
  char buf[MAXLEN];
  
  if (snprintf(buf, sizeof(buf), "%s%s", pref, layout) > 0 &&
      puts(buf) > 0 &&
      fflush(stdout) != EOF)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  Display *dpy = XOpenDisplay(NULL);

  if (dpy == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(EXIT_FAILURE);
  }

  char *pref = "";
  bool up = false;
  int interval = INTERVAL;
  bool snoop = false;
  
  int opt;
  while ((opt = getopt(argc, argv, "hsup:")) != -1) {
    switch (opt) {
      case 'h':
        printf("kbstate [-h|-s|-u|-p PREFIX]\n");
        exit(EXIT_SUCCESS);
        break;
      case 's':
        snoop = true;
        break;
      case 'u':
        up = true;
        break;
      case 'p':
        pref = optarg;
        break;
    }
  }

  if (strlen(pref) + 2 + 1 > MAXLEN) {
    fprintf(stderr, "Result string is too long.\n");
    exit(EXIT_FAILURE);
  }

  int exit_code;

  if (snoop)
    while ((exit_code = put_infos(pref, get_layout(dpy, up))) != EXIT_FAILURE)
      sleep(interval);
    else
      exit_code = put_infos(pref, get_layout(dpy, up));

  XCloseDisplay(dpy);
  return exit_code;
}
