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

/*
 * casts two pointer values to int64_t and compares
 * 
 * Parameters:
 *   a - pointer to a const void value
 *   b - pointer to a const void value
 *
 * Returns:
 *   int, -1 if first parameter less than second, 1 if first parameter greater than second, 0 if equal
 */
int compare_i64(const void * a, const void * b) {
  if (*(int64_t*)a < *(int64_t*)b) return -1;
  if (*(int64_t*)a > *(int64_t*)b) return 1;
  return 0;
}

/*
 * Merge the elements in the sorted ranges [begin, mid) and [mid, end),
 * copying the result into temparr.
 * 
 * Parameters:
 *   arr - pointer to array of int64_t values being referenced
 *   begin - size_t marking index of first value copied
 *   mid - size_t marking index of middle value copied
 *   end - size_t marking index of last value copied
 *   temparr - pointer to an array where sorted arr values from index begin to end will be copied into in sorted order
 *
 * Returns:
 *   void
 */
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

/*
 * Recursive call by child process, helper function for sorting
 * 
 * Parameters:
 *   arr - pointer to array of int64_t values being referenced
 *   begin - size_t marking index of first value copied
 *   end - size_t marking index of last value copied
 *   threshold - size_t number of elements below (inclusive) which the program should use a sequential sort
 *
 * Returns:
 *   return code
 */
int do_child_work(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  merge_sort(arr, begin, end, threshold);
  return 0;
}

/*
 * Recursively splitting elements in array to be sorted using merge sort, parallel implementation
 * 
 * Parameters:
 *   arr - pointer to array of int64_t values being referenced
 *   begin - size_t marking index of first value copied
 *   end - size_t marking index of last value copied
 *   threshold - size_t number of elements below (inclusive) which the program should use a sequential sort
 *
 * Returns:
 *   void
 */
void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  int numelements = end - begin;
  size_t mid = begin + numelements/2; 
  int64_t *temparr = (int64_t*)malloc(numelements*sizeof(int64_t));
  int wstatus_l, wstatus_r;
  if (numelements <= threshold) { //base case; sort the elements sequentially
    qsort(&(arr[begin]), numelements, sizeof(int64_t), compare_i64);
  } else { //not base case, recursively sort the left and right sides
    pid_t pid_l, pid_r;
    pid_l = fork(); //create child process to sort left
    if (pid_l == -1) { // failure to create a child process using fork
      fprintf(stderr, "Error: left fork failed to start a new process");
      exit(8);
    } else if (pid_l == 0) { //in left child process, recursively sort left
      int retcode = do_child_work(arr, begin, mid, threshold);
      exit(retcode);
    } 
    pid_r = fork(); //create child process to sort right
    if (pid_r == -1) { // failure to create a child process using fork
      fprintf(stderr, "Error: right fork failed to start a new process");
      exit(9);
    } else if (pid_r == 0) { //in right child process, recursively sort right
      int retcode = do_child_work(arr, mid, end, threshold);
      exit(retcode);
    } 
    // in the parent process --> block until the processes identified by pid_1 and pid_r complete
    pid_t actual_pid_l = waitpid(pid_l, &wstatus_l, 0); 
    pid_t actual_pid_r = waitpid(pid_r, &wstatus_r, 0);
    if (actual_pid_l == -1 || actual_pid_r == -1) { // handle waitpid failure
      fprintf(stderr, "Error: waitpid failure");
      exit(10);
    }
    if (!WIFEXITED(wstatus_l) || !WIFEXITED(wstatus_r)) {
      fprintf(stderr, "Error: subprocess crashed, was interrupted, or did not exit normally");
      exit(11);
    }
    if (WEXITSTATUS(wstatus_l) != 0 || WEXITSTATUS(wstatus_r) != 0) {
      fprintf(stderr, "Error: subprocess returned a non-zero exit code");
      exit(12);
    } 
    merge(arr, begin, mid, end, temparr); //children dead, merge the sorted sequences into a temp array
    memcpy(arr+begin, temparr, (size_t)(numelements*sizeof(int64_t))); //copy the contents of the temp array back to the original array
    free(temparr); 
  }
}

/*
 * Recursively splitting elements in array to be sorted using merge sort, parallel implementation
 * 
 * Parameters:
 *   argc - integer, number of arguments passed to the program (argument count)
 *   argv - string pointer to pointer, vector of arguments passed to program
 *
 * Returns:
 *   int, exit code
 */
int main(int argc, char **argv) {
  if (argc != 3) { // check for correct number of command line arguments
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  } 
  const char *filename = argv[1]; // process command line arguments
  char *end; 
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) { // report an error (threshold value is invalid) 
    fprintf(stderr, "Error: threshold value is invalid");
    return 2;
  }
  int fd = open(filename, O_RDWR); // open the file
  if (fd < 0) { //failure to open the file with the integers to be sorted
    fprintf(stderr, "Error: file couldn't be opened");
    return 3;
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf); // use fstat to determine the size of the file
  if (rc != 0) { // handle fstat error and exit
    fprintf(stderr, "Error: fstat error");
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // map the file into memory using mmap
  if (data == MAP_FAILED) { //failure to mmap the file data
    fprintf(stderr, "Error: mmap error"); 
    return 5;
  }
  int c = close(fd);
  if (c != 0) { //failure of "top-level" process to close file
    fprintf(stderr, "Error: failed to close file"); 
    return 6;
  }
  merge_sort(data, 0, (file_size_in_bytes/sizeof(int64_t)), threshold); // sort the data
  int unmap = munmap(data, file_size_in_bytes); // unmap file
  if (unmap != 0) { //failure of "top-level" process to munmap the file data
    fprintf(stderr, "Error: munmap error"); 
    return 7;
  } 
  return 0; // exit with a 0 exit code if sort was successful
}
