#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/////MULTIPLAYER NADARAD :)

int width;
int height;
int i , j , k , h , dst_x , dst_y , height_edit , width_edit;
int Cellule_number;
int num_save = 0;int counter = 0;
int tool = 34 , arz = 29 , pos_x = 105 , pos_y = 0;
const char str[][27] = {
                    "Load","New single player game","New Multiplayer game","Map Editor","Exit",     //0-5
                    "Move","Split a cell","Boost energy","Save","Exit",                             //5-10
                    "North","South","Northeast","Northwest","Southeast","Southwest",                //10-16
                    "Energy Block","Mitose Block","Blocked Blcok","normal Block"                    //16-20
                    };

typedef struct{
    int Block_Energy;
    char Block_Type;
    int Block_x;
    int Block_y;
    int Block_defulat_value;
    int Cellule_energy;
}block_info;

typedef struct{
    int cell_id;
    int move_direction;
}cell_move_data;

int rand_int(int x){
    time_t t = time(NULL);
    srand(t);
    return (rand()*(rand()+1) % x);
}

void gotoxy(int x , int y){
    COORD pos;
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    if (INVALID_HANDLE_VALUE != hConsole){
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition( hConsole, pos );
    }
}

int find_value(char type , int height_find , int width_find , block_info Block[height_find][width_find]){
    for(j = 0 ; j < height_find ; j++){
        for(i = 0 ; i < width_find ; i++){
            if(Block[j][i].Block_Type == type)
                return -1;
        }
    }
    return 0;
}

int find_number(int number , int height_find , int width_find , block_info Block[height_find][width_find]){
    int ko = 0 , col , row;
    for(col = 0 ; col < height_find ; col++){
        for(row = 0 ; row < width_find ; row++){
            if(Block[col][row].Block_defulat_value == number)
                ko++;
        }
    }
    return ko;
}

void make_defualt(int height_def , int width_def , block_info Block[height_def][width_def]){
    for(j = 0 ; j < height_def ; j++){
        for(i = 0 ; i < width_def ; i++){
            Block[j][i].Block_defulat_value = -1 ;
            Block[j][i].Cellule_energy = 0;
            Block[j][i].Block_Type = '5';
            Block[j][i].Block_Energy = 0;
        }
    }
}

void box (int tool , int arz , int x ,int y , int ascii){
    int i;
    gotoxy (x,y);
    for(i=1 ; i<=tool ; i++){
        printf("%c" , ascii);
    }
    for (i=1 ; i<=arz ; i++){
        gotoxy (x,y+i-1);
        printf("%c" , ascii);
    }
    for (i=1 ; i<=arz ; i++){
        gotoxy (x+tool ,y+i-1);
        printf("%c" , ascii);
    }
    gotoxy (x , y+arz-1);
    for(i=1 ; i<=tool ; i++){
        printf("%c" , ascii);
    }
}

void print_box(const char * str , int x , int y){
    gotoxy(x , y);
    printf("%s" , str);
}

void clear_box(int x , int y , int w_box , int h_box){
    int h , k;
    gotoxy(x + 1 , y + 1);
    for(h = 0 ; h + 1< h_box  ; h++){
        for(k = 0 ; k + 1 < w_box ; k++){
            printf(" ");
        }
        gotoxy(x + 1 , y + 1 + h );
    }
    gotoxy(x , y);
}

void print_indentation(int number_desire , int pos_x , int pos_y , int indent){
    int kl , col = 1;
    for(kl = 1 ; kl < number_desire*indent ; kl += indent , col++){
        print_box("[" , pos_x + 4 , pos_y + kl + 1);
        printf("%d" , col);
        print_box("]" , pos_x + 7 , pos_y + kl + 1);
    }
}

void print_list(const char str[][27] , int start , int end , int pos_x , int pos_y){
    int k , i;
    for(k = start , i = 0; k < end ; k++ , i++){
        print_box(str[k] , pos_x + 9 , pos_y + i + 2);
    }

}

void print_menu(const char str[][27] , int start , int end , int pos_x , int pos_y){
    box(tool , arz , pos_x , pos_y , 219);
    clear_box(pos_x , pos_y , tool , arz);
    print_indentation(end - start + 1 , pos_x , pos_y , 1);
    print_list(str , start , end , pos_x , pos_y);
}

void basic_info(FILE * fchart){
    fread(&width , sizeof(int) , 1 , fchart);
    fread(&height , sizeof(int) , 1 , fchart);
}

void Read_Chart(FILE * fchart , block_info Block[height][width]){
    fseek(fchart , 2 * sizeof(int) , SEEK_SET);
    for(j = 0 ; j < height ; j++){
        for(i = 1 ; i <= width ; i++){
            fread(&Block[j][width - i].Block_Energy , sizeof(int) , 1 , fchart);
            fread(&Block[j][width - i].Block_Type , sizeof(char) , 1 , fchart);
        }
    }
}

int Block_Type_mention(char Block_Type){
    switch (Block_Type){
        case '1':
            return 160; //green  ---->>    energy
        case '2':
            return 830; //cyan   ---->>    duplication
        case '3':
            return 7502;//red    ---->>    blocked cellule
        case '4':
            return 368; //gray   ---->>    normal
        case '5':
            return 5616;//white  ---->>    default

    }
}

void print_Hexagonal(int x , int y , HANDLE hConsole , int COLOR , int i , int j , int height_hexa , int width_hexa , block_info Block[height_hexa][width_hexa]){
    gotoxy(x + 6 , y + 1);
    SetConsoleTextAttribute(hConsole, COLOR);
    printf("       ");
    gotoxy(x + 4 , y + 2);
    printf("           ");
    gotoxy(x + 2 , y + 3);
    printf("               ");
    gotoxy(x + 4 , y + 4);
    printf("        %3d",Block[j][i].Block_Energy);
    gotoxy(x + 6 , y + 5);
    printf(" (%d,%d) " , i , j);
    Block[j][i].Block_x = x + 9;
    Block[j][i].Block_y = y + 3;
}

void Chart_Hexagonal(HANDLE hConsole , int height_hexa , int width_hexa , block_info Block[height_hexa][width_hexa]){
    for(i = 0 ; i < 11 * width_hexa ; i += 11){
        for(j = 0 ; j < 6 * height_hexa ; j += 6){
            k = (i/11);
            h = height_hexa - (j/6 + 1);
            if((i/11) % 2 != 0){
                print_Hexagonal(i , j + 3 , hConsole , Block_Type_mention(Block[h][k].Block_Type) , k , h , height_hexa , width_hexa , Block);
            }
            else{
                print_Hexagonal(i , j , hConsole , Block_Type_mention(Block[h][k].Block_Type) , k , h , height_hexa , width_hexa , Block);
            }
            if(Block[h][k].Block_defulat_value == 0){
                gotoxy(Block[h][k].Block_x , Block[h][k].Block_y);
                SetConsoleTextAttribute(hConsole, 15);
                printf("%c" , 219);
            }
        }
    }
}

void create_cellule(block_info Block[height][width]){
    for(k = 0 ; k < Cellule_number ; ){
        j = rand_int(height);
        i = rand_int(width);
        if (Block[j][i].Block_defulat_value == -1 && Block[j][i].Block_Type != '3'){
            gotoxy(Block[j][i].Block_x , Block[j][i].Block_y);
            printf("%c",219);
            Block[j][i].Block_defulat_value = 0;
            k++;
        }
    }
}

int move(block_info Block[height][width] , int i , int j , int k , const char * call){
    if(Block[j][i].Block_defulat_value == 0 && i % 2 == 0){
        switch (k){
            case 1:
                if(j + 1 < height && Block[j + 1][i].Block_defulat_value == -1 && Block[j + 1][i].Block_Type != '3'){
                    Block[j + 1][i].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j + 1][i].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j + 1][i].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 2:
                if (j > 0 && Block[j - 1][i].Block_defulat_value == -1 && Block[j - 1][i].Block_Type != '3'){
                    Block[j - 1][i].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j - 1][i].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j - 1][i].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 3:
                if (i + 1 < width && j + 1 < height && Block[j + 1][i + 1].Block_defulat_value == -1 && Block[j + 1][i + 1].Block_Type != '3'){
                    Block[j + 1][i + 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j + 1][i + 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j + 1][i + 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 4:
                if (i > 0 && j + 1 < height && Block[j + 1][i - 1].Block_defulat_value == -1 && Block[j + 1][i - 1].Block_Type != '3'){
                    Block[j + 1][i - 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j + 1][i - 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j + 1][i - 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 5:
                if (i + 1 < width && Block[j][i + 1].Block_defulat_value == -1 && Block[j][i + 1].Block_Type != '3'){
                    Block[j][i + 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j][i + 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j][i + 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 6:
                if (i > 0 && Block[j][i - 1].Block_defulat_value == -1 && Block[j][i - 1].Block_Type != '3'){
                    Block[j][i - 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j][i - 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j][i - 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
        }
    }
    if(Block[j][i].Block_defulat_value == 0 && i % 2 == 1){
        switch (k){
            case 1:
                if(j + 1 < height && Block[j + 1][i].Block_defulat_value == -1 && Block[j + 1][i].Block_Type != '3'){
                    Block[j + 1][i].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j + 1][i].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j + 1][i].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 2:
                if (j > 0 && Block[j - 1][i].Block_defulat_value == -1 && Block[j - 1][i].Block_Type != '3'){
                    Block[j - 1][i].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j - 1][i].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j - 1][i].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 3:
                if (i + 1 < width && Block[j][i + 1].Block_defulat_value == -1 && Block[j][i + 1].Block_Type != '3'){
                    Block[j][i + 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j][i + 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j][i + 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 4:
                if (i > 0 && Block[j][i - 1].Block_defulat_value == -1 && Block[j][i - 1].Block_Type != '3'){
                    Block[j][i - 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j][i - 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j][i - 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 5:
                if (i + 1 < width && j > 0 && Block[j - 1][i + 1].Block_defulat_value == -1 && Block[j - 1][i + 1].Block_Type != '3'){
                    Block[j - 1][i + 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j - 1][i + 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j - 1][i + 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
            case 6:
                if (i > 0 && j > 0 && Block[j - 1][i - 1].Block_defulat_value == -1 && Block[j - 1][i - 1].Block_Type != '3'){
                    Block[j - 1][i - 1].Block_defulat_value = 0;
                    Block[j][i].Block_defulat_value = -1;
                    if (call == "split"){
                        Block[j - 1][i - 1].Cellule_energy = Block[j][i].Cellule_energy / 2;
                        Block[j][i].Cellule_energy /= 2;
                    }
                    else if(call == "normal"){
                        Block[j - 1][i - 1].Cellule_energy = Block[j][i].Cellule_energy;
                        Block[j][i].Cellule_energy = 0;
                    }
                }
                else
                    return -2;
                break;
        }
    }
    return 0;
}

void move_cellule_to_dst(block_info Block[height][width] , int i , int j){
    print_menu(str , 10 , 16 , pos_x , pos_y);
    print_box("what is your choice : " , pos_x + 4 , pos_y + 8);
    scanf("%d",&k);
    move(Block , i , j , k , "normal");
}

void split_cellule(block_info Block[height][width] , int i , int j){ //dose not work actually :/
    int random_p , MRV = -2 ;
    if (Block[j][i].Cellule_energy >= 80 && Block[j][i].Block_Type == '2'){
        while(MRV == -2){
            random_p = rand_int(6) + 1;
            MRV = move(Block , i , j , random_p , "split");
            Block[j][i].Block_defulat_value = 0;
        }
    }
}

void save(const char * name , int height_save , int width_save , block_info Block[height_save][width_save]){
    FILE *fp;
    fp = fopen(name , "w");
    if(fp == NULL){
        printf("Cannot open this file\n");
        return -1;
    }
    fprintf(fp , "%d %d\n" , height_save , width_save);
    for(j = 0 ; j < height_save ; j++){
        for(i = 0 ; i < width_save ; i++){
            fprintf(fp , "%d " , Block[j][i].Block_Energy);
            fprintf(fp , "%c " , Block[j][i].Block_Type);
            fprintf(fp , "%d " , Block[j][i].Block_x);
            fprintf(fp , "%d " , Block[j][i].Block_y);
            fprintf(fp , "%d " , Block[j][i].Block_defulat_value);
            fprintf(fp , "%d\n", Block[j][i].Cellule_energy);
        }
    }
    fclose(fp);
}

void basic_load(const char * name){
    FILE *fp;
    fp = fopen(name , "r");
    if(fp == NULL){
        print_box("Cannot open this file" , pos_x + 4 , pos_y + 4);
        exit(-1);
    }
    fscanf(fp , "%d %d\n" , &height , &width);
    fclose(fp);
}

void load(const char * name , int height_load , int width_load , block_info Block[height_load][width_load]){
    FILE *fp;
    fp = fopen(name , "r");
    if(fp == NULL){
        print_box("Cannot open this file" , pos_x + 4 , pos_y + 4);
        exit(-1);
    }
    fscanf(fp , "%d %d\n" , &height_load , &width_load);
    for(j = 0 ; j < height_load ; j++){
        for(i = 0 ; i < width_load ; i++){
            fscanf(fp , "%d " , &Block[j][i].Block_Energy);
            fscanf(fp , "%c " , &Block[j][i].Block_Type);
            fscanf(fp , "%d " , &Block[j][i].Block_x);
            fscanf(fp , "%d " , &Block[j][i].Block_y);
            fscanf(fp , "%d " , &Block[j][i].Block_defulat_value);
            fscanf(fp , "%d\n", &Block[j][i].Cellule_energy);
        }
    }
    fclose(fp);
}

void save_address_of_saves(char name_for_save[30]){
    FILE *fpSave;
    fpSave = fopen("save_name_files.txt" , "r+");
    if(fpSave == NULL){
        fpSave = fopen("save_name_files.txt" , "w");
        if(fpSave == NULL){
        print_box("Cannot open this file" , pos_x + 4 , pos_y + 5);
        return -1;
        }
    }
    clear_box(pos_x , pos_y , tool , arz);
    fseek(fpSave , 0 , SEEK_END);
    print_box("Choose a name for your " , pos_x + 4 , pos_y + 2);
    print_box("save file[remember to- " , pos_x + 4 , pos_y + 3);
    print_box("add .txt at the end] : " , pos_x + 4 , pos_y + 4);
    gotoxy(pos_x + 4 , pos_y + 5);
    scanf("%s" , name_for_save);
    fprintf(fpSave , "%s\r\n" , name_for_save);
    fclose(fpSave);

}

int load_address_of_saves(char name_for_load[30]){
    FILE *fpSave;
    fpSave = fopen("save_name_files.txt" , "r");
    if(fpSave == NULL){
        clear_box(pos_x , pos_y , tool , arz);
        print_box("you have no save..." , pos_x + 4 , pos_y + 3 + counter);
        return -1;
    }
    clear_box(pos_x , pos_y , tool , arz);
    print_box("Choose a file to load: " , pos_x + 4 , pos_y + 2);
    fseek(fpSave , 0 , SEEK_SET);
    do{
        fscanf(fpSave , "%s\r\n" , name_for_load);
        print_box(name_for_load , pos_x + 4 , pos_y + 3 + counter);
        counter++;
    }while (!feof(fpSave));
    fclose(fpSave);
    return 0;
}

void basic_map_info(){
    clear_box(pos_x , pos_y , tool , arz);
    print_box("what is the height and " , pos_x + 4 , pos_y + 2);
    print_box("width of your map?: " , pos_x + 4 , pos_y + 3);
    scanf("%d %d" , &height_edit , &width_edit);
}

void create_map(block_info Block_edit[height_edit][width_edit]){
    char type;
    int x , y , energy;
    box(tool , arz , pos_x , pos_y , 219);
    print_box("choose a Block : " , pos_x + 4 , pos_y + 2);
    scanf("%d %d" , &x , &y);
    print_menu(str , 16 , 20 , pos_x , pos_y);
    gotoxy(pos_x + 4 , pos_y + 7);
    scanf("%s" , &type);
    Block_edit[y][x].Block_Type = type;
    if(type == '1'){
        print_box("Energy you need?: " , pos_x + 4 , pos_y + 8);
        scanf("%d" , &energy);
        Block_edit[y][x].Block_Energy = energy;
    }

}

void map_editor(HANDLE hConsole , block_info Block_edit[height_edit][width_edit]){
    char name_for_save_edit_map[30];
    int shomarande , x , y , z;
    char ans;
    system("cls");
    Chart_Hexagonal(hConsole , height_edit , width_edit , Block_edit);
    SetConsoleTextAttribute(hConsole, 15);
    for(shomarande = 0 ; find_value('5' , height_edit , width_edit , Block_edit) == -1 ; shomarande++){
        create_map(Block_edit);
        system("cls");
        Chart_Hexagonal(hConsole , height_edit , width_edit , Block_edit);
        SetConsoleTextAttribute(hConsole, 15);
    }
    box(tool , arz , pos_x , pos_y , 219);
    clear_box(pos_x , pos_y , tool , arz);
    print_box("number of cellules : " , pos_x + 4 , pos_y + 2);
    scanf("%d" , &z);
    shomarande = 0;
    while(shomarande < z){
        box(tool , arz , pos_x , pos_y , 219);
        clear_box(pos_x , pos_y , tool , arz);
        print_box("now place the cellule" , pos_x + 4 , pos_y + 2);
        print_box("choose a Block : " , pos_x + 4 , pos_y + 3);
        scanf("%d %d" , &x , &y);
        if (Block_edit[y][x].Block_defulat_value == -1 && Block_edit[y][x].Block_Type != '3'){
            Block_edit[y][x].Block_defulat_value = 0;
            system("cls");
            Chart_Hexagonal(hConsole , height_edit , width_edit , Block_edit);
            SetConsoleTextAttribute(hConsole, 15);
            shomarande++;
        }
    }
    clear_box(pos_x , pos_y , tool , arz);
    box(tool , arz , pos_x , pos_y , 219);
    print_box("do you want to save?[y/n]: " , pos_x + 4 , pos_y + 2);
    scanf("%s" , &ans);
    switch(ans){
        case 'y':
            save_address_of_saves(name_for_save_edit_map);
            save(name_for_save_edit_map , height_edit , width_edit , Block_edit);
            break;
        case 'n':
            break;
    }
}

int singel_player_menu(int height_menu , int width_menu , block_info Block[height_menu][width_menu]){
    char name_for_save[30];
    print_menu(str , 5 , 10 , pos_x , pos_y);
    print_box("what is your choice : " , pos_x + 4 , pos_y + 7);
    scanf("%d",&k);
    switch (k){
        case 1:
            print_box("which Block? : " , pos_x + 4 , pos_y + 8);
            scanf("%d %d" , &i , &j);
            move_cellule_to_dst(Block , i , j);
            break;
        case 2:
            print_box("which Block? : " , pos_x + 4 , pos_y + 8);
            scanf("%d %d" , &i , &j);
            split_cellule(Block , i , j);
            break;
        case 3:
            print_box("which Block? : " , pos_x + 4 , pos_y + 8);
            scanf("%d %d" , &i , &j);
            if(Block[j][i].Block_Energy >= 15 && Block[j][i].Block_defulat_value == 0){
                Block[j][i].Block_Energy -= 15;
                Block[j][i].Cellule_energy += 15;
            }
            else{
                Block[j][i].Cellule_energy += Block[j][i].Block_Energy;
                Block[j][i].Block_Energy = 0;
            }
            break;
        case 4:
            save_address_of_saves(name_for_save);
            save(name_for_save , height_menu , width_menu , Block);
            break;
        case 5:
            return -1;
            break;
        default:
            system("cls");
            exit(-1);
    }
    return 0;
}

int start_menu(char name_for_load[30]){
    print_menu(str , 0 , 5 , pos_x , pos_y);
    print_box("what is your choice : " , pos_x + 4 , pos_y + 7);
    scanf("%d" , &k);
    switch(k){
        case 1:
            k = load_address_of_saves(name_for_load);
            if(k == -1){
                getch();
                return -2;
            }
            gotoxy(pos_x + 4 , pos_y + 4 + counter);
            scanf("%s" , name_for_load);
            counter = 0;
            return 1;
            break;
        case 2:
            print_box("number of cellule : " , pos_x + 4 , pos_y + 8);
            scanf("%d" , &Cellule_number);
            system("cls");
            break;
        case 3:
            exit(-1);
        case 4:
            return 2;
            break;
        case 5:
            system("cls");
            return -1;
            break;
        default:
            system("cls");
            exit(-1);
    }
    return 0;
}

void score(int height_score , int width_score , block_info Block[height_score][width_score]){
    int counter_i = 0 , counter_j = 0 , ko = 0 , lo;
    box(34 , 29 , 71 , 0 , 219);
    lo = find_number(0 , height_score , width_score , Block);
    gotoxy(3 , 100);
    printf("%d",lo);
    for (counter_j = 0 ; counter_j < height_score ; counter_j++){
        for (counter_i = 0 ; counter_i < width_score ; counter_i++){
            if (Block[counter_j][counter_i].Block_defulat_value == 0){
                print_box("list of your cellule :" , 75 , 2);
                print_indentation(lo , 71 , 2 , 2);
                print_box("in " , 80 , 4 + ko);
                printf("(%d , %d)" , counter_i , counter_j);
                print_box("Cellule energy : " , 83 , 5 + ko);
                printf("%d" , Block[counter_j][counter_i].Cellule_energy);
                ko += 2;
            }
        }
    }
}

void helper_run(HANDLE hConsole , int height_help , int width_help , block_info Block[height_help][width_help]){
    while(k != -1){
        Chart_Hexagonal(hConsole , height_help , width_help , Block);
        SetConsoleTextAttribute(hConsole, 15);
        score(height_help , width_help , Block);
        k = singel_player_menu(height_help , width_help , Block);
        system("cls");
    }
}

void play_game(HANDLE hConsole , FILE * fchart){
    char name_for_load[30];
    k = start_menu(name_for_load);
    if (k == -1){
        exit(-1);
    }
    else if(k == 1){
        basic_load(name_for_load);
        block_info Block[height][width];
        make_defualt(height , width , Block);
        load(name_for_load , height , width , Block);
        helper_run(hConsole , height , width , Block);
    }
    else if(k == 0){
        basic_info(fchart);
        block_info Block[height][width];
        make_defualt(height , width , Block);
        Read_Chart(fchart , Block);
        Chart_Hexagonal(hConsole , height , width , Block);
        SetConsoleTextAttribute(hConsole, 15);
        create_cellule(Block);
        score(height , width , Block);
        k = singel_player_menu(height , width , Block);
        system("cls");
        helper_run(hConsole , height , width , Block);
    }
    else if(k == 2){
        basic_map_info();
        block_info Block_edit[height_edit][width_edit];
        make_defualt(height_edit , width_edit , Block_edit);
        map_editor(hConsole , Block_edit);
        system("cls");
        play_game(hConsole , fchart);
    }
    play_game(hConsole , fchart);
}

int main(void){
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE * fchart;
    fchart = fopen("map.bin" , "rb");
    if(fchart == NULL){
        printf("Cannot open this file\n");
        return -1;
    }
    play_game(hConsole , fchart);

    SetConsoleTextAttribute(hConsole, 15);
    system("cls");
    return 0;
}
