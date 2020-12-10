// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "howard.edu";
    char another_fixed_str[] = "gobison.org";
    char input_str[100]; 
  
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        char concat_str[100];
        close(fd1[0]);  // Close reading end of pipes 
        //close(fd2[0]);
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]); //jj
        
  
        // Wait for child to send a string 
        wait(NULL); 
  
        close(fd2[1]); // Close writing end of pipes 
        //close(fd1[1]);
      
        char temp[100];
        read(fd2[0], temp, 100); 
        close(fd2[0]);
        printf("First concatenated string: %s\n", temp); 

        int i;

        // Copy the first concatanated string i.e temp to concat_str
        for (i = 0; i < strlen(temp);i++){
          concat_str[i] = temp[i];
        }

        int k = strlen(concat_str);
        int j;

        // Concatanate "gobison.org" to the concat_str
        for(j = 0; j < strlen(another_fixed_str); j++){
          concat_str[k++] = another_fixed_str[j];
        }
        concat_str[k] = '\0'; // string ends with '\0'
        printf("Final string after further concatanation %s\n", concat_str);
        printf("\n");
      }
   
  
    // child process 
    else
  { 
      // Read a string using first pipe 
      char concat_str[100]; 
      //char thing[100];
    
      close(fd1[1]);  // Close writing end of first pipe 
      
      read(fd1[0], concat_str, 100);
      close(fd1[0]);
        
      // Concatenate a fixed string with it 
      int k = strlen(concat_str); 
      int i; 
      for (i=0; i<strlen(fixed_str); i++) 
          concat_str[k++] = fixed_str[i]; 
      concat_str[k] = '\0';   // string ends with '\0' 

      // Close reading end
      close(fd2[0]); 
    
      write(fd2[1], concat_str, strlen(concat_str)+1); 
      close(fd2[1]);
      exit(0); 
  } 
} 