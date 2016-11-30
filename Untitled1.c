 //box(30 , 10 , 15 * width , 2 * height , 219);
    /*gotoxy(15 * width + 2 , 2 * height + 2);
    printf("[1] Load");
    gotoxy(15 * width + 2 , 2 * height + 3);
    printf("[2] New single player game");
    gotoxy(15 * width + 2 , 2 * height + 4);
    printf("[3] New Multiplayer game");
    gotoxy(15 * width + 2 , 2 * height + 5);
    printf("[4] Exit");
    */
    gotoxy(0 , 7 * height);
    /*for(j = 0 ; j < height ; j ++){
        for(i = 0 ; i < width ; i ++){
            printf("%d , %d\n" , Block_center_points[j][i].x , Block_center_points[j][i].y );
        }
    }*/


    /*
void create_cellule(tak_Cellulee tak_cellulee[]){
    j = rand_int(height);
    i = rand_int(width);
    tak_cellulee[].place.x = i;
    tak_cellulee[].place.y = j;
    tak_cellulee[].Energy = 0;
}
*/



for(j = 0 ; j < height ; j ++){
        for(i = 0 ; i < width ; i ++){
            printf("(%2d , %2d) ----- Block_center_points[%d][%d]\n" , Block_center_points[j][i].x , Block_center_points[j][i].y , j , i);
        }
    }
















