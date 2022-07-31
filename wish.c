/* Olli Häkli wish.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX 100

int main(int argc, char *argv[]) {

    //Muuttujien alustus ja määrittely
    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    char *token;
    char dir[100];
    char directory[MAX];
    char initPath[MAX] = "/bin/";
    char path[MAX];
    char *myargs[MAX];
    char error_message[30] = "An error has occurred\n";

    buffer = (char *)malloc(bufsize * sizeof(char));
            if (buffer == NULL) {
                printf("error: malloc failed");
                exit(1);
            }
    // Toiminnallinen osuus
    if (argc < 2) { //Jos ei annettu tiedostoa, interactive mode
        while(1) {
            printf("wish>"); 
            getline(&buffer, &bufsize, stdin); //luetaan syöte bufferiin
            buffer[strlen(buffer)-1] = '\0';
            token = strtok(buffer, " "); //parsitaan ensimmäinen sana
            while(token != NULL) { //käydään syöte läpi sana sanalta
                //Lopetus
                if((strcmp(token, "exit")) == 0) { 
                    token = strtok(NULL, " ");
                    if(token == NULL) {
                        exit(0);
                    } else {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        //printf("exit error");
                        exit(1);
                    }
                // cd
                }if((strcmp(token, "cd")) == 0) {
                    token = strtok(NULL, " ");
                    if(token == NULL) { //ei argumenttia -> exit
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        //printf("no arguments after cd");
                        exit(1);
                    } else {
                        strcpy(directory, token);
                        char s[MAX];
                        printf("%s\n", getcwd(s, 100));
                        token = strtok(NULL, " ");
                        if(token != NULL) { // Liikaa argumentteja -> exit
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            //printf("Too many arguments after cd"); 
                            exit(1);
                        } else {
                            
                            //strcpy(directory, strcat("/", directory));
                            //printf("%s", directory);
                            
                            if (chdir(directory) != 0) {
                                write(STDERR_FILENO, error_message, strlen(error_message));
                                //printf("error cd failed");
                                exit(1);
                            }
                            printf("%s\n", getcwd(s, 100));
                            continue;    
                        }
                    }
                }
                //path
                if((strcmp(token, "path")) == 0) {
                    token = strtok(NULL, " ");
                    strcpy(path, token);
                    break;
                    

                //Ohjelman suoritus
                } else {
                    strcpy(initPath, "/bin/");
                    strcpy(path, buffer);
                    strcat(initPath, path);
                    if(access(initPath, X_OK) != 0) {  //testataan löytyykö tiedostoa
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        //printf("error ei löydy");
                        break;
                    }
                    int rc = fork(); //lähde: https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf
                    if (rc < 0) { // fork failed; exit
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } else if (rc == 0) { // child (new process)
                        //myargs[0] = strdup(token);
                        myargs[0] = initPath;
                        myargs[1] = NULL;
                        printf("lapsi tämä toimii");
                        execv(myargs[0], myargs);

                    } else { // parent goes down this path (main)
                        int rc_wait = wait(NULL);
                        printf("vanhempi toimii");
                    }
                    token = strtok(NULL, " ");
                    free(buffer);
                }
            }
        }
    } else if(argc == 2) { // Batch Sama toiminnallisuus mutta tiedostosta lukien.
        char tiedostoNimi[MAX];
        strcpy(tiedostoNimi, argv[1]);
        printf("%s", tiedostoNimi);
        //buffer = NULL;
        //bufsize = 0;
        FILE *tiedosto; //Tiedoston luku
        if ((tiedosto = fopen(tiedostoNimi, "r")) == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", tiedostoNimi);
            exit(1);
        }
        while (getline(&buffer, &bufsize, tiedosto) != -1){
            printf("%s", buffer);
            buffer[strlen(buffer)-1] = '\0';
            token = strtok(buffer, " "); //parsitaan ensimmäinen sana
            while(token != NULL) {
                //Lopetus
                if((strcmp(token, "exit")) == 0) { 
                    token = strtok(NULL, " ");
                    if(token == NULL) {
                        printf("lopetus");
                        exit(0);
                    } else {
                        printf("exit error");
                        exit(1);
                    }
                // cd
                }if((strcmp(token, "cd")) == 0) {
                    token = strtok(NULL, " ");
                    if(token == NULL) { //ei argumenttia -> exit
                        printf("no arguments after cd");
                        exit(1);
                    } else {
                        strcpy(directory, token);

                        printf("%s", directory);
                        token = strtok(NULL, " ");
                        if(token != NULL) { // Liikaa argumentteja -> exit
                            printf("Too many arguments after cd"); 
                            exit(1);
                        } else {
                            
                            //strcpy(directory, strcat("/", directory));
                            //printf("%s", directory);
                            
                            if (chdir(directory) != 0) {
                                printf("error cd failed");
                                exit(1);
                            }
                            printf("%s\n", getcwd(directory, 100));
                            continue;    
                        }
                    }
                }
                //path
                if((strcmp(token, "path")) == 0) {
                    token = strtok(NULL, " ");
                    strcpy(path, token);
                    break;
                    

                //Ohjelman suoritus
                } else {
         
                    strcpy(path, token);
                    strcat(initPath, path);
                    if(access(initPath, X_OK) != 0) {  //testataan löytyykö tiedostoa
                            printf("error ei löydy");
                            break;
                        }
                    int rc = fork(); //lähde: https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf
                    if (rc < 0) { // fork failed; exit
                        fprintf(stderr, "fork failed\n");
                        exit(1);
                    } else if (rc == 0) { // child (new process)
                        //myargs[0] = strdup(token);
                        myargs[0] = initPath;
                        myargs[1] = NULL;
                        printf("lapsi tämä toimii");
                        execv(myargs[0], myargs);

                    } else { // parent goes down this path (main)
                        int rc_wait = wait(NULL);
                        printf("vanhempi toimii");
                    }
                    token = strtok(NULL, " ");
                    continue;
                }
            }
            free(buffer);
        }

        fclose(tiedosto);

    } else{ //Ei interactive eikä batch mode
        printf("Too many arguments");
        exit(1);
    }

    return(0);
}
/*EOF*/
