#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define offset 40
int no_attributes, no_rows;
/*#define no_attributes 2*/
/*#define no_rows 4*/
FILE *fp;

/*Gets the number or attributes and rows in the database*/
void get_rows_and_columns()
{
    int count = 0;
    char c;
    char firstLine[1000];
    int columns = 0;
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            count = count + 1;
    count=count-1;
    no_rows = count;   
    fseek(fp, 0, SEEK_SET);
   
    for (c = getc(fp); c != '\n'; c = getc(fp))     {
            char nextChar = getc(fp);
            fseek(fp, -1, SEEK_CUR);
            if(nextChar == '\n')    break;
            if(c == '#' && nextChar != '#') columns++;
    }
    columns++;
    no_attributes = columns;
}
/*Creates a 3D array of characters*/
char*** new_table(char ***table)
{
    int i, j, k;
    table = (char ***)malloc((no_rows) * sizeof(char **));
    for( i=0; i<=no_rows; i++)
    {
        *(table + i) = (char **)malloc((no_attributes) * sizeof(char *));
        for( j=0; j<=no_attributes; j++)
            *(*(table + i) + j) = (char *)malloc(offset * sizeof(char));
    }
    return table;
}

/*Calculates offset necessary to access a char in a file using the function fseek when given row and column values*/
int calculate_offset(int row, int col)
{
    return((row*offset*no_attributes) + ((col - 1)* offset));
}

/*Maps attribute name to a particular index*/
int map_column(char *column_name)
{
    if (!(strcmp(column_name, "essn")))
        return 1;
    else if (!(strcmp(column_name, "name")))
        return 2;
    else if (!(strcmp(column_name, "salary")))
        return 3;
    else
        return -1;
}

/*Copies all values of a particular atrribute to a temporary table*/
void get_column(int col, char ***table)
{
    int location, i, j;
    char c;
/*    char *entry[no_rows], c;*/
/*    printf("\n");*/
    for( i=0; i<= no_rows; i++)
    {
/*        entry[i] = (char*)malloc(40*sizeof(char));*/
        location = calculate_offset(i, col);
/*        printf("\n%d ", location);*/
        fseek(fp, calculate_offset(i,col), SEEK_SET);
        j = 0;
        while(1)
        {
            c = getc(fp);
/*            printf("%c", c);*/
            if (c == '#')
                break;
            table[i][col][j++] = c;
        }
/*        printf("%s\n", entry[i]);*/
    }
}

/*Prints a given table*/
void print_table(char ***table)
{
    int i, j;
    for( i=0; i<=no_rows; i++)
     {
        for( j=1; j<=no_attributes; j++)
            printf("%s ", table[i][j]);
        printf("\n");
     }
}

/*Copies the database in a file to a temporary table*/
char *** get_db(char *** table)
{
    int j;
    for( j=1; j<=no_attributes; j++)
        get_column(j, table);
    return table;
}

/*All the different cases of the below menu driven program*/
void case1()
{
    char ***table;
    table = new_table(table);
    table = get_db(table);
    print_table(table);
}
void case2()
{
    int flag, na, i, j;
    char ***table, *c;
    c = (char *)malloc(sizeof(char));
    flag = 0;
    table = new_table(table);   
    printf("\nEnter the number of colums to be fetched.");
    scanf("%d", &na);
    printf("\nEnter attribute names.");
    for (i=0; i<na; i++)
    {   
        scanf("%s", c);
        j = map_column(c);
        if (j == -1)
        {
            printf("\nNo such column %s. Try again.\n", c);
            i--;
            continue;
        }
        get_column(j, table);
        printf("\n");
    }
    printf("\n");
    print_table(table);
}
void case3(int which, char ***table, char ***temp_table, int attribute_list[], int na)
{
    int i, j, nc, choice, map, map2, flag, test;
    char sign, *c, *c2, *value, junk;
    c = (char *)malloc(sizeof(char));
    c2 = (char *)malloc(sizeof(char));
    value = (char *)malloc(sizeof(char));
    flag = 0;
    
    /*            printf("\nEnter the number of conditions.")*/
    /*            scanf("%d", &nc);*/
    printf("\nEnter conditions");
    /*            for( i=0; i<n; i++)*/
    /*            {*/

    while(1)
    {
        scanf("%s", c);
        map = map_column(c);
        if (map == -1)
        {
            printf("\nNo such column %s. Try again.\n", c);
            continue;
        }
        else
            break;
    }

    scanf("%c", &sign);
    scanf("%c", &sign);

    if (which == 3)
    {
        while(1)
        {
            scanf("%s", c2);
            map2 = map_column(c2);
            if (map2 == -1)
            {
                printf("\nNo such column %s. Try again.\n", c2);
                continue;
            }
            else
                break;
        }
    }
    
    else
    {
        printf("\nEnter value to be compared with ");
        scanf("%s", value);
    }

/*    All the different binary operators(conditions)*/
    switch(sign)
    {
        case '=':
            for(i=1; i<=no_rows; i++)
            {
                if (which == 3)
                    test = strcmp(temp_table[i][map], temp_table[i][map2]);
                else
                    test = strcmp(temp_table[i][map], value);
                                    
/*                Copy the required values if the condition is satisfied*/
                if(!test)
                {
                    flag = 1; 
/*                    printf("\nthis is it saar ");*/
                    for(j=0; j<na; j++)                        
                    {
/*                        printf(" %d", attribute_list[j]);*/
                        strcpy(table[i][attribute_list[j]], temp_table[i][attribute_list[j]]);
                        strcpy(table[0][attribute_list[j]], temp_table[0][attribute_list[j]]);
                    }
/*                    printf("\n");*/
                }
            }
            break;
        case '>':
            for(i=1; i<=no_rows; i++)
            {
                if (which == 3)
                    test = strcmp(temp_table[i][map], temp_table[i][map2]);
                else
                    test = strcmp(temp_table[i][map], value);
                
                if(test > 0)
                {
                    flag = 1; 
                    for(j=0; j<na; j++)                        
                    {
                        strcpy(table[i][attribute_list[j]], temp_table[i][attribute_list[j]]);
                        strcpy(table[0][attribute_list[j]], temp_table[0][attribute_list[j]]);
                    }
                }
            }
            break;
        case '<':
            for(i=1; i<=no_rows; i++)
            {
                if (which == 3)
                    test = strcmp(temp_table[i][map], temp_table[i][map2]);
                else
                    test = strcmp(temp_table[i][map], value);
                    
                if(test < 0)
                {
                    flag = 1; 
                    for(j=0; j<na; j++)                        
                    {
                        strcpy(table[i][attribute_list[j]], temp_table[i][attribute_list[j]]);
                        strcpy(table[0][attribute_list[j]], temp_table[0][attribute_list[j]]);
                    }
                }
            }
            break;
            
        case 'l':
            for(i=1; i<=no_rows; i++)
            {
                if (which == 3)
                    test = strcmp(temp_table[i][map], temp_table[i][map2]);
                else
                    test = strcmp(temp_table[i][map], value);
                    
                if(test <= 0)
                {
                    flag = 1; 
                    for(j=0; j<na; j++)                        
                    {
                        strcpy(table[i][attribute_list[j]], temp_table[i][attribute_list[j]]);
                        strcpy(table[0][attribute_list[j]], temp_table[0][attribute_list[j]]);
                    }
                }
            }
            break;
            
        case 'g':
            for(i=1; i<=no_rows; i++)
            {
                if (which == 3)
                    test = strcmp(temp_table[i][map], temp_table[i][map2]);
                else
                    test = strcmp(temp_table[i][map], value);
                    
                if(test >= 0)
                {
                    flag = 1; 
                    for(j=0; j<na; j++)                        
                    {
                        strcpy(table[i][attribute_list[j]], temp_table[i][attribute_list[j]]);
                        strcpy(table[0][attribute_list[j]], temp_table[0][attribute_list[j]]);
                    }
                }
            }
            break;
        
        default:
            printf("\no such operator");
    }
    if (flag)
    {
        printf("\n");
        print_table(table);
    }
    else
        printf("\nNO SUCH ENTRIES\n");
}
int main()
{
    
    int i, j, i1, j1, na, nc, choice, map, map2, attribute_list[no_attributes], flag, count, join, decision, disp_attribute_list[no_attributes], three_or_four;
    char sign, *c, *c2, ***table, ***temp_table, ***and_table, junk;
    table = new_table(table);
    c = (char *)malloc(sizeof(char));
    c2 = (char *)malloc(sizeof(char));
    fp = fopen("database2.txt", "r");
    get_rows_and_columns();
    printf("\n1.View table\n2.Fetch column\n3.RA select or Fetch columns with conditionRA select\n");
    scanf("%d", &choice);
    switch(choice)
    {
        case 1:
            flag = 0;
            case1();
            break;
        case 2:
            case2();
            break;
        case 3:
            printf("\nEnter 4 for RA select and 3 for Fetching columns with condition");
            scanf("%d", &three_or_four);
            if (three_or_four == 3)
            {
                printf("\nEnter the number of colums to be fetched. ");
                scanf("%d", &na);
                printf("\nEnter attribute names. ");
                for (i=0; i<na; i++)
                {   
                    scanf("%s", c);
                    j = map_column(c);
                    if (j == -1)
                    {
                        printf("\nNo such column %s. Try again.\n", c);
                        i--;
                        continue;
                    }
                    disp_attribute_list[i] = j;
                }
            }
            na = no_attributes;
            for (i=0; i<na; i++)
            {   
                attribute_list[i] = i + 1;
            }
            printf("\nEnter the number of conditions\n");
            scanf("%d", &nc);
            table = new_table(table);
            temp_table = new_table(temp_table);
            temp_table = get_db(temp_table);
            for (i=0; i<nc; i++)
            {
                printf("Enter type 3.C1 = C2 4.C1 = a123\n");
/*                Decision decides if it belongs to case 3 or case 4*/
                scanf("%d", &decision);
                if (i==0)
                    case3(decision, table, temp_table, attribute_list, na);
                else
                {
                    printf("\n1.AND 2.OR\n");
                    scanf("%d", &join);
                    if (join == 1)
                    {
                        temp_table = table;
                        and_table = new_table(and_table);
                        table = and_table;
                        case3(decision, table, temp_table, attribute_list, na);
                    }
                    else
                    {
                        temp_table = new_table(temp_table);
                        temp_table = get_db(temp_table);
                        case3(decision, table, temp_table, attribute_list, na);
                    }
                }
            }
            int i1, j1, flag1 = 0;
            if (three_or_four == 3)
            {
                printf("\nThe final table is \n");
                for( i=0; i<=no_rows; i++)
                {
                    for( j=1; j<=no_attributes; j++)
                    {
                        flag1 = 0;
                        for( i1=0; i1<na; i1++)
                            if(j == disp_attribute_list[i1])
                            {
                                flag1 = 1;
                                break;
                            }
                        if(flag1)
                            printf("%s ", table[i][j]);
                    }
                    printf("\n");
                }
            }
            break;
        default: printf("wrong choice");
    }
    return 0;
}
