#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
#define NOT_FOUND -1

char *delete(char *source, int index, int n);
char *do_edit(char *source, char command);
char get_command(void);
char *insert(char *source, const char *to_insert, int index);
int pos(const char *source, const char *to_find);

int main(void)
{
      char source[MAX_LEN], command;
      printf("Enter the source string:\n> ");
      gets(source);

      for (command = get_command(); command != 'Q'; command = get_command())
      {
          do_edit(source, command);
          printf("New source: %s\n\n", source);

      }

      printf("String after editing: %s\n", source);
      return (0);
}

char* delete(char *source,  /* input/output - string from which to delete part   */
       int   index,     /* input - index of first char to delete           */
       int   n)         /* input - number of chars to delete               */
{
    char rest_str_1[MAX_LEN]; /* copy of source substring following
                             characters to delete */
    
    char rest_str2[MAX_LEN]; /* copy of source substring following
                              characters to delete */
    
    int length_val = strlen(source);
    
    /* If there are no characters in source following portion to
     delete, delete rest of string */
    if (strlen(source) <= index + n) {
        source[index] = '\0';
        
        /* Otherwise, copy the portion following the portion to delete
         and place it in source beginning at the index position          */
    } else {
        strncpy(rest_str_1, source, index);
        strncpy(rest_str2, &source[index + n], n);
        
        strcat(rest_str_1, rest_str2);
        
        
        strcpy(source, rest_str_1);
    }
    
    return (source);
    
}


char* do_edit(char *source, /* input/output - string to modify or search */
        char command) /* input - character indicating operation    */
{
        char str[MAX_LEN]; /* work string */
        int index;
        int num;

        switch (command) {
        case 'D':
                printf("String to delete> ");
                scanf("%d", &num);
                delete(source, index, num);
                
              break;

       case 'I':
            //
            break;

       case 'F':
         
                printf("String to find> ");

                pos(source, str);
             break;

       default:
             printf("Invalid edit command '%c'\n", command);
       }

       return (source);
}

char get_command(void)
{
       char command, ignore;

       printf("Enter D(Delete), I(Insert), F(Find), or Q(Quit)> ");
       scanf(" %c", &command);

       do
            ignore = getchar();
       while (ignore != '\n');

       return (toupper(command));
}

char* insert(char       *source,    /* input/output - target of insertion */
       const char *to_insert, /* input - string to insert           */

        int index)            /* input - position where to_insert
                                         is to be inserted          */
{
       char rest_str[MAX_LEN]; /* copy of rest of source beginning
                                  with source[index] */

       if (strlen(source) <= index) {
             strcat(source, to_insert);
       } else {
             //
       }

       return (source);
}

int pos(const char *source,  /* input - string in which to look for to_find */
    const char *to_find) /* input - string to find                      */

{
        int i = 0, find_len, found = 0, position;
        char substring[MAX_LEN];

        find_len = strlen(to_find);
        while (!found && i <= strlen(source) - find_len) {
            strncpy(substring, &source[i], find_len);
            substring[find_len] = '\0';

            if (strcmp(substring, to_find) == 0)
                  found = 1;
            else
                  ++i;
        }

            if (found)
                  position = i;
            else
                  position = NOT_FOUND;

            return (position);
}
