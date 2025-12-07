#include "types.h"
#include "stat.h"
#include "user.h"

#define LONG_LEN 1000

int
main(int argc, char *argv[])
{
  char buf[LONG_LEN];
  int i;
  char done[] = "Done\n";

  for(i = 0; i < LONG_LEN; i++)
    buf[i] = 'A';

  write(1, buf, sizeof(buf));
  write(1, "\n", 1);
  write(1, done, sizeof(done) - 1);

  exit();
}
