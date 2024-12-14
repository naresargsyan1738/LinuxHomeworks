#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>

void split(char *input, char **result)
{
        char *tmp = strtok(input, " ");
        int i = 0;
        while(tmp != nullptr)
        {
                result[i] = tmp;
                i++;
                tmp = strtok(nullptr, " ");
        }
        result[i] = nullptr;
}

void execute(char *input)
{
 while(*input == ' ') input++;
 if(strncmp(input, "exit", 4) == 0)
 {
  exit(0);
 }
 if(strncmp(input, "silent", 6) == 0)
 {
  input += 6;
  pid_t pid = fork();
  if(pid < 0)
  {
   std::cout << "Error fork" << std::endl;
   exit(errno); 
  }
  char *result[256];
  if(pid == 0)
  {
   std::string filename = std::to_string(getpid()) + ".log";
   close(1);
   int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 0644);
   if(fd < 0)
   {
    std::cout << "Error opening file" << std::endl;
    exit(errno);
   }
   split(input, result);
   execvp(result[0], result);
   std::cout << "Command not found" << std::endl;
   exit(1);
  }
  else
  {
   wait(NULL);
   return;
  }
 }
        pid_t pid = fork();
        if(pid < 0)
        {
        std::cout << "Error fork" << std::endl;
         exit(errno);
 }
 char *result[256];
        if(pid == 0)
        {
         split(input, result);
         execvp(result[0], result);
         std::cout << "Command not found" << std::endl;
         exit(1);
       }
        else
        {
                wait(NULL);
        } 
}

int main()
{
 char input[256];
 while(true)
 {
  std::cin.getline(input, 256);
  execute(input);
 }
}
