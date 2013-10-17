/*
** gest_fichier.c for  in /Users/Ophelie/Sites/SpaceInvaders/space-invaders-master
** 
** Made by Ophelie Volga
** Login   <volga_o@etna-alternance.net>
** 
** Started on  Thu Oct 17 21:48:49 2013 Ophelie Volga
** Last update Thu Oct 17 21:48:51 2013 Ophelie Volga
*/
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
    char *text;
    int length;

    text = malloc(255 * sizeof(char));
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
            if (j == 1)
                text = "Fisrt";
            else if (j == 2)
                text = "Second";
            else
                text = "Third";
            length += sprintf(result + length, "%s %s\n", text, current);
            j++;
             i=0;
         }
     }
     fclose(f);
     return result;
}
