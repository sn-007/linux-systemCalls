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

void printPercentage(lli numerator,lli denominator)
{
    float percentage = (float)numerator / denominator;
    percentage *= 100;
    char *percentageArray = malloc(sizeof(char) * 20);
    sprintf(percentageArray, " STATUS:%.2f%c\r", percentage, 37);
    write(1, percentageArray, strlen(percentageArray));
    //free(percentageArray);
    percentageArray="\0";
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
  int inputFileLength = strlen(inputFileName);
  int totalPieces = argv[2][0] - '0', pieceTobeReversed = argv[3][0] - '0';

  // output file path manipulations
  char temp[13] = "Assignment/2_", outputFilePath[13 + inputFileLength];
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
  lli chunkSize = 1e6, sizePerPiece = sb.st_size / totalPieces;
  lli rem = sizePerPiece % chunkSize, numOfchunks = sizePerPiece / chunkSize,
      readchunks = 0;

  


  lseek(fd1, pieceTobeReversed * sizePerPiece,SEEK_SET); // moving to the end of the required piece.

  if (rem != 0) {
    char *remArray = malloc(sizeof(char) * rem);

    lseek(fd1, -rem, SEEK_CUR);

    read(fd1, remArray, rem);
    reverseArray(remArray, rem);
    write(fd2, remArray, rem);

    readchunks += rem;

    //free(remArray);
    remArray="\0";
    printPercentage(readchunks, sizePerPiece);

    
  }

  while (numOfchunks--) {
    char *chunkArray = malloc(sizeof(char) * chunkSize);

    lseek(fd1, pieceTobeReversed * sizePerPiece,SEEK_SET); // moving to the end of the required piece.
    lseek(fd1, -chunkSize-readchunks, SEEK_CUR);

    read(fd1, chunkArray, chunkSize);
    reverseArray(chunkArray, chunkSize);
    write(fd2, chunkArray, chunkSize);

    readchunks += chunkSize;

    //free(chunkArray);
    chunkArray="\0";
    printPercentage(readchunks, sizePerPiece);

    
  }

  close(fd1);
  close(fd2);
  chmod(outputFilePath, S_IRUSR | S_IWUSR);

  return 0;
}