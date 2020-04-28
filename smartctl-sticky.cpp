#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  const char *script = R"EOF(
PATH=/usr/bin
if printf %s "$*" | egrep -q -f /etc/smartctl-sticky.conf; then
    /usr/bin/smartctl "$@"
elif [[ -f /etc/smartctl-sticky.conf ]]; then
    echo >&2 "Unwhitelisted arguments: $*"
    exit 1
else
    echo >&2 "Whitelist file does not exist: /etc/smartctl-sticky.conf"
    exit 1
fi
  )EOF";
  char *shargv[4 + argc];
  shargv[0] = (char *)"bash"; // Plain sh does not support "$@"
  shargv[1] = (char *)"-c";
  shargv[2] = (char *)script;
  shargv[3] = argv[0];
  for (int i = 0; i < argc; i++) {
    shargv[4 + i] = argv[i + 1];
  }
  // The last NULL in argv is copied to shargv
  char *shenv[] = {NULL};
  setuid(0);
  execve("/usr/bin/bash", shargv, shenv);
  perror("could not execve");
  return 1;
}
