#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define maxLineSize 1024
#define maxArgsSize 64
#define whiteSpace " \t\r\n\a"

// forward decs
int cd(char **args);
int exit(char **args);
int mkdir(char **args);
int rmdir(char **args);
int del(char **args);
int cls(char **args);
int copy(char **args);
int echo(char **args);
int type(char **args);

typedef struct Command {
    const char *name;
    int (*fptr)(char **); // pointer to a function with char** args that returns int
} cmd;

cmd inbuiltList[] = {
    {"cd", &cd},
    {"exit", &exit},
    {"mkdir", &mkdir},
    {"rmdir", &rmdir},
    {"del", &del},
    {"cls", &cls},
    {"copy", &copy},
    {"echo", &echo},
    {"type", &type}
};

int exit(char **args) {
    printf("bye bye");
    return 0;
}

int cls(char **args) {
    printf("\e[1;1H\e[2J");
    return 0;
}

int echo(char **args) {
    if (args[1] == NULL) {
        printf("nah vro ts PMO");
    } else {
        printf("%s\n", args[1]);
    }

    return 0;
}

int type(char **args) {
    if (args[1] == NULL) {
        printf("no file? BUNS BUNS BUNSBUNS\n");
    } else {
        FILE *filePtr; 
        filePtr = fopen(args[1], "r");

        if (filePtr == NULL) {
            printf("your file is poop sorry :((( DIE\n");
        } else {
            char ch;
            while ((ch = fgetc(filePtr)) != EOF) {
                printf("%c", ch);
            }
        }
    }

    return 0;
}

int cd(char **args) {
    if (args[1] == NULL) {
        printf("genius masterwork you bum ass idiot\n");
    } else {
        if (!SetCurrentDirectory(args[1])) {
            printf("your shit is cooked and ass please STOP...\n");
        }
    }

    return 1;
}

int mkdir(char **args) {
    if (args[1] == NULL) {
        printf("soooo if you dont name it how tf am i going do that boy?\n");
    } else {
        if (!CreateDirectoryA(args[1], NULL)) {
            printf("YOU FAILED booooooo\n");
        }
    }

    return 1;
}

int rmdir(char **args) {
    if (args[1] == NULL) {
        printf("buns?\n");
    } else {
        if (!RemoveDirectoryA(args[1])) {
            printf("just stop bro\n");
        }
    }

    return 1;
}

int del(char **args) {
    if (args[1] == NULL) {
        printf("delete WHAT?\n");
    } else {
        if (!DeleteFileA(args[1])) {
            printf("yayaya yur poops\n");
        }
    }

    return 1;
}

int copy(char **args) {
    if ((args[1] == NULL) || (args[2] == NULL)) {
        printf("how about you die?\n");
    } else {
        if (!CopyFileA(args[1], args[2], false)) {
            printf("so we didn't  copy FUCK YOU\n");
        }
    }

    return 1;
}

int launchProcess(char **args) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, args[0], NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
        printf("you FAILED DIE\n");
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1;
}

int executeLine(char **args) {
    if (args[0] == NULL) { return 1; }

    //int test = sizeof(inbuiltList) / sizeof(cmd);
    //printf("%d", test);

    int numOfBuiltinCmds = (sizeof(inbuiltList) / sizeof(cmd));
    for (int i = 0; i < numOfBuiltinCmds; i++) {
        if (strcmp(args[0], inbuiltList[i].name) == 0) {
            return (*inbuiltList[i].fptr)(args);
        } 
    }

    return launchProcess(args);
}

char **getArgs(char *line) {
    int newBufferSize = maxArgsSize;
    char **argBuffer = (char **)malloc(sizeof(char *) * maxArgsSize);

    if (argBuffer == NULL) {
        fprintf(stderr, "fuck\n");
        exit(1); 
    }

    int pos = 0;
    char *token = strtok(line, whiteSpace);
    while (token != NULL) {
        argBuffer[pos] = token;
        pos++;

        if (pos >= newBufferSize) {
            newBufferSize += maxArgsSize;
            argBuffer = (char **)realloc(argBuffer, newBufferSize * sizeof(char *));

            if (argBuffer == NULL) {
                fprintf(stderr, "fuck\n");
                exit(1); 
            }
        }

        token = strtok(NULL, whiteSpace);
    }
    argBuffer[pos] = NULL;
    return argBuffer;
}

char *getLine() {
    int newBufferSize = maxLineSize;
    char *buffer = (char *)malloc(sizeof(char) * maxLineSize);

    if (buffer == NULL) { 
        fprintf(stderr, "fuck\n");
        exit(1); 
    }

    int pos = 0;
    int ch;
    while (true) {
        ch = getchar();

        if (ch == EOF || ch == '\n') {
            buffer[pos] = '\0';
            //debugPrint(buffer, sizeof(buffer));
            return buffer;
        }

        buffer[pos] = ch;
        pos++;

        if (pos >= newBufferSize) {
            newBufferSize += maxLineSize;
            buffer = (char *)realloc(buffer, newBufferSize);

            if (buffer == NULL) {
                fprintf(stderr, "fuck\n");
                exit(1);
            }
        }
    }
}

int main() {

    printf("SSShell - Super Sigma Shell. What's a copyright (father?)\n");

    int status;

    do {
        printf("ssshell >");

        char *command = getLine();
        char **args = getArgs(command);
        int status = executeLine(args);

    } while (status);
    

    return 0;    
}