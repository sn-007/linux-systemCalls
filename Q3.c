#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define lli long long int

void check(char *member, char permission, char *fileName) {

  int x = permission - '0';
  char *Read = malloc(sizeof(char) * 3);
  char *Write = malloc(sizeof(char) * 3);
  char *Execute = malloc(sizeof(char) * 3);

  if (x & 1)
    Execute = "Yes";
  else
    Execute = "No";
  if (x & 2)
    Write = "Yes";
  else
    Write = "No";
  if (x & 4)
    Read = "Yes";
  else
    Read = "No";

  char *ReadPrint = malloc(sizeof(char) * 75);
  char *WritePrint = malloc(sizeof(char) * 75);
  char *ExecutePrint = malloc(sizeof(char) * 75);

  sprintf(ReadPrint, "%s has read permission on %s: %s\n", member, fileName,
          Read);
  sprintf(WritePrint, "%s has write permission on %s: %s\n", member, fileName,
          Write);
  sprintf(ExecutePrint, "%s has execute permission on %s: %s\n\n", member,
          fileName, Execute);

  write(1, ReadPrint, strlen(ReadPrint));
  write(1, WritePrint, strlen(WritePrint));
  write(1, ExecutePrint, strlen(ExecutePrint));
};

int main(int argc, char *argv[]) {
  // INPUT VALIDATIONS:
  struct stat sb, sd, sb1,
      sb2; // for input file, 2 output text files and one directory.
  if (stat(argv[1], &sb) == -1) {
    perror(argv[1]);
    return 0;
  }

  if (S_ISDIR(sb.st_mode)) {
    write(1, "Input is not a Text file BRO\n",
          strlen("Input is not a Text file BRO\n"));
    // return 0;
  }

  // extracting the file_name from directory name(just in case senpai).
  lli il = strlen(argv[1]);
  lli i = il - 1; // i contains the location of '/' incase of a directory.
  for (i = il - 1; i >= 0; i--) {
    if (argv[1][i] == '/')
      break;
  }

  // input file details
  char *inputFileName = &argv[1][i + 1];
  lli inputFileLength = strlen(inputFileName);

  // output file path manipulations
  char temp1[13] = "Assignment/1_", outputFile1Path[13 + inputFileLength];
  char temp2[13] = "Assignment/2_", outputFile2Path[13 + inputFileLength];
  for (i = 0; i < 13; i++) {

    outputFile1Path[i] = temp1[i];
    outputFile2Path[i] = temp2[i];
  }

  for (i = 0; i < inputFileLength; i++) {
    outputFile1Path[13 + i] = inputFileName[i];
    outputFile2Path[13 + i] = inputFileName[i];
  }

  if (stat("Assignment", &sd) == -1) {
    write(1, "Assignment Directory Doesn't exist",
          strlen("Assignment Directory Doesn't exist"));
    return 0;
  } else {
    char directoryPermissions[10];
    sprintf(directoryPermissions, "%o", sd.st_mode);
    check("User", directoryPermissions[strlen(directoryPermissions) - 3],
          "directory");
    check("Group", directoryPermissions[strlen(directoryPermissions) - 2],
          "directory");
    check("Others", directoryPermissions[strlen(directoryPermissions) - 1],
          "directory");
  }

  if (stat(outputFile1Path, &sb1) == -1) {
    write(1, "OutputFile_1 Doesn't exist\n",
          strlen("OutputFile_1 Doesn't exist\n"));
          
  } else {
    char output_file1Permissions[10];
    sprintf(output_file1Permissions, "%o", sb1.st_mode);
    check("User", output_file1Permissions[strlen(output_file1Permissions) - 3],
          "output_file1");
    check("Group", output_file1Permissions[strlen(output_file1Permissions) - 2],
          "output_file1");
    check("Others", output_file1Permissions[strlen(output_file1Permissions) - 1],
          "output_file1");
  }

  if (stat(outputFile2Path, &sb2) == -1) {
    write(1, "OutputFile_2 Doesn't exist\n",
          strlen("OutputFile_2 Doesn't exist\n"));
          
  } else {
    char output_file2Permissions[10];
    sprintf(output_file2Permissions, "%o", sb2.st_mode);
    check("User", output_file2Permissions[strlen(output_file2Permissions) - 3],
          "output_file2");
    check("Group", output_file2Permissions[strlen(output_file2Permissions) - 2],
          "output_file2");
    check("Others", output_file2Permissions[strlen(output_file2Permissions) - 1],
          "output_file2");
  }
  

  return 0;
}