#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BUFSIZE 512

static void
usage(void)
{
  printf(2, "usage: head [-N | -n N] [file ...]\n");
  exit();
}

static int
parse_lines(char *s, int *lines)
{
  int i, v;

  if(s[0] == 0)
    return -1;

  v = 0;
  for(i = 0; s[i]; i++){
    if(s[i] < '0' || s[i] > '9')
      return -1;
    v = v * 10 + (s[i] - '0');
  }
  *lines = v;
  return 0;
}

static int
output_head(int fd, int max_lines)
{
  char buf[BUFSIZE];
  int n, lines, start, i;

  if(max_lines == 0)
    return 0;

  lines = 0;
  while(lines < max_lines){
    n = read(fd, buf, sizeof(buf));
    if(n < 0){
      printf(2, "head: read error\n");
      return -1;
    }
    if(n == 0)
      break;

    start = 0;
    for(i = 0; i < n; i++){
      if(buf[i] == '\n'){
        int len = i + 1 - start;
        if(write(1, buf + start, len) != len){
          printf(2, "head: write error\n");
          return -1;
        }
        start = i + 1;
        lines++;
        if(lines == max_lines)
          break;
      }
    }

    if(lines == max_lines)
      break;

    if(start < n){
      int len = n - start;
      if(write(1, buf + start, len) != len){
        printf(2, "head: write error\n");
        return -1;
      }
    }
  }

  return 0;
}

int
main(int argc, char *argv[])
{
  int lines, argi, expect_number, file_start;
  int multi, first_file;
  int had_error;

  lines = 10;
  argi = 1;
  expect_number = 0;

  while(argi < argc){
    char *arg = argv[argi];
    if(expect_number){
      if(parse_lines(arg, &lines) < 0)
        usage();
      expect_number = 0;
      argi++;
      continue;
    }

    if(arg[0] != '-' || arg[1] == 0)
      break;

    if(arg[1] == 'n'){
      if(arg[2] == 0){
        expect_number = 1;
      } else {
        if(parse_lines(arg + 2, &lines) < 0)
          usage();
      }
    } else {
      if(parse_lines(arg + 1, &lines) < 0)
        usage();
    }
    argi++;
  }

  if(expect_number)
    usage();

  file_start = argi;
  if(file_start == argc){
    if(output_head(0, lines) < 0)
      exit();
    exit();
  }

  multi = (argc - file_start) > 1;
  had_error = 0;
  first_file = 1;

  for(argi = file_start; argi < argc; argi++){
    char *name = argv[argi];
    int fd;
    int from_stdin = 0;

    if(name[0] == '-' && name[1] == 0){
      fd = 0;
      from_stdin = 1;
    } else {
      fd = open(name, O_RDONLY);
      if(fd < 0){
        printf(2, "head: cannot open %s\n", name);
        had_error = 1;
        continue;
      }
    }

    if(multi){
      if(!first_file)
        printf(1, "\n");
      printf(1, "==> %s <==\n", name);
    }
    first_file = 0;

    if(output_head(fd, lines) < 0)
      had_error = 1;

    if(!from_stdin)
      close(fd);
  }

  if(had_error)
    exit();
  exit();
}
