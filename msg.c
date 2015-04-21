/* 20:07 2015-04-21 Tuesday */
#include <head.h>

typedef struct
{
    long type;
    char buf[1024];
}MSG;

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr,"Usage: %s sndtype rcvtype\n",argv[0]);
            exit(EXIT_FAILURE);
    }

    int sndtype;
    int rcvtype;
    sndtype = atoi(argv[1]);
    rcvtype = atoi(argv[2]);

    MSG msg={sndtype,""};
    int msg_id;
    key_t key;
    pid_t pid;


    key = ftok(".",'k');
    if (key < 0)
    {
        perror("Fail to ftok");
        exit(EXIT_FAILURE);
    }
    
    msg_id = msgget(key,IPC_CREAT|0666);
    if (msg_id < 0) 
    {
        perror("Fail to msgget");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        while (1)
        {
            fgets(msg.buf,sizeof(msg.buf),stdin);
            msg.buf[strlen(msg.buf)-1] = '\0';

            if (msgsnd(msg_id,&msg,sizeof(msg)-sizeof(long),0) < 0)  
            {
                perror("Fail to msgsnd");
                exit(EXIT_FAILURE);
            }

            if (strncmp(msg.buf,"quit",4) == 0)
            {
                break;
            }
        }
    }
    if (pid == 0)
    {
        while (1)
        {
            if (msgrcv(msg_id,&msg,sizeof(msg)-sizeof(long),rcvtype,0) != 0)
            {
                printf("============\n");
                fprintf(stdout,"RCV From %d : %s\n",rcvtype,msg.buf);
                printf("============\n");
            }

            if (strncmp(msg.buf,"quit",4) == 0)
            {
                break;
            }
        }
    }

    return 0;
}
