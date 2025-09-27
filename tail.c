#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_LINES 14  // Reduced from 1000 to fit in xv6's limited stack
#define MAX_LINE_LENGTH 100  // Reduced from 512

// Count total lines in file
int
count_lines(int fd)
{
  char buf[512];
  int n, i, lines;
  
  lines = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0) {
    for(i = 0; i < n; i++) {
      if(buf[i] == '\n')
        lines++;
    }
  }
  return lines;
}

// Print last num_lines lines from file
void
tail_file(char *filename, int num_lines)
{
  char buf[512];
  int fd, total_lines, skip_lines, current_line;
  int n, i;
  
  // Open file and count lines
  if((fd = open(filename, 0)) < 0) {
    printf(1, "tail: cannot open %s\n", filename);
    return;
  }
  
  total_lines = count_lines(fd);
  close(fd);
  
  // Reopen file for reading
  if((fd = open(filename, 0)) < 0) {
    printf(1, "tail: cannot open %s\n", filename);
    return;
  }
  
  // Calculate how many lines to skip
  if(total_lines <= num_lines) {
    skip_lines = 0;  // Print all lines
  } else {
    skip_lines = total_lines - num_lines;
  }
  
  // Read file and skip/print appropriately
  current_line = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0) {
    for(i = 0; i < n; i++) {
      // Print character if we're past the skip point
      if(current_line >= skip_lines) {
        write(1, &buf[i], 1);
      }
      // Count newlines
      if(buf[i] == '\n') {
        current_line++;
      }
    }
  }
  
  close(fd);
}

// Proper stdin handler - buffer lines and print last N
void
tail_stdin(int num_lines)
{
  char buf[512];
  char lines[MAX_LINES][MAX_LINE_LENGTH];
  int line_count = 0;
  int current_pos = 0;
  int n, i, j;
  int start_line;
  int circular_start;
  
  // If requested lines exceed buffer, just use buffer size
  if(num_lines > MAX_LINES) {
    num_lines = MAX_LINES;
  }
  
  // Read all input and store lines in circular buffer
  while((n = read(0, buf, sizeof(buf))) > 0) {
    for(i = 0; i < n; i++) {
      if(buf[i] == '\n') {
        // End of line - null terminate and move to next
        lines[line_count % MAX_LINES][current_pos] = '\0';
        line_count++;
        current_pos = 0;
      } else if(current_pos < MAX_LINE_LENGTH - 1) {
        // Add character to current line
        lines[line_count % MAX_LINES][current_pos] = buf[i];
        current_pos++;
      }
      // If line is too long, we truncate it
    }
  }
  
  // If there's a partial last line without newline, count it
  if(current_pos > 0) {
    lines[line_count % MAX_LINES][current_pos] = '\0';
    line_count++;
  }
  
  // Calculate starting line to print
  if(line_count <= num_lines) {
    start_line = 0;
    circular_start = 0;
  } else if(line_count <= MAX_LINES) {
    start_line = line_count - num_lines;
    circular_start = start_line;
  } else {
    // We've wrapped around the circular buffer
    start_line = line_count - num_lines;
    circular_start = start_line % MAX_LINES;
  }
  
  // Print the last num_lines lines
  j = 0;
  for(i = start_line; i < line_count && j < num_lines; i++, j++) {
    int idx = (circular_start + j) % MAX_LINES;
    printf(1, "%s", lines[idx]);
    // Add newline if the line doesn't already end with one
    if(strlen(lines[idx]) > 0 && lines[idx][strlen(lines[idx])-1] != '\n') {
      printf(1, "\n");
    } else if(strlen(lines[idx]) == 0) {
      printf(1, "\n");
    }
  }
}

// strlen is already available in user.h

int
main(int argc, char *argv[])
{
  int i;
  int num_lines;
  int arg_start;
  
  // Default values
  num_lines = 10;
  arg_start = 1;
  
  // Parse arguments
  if(argc >= 2 && argv[1][0] == '-') {
    // Check for -n N format (lowercase n)
    if(argv[1][1] == 'n' && argv[1][2] == '\0' && argc >= 3) {
      num_lines = atoi(argv[2]);
      arg_start = 3;
    }
    // Check for -N N format (uppercase N)
    else if(argv[1][1] == 'N' && argv[1][2] == '\0' && argc >= 3) {
      num_lines = atoi(argv[2]);
      arg_start = 3;
    }
    // Check for -NUM format (e.g., -10)
    else if(argv[1][1] >= '0' && argv[1][1] <= '9') {
      num_lines = atoi(&argv[1][1]);
      arg_start = 2;
    }
    
    if(num_lines <= 0) {
      num_lines = 10;
    }
  }
  
  // If no files, read from stdin
  if(argc <= arg_start) {
    tail_stdin(num_lines);
    exit();
  }
  
  // Process files
  for(i = arg_start; i < argc; i++) {
    // Print header for multiple files
    if((argc - arg_start) > 1) {
      printf(1, "==> %s <==\n", argv[i]);
    }
    
    tail_file(argv[i], num_lines);
    
    // Print separator between files
    if((argc - arg_start) > 1 && i < argc - 1) {
      printf(1, "\n");
    }
  }
  
  exit();
}