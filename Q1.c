#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define lli long long int

void reverseArray(char *array, lli size) {
  lli start = 0, end = size - 1;
  while (start < end) {
    char reversechar = array[start];
    array[start] = array[end];
    array[end] = reversechar;
    start++;
    end--;
  }
}

int main(int argc, char *argv[]) {
  // INPUT VALIDATIONS:
  struct stat sb;
  if (stat(argv[1], &sb) == -1) {
    perror(argv[1]);
    return 0;
  }

  if (S_ISDIR(sb.st_mode)) {
    write(1, "Input is not a Text file BRO\n",
          strlen("Input is not a Text file BRO\n"));
    return 0;
  }

  // extracting the file_name from directory name(just in case senpai).
  lli il = strlen(argv[1]); // il stands for string length of argument.
  lli i = il - 1; // i contains the location of '/' incase of a directory.
  for (i = il - 1; i >= 0; i--) {

    if (argv[1][i] == '/')
      break;
  }

  // input file details
  char *inputFileName = &argv[1][i + 1];
  lli inputFileLength = strlen(inputFileName);

  // output file path manipulations
  char temp[13] = "Assignment/1_", outputFilePath[13 + inputFileLength];
  for (i = 0; i < 13; i++)
    outputFilePath[i] = temp[i];

  for (i = 0; i < inputFileLength; i++)
    outputFilePath[13 + i] = inputFileName[i];

  // opening Output file and creating Directory
  int fd1 = open(argv[1], O_RDONLY);
  mkdir("Assignment", 0777);

  int fd2 = open(outputFilePath, O_RDWR);
  if (fd2 > 0)
    unlink(outputFilePath);
  fd2 = open(outputFilePath, O_CREAT | O_RDWR, 0644); //-rw-r--r--

  // READING 1e6 bytes at a time. AND writing them to fd2
  lli partSize = 1e6;
  lli rem = sb.st_size % partSize, numOfParts = sb.st_size / partSize,
      readParts = 0;

  lseek(fd1, 0, SEEK_END); // moving to the end of the input file

  if (rem != 0) {

    char *remArray = malloc(sizeof(char) * rem);
    lseek(fd1, -rem, SEEK_CUR);
    read(fd1, remArray, rem);
    reverseArray(remArray, rem);
    write(fd2, remArray, rem);
    readParts += rem;
    free(remArray);
    float percentage = (float)readParts / sb.st_size;
    percentage *= 100;
    char *percentageArray = malloc(sizeof(char) * 20);
    sprintf(percentageArray, "STATUS:%.2f%c \r", percentage, 37);
    write(1, percentageArray, strlen(percentageArray));
    free(percentageArray);

  }

  while (numOfParts--) {
    char *partArray = malloc(sizeof(char) * partSize);
    lseek(fd1, -partSize - readParts, SEEK_END);
    read(fd1, partArray, partSize);
    reverseArray(partArray, partSize);
    write(fd2, partArray, partSize);
    readParts += partSize;
    free(partArray);
    float percentage = (float)readParts / sb.st_size;
    percentage *= 100;

    char *percentageArray = malloc(sizeof(char) * 20);

    sprintf(percentageArray, "STATUS:%.2f%c \r", percentage, 37);
    write(1, percentageArray, strlen(percentageArray));
    free(percentageArray);
  }

  close(fd1);
  close(fd2);
  // Assigning permissions
  chmod("Assignment", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  chmod(outputFilePath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

  return 0;
}