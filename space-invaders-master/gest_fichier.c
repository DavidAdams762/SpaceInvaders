#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "space.h"

int file_exists(const char * filename)
{
    FILE *file  = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void create_file_scores(char *file)
{
    FILE *f;
    long lfile;

    f = fopen(file, "w");
    fseek(f, 0, SEEK_END);
    lfile=ftell(f);
    if(lfile==0)
    fprintf (f, "0;0;0;\n");
    fclose(f);
}

char *get_scores(char *file, int pts)
{
    FILE *f;
    char *tmp = malloc(255 * sizeof(char));
    char current[255];
    int length;
    int i;
    int c;
    i = 0;
    length = 0;

    f = fopen(file, "r");
    fseek(f, 0, SEEK_SET);

     while((c = fgetc(f)) != EOF && c != '\n')  {
        if (c != ';') {
            current[i] = c;
            i++;
        }
        if (c == ';') {
            current[i] = '\0';
            if (pts < atoi(current))   {
                length += sprintf(tmp + length, "%s;", current);
            } else {
                length += sprintf(tmp + length, "%d;", pts);
                pts = atoi(current);
            }
            i=0;
         }
     }
     fclose(f);
     return tmp;
}

void change_scores(char *file, char *tmp)
{
    FILE *f;
    f = fopen(file, "w");
    fseek(f, 0, SEEK_END);
    fprintf (f, "%s\n", tmp);
    fclose(f);
}

char *get_format_scores(char *file)
{
    FILE *f;
    char *result;
    int i;
    int j;
    int c;
    char current[255];
    int length;
    result = malloc(255 * sizeof(char));
    i = 0;
    length = 0;

    f = fopen(file, "r");
    fseek(f, 0, SEEK_SET);
     i = 0;
     j = 1;
     length = 0;
     while((c = fgetc(f)) != EOF && c != '\n')  {
        if (c != ';') {
            current[i] = c;
            i++;
        }
        if (c == ';') {
            current[i] = '\0';
            length += sprintf(result + length, "%d. %s\n", j, current);
            j++;
             i=0;
         }
     }
     fclose(f);
     return result;
}
