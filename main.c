#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_CHAR_BUFFER 256
#define TABLE_SIZE 20

// aliases from https://github.com/aLEGEND21/BrainrotLang/blob/master/src/aliases.ts
char aliases[TABLE_SIZE][2][MAX_CHAR_BUFFER] = {
    {"cooked", "break"},
    {"opp", "catch"},
    {"sigma", "const"},
    {"jelq", "continue"},
    {"zesty", "else"},
    {"cap", "false"},
    {"cringe", "finally"},
    {"ratio", "for"},
    {"diddy", "function"},
    {"ohio", "if"},
    {"mew", "in"},
    {"sus", "let"},
    {"shyt", "null"},
    {"looksmaxx", "return"},
    {"aura", "true"},
    {"edge", "try"},
    {"mog", "typeof"},
    {"alpha", "var"},
    {"goon", "while"},
    {"yap", "console.log"}
};

int main(int argc, char **argv) {
    // check command syntax
    if (argc != 2) {
        fprintf(stderr, "brlc [file]");
        return -1;
    }
    
    if (access(argv[1], F_OK) != 0) {
        fprintf(stderr, "%s%s\n", "no such file exists ", argv[1]);
        return -1;
    }
    


    // read source file
    char *src = NULL;
    size_t size = 0;
    FILE *fp_src = fopen(argv[1], "r");

    // get size of file
    fseek(fp_src, 0, SEEK_END);
    size = ftell(fp_src);

    // set pointer back to start of file
    rewind(fp_src);

    // allocate memory in src buffer
    src = malloc((size + 6) * sizeof(*src));

    // read file and write to buffer
    fread(src, size, 1, fp_src);
    src[size] = '\0';



    // start compile timer
    clock_t start_compile = clock();
    // start compiling

    // replace string
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];

    for (int i = 0; i < TABLE_SIZE; i++) {
        const char *tmp = src;
        size_t repl_len = strlen(aliases[i][1]);

        while (1) {
            const char *p = strstr(tmp, aliases[i][0]);

            // if no more occurances of alias, write rest of file
            if (p == NULL) {
                strcpy(insert_point, tmp);
                break;
            }

            // insert before alias
            memcpy(insert_point, tmp, p - tmp);
            insert_point += p - tmp;

            // replace alias
            memcpy(insert_point, aliases[i][1], repl_len);
            insert_point += repl_len;

            // adjust tmp pointer
            tmp = p + strlen(aliases[i][0]);
        }

        // rewind insert pointer to start of buffer
        insert_point = &buffer[0];
        // write buffer back to target
        strcpy(src, buffer);
    }
    
    // write to file
    FILE *fp_out = fopen("out.js", "w");
    fprintf(fp_out, "%s", src);
    fclose(fp_out);
    fflush(fp_out);

    // end compile timer
    clock_t end_compile = clock();

    // execute and get execution time
    FILE *fp;
    char exec_time[32];
    fp = popen("strace -c node out.js 2>&1 | awk \'END{print $2}\'", "r");

    // read stdout
    fgets(exec_time, sizeof(exec_time), fp);

    pclose(fp);

    printf("%s\t\t%f\n", "compile time: ", (double)(end_compile - start_compile) / CLOCKS_PER_SEC);
    printf("%s\t%s\n", "execution time: ", exec_time);
}
