#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char names[1024][20000];
static int counts[20000];
static int sortCounts[20000];

char *strxtok(char *buffer, char *match)
{
    static char *next_token = NULL;
    char *cp, *cp2, c, *rv;
    if (buffer)
        next_token = buffer;
    if (next_token && (*next_token == '\0'))
        return (NULL);
    cp = next_token;
    while (1)
    {
        c = *cp;
        if (c == '\0')
            return (NULL);
        for (cp2 = match; *cp2; cp2++)
            if (c == *cp2)
                break;
        if (*cp2 == 0)
            break;
        cp++;
    }
    next_token = cp;
    for (cp = next_token; *cp; cp++)
    {
        c = *cp;
        for (cp2 = match; *cp2; cp2++)
            if (c == *cp2)
            {
                *cp = '\0';
                rv = next_token;
                next_token = cp + 1;
                return (rv);
            }
    }
    rv = next_token;
    next_token = cp;
    return (rv);
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

int count_index(int count)
{
    for (int i = 0; i < 20000; i++)
    {
        if (counts[i] == 0)
        {
            return -1;
        }
        if (counts[i] == count)
        {
            return i;
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
    {
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
    {
        int j = 0;
        for (int i = 0; i < 20000; i++)
        {
            counts[i] = 0;
        }

        if ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)
        {
            record = strxtok(line, ",");

            while (record != NULL)
            {
                if (strcmp(record, "name") == 0)
                {
                    nameCol = j;
                }
                record = strxtok(NULL, ",");
                j++;
            }
            colNum = j;
            j = 0;
        }

        if (nameCol == -1)
        {
            printf("Invalid Input Format\n");
            exit(0);
        }

        while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)
        {
            record = strxtok(line, ",");
            while (record != NULL)
            {
                if (j == nameCol)
                {
                    int index = name_index(record);
                    if (index == -1)
                    {
                        strcpy(names[nameCount], record);
                        counts[nameCount] = 1;
                        nameCount++;
                    }
                    else
                    {
                        counts[index]++;
                    }
                }
                record = strxtok(NULL, ",");
                j++;
            }

            if (colNum != j)
            {
                printf("Invalid Input Format\n");
                exit(0);
            }
            j = 0;
        }
        fclose(fp);
        fp = NULL;
        for (int i = 0; i < 20000; i++)
        {
            sortCounts[i] = counts[i];
        }

        sort();

        for (int i = 0; i < 10; i++)
        {
            int count = sortCounts[i];
            if (count == 0)
                break;
            int index = count_index(count);
            printf("%s: %d\n", names[index], count);
        }
        return 1;
    }
    printf("Invalid Input Format\n");
    return 0;
}
