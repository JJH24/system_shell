#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <errno.h>

void sigign();
void handler1(int signo);
void handler2(int signo);
void interrupt();
int getargs(char *cmd, char **argv);
void change_dir(char **argv);
//void redirection();


char buf[256];
char *argv[50];
char path[100] = "/home/pray";
int narg, status;
int background = 0;
pid_t pid;


int main(){
	

	while(1){
	    getcwd(path, 100);
	    printf("cat@shell:%s$ ", path);
        gets(buf);

        if(!strcmp(buf, "exit")){
            kill(0, SIGINT);
        }


        if((background = (buf[strlen(buf)-1] == '&'))){
            buf[strlen(buf)-1] = '\0';
        }


        printf("narg: %d\n", narg = getargs(buf, argv));

        pid = fork();
	
      	if(pid == 0){
                //redirect_in(argv); 리다이렉션 인
                //redirect_out(argv);리다이렉션 아웃
               // pipe_cat(argv); 실패
                interrupt();

		    if(!strcmp(argv[0], "cd")){
		        change_dir(argv);
		    }
		    else{
                execvp(argv[0], argv);
		    }
		}
		else if(pid>0) {
            if (!background)
                wait((int)0);
        }
		else{
		    printf("failed\n");
		}
	}
}

void change_dir(char **argv){
    if(argv[1] == '\0'){
        chdir(getenv("HOME"));
    }
    else{
        if(chdir(argv[1]))
            printf("No directory\n");
    }
    return;
}

int getargs(char *cmd, char **argv){
	int narg = 0;
	while(*cmd){
		if(*cmd == ' ' || *cmd == '\t')
			*cmd++ = '\0';
		else{
			argv[narg++] = cmd++;
			while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
				cmd++;
		}
	}
	argv[narg] = NULL;
	return narg;
}

void handler1(int signo){
	kill(getpid(), SIGINT);
}
void handler2(int signo){

}

void interrupt(){
	struct sigaction act;
	act.sa_handler = handler1;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL);
}



/*
void redirect_in(char **argv){
    int i;
    int fd;

    for(i=0; argv[i] != NULL; i++){
        if(!strcmp(argv[i], "<"))
            break;
    }

    if(argv[i]){
        if(!argv[i+1]){
            return -1;
        }
        else{
            if((fd = open(argv[i+1], O_RDONLY)) == -1){
                perror(argv[i+1]);
                return -1;
            }
        }

        dup2(fd, STDIN_FILENO);
        close(fd);

        for(i=i; argv[i] != NULL; i++){
            argv[i] = argv[i+2];
        }

        argv[i] = NULL;
    }
}


void redirect_out(char **argv){
    int i;
    int fd;

    for(i=0; argv[i] != NULL; i++){
        if(!strcmp(argv[i], ">"))
            break;
    }

    if(argv[i]){
        if(!argv[i+1]){
            return -1;
        }
        else{
            if((fd = open(argv[i+1], O_RDWR | O_CREAT| S_IRUSR, 0644)) == -1){
                perror(argv[i+1]);
                return -1;
            }
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
        argv[i] = NULL;
        argv[i+1] = NULL;
        for(i=i; argv[i] != NULL; i++){
            argv[i] = argv[i+2];
        }

        argv[i] = NULL;
    }
}

void pipe_cat(char **argv){
    int i;
    int k = 0;
    pid_t pid1, pid2;
    int fd[2];
    char *cmdvectorPipe1;
    char *cmdvectorPipe2;

    for(i=0; argv[i] != NULL; i++){
        if(!strcmp(argv[i], "|")){
            break;
        }
    }
    
    cmdvectorPipe1 = argv[0];
    cmdvectorPipe2 = argv[i+1];


    if(strcmp(argv[i], "|") == 0){
        pipe(fd);

        pid1 = fork();
        switch(pid1){
            case -1: perror("fork error"); break;
            case 0:
                     redirect_in(cmdvectorPipe1);
                     dup2(fd[1], STDOUT_FILENO);
                     close(fd[1]);
                     close(fd[0]);
		     execvp(cmdvectorPipe1[0], cmdvectorPipe1);
        }
        pid2 = fork();
        switch(pid2){
            case -1: perror("fork error"); break;
            case 0:
                     redirect_out(cmdvectorPipe2);
                     dup2(fd[1], STDIN_FILENO);
                     close(fd[1]);
                     close(fd[0]);
		     execvp(cmdvectorPipe2[0], cmdvectorPipe2);
        }
    }
}*/
