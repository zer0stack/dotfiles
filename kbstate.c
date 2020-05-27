#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#define MAXLEN 8
#define INTERVAL 1

/* Compile with:
 * gcc -I/usr/include kbstate.c -lX11 -lxkbfile -o kbstate */

int put_infos(Display *dpy, char *pref, bool isUpper) {
  XkbStateRec state;
  XkbGetState(dpy, XkbUseCoreKbd, &state);

  XkbRF_VarDefsRec vd;
  XkbRF_GetNamesProp(dpy, NULL, &vd);
  
  char *tok = strtok(vd.layout, ",");

  for (int i = 0; i < state.group; i++) {
    tok = strtok(NULL, ",");
    if (tok == NULL)
      return EXIT_FAILURE;
  }
  
  if (isUpper)
    for (char *p = tok; *p; p++)
      *p = toupper((unsigned char) *p);
  
  char buf[MAXLEN];
  int concat_code;
  
  concat_code = snprintf(buf, sizeof(buf), "%s%s", pref, tok);
 
  if (concat_code > -1 &&
      concat_code < sizeof(buf) &&
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
  bool isUpper = false;
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
        isUpper = true;
        break;
      case 'p':
        pref = optarg;
        break;
    }
  }

  int exit_code;

  if (snoop)
    while ((exit_code = put_infos(dpy, pref, isUpper)) != EXIT_FAILURE)
      sleep(interval);
    else
      exit_code = put_infos(dpy, pref, isUpper);

  XCloseDisplay(dpy);
  return exit_code;
}
