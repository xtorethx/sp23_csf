#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
int compare_i64(const void * a, const void * b) {
  if (*(int64_t*)a < *(int64_t*)b) return -1;
  if (*(int64_t*)a > *(int64_t*)b) return 1;
  return 0;
}

void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

int do_child_work(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  merge_sort(arr, begin, end, threshold);
  return 0;
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  size_t mid = (end - begin) /2;
  int numelements = end - begin;
  int64_t *temparr = (int64_t*)malloc(numelements*sizeof(int64_t));
  int wstatus_l, wstatus_r;
  if (numelements <= threshold) {
    //sort the elements sequentially
    qsort(arr, numelements, sizeof(int64_t), compare_i64);
  }
  else { //not base case, recursively sort the left half of the sequence (merge_sort(arr, begin, mid, threshold);)
    pid_t pid_l, pid_r;
    pid_l = fork(); //create child process to sort left
    if (pid_l < 0) { // fork failed to start a new process
      fprintf(stderr, "Error: fork failed to start a new process");
      exit(6);
    } else if (pid_l == 0) { //child process, recursively sort left
      int retcode = do_child_work(arr, begin, mid, threshold);
      exit(retcode);
    } else { // if pid is not 0 or <0, we are in the parent process --> wait until left child done
      pid_t actual_pid_l = waitpid(pid_l, &wstatus_l, 0); // blocks until the process indentified by pid_to_wait_for completes
      if (actual_pid_l == -1) { // handle waitpid failure
        fprintf(stderr, "Error: waitpid failure");
        exit(7);
      }
      if (!WIFEXITED(wstatus_l)) {
          fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally");
          exit(8);
      }
      if (WEXITSTATUS(wstatus_l) != 0) {
          fprintf(stderr, "Error: subprocess returned a non-zero exit code");
          exit(9);
      }
      //left sorting child complete, recursively sort the right half of the sequence by forking a second time (merge_sort(arr, mid, end, threshold);)
      pid_r = fork(); //create child process to sort right
      if (pid_r < 0) { // fork failed to start a new process
          fprintf(stderr, "Error: fork failed to start a new process");
          exit(10);
      } else if (pid_r == 0) { //in right child, recursively sort right
          int retcode = do_child_work(arr, mid, end, threshold);
          exit(retcode);
      } else { //in parent process, wait for child to finish
        pid_t actual_pid_r = waitpid(pid_r, &wstatus_r, 0);
        if (actual_pid_r == -1) {
          // handle waitpid failure
          fprintf(stderr, "Error: waitpid failure");
          exit(11);
        }
        if (!WIFEXITED(wstatus_r)) {
            fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally");
            exit(12);
        }
        if (WEXITSTATUS(wstatus_r) != 0) {
            fprintf(stderr, "Error: subprocess returned a non-zero exit code");
            exit(13);
        }
        merge(arr, begin, mid, end, temparr); //children dead, merge the sorted sequences into a temp array
        //copy the contents of the temp array back to the original array
        memcpy(arr, temparr, (size_t)(numelements*sizeof(int64_t)));
      }
    }
  }
  free(temparr); 
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  } 

  // process command line arguments
  const char *filename = argv[1];
  char *end; 
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    /* TODO: report an error (threshold value is invalid) */;
    fprintf(stderr, "Error: threshold value is invalid");
    return 2;
  }

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "Error: file couldn't be opened");
    return 3;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
      fprintf(stderr, "Error: fstat error");
      return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  // you should immediately close the file descriptor here since mmap maintains a separate
  // reference to the file and all open fds will gets duplicated to the children, which will
  // cause fd in-use-at-exit leaks.
  // TODO: call close()
  close(fd);
  if (data == MAP_FAILED) {
      // handle mmap error and exit
      fprintf(stderr, "Error: mmap error"); 
      return 5;
  }
  // *data now behaves like a standard array of int64_t. Be careful though! Going off the end
  // of the array will silently extend the file, which can rapidly lead to disk space
  // depletion!

  // TODO: sort the data!
  merge_sort(data, 0, (file_size_in_bytes/sizeof(data[0])), threshold);

  // TODO: unmap and close the file
  munmap(data, file_size_in_bytes);
  //close(data);

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
