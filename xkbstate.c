#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#define MAXLEN 8

/* Compile with:
 * gcc -I/usr/include kbstate.c -lX11 -lxkbfile -o kbstate */

char *get_layout_name(Display *dpy, bool isUp) {
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
  
  if (isUp)
    for (char *p = tok; *p; p++)
      *p = toupper((unsigned char) *p);
  
  return tok;
}

int put_infos(char *pref, char *layout) {
  char buf[MAXLEN];
  int print_code;
  
  print_code = snprintf(buf, sizeof(buf), "%s%s", pref, layout);
 
  if (print_code > -1 &&
      print_code < sizeof(buf) &&
      puts(buf) != EOF &&
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
  bool isUp = false;
  int interval = 1;
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
        isUp = true;
        break;
      case 'p':
        pref = optarg;
        break;
    }
  }

  int exit_code;
  char *prev_layout, *layout;
  
  if ((prev_layout = get_layout_name(dpy, isUp)) != NULL &&
      (exit_code = put_infos(pref, prev_layout)) != EXIT_FAILURE) {
    if (snoop) {
      while ((layout = get_layout_name(dpy, isUp)) != NULL && 
          exit_code != EXIT_FAILURE) {
        if (strcmp(prev_layout, layout) != 0) {
          exit_code = put_infos(pref, layout);
          prev_layout = layout;
        }
        sleep(interval);
      }
    } 
  } 
  else
    return EXIT_FAILURE;

  XCloseDisplay(dpy);
  return exit_code;
}
