#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char names[1024][20000];
static int counts[20000];
static int sortCounts[20000];

char * strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;

  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;

  } else if (*src) {
    ret = src;
    src = NULL;
  }

  return ret;
}

int name_index(char *name)
{
    for (int i = 0; i < 20000; i++)
    {
        if (strcmp(names[i], "") == 0)
        {
            return -1;
        }
        if (strcmp(names[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int count_index(int count, int dup)
{
    int times = 0;
    for (int i = 0; i < 20000; i++)
    {
        if (counts[i] == 0)
        {
            return -1;
        }
        if (counts[i] == count)
        {
          if(times == dup) {
            return i;
          }
          times++;
        }
    }
    return -1;
}

void sort()
{
    int i, j, temp;
    for (i = 0; i < 20000 - 1; i++)
    {
        for (j = 0; j < 20000 - 1 - i; j++)
        {
            if (sortCounts[j] < sortCounts[j + 1])
            {
                temp = sortCounts[j];
                sortCounts[j] = sortCounts[j + 1];
                sortCounts[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    { //didn't recieve a csv file as arg
        printf("No arg\n");
        printf("Invalid Input Format\n");
        exit(0);
    }
    FILE *fp = NULL;
    char *line, *record;
    char buffer[1024];

    int nameCol = -1;
    int colNum = 0;
    int nameCount = 0;
    if ((fp = fopen(argv[1], "r")) != NULL)
    { //open cvs for reading
        int j = 0;
        for (int i = 0; i < 20000; i++)
        { //initialize count array
            counts[i] = 0;
        }

        if ((line = fgets(buffer, sizeof(buffer), fp)) != NULL) //get header line
        {
            //printf("%s\n", line);
            record = strtok_single(line, ","); //get first token deliminated by commas

            while (record != NULL)
            {
                if (strcmp(record, "name") == 0)
                {
                    nameCol = j;
                    //printf("%d\n", nameCol);
                }
                record = strtok_single(NULL, ","); //get next token
                j++;
            }
            colNum = j;
            j = 0;
        }

        if (nameCol == -1)
        {
          //couldn't find name in header
            printf("No name\n");
            printf("Invalid Input Format\n");
            exit(0);
        }

        while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)
        {
            record = strtok_single(line, ","); //get first column of csv

            while (record != NULL)
            {
              printf("%d %s\n", j, record);
                if (j == nameCol) //at name col
                {
                    int index = name_index(record); //find index for name in record
                    if (index == -1)
                    {
                      printf("%d\n", nameCount);
                      if(nameCount >= 20000) {
                        printf("Invalid Input Format\n");
                        exit(0);
                      }
                      
                        strcpy(names[nameCount], record); //add new name record
                        counts[nameCount] = 1;
                        nameCount++;
                    }
                    else
                    {
                        counts[index]++; //found existing name, increase count
                    }
                }
                record = strtok_single(NULL, ","); //get next column
                j++;
            }

            if (colNum != j)
            { //column number doesn't match header
                printf("col doesn't match\n");
                printf("Invalid Input Format\n");
                exit(0);
            }
            j = 0;
        }
        //close file
        fclose(fp);
        fp = NULL;
        for (int i = 0; i < 20000; i++)
        {
            sortCounts[i] = counts[i];
        }

        sort();
        int dup = 0;
        int count = sortCounts[0];
        int index = count_index(count, 0);
        printf("%s: %d\n", names[index], count);
        for (int i = 1; i < 10; i++)
        {
            count = sortCounts[i];
            if(count == sortCounts[i - 1]) {
              dup ++;
            } else {
              dup = 0;
            }
            if (count == 0)
                break;
            index = count_index(count, dup);
            printf("%s: %d\n", names[index], count);
        }
        return 1;
    }
    //couldn't open csv file
    printf("couldn't open csv\n");
    printf("Invalid Input Format\n");
    return 0;
}
