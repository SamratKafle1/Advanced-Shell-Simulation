#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <sys/stat.h>

#define MAX_USERS 32
#define BUF 512

typedef struct {char user[64]; char role[16]; char hash[65];} User;
User users[MAX_USERS]; int user_count=0;
char current_user[64]=""; char current_role[16]=""; int sudo_mode=0;

void sha256_hex(const char *input, char out[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input, strlen(input), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) sprintf(out + i*2, "%02x", hash[i]);
    out[64]='\0';
}

void ensure_default_users(){
    struct stat st={0};
    if (stat("users.db", &st)==-1){
        FILE *f=fopen("users.db","w");
        char h1[65],h2[65];
        sha256_hex("admin123",h1); sha256_hex("user123",h2);
        fprintf(f,"admin:admin:%s\n",h1);
        fprintf(f,"user:standard:%s\n",h2);
        fclose(f);
    }
}

void load_users(){
    ensure_default_users();
    FILE *f=fopen("users.db","r"); if(!f){perror("users.db"); exit(1);} 
    char line[256]; user_count=0;
    while(fgets(line,sizeof(line),f)&&user_count<MAX_USERS){
        line[strcspn(line,"\n")]='\0';
        if(strlen(line)<5) continue;
        sscanf(line,"%63[^:]:%15[^:]:%64s", users[user_count].user, users[user_count].role, users[user_count].hash);
        user_count++;
    }
    fclose(f);
}

int login(){
    char u[64],p[64],h[65];
    printf("=== Advanced Shell Simulation Login ===\n");
    printf("Username: "); fflush(stdout); if(!fgets(u,sizeof(u),stdin)) return 0; u[strcspn(u,"\n")]='\0';
    printf("Password: "); fflush(stdout); if(!fgets(p,sizeof(p),stdin)) return 0; p[strcspn(p,"\n")]='\0';
    sha256_hex(p,h);
    for(int i=0;i<user_count;i++){
        if(strcmp(u,users[i].user)==0 && strcmp(h,users[i].hash)==0){
            strcpy(current_user,u); strcpy(current_role,users[i].role);
            printf("Login successful. Role: %s\n", current_role);
            return 1;
        }
    }
    printf("Login failed.\n"); return 0;
}

void demo_scheduler(const char *mode){
    printf("Scheduler mode: %s\n", mode);
    if(strcmp(mode,"rr")==0){
        printf("Ready Queue: [cpu_job_A pid=2001 burst=5] [cpu_job_B pid=2002 burst=5]\n");
        printf("Quantum=2 -> Dispatch order: 2001 -> 2002 -> 2001 -> 2002\n");
    } else {
        printf("Ready Queue: [cpu_job_B pid=2002 prio=1] [cpu_job_A pid=2001 prio=5]\n");
        printf("Dispatch order: 2002 -> 2001\n");
    }
}

void demo_paging(const char *mode){
    printf("Page replacement mode: %s\n", mode);
    if(strcmp(mode,"fifo")==0){
        printf("Page Table: P0->F1 P1->F2 P2->F3 P3->MISS\n");
        printf("FIFO Evict: P0, Load P3 into F1\n");
    } else {
        printf("Page Table: P0->F1 P1->F2 P2->F3 P3->MISS\n");
        printf("LRU Evict: P1, Load P3 into F2\n");
    }
}

int buffer=0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void*arg){
    (void)arg;
    pthread_mutex_lock(&mtx);
    while(buffer==1) pthread_cond_wait(&cond,&mtx);
    buffer=1; printf("Producer: produced item\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    return NULL;
}
void* consumer(void*arg){
    (void)arg;
    pthread_mutex_lock(&mtx);
    while(buffer==0) pthread_cond_wait(&cond,&mtx);
    buffer=0; printf("Consumer: consumed item\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void run_sync_demo(){
    pthread_t p,c;
    buffer=0;
    pthread_create(&c,NULL,consumer,NULL);
    usleep(50000);
    pthread_create(&p,NULL,producer,NULL);
    pthread_join(p,NULL); pthread_join(c,NULL);
    printf("Synchronization demo complete (mutex + condition variable).\n");
}

void prompt(){
    char cmd[BUF];
    while(1){
        printf("%s@adv-shell$ ", current_user); fflush(stdout);
        if(!fgets(cmd,sizeof(cmd),stdin)) break;
        cmd[strcspn(cmd,"\n")]='\0';
        if(strcmp(cmd,"exit")==0) break;
        else if(strcmp(cmd,"help")==0){
            printf("Commands: ls|grep etc, scheduler rr|priority, paging fifo|lru, sync_demo, su <user> <pass>, sudo <pass>, cat admin_secret.txt, exit\n");
        } else if(strncmp(cmd,"scheduler ",10)==0){
            demo_scheduler(cmd+10);
        } else if(strncmp(cmd,"paging ",7)==0){
            demo_paging(cmd+7);
        } else if(strcmp(cmd,"sync_demo")==0){
            run_sync_demo();
        } else if(strncmp(cmd,"su ",3)==0){
            char u[64],p[64],h[65];
            if(sscanf(cmd+3,"%63s %63s",u,p)==2){
                sha256_hex(p,h);
                int ok=0;
                for(int i=0;i<user_count;i++) if(strcmp(u,users[i].user)==0&&strcmp(h,users[i].hash)==0){strcpy(current_user,u);strcpy(current_role,users[i].role);sudo_mode=0;ok=1;}
                printf(ok?"Switched user to %s (%s)\n":"su failed\n", current_user,current_role);
            }
        } else if(strncmp(cmd,"sudo ",5)==0){
            char p[64],h[65]; sscanf(cmd+5,"%63s",p); sha256_hex(p,h);
            for(int i=0;i<user_count;i++) if(strcmp("admin",users[i].user)==0 && strcmp(h,users[i].hash)==0){sudo_mode=1; printf("sudo enabled for next privileged command\n"); goto done;}
            printf("sudo authentication failed\n");
            done:;
        } else if(strcmp(cmd,"cat admin_secret.txt")==0){
            if(strcmp(current_role,"admin")==0||sudo_mode){
                printf("[admin_secret] root_key=SIM-42-SECRET\n");
                sudo_mode=0;
            } else {
                printf("Permission denied: admin-only file\n");
            }
        } else {
            int rc = system(cmd);
            (void)rc;
        }
    }
}

int main(){
    load_users();
    if(!login()) return 1;
    prompt();
    printf("Goodbye.\n");
    return 0;
}
