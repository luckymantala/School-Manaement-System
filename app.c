#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

int main(void);
void input(void);
void display(void);
void search(void);
void searchByRoll(void);
void searchByName(void);
int strcmpi(char *, char *);
void modify(void);
void del(void);
void sort(void);
void sortByRoll(void);
void sortByName(void);
/* sleep until a key is pressed and return value. echo = 0 disables key echo. */
int keypress(unsigned char echo)
{
    struct termios savedState, newState;
    int c;

    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;     /* error on tcgetattr */
    }

    newState = savedState;

    if ((echo = !echo)) /* yes i'm doing an assignment in an if clause */
    {
        echo = ECHO;    /* echo bit to disable echo */
    }

    /* disable canonical input and disable echo.  set minimal input to 1. */
    newState.c_lflag &= ~(echo | ICANON);
    newState.c_cc[VMIN] = 1;

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;     /* error on tcsetattr */
    }

    c = getchar();      /* block (withot spinning) until we get a keypress */

    /* restore the saved state */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;     /* error on tcsetattr */
    }

    return c;
}

struct student
{
    char na[30];
    char cl[10];
    int rl;
    char date[12];
} s;

FILE *fp;
long sz = sizeof(s);
char enter;
// void clear(void)
// {
//     while (getchar() != '\n')
//         ;
// }

int main()
{

    int ch;

    while (1)
    {
        system("clear");
        printf("School Managerment\n");
        printf("1.Add Student\n");
        printf("2.Display All Students Data\n");
        printf("3.Search\n");
        printf("4.Modify\n");
        printf("5.Delete\n");
        printf("6.Sort\n");
        printf("0.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 0:
            exit(0);
        case 1:
            input();
            break;
        case 2:
            display();
            break;
        case 3:
            search();
            break;
        case 4:
            modify();
            break;
        case 5:
            del();
            break;
        case 6:
            sort();
            break;
        }

        printf("Hit TAB to continue...");
        #ifdef _WIN32
            getch();
        #elif __linux__
            while(keypress(0) != '\t');
        #endif    
        // scanf(" %c", &enter);
    }

    return 0;
}

void input()
{
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(s.date, myDate);

    fp = fopen("stud.txt", "ab");
    printf("Enter student name ");
    // fgets(s.na, 30, stdin);
    scanf(" %[^\n]", s.na);
    printf("Enter class ");
    // fgets(s.cl, 10, stdin);
    scanf(" %[^\n]", s.cl);
    printf("Enter roll no ");
    scanf("%d", &s.rl);
    fwrite(&s, sz, 1, fp);
    printf("Record saved sucessfully...\n");
    fclose(fp);
}

void display()
{
    system("clear");
    printf("Student info\n");
    printf("%-30s %-20s %-20s %s\n", "NAME", "CLASS", "ROLL", "DATE");
    fp = fopen("stud.txt", "rb");
    while (fread(&s, sz, 1, fp) == 1)
    {
        printf("%-30s %-20s %-20d %s\n", s.na, s.cl, s.rl, s.date);
    }
    fclose(fp);
}

void search()
{
    int ch;
    while (1)
    {
        system("clear");
        printf("===>Search<===\n");
        printf("1.Search by Roll\n");
        printf("2.Search by Name\n");
        printf("0. Back to Main Menu\n");
        printf("\n\n Enter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 0:
            main();
            break;

        case 1:
            searchByRoll();
            break;

        case 2:
            searchByName();
            break;

        default:
            printf("Invalid choice\n");
        }
        printf("Hit TAB to continie...");
        #ifdef _WIN32
            getch();
        #elif __linux__
            while(keypress(0) != '\t');
        #endif    
    }
}

void searchByRoll()
{
    int rl, f = 0;

    fp = fopen("stud.txt", "rb");

    printf("Enter roll to search: ");
    scanf("%d", &rl);

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (s.rl == rl)
        {
            f = 1;
            printf("%-30s %-20s %-20s %s\n", "NAME", "CLASS", "ROLL", "DATE");
            printf("%-30s %-20s %-20d %s\n", s.na, s.cl, s.rl, s.date);
            break;
        }
    }
    fclose(fp);
    if (f == 1)
    {
        printf("Record found sucessfully\n");
    }
    else
    {
        printf("Record not found\n");
    }
}
void searchByName()
{
    char na[30];
    int f = 0;

    fp = fopen("stud.txt", "rb");

    printf("Enter name to search: ");
    // fgets(na,30,stdin);
    scanf(" %[^\n]", na);

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (strcmpi(na, s.na) == 0)
        {
            f = 1;
            printf("%-30s %-20s %-20s %s\n", "NAME", "CLASS", "ROLL", "DATE");
            printf("%-30s %-20s %-20d %s\n", s.na, s.cl, s.rl, s.date);
            break;
        }
    }
    fclose(fp);
    if (f == 1)
    {
        printf("Record found sucessfully\n");
    }
    else
    {
        printf("Record not found\n");
    }
}

int strcmpi(char *s1, char *s2)
{
    int i;

    if (strlen(s1) != strlen(s2))
        return -1;

    for (i = 0; i < strlen(s1); i++)
    {
        if (toupper(s1[i]) != toupper(s2[i]))
            return s1[i] - s2[i];
    }
    return 0;
}

void modify()
{
    int rl, f = 0;

    fp = fopen("stud.txt", "rb+");

    printf("Enter roll num to Modify: ");
    scanf("%d", &rl);

    while (fread(&s, sz, 1, fp) == 1)
    {
        if (s.rl == rl)
        {
            f = 1;
            printf("Enter student name ");
            // fgets(s.na, 30, stdin);
            scanf(" %[^\n]", s.na);
            printf("Enter New Class ");
            // fgets(s.cl, 10, stdin);
            scanf(" %[^\n]", s.cl);
            printf("Enter New Roll no ");
            scanf("%d", &s.rl);
            fseek(fp, -sz, 1);
            fwrite(&s, sz, 1, fp);
            fclose(fp);
            break;
        }
    }

    if (f == 1)
    {
        printf("Record updated sucessfully\n");
    }
    else
    {
        printf("Record not found\n");
    }
}

void del()
{
    int rl, f = 0;

    fp = fopen("stud.txt", "rb");

    FILE *ft;
    ft = fopen("temp.txt", "ab");
    printf("Enter roll to delete: ");
    scanf("%d", &rl);
    while (fread(&s, sz, 1, fp) == 1)
    {
        if (rl == s.rl)
        {
            f = 1;
        }
        else
        {
            fwrite(&s, sz, 1, ft);
        }
    }
    fclose(fp);
    fclose(ft);
    remove("stud.txt");
    rename("temp.txt", "stud.txt");

    if (f == 1)
    {
        printf("Record deleted sucessfully\n");
    }
    else
    {
        printf("Record not found\n");
    }
}

void sort()
{
    int ch;
    while (1)
    {
        system("clear");
        printf("SEarch\n");
        printf("1.Sort by Roll Num\n");
        printf("2.sort by Name\n");
        printf("0. Back to Main Menu\n");
        printf("\n\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 0:
            main();
            break;

        case 1:
            sortByRoll();
            break;

        case 2:
            sortByName();
            break;

        default:
            printf("Invalid choice\n");
        }
        
        printf("Hit TAB to continie...");
            #ifdef _WIN32
                getch();
            #elif __linux__
                while(keypress(0) != '\t');
            #endif    
    }
}

void sortByRoll()
{
    int c = 0, i, j;
    struct student s1[50], t;

    system("clear");
    printf("Student Info Sorted By Roll Num\n");
    printf("%-30s %-20s %-20s %s\n", "NAME", "CLASS", "ROLL", "DATE");
    fp = fopen("stud.txt", "rb");
    while (fread(&s, sz, 1, fp) == 1)
    {
        s1[c++] = s;
    }
    for (i = 0; i < c - 1; i++)
    {
        for (j = i + 1; j < c; j++)
        {
            if (s1[i].rl > s1[j].rl)
            {
                t = s1[i];
                s1[i] = s1[j];
                s1[j] = t;
            }
        }
    }

    for (i = 0; i < c; i++)
    {
        printf("%-30s %-20s %-20d %s\n", s1[i].na, s1[i].cl, s1[i].rl, s1[i].date);
    }

    fclose(fp);
}

void sortByName()
{
    int c = 0, i, j;
    struct student s1[50], t;

    system("clear");
    printf("Student Info Sorted By Name\n");
    printf("%-30s %-20s %-20s %s\n", "NAME", "CLASS", "ROLL", "DATE");
    fp = fopen("stud.txt", "rb");
    while (fread(&s, sz, 1, fp) == 1)
    {
        s1[c++] = s;
    }
    for (i = 0; i < c - 1; i++)
    {
        for (j = i + 1; j < c; j++)
        {
            if (strcmp(s1[i].na,s1[j].na)>0)
            {
                t = s1[i];
                s1[i] = s1[j];
                s1[j] = t;
            }
        }
    }

    for (i = 0; i < c; i++)
    {
        printf("%-30s %-20s %-20d %s\n", s1[i].na, s1[i].cl, s1[i].rl, s1[i].date);
    }

    fclose(fp);
}
