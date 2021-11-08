#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct entry{int year; int population; char boro[15];};
void read_csv();
void read_data();
void add_data();
void update_data();


void read_csv(){
    // open file
    int foo = open("nyc_pop.csv", O_RDONLY);
    if (foo == -1){
        printf("Error: %s\n\n", strerror(errno));
        return;
    }

    printf("reading nyc_pop.csv\n");

    struct stat *fileinput = malloc(sizeof(struct stat));
    stat("nyc_pop.csv", fileinput);

    // read file
    char *buffer = calloc(1, fileinput->st_size);
    read(foo, buffer, fileinput->st_size);

    char *tmpp = buffer;

    char *headings[5] = {"Manhattan", "Brooklyn", "Queens", "Bronx", "Staten Island"};

    struct entry database[128];
    int line;
    int j;
    int line_data[5];
    int year;

    tmpp = strchr(tmpp, '\n') + 1;

    for (line = 0; line < 23; line++){
        sscanf(tmpp, "%d,%d,%d,%d,%d,%d\n", &year, &line_data[0], &line_data[1], &line_data[2], &line_data[3], &line_data[4]);

        for (j = 0; j < 5; j++){
 
            database[5*line+j].year = year;
            strcpy(database[5*line+j].boro, headings[j]);
            database[5*line+j].population = line_data[j];
        }

        tmpp = strchr(tmpp, '\n') + 1;
    }

    // testing database has right values
    // for (line = 0; line < 23 * 5; line++){
    //     printf("%d, %s, %d\n", database[line].year, database[line].boro, database[line].population);
    // }

    // create new file
    int goo = open("nyc_pop.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (goo == -1){
        printf("Error: %s\n\n", strerror(errno));
        return;
    }

    int gr = write(goo, database, sizeof(database));

    struct stat *fileoutput = malloc(sizeof(struct stat));
    stat("nyc_pop.txt", fileoutput);
    int size = fileoutput->st_size;

    printf("Wrote %d bytes to nyc_pop.txt\n\n", size);

    printf("DONE\n");
}

void read_data(){
    struct entry *database = calloc(115, sizeof(struct entry));
    int goo = open("./nyc_pop.txt", O_RDONLY);
    int len = read(goo, database, sizeof(database));
    int i;
    for (i = 0; i < 115; i++){
        printf("%d: year: %d\tboro: %s\tpop: %d\n", i, database[i].year, database[i].boro, database[i].population);
    }
}

void add_data(){
    printf("Enter year boro pop: ");
    struct entry tmp;
    char buffer[128], boro[15];
    fgets(buffer, 128, stdin);
    sscanf(buffer, "%d %s %d\n", &tmp.year, tmp.boro, &tmp.population);

    int goo = open("./nyc_pop.txt", O_WRONLY | O_APPEND);

    int humph = write(goo, &tmp, sizeof(tmp));

    printf("Appended data to file: year: %d\t boro: %s\t population: %d\n", tmp.year, tmp.boro, tmp.population);
}

void update_data(){
    int goo = open("./nyc_pop.txt", O_RDWR | O_APPEND);

    struct stat *fileinput = malloc(sizeof(struct stat));
    stat("nyc_pop.csv", fileinput);
    int size = fileinput->st_size;
    int entries = size / sizeof(struct entry);

    int change_this_index;

    printf("entry to update: ");
    char changes[128];
    fgets(changes, 128, stdin);
    sscanf(changes, "%d\n", &change_this_index);
    
    printf("Enter year boro pop: ");
    struct entry tmp;
    char buffer[128], boro[15];
    fgets(buffer, 128, stdin);
    sscanf(buffer, "%d %s %d\n", &tmp.year, tmp.boro, &tmp.population);

    lseek(goo, change_this_index * sizeof(struct entry), SEEK_SET);

    int humphies = write(goo, &tmp, sizeof(struct entry));

    printf("File updated.\n");
}

int main(int argc, char *argv[]){
    char input[15];

    // if (argc == 1){
    //     printf("Please enter what you would like to do\n> ");
    //     fgets(input, 15, stdin);
    //     *strchr(input, '\n') = '\0';
    // }
    // else{
    //     strcpy(input, argv[1]);

    fgets(input, 15, stdin);
    if (strcmp(input, "-read_csv") == 0){
        read_csv();
    }
    else if (strcmp(input, "-read_data") == 0){
        read_data();
    }
    else if (strcmp(input, "-add_data") == 0){
        add_data();
    }
    else if (strcmp(input, "-update_data") == 0){
        update_data();
    }

    return 0;
}
