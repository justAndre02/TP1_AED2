#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

//==============================================Estruturas===========================================================
typedef struct _sets {
    char * num;
    char * name;
    int year;
    char * theme;

    struct _sets *prev, *next, *last, *first;
} Sets;

typedef struct _parts {
    char * num;
    char * name;
    char * class;
    int stock;

    struct _parts *prev, *next, *last, *first;
} Parts;

typedef struct part_sets {
    char * set_num;
    int quantity;
    char * part_num;

    struct part_sets *prev, *next, *last, *first;
} PS;
//================================================Construtores=========================================================
Sets * createSetsList (char * num, char * name, int year, char * theme)
{
    Sets * list = (Sets*) malloc(sizeof(Sets));
    list->num = strdup(num);
    list->name = strdup(name);
    list->year = year;
    list->theme = strdup(theme);
    list->next = NULL;
    list->prev = NULL;
    list->last = list;
    list->first = list;
    
    return list;
}

Sets * Sets_insert (Sets * list, char * num, char * name, int year, char * theme)
{
    assert(list);

    list->last->next = createSetsList(num, name, year, theme);
    list->last->next->prev = list->last;
    list->last->next->first = list;
    list->last = list->last->next; 
    
    return list;
}

Parts * createPartsList (char * num, char * name, char * class, int stock)
{
    Parts * list = (Parts*) malloc(sizeof(Parts));
    list->num = strdup(num);
    list->name = strdup(name);
    list->class = strdup(class);
    list->stock = stock;
    list->next = NULL;
    list->prev = NULL;
    list->last = list;
    list->first = list;

    return list;
}

Parts * Parts_insert (Parts * list, char * num, char * name, char * class, int stock)
{
    assert(list);

    list->last->next = createPartsList(num, name, class, stock);
    list->last->next->prev = list->last;
    list->last->next->first = list;
    list->last = list->last->next; 

    return list;
}

PS * createPSList (char * set_num, int quantity, char * part_num)
{
    PS * list = (PS*) malloc(sizeof(PS));
    list->set_num = strdup(set_num);
    list->quantity = quantity;
    list->part_num = strdup(part_num);
    list->next = NULL;
    list->prev = NULL;
    list->last = list;
    list->first = list;

    return list;
}

PS * PS_insert (PS * list, char * set_num, int quantity, char * part_num)
{
    assert(list);

    list->last->next = createPSList(set_num, quantity, part_num);
    list->last->next->prev = list->last;
    list->last->next->first = list;
    list->last = list->last->next; 

    return list;
}
//===============================================LER Ficheiros=========================================================
Sets * loadSets(char * PATH) {
    Sets * SetsList;
    if(access(PATH, F_OK) == 0) {
        FILE * SetsFile = fopen(PATH, "r");
        int counter = 0;

        while (!feof(SetsFile)) {
            char num[1000];
            char name[1000];
            int year;
            char theme[1000];
            char line[1000];
            
            fgets(line, 1000, SetsFile);

            if(feof(SetsFile)) break;

            if(counter >= 0) {
                sscanf(line, "%[^\t]\t%[^\t]\t%d\t%[^\n]", num, name, &year, theme);
            }

            if(counter == 0) {
                SetsList = createSetsList(num, name, year, theme);
                //printf("CREATED LIST: %s, %d, %s", numPart, quantity, numSet);
            } 
            else { 
                SetsList = Sets_insert(SetsList, num, name, year, theme);
                //printf("INSERTED: %s, %d, %s", numPart, quantity, numSet);
            }

            counter++;
        }
        fclose(SetsFile);
        return SetsList;
    } else {
        return NULL;
    }
}

Parts * loadParts(char * PATH) {
    Parts * PartsList;
    if(access(PATH, F_OK) == 0) {
        FILE * PartsFile = fopen(PATH, "r");
        int counter = 0;

        while (!feof(PartsFile)) {
            char num[1000];
            char name[1000];
            int stock;
            char class[1000];
            char line[1000];
            
            fgets(line, 1000, PartsFile);

            if(feof(PartsFile)) break;

            if(counter >= 0) {
                sscanf(line, "%[^\t]\t%[^\t]\t%[^\t]\t%d", num, name, class, &stock);
            }

            if(counter == 0) {
                PartsList = createPartsList(num, name, class, stock);
                //printf("CREATED LIST: %s, %d, %s", numPart, quantity, numSet);
            } 
            else { 
                PartsList = Parts_insert(PartsList, num, name, class, stock);
                //printf("INSERTED: %s, %d, %s", numPart, quantity, numSet);
            }

            counter++;
        }
        fclose(PartsFile);
        return PartsList;
    } else {
        return NULL;
    }
}

PS * loadPS(char * PATH) {
    PS * PSList;
    if(access(PATH, F_OK) == 0) {
        FILE * PSFile = fopen(PATH, "r");
        int counter = 0;

        while (!feof(PSFile)) {
            char set_num[1000];
            char part_num[1000];
            int quantity = 0;
            char line[1000];
            
            fgets(line, 1000, PSFile);

            if(feof(PSFile)) break;

            if(counter >= 0) {
                sscanf(line, "%[^\t]\t%d\t%[^\n]", set_num, &quantity, part_num);
            }

            if(counter == 0) {
                PSList = createPSList(set_num, quantity, part_num);
                //printf("CREATED LIST: %s, %d, %s", numPart, quantity, numSet);
            } 
            else { 
                PSList = PS_insert(PSList, set_num, quantity, part_num);
                //printf("INSERTED: %s, %d, %s", numPart, quantity, numSet);
            }

            counter
            ++;
        }
        fclose(PSFile);
        return PSList;
    } else {
        return NULL;
    }
}
//===================================================Correçao de warnings====================================================
void MenuE(Sets * SetsList, Parts * PartsList, PS * PSList);
void MenuL(Sets * SetsList, Parts * PartsList, PS * PSList);
void MenuP(Sets * SetsList, Parts * PartsList, PS * PSList);
//====================================================Funçoes para Listar===================================================
//1
void PrintSetsT(Sets * SetsList, Parts * PartsList, PS * PSList){
    char themec[1000];
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Tema: ");
    gets(themec);
    system("cls || clear");
    Sets * aux = SetsList;
    Sets * aux2;
    int count = 0;
    while(aux){
        if(strcmp(aux->theme, themec) == 0){
            if(count == 0) {
                aux2 = createSetsList(aux->num, aux->name, aux->year, aux->theme);
            } 
            else { 
                aux2 = Sets_insert(aux2, aux->num, aux->name, aux->year, aux->theme);
            }
            count++;
        }
        aux = aux->next;
    }
    if(aux2)
    {
        Sets * aux1 = aux2->first;
        while(aux1){
            Sets * aux3 = aux2->first;
            while(aux3)
            {
                if (aux1->year<aux3->year)
                {
                    char *num = aux1->num;
                    char *name = aux1->name;
                    int year = aux1->year;
                    char *theme = aux1->theme;

                    aux1->num = aux3->num;
                    aux1->name = aux3->name;
                    aux1->year = aux3->year;
                    aux1->theme = aux3->theme;

                    aux3->num = num;
                    aux3->name = name;
                    aux3->year = year;
                    aux3->theme = theme;
                }
                aux3 = aux3->next;
            }
            aux1 = aux1->next;
        }
        Sets * aux4 = aux2->first;
        while(aux4)
        {
            printf("---------------------------\n");
            printf(" ID: %s\n", aux4->num);
            printf(" Nome: %s\n", aux4->name);
            printf(" Ano: %d\n", aux4->year);
            printf(" Theme: %d\n", aux4->theme);
            aux4 = aux4->next;
        }
    }
    else{
        printf("---------------------------\n");
        printf("|  Este tema nao existe!  |\n");
    }
    
    
    printf("---------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
}
//2
void PrintPartsTC(Sets * SetsList, Parts * PartsList, PS * PSList){
    char setnum[1000];
    char class[1000];
    bool error = true;
    bool error1 = true;

    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Conjunto: ");
    gets(setnum);
    printf("Tipo de peca: ");
    gets(class);
    
    PS * aux = PSList->first;
    system("cls || clear");
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    Parts * aux1 = PartsList->first;
    while(aux1)
    {
        if(strcmp(aux1->class, class) == 0)
        {
            error1 = false;
        }else if(aux1->next == NULL && error1 == true)
        {
           error1 = true; 
        }
        aux1 = aux1->next;
    }

    aux = PSList->first;
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            aux1 = PartsList->first;

            while(aux1)
            {
                if(strcmp(aux1->num, aux->part_num) == 0 && strcmp(aux1->class, class) == 0)
                {
                    printf("-----------------------------\n");
                    printf(" ID: %s\n", aux1->num);
                    printf(" Nome: %s\n", aux1->name);
                    printf(" Stock: %d\n", aux1->stock);
                    printf(" Quantidade: %d\n", aux->quantity);
                }
                aux1 = aux1->next;
            }
        }
        aux = aux->next;
    }

    if(error == true)
    {
        printf("-----------------------------\n");
        printf("| Este conjunto nao existe! |\n");
    }
    else if(error1 == true)
    {
        printf("-----------------------------\n");
        printf("|  Esta classe nao existe!  |\n");
    }
    printf("-----------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
    
}
//3
void PrintPartsC(Sets * SetsList, Parts * PartsList, PS * PSList){
    char setnum[1000];
    bool error = true;

    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Conjunto: ");
    gets(setnum);
    
    PS * aux = PSList->first;
    system("cls || clear");
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    aux = PSList->first;
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            Parts * aux1 = PartsList->first;

            while(aux1)
            {
                if(strcmp(aux1->num, aux->part_num) == 0)
                {
                    printf("--------------------------\n");
                    printf(" ID: %s\n", aux1->num);
                    printf(" Nome: %s\n", aux1->name);
                    printf(" Class: %s\n", aux1->class);
                    printf(" Stock: %d\n", aux1->stock);
                    printf(" Quantidade: %d\n", aux->quantity);
                }
                aux1 = aux1->next;
            }
        }
        aux = aux->next;
    }
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("| Este conjunto nao existe! |\n");
    }
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
    
}
//4
void PrintPartsTotalS(Sets * SetsList, Parts * PartsList, PS * PSList){
    getchar();
    Parts * aux = PartsList->first;
    int total = 0;
    while(aux)
    {
        total += aux->stock;
        aux = aux->next;
    }
    printf("------------------------------\n");
    printf("| Total pecas stock : %d |\n", total);
    printf("------------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
    
}
//5
void PrintPartsTotalC(Sets * SetsList, Parts * PartsList, PS * PSList){
    char setnum[1000];
    int counter = 0;
    bool error = true;
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Conjunto: ");
    gets(setnum);

    PS * aux = PSList->first;
    system("cls || clear");
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    aux = PSList->first;
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            Parts * aux1 = PartsList->first;

            while(aux1)
            {
                if(strcmp(aux1->num, aux->part_num) == 0)
                {
                    printf("--------------------------\n");
                    printf(" ID: %s\n", aux1->num);
                    printf(" Nome: %s\n", aux1->name);
                    printf(" Class: %s\n", aux1->class);
                    printf(" Stock: %d\n", aux1->stock);
                    printf(" Quantidade: %d\n", aux->quantity);
                    counter+=aux->quantity;
                }
                aux1 = aux1->next;
            }
        }
        aux = aux->next;
    }
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("| Este conjunto nao existe! |\n");
    }
    printf("------------------------------------\n");
    printf("| Total pecas para o conjunto : %d |\n", counter);
    printf("------------------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
    
}
//6
void PrintPartsMU(Sets * SetsList, Parts * PartsList, PS * PSList){
    getchar();
    int totalt, totalg, x;
    char name[100];
    Parts * aux = PartsList->first;
    x=0;
    printf("Loading Data...");
    while(aux)
    {
        totalt = 0;
        PS * aux2 = PSList->first;
        while(aux2)
        {
            if(strcmp(aux->num, aux2->part_num) == 0)
            {
                totalt++;
            }
            aux2 = aux2->next;
        }
        
        if(x==0)
        {
            totalg = totalt;
            strcpy(name, aux->name);
        }
        else if(totalt > totalg)
        {
            totalg = totalt;
            strcpy(name, aux->name);
        }
        aux = aux->next;
        x++;
    }
    system("cls || clear");
    printf("---------------------------\n");
    printf("|   Peca mais utilizada   |\n");
    printf("---------------------------\n");
    printf(" Nome: %s\n", name);
    printf("------------------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
    
}
//7
void PrintSetsWC(Sets * SetsList, Parts * PartsList, PS * PSList){
    getchar();
    Sets * aux = SetsList->first;
    int check, check2, counter;
    counter = 0;
    bool impossible = false;
    while(aux)
    {
        PS * aux1 = PSList->first;
        check = 0;
        check2 = 0;
        impossible = false;
        while(aux1)
        {   
            if(impossible == false)
            {
                if(strcmp(aux->num, aux1->set_num) == 0){
                    Parts * aux2 = PartsList->first;
                    while(aux2)
                    {
                        if(strcmp(aux1->part_num, aux2->num) == 0)
                        {
                            check++;
                            if(aux1->quantity <= aux2->stock)
                            {
                                check2++;
                            }
                            else
                            {
                                impossible = true;
                                break;
                            }

                        }
                        aux2 = aux2->next;
                    } 
                }
            }
            else
            {
                break;
            }
            aux1 = aux1->next;    
        }
        if(check == check2)
        {
            counter++;
            printf("-------------%d-------------\n", counter);
            printf("Set: %s\n", aux->name);
        }
        aux = aux->next;
    }
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuL(SetsList, PartsList, PSList);
}
//=================================================Extras===========================================================
//1
void ChangeSP(Sets * SetsList, Parts * PartsList, PS * PSList){
    char partnum[1000];
    bool find;
    int stock = 0;
    find = false;
    bool error = true;
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Peca: ");
    gets(partnum);
    system("cls || clear");
    Parts * aux = PartsList->first;
    while(aux)
    {
        if(strcmp(aux->num, partnum) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    aux = PartsList->first;
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("|   Esta peca nao existe!   |\n");
    }
    else
    {
        while(aux)
        {
            if(find == false){
                if(strcmp(aux->num, partnum) == 0)
                {
                    find = true;
                    printf("---------------------\n");
                    printf("|    Informacoes    |\n");
                    printf("---------------------\n");
                    printf(" ID: %s\n", aux->num);
                    printf(" Nome: %s\n", aux->name);
                    printf(" Class: %s\n", aux->class);
                    printf(" Stock: %d\n", aux->stock);
                    printf("---------------------\n");
                    printf("Novo stock: ");
                    scanf("%d", &stock);
                    aux->stock = stock;
                }
                aux = aux->next;
            }
            else
            {
                break;
            }
        }
    }
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    getchar();
    system("cls || clear");
    MenuE(SetsList, aux, PSList);
    
}
//2
void ChangeSPC(Sets * SetsList, Parts * PartsList, PS * PSList){
    char setnum[1000];
    bool find;
    int n = 0;
    int x = 0;
    find = false;
    bool error;
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Conjunto: ");
    gets(setnum);
    printf("Quantidade de Conjuntos: ");
    scanf("%d", &n);
    
    PS * aux = PSList->first;
    Parts * aux1 = PartsList->first;
    while(aux)
    {
        if(strcmp(aux->set_num, setnum) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    system("cls || clear");
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("| Este conjunto nao existe! |\n");
    }
    else{
        aux = PSList->first;
        aux1 = PartsList->first;
        while(aux)
        {
            if(strcmp(aux->set_num, setnum) == 0)
            {
                find = false;
                aux1 = aux1->first;
                while(aux1)
                {
                    if(find == false)
                    {
                        if(strcmp(aux->part_num, aux1->num) == 0)
                        {
                            n = n * aux->quantity;
                            aux1->stock = aux1->stock + n;
                            find = true;
                        }
                        aux1 = aux1->next;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }
            aux = aux->next;
        }
    }
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    getchar();
    system("cls || clear");
    MenuE(SetsList, aux1, PSList);
    
}
//3
void DeletePC(Sets * SetsList, Parts * PartsList, PS * PSList){
    char Class[1000];
    bool find;
    int n = 0;
    int x = 0;
    find = false;
    bool error = true;
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Classe: ");
    gets(Class);
    
    Parts * aux = PartsList->first;
    system("cls || clear");
    while(aux)
    {
        if(strcmp(aux->class, Class) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("|   Esta class nao existe!  |\n");
    }
    else{
        aux = PartsList->first;
        while(aux)
        {
            if (strcmp(aux->class, Class) == 0)
            {
                Parts * next = aux->next;
                if (aux == aux->last)
                {
                    aux->prev->last = aux->prev;
                }
                if (aux->next)
                {
                    aux->next->prev = aux->prev;
                }
                if (aux->prev)
                {
                    aux->prev->next = aux->next;
                }
                
                free(aux->num);
                free(aux->class);
                free(aux->name);
                free(aux);

                aux = next;
            }
            else
            {
                aux = aux->next;
            }
        } 
    }
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    getchar();
    system("cls || clear");
    MenuE(SetsList, aux, PSList);
    
}
//4
void DeleteST(Sets * SetsList, Parts * PartsList, PS * PSList){
    char theme[1000];
    bool find;
    int n = 0;
    int x = 0;
    find = false;
    bool error = true;
    getchar();
    printf("---------------------\n");
    printf("|    Informacoes    |\n");
    printf("---------------------\n");
    printf("Tema: ");
    gets(theme);
    
    Sets * aux = SetsList->first;;
    system("cls || clear");
    while(aux)
    {
        if(strcmp(aux->theme, theme) == 0)
        {
            error = false;
        }else if(aux->next == NULL && error == true)
        {
           error = true; 
        }
        aux = aux->next;
    }
    aux = SetsList->first;
    if(error == true)
    {
        printf("-----------------------------\n");
        printf("|   Este tema nao existe!   |\n");
    }
    else{
        while(aux)
        {
            if (strcmp(aux->theme, theme) == 0)
            {
                Sets * next = aux->next;
                if (aux == aux->last)
                {
                    aux->prev->last = aux->prev;
                }
                if (aux->next)
                {
                    aux->next->prev = aux->prev;
                }
                if (aux->prev)
                {
                    aux->prev->next = aux->next;
                }
                
                free(aux->num);
                free(aux->theme);
                free(aux->name);
                //free(partList->stock); como é int nao e preciso dar clear
                free(aux);

                aux = next;
            }
            else
            {
                aux = aux->next;
            }
        } 
    } 
    printf("--------------------------\n");
    printf("Prima qualquer tecla para voltar ao menu.");
    getchar();
    system("cls || clear");
    MenuE(aux, PartsList, PSList);
    
}
//=================================================Menus===========================================================
void MenuL(Sets * SetsList, Parts * PartsList, PS * PSList){
    int op = 0;
    printf("-----------------------------------------------------------------------\n");
    printf("|                              Menu Listar                            |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("|                                                                     |\n");
    printf("| 1 - Listar conjuntos de um tema (ordenado por ano)                  |\n");
    printf("|                                                                     |\n");
    printf("| 2 - Listar as pecas de um determinado tipo num determinado conjunto |\n");
    printf("|                                                                     |\n");
    printf("| 3 - Listar as pecas de um determinado conjunto                      |\n");
    printf("|                                                                     |\n");
    printf("| 4 - Listar o total de pecas em stocks                               |\n");
    printf("|                                                                     |\n");
    printf("| 5 - Listar o total de pecas num determinado conjunto                |\n");
    printf("|                                                                     |\n");
    printf("| 6 - Listar a peca mais utilizada em todos os conjuntos              |\n");
    printf("|                                                                     |\n");
    printf("| 7 - Listar conjuntos que podemos construir com o stock atual        |\n");
    printf("|                                                                     |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("|                                                                     |\n");
    printf("| 0 - Voltar                                                          |\n");
    printf("|                                                                     |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Opcao: ");
    scanf(" %d", &op);

    switch(op)
    {
        case 1: 
            system("cls || clear");
            PrintSetsT(SetsList, PartsList, PSList);
            break;
        case 2:
            system("cls || clear");
            PrintPartsTC(SetsList, PartsList, PSList);
            break;
        case 3:
            system("cls || clear");
            PrintPartsC(SetsList, PartsList, PSList);
            break;
        case 4:
            system("cls || clear");
            PrintPartsTotalS(SetsList, PartsList, PSList);
            break;
        case 5:
            system("cls || clear");
            PrintPartsTotalC(SetsList, PartsList, PSList);
            break;
        case 6:
            system("cls || clear");
            PrintPartsMU(SetsList, PartsList, PSList);
            break;

        case 7:
            system("cls || clear");
            PrintSetsWC(SetsList, PartsList, PSList);
            break;
        case 0:
            system("cls || clear");
            MenuP(SetsList, PartsList, PSList);
            break;
        default:
        system("cls || clear");
        MenuL(SetsList, PartsList, PSList);
    }
}

void MenuE(Sets * SetsList, Parts * PartsList, PS * PSList){
    int op = 0;
    printf("-----------------------------------------------------------------------\n");
    printf("|                              Menu Listar                            |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("|                                                                     |\n");
    printf("| 1 - Alterar o numero de pecas em stock                              |\n");
    printf("|                                                                     |\n");
    printf("| 2 - Adicionar stock de pecas consoante o conjunto                   |\n");
    printf("|                                                                     |\n");      
    printf("| 3 - Deletar pecas de uma determinada classe                         |\n");
    printf("|                                                                     |\n");
    printf("| 4 - Deletar conjuntos de um determinado tema                        |\n");
    printf("|                                                                     |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("|                                                                     |\n");
    printf("| 0 - Voltar                                                          |\n");
    printf("|                                                                     |\n");
    printf("-----------------------------------------------------------------------\n");
    printf("Opcao: ");
    scanf(" %d", &op);

    switch(op)
    {
        case 1: 
            system("cls || clear");
            ChangeSP(SetsList, PartsList, PSList);
            break;
        case 2:
            system("cls || clear");
            ChangeSPC(SetsList, PartsList, PSList);
            break;
        case 3:
            system("cls || clear");
            DeletePC(SetsList, PartsList, PSList);
            break;
        case 4:
            system("cls || clear");
            DeleteST(SetsList, PartsList, PSList);
            break;
        case 0:
            system("cls || clear");
            MenuP(SetsList, PartsList, PSList);
            break;
        default:
        system("cls || clear");
        MenuL(SetsList, PartsList, PSList);
    }
}

void MenuP(Sets * SetsList, Parts * PartsList, PS * PSList){
    int op = 0;
    printf("--------------------------\n");
    printf("|          Lego          |\n");
    printf("--------------------------\n");
    printf("|                        |\n");
    printf("| 1 - Editar Informacoes |\n");
    printf("|                        |\n");
    printf("| 2 - Listar Informacoes |\n");
    printf("|                        |\n");
    printf("--------------------------\n");
    printf("|                        |\n");
    printf("| 0 - Sair               |\n");
    printf("|                        |\n");
    printf("--------------------------\n");
    printf("Opcao: ");
    scanf(" %d", &op);
    switch (op)
    {
        case 1:
            system("cls || clear");
            MenuE(SetsList, PartsList, PSList);
            break;

        case 2:
            system("cls || clear");
            MenuL(SetsList, PartsList, PSList);
            break;
        case 0:
            system("cls || clear");
            exit(0);
            break;
    default:
        system("cls || clear");
        MenuP(SetsList, PartsList, PSList);
    }
}
//================================================Main===============================================================
int main() {
    
    Sets * SetsList = loadSets("./sets.tsv");
    Parts * PartsList = loadParts("./parts.tsv");
    PS * PSList = loadPS("./parts_sets.tsv");
    MenuP(SetsList, PartsList, PSList);
    //printParts(PartsList);
    system("pause");
    return 0;
}
