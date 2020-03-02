//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include "data_type.h"
#include "func.h"
#include <malloc/malloc.h>
#include <termios.h>

// (0) 기타
void print_bag(Pokemon ***pokemon_bag){
    int i, j, k;
    for(i = 0; i < BAG_NUM; i++){
        printf("가방 %d\n", i + 1);
        for(j = 0; j < BAG_ROW; j++){
            for(k = 0; k <= j; k++){
                if(pokemon_bag[i][j][k].level != 0){
                    printf("[Lv.%d %s] ", pokemon_bag[i][j][k].level, pokemon_bag[i][j][k].name);
                }
                else printf("[비어있음] ");
            }
            printf("\n");
        }
        printf("\n");
    }
} // 가방에 있는 포켓몬 보는 함수
void gotoxy(int x, int y){
    printf("\033[%d;%df", y - 1, x - 1);
    fflush(stdout);
} // 커서 이동
int getch(void){
    int ch;
    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}  // 터미널 현재 입력 확인 함수
void print_battle_pokemon(Player *p1, int len){
    int i;
    printf("현재 포켓몬 정보입니다.\n");
    for(i = 0; i < len; i++){
        printf("%d. Lv.%d %s  Hp: [%d / %d]  power: [%d]  exp: [%d / %d] \n", i + 1, (p1->battle_pokemon)[i].level,
               (p1->battle_pokemon)[i].name,
               (p1->battle_pokemon)[i].hp, (p1->battle_pokemon)[i].max_hp, (p1->battle_pokemon)[i].power, (p1->battle_pokemon)[i].exp,
               (p1->battle_pokemon)[i].level * 2);
    }
}
int battle_pokemon_len(Player *p){
    int i, num = 0;
    for(i = 0; i < 6; i++){
        if(p->battle_pokemon[i].attribute != 0) num++;
    }
    return num;
}
void cheat(Player *p1, Pokemon **pokemon_book){
    p1->battle_pokemon[1] = pokemon_book[1][2];
    p1->battle_pokemon[2] = pokemon_book[2][2];
    p1->battle_pokemon[3] = pokemon_book[3][0];
    p1->battle_pokemon[0].level = 9;
    p1->battle_pokemon[1].level = 12;
    p1->battle_pokemon[2].level = 12;
    p1->battle_pokemon[3].level = 14;
    pokemon_setting(&(p1->battle_pokemon[0]));
    pokemon_setting(&(p1->battle_pokemon[1]));
    pokemon_setting(&(p1->battle_pokemon[2]));
    pokemon_setting(&(p1->battle_pokemon[3]));
    p1->battle_pokemon[0].exp = 17;
    p1->badge = 4;
}
void my_fflush(){
    char c;
    while(1){
        scanf("%c", &c);
        if(c == '\n') break;
    }
} // 버퍼 비우기

// (1) intro
int main_UI(){
    int i = 0;
    int x = 9, y = 2;
    int level = 0;

    system("clear");
    setcolor(black); // black
    gotoxy(x + 1, y + 1);;
    setcolor(red); // red
    printf("■");
    for(i = 0; i < 29; i++){
        setcolor(black); // black
        printf("--");
    }
    setcolor(red); // red
    printf("■");

    for(i = 1; i < 22; i++){
        y++;
        setcolor(black); // black
        gotoxy(x + 1, y + 1);;
        printf("|");
        if(i == 8){
            setcolor(yellow); // yellow
            gotoxy(31, i + 1);
            printf("\t◇ 포켓몬스터 ◇ \n\n\n\n\n\n\t\t\t\t 1. 새로하기 \n\n\n\t\t\t\t 2. 이어하기 \n  ");
        }

        x += 61;
        setcolor(black); // black
        gotoxy(x + 1, y + 1);;
        printf("|");
        x = 9;
    }
    setcolor(red); // red
    gotoxy(x + 1, y + 1);;
    printf("■");
    setcolor(black); // black
    for(i = 0; i < 29; i++)
        printf("--");
    setcolor(red); // red
    printf("■ \n\n");
    setcolor(black); // black

    while(1){
        printf("\t\t 원하는 모드를 입력하고 엔터를 눌러주세요 : ");
        scanf("%d", &level);
        my_fflush();
        if(level == 1 || level == 2){
            return level;
        }
        else{
            printf("\t\t 잘못 누르셨습니다. 다시 눌러주세요.\n");
        }
    }
//    level = get_num('2'); //1,2만 입력받는 함수
    return level;
}

void setcolor(char color[]){
    printf("\x1b[%s", color);
}




// (2) 포켓몬 도감 생성
int makeRow(Pokemon **p_addr, int row){
    *p_addr = (Pokemon *) malloc(sizeof(Pokemon) * row);

    if(*p_addr == NULL){
        printf("할당 실패\n");
        return 1;
    }
    memset(*p_addr, 0x0, sizeof(Pokemon) * row);
    return 0;
}

int makeCol(Pokemon ***p_addr, int col){
    *p_addr = (Pokemon **) malloc(sizeof(Pokemon *) * col);

    if(*p_addr == NULL){
        printf("할당 실패\n");
        return 1;
    }
    memset(*p_addr, 0x0, sizeof(Pokemon *) * col);
    return 0;
}

int make_pokemon_book(Pokemon ***p_addr){
    int malloc_fail;
    int i, j, k = 0, l = 0;
    FILE *f1;

    malloc_fail = makeCol(p_addr, 4);
    if(malloc_fail == 1){
        printf("공간 할당이 실패했습니다.\n");
        return malloc_fail;
    }
    for(i = 0; i < 4; i++){
        malloc_fail = makeRow(*p_addr + i, 4);
        if(malloc_fail == 1){
            printf("공간 할당이 실패했습니다.\n");
            return malloc_fail;
        }
        else
            for(j = 0; j < 4; j++){
                (*(*p_addr + i))[j].name_list = (char **) malloc(sizeof(char *) * 3);
                (*(*p_addr + i))[j].name_list[0] = (char *) malloc(sizeof(char) * 20);
                (*(*p_addr + i))[j].name_list[1] = (char *) malloc(sizeof(char) * 20);
                (*(*p_addr + i))[j].name_list[2] = (char *) malloc(sizeof(char) * 20);
                (*(*p_addr + i))[j].skill_info[0].skill_name = (char *) malloc(sizeof(char) * 20);
                (*(*p_addr + i))[j].skill_info[1].skill_name = (char *) malloc(sizeof(char) * 20);
                (*(*p_addr + i))[j].skill_info[2].skill_name = (char *) malloc(sizeof(char) * 20);
            }
    }
    f1 = fopen("/Users/KDH/C_Programming/Pokemon/pokemon.txt", "r");
    if(f1 != NULL){
        while(!feof(f1)){
            fscanf(f1, "%s", (*p_addr)[k][l].name_list[0]);
            fscanf(f1, "%s", (*p_addr)[k][l].name_list[1]);
            fscanf(f1, "%s", (*p_addr)[k][l].name_list[2]);
            fscanf(f1, "%d", &(*p_addr)[k][l].attribute);
            fscanf(f1, "%d", &(*p_addr)[k][l].power);
            fscanf(f1, "%d", &(*p_addr)[k][l].max_hp);
            fscanf(f1, "%s", (*p_addr)[k][l].skill_info[0].skill_name);
            fscanf(f1, "%d", &(*p_addr)[k][l].skill_info[0].type);
            fscanf(f1, "%s", (*p_addr)[k][l].skill_info[1].skill_name);
            fscanf(f1, "%d", &(*p_addr)[k][l].skill_info[1].type);
            fscanf(f1, "%s", (*p_addr)[k][l].skill_info[2].skill_name);
            fscanf(f1, "%d", &(*p_addr)[k][l].skill_info[2].type);
            l++;
            if(l == 4){
                k++;
                l = 0;
            }
        }
    }
    return 0;
}


// (3) 포켓몬 도감 스킬 세팅
void attack_skill(char skill_name[], int i, Pokemon *x, Pokemon *y){
    int damage;
    int tmp;
    if(i == 1) damage = x->power * 1.2;
    else if(i == 2) damage = x->power * 1.4;
    else if(i == 3) damage = x->power * 1.6;
    if(x->attribute == 1){
        if(y->attribute == 2) damage *= 1.25;
        else if(y->attribute == 4 || y->attribute == 5) damage *= 0.75;
    }
    else if(x->attribute == 2){
        if(y->attribute == 3) damage *= 1.25;
        else if(y->attribute == 1 || y->attribute == 5) damage *= 0.75;
    }
    else if(x->attribute == 3){
        if(y->attribute == 4) damage *= 1.25;
        else if(y->attribute == 2 || y->attribute == 5) damage *= 0.75;
    }
    else if(x->attribute == 4){
        if(y->attribute == 1) damage *= 1.25;
        else if(y->attribute == 3 || y->attribute == 5) damage *= 0.75;
    }

    printf("%s의 %s!\n", x->name, skill_name);
    tmp = y->hp;
    y->hp -= damage;
    printf("%s는 %d의 데미지를 입었다.\n", y->name, damage);
    printf("%s의 hp: [%d] --> [%d]\n", y->name, tmp, y->hp);
    printf("=================================\n");
}

void heal_skill(char skill_name[], int i, Pokemon *x, Pokemon *y){
    int heal;
    heal = x->power * 1.1;

    printf("%s의 %s!\n", x->name, skill_name);
    if(x->max_hp - x->hp <= heal) x->hp = x->max_hp;
    else x->hp += heal;
    printf("%s는 %d의 hp를 회복했다.\n", x->name, heal);
}

void skill_setting(Pokemon ***p_addr){
    int i, j, k;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 3; k++){
                if((*p_addr)[i][j].skill_info->type == 0) (*p_addr)[i][j].skill[k] = &attack_skill;
                else (*p_addr)[i][j].skill[k] = &heal_skill;
            }
        }
    }
}


// (4) 플레이어 공간 생성
char *player_name_malloc(char *name){
    name = (char *) malloc(sizeof(char) * MAX_NAME);
    if(name == NULL){
        printf("플레이어 or 보스 이름 공간 할당 실패\n");
    }
    else{
        memset(name, 0x0, sizeof(char) * MAX_NAME);
    }
    return name;
}

Pokemon *make_battle_pokemon_malloc(Pokemon *battle_pokemon){
    battle_pokemon = (Pokemon *) malloc(sizeof(Pokemon) * BATTLE_POKEMON_COUNT);
    if(battle_pokemon == NULL){
        printf("플레이어 or 보스 배틀 포켓몬 공간 할당 실패\n");
        exit(1);
    }
    else{
        memset(battle_pokemon, 0x0, sizeof(Pokemon) * BATTLE_POKEMON_COUNT);
    }
    return battle_pokemon;
}

Inventory *make_inventory(Inventory *inventory){
    inventory = (Inventory *) malloc(sizeof(Inventory) * 1);
    if(inventory == NULL){
        printf("인벤토리 공간 할당실패\n");
    }
    else{
        memset(inventory, 0x0, sizeof(Inventory) * 1);
    }
    return inventory;
}

Pokemon **make_col_pokemon_bag(Pokemon **pokemon_bag){
    pokemon_bag = (Pokemon **) malloc(sizeof(Pokemon *) * BAG_ROW);
    if(pokemon_bag == NULL){
        printf("플레이어의 포켓몬 가방의 열 공간 할당 실패\n");
        exit(1);
    }
    else
        memset(pokemon_bag, 0x0, sizeof(Pokemon **) * BAG_ROW);
    return pokemon_bag;
}

Pokemon *make_row_pokemon_bag(Pokemon *pokemon_bag, int num){
    pokemon_bag = (Pokemon *) malloc(sizeof(Pokemon) * num);
    if(pokemon_bag == NULL){
        printf("플레이어의 포켓몬 가방의 행 공간 할당 실패\n");
        exit(1);
    }
    else
        memset(pokemon_bag, 0x0, sizeof(Pokemon *) * num);
    return pokemon_bag;
}

Pokemon ***make_pokemon_bag(Pokemon ***pokemon_bag){
    int i, j;
    pokemon_bag = (Pokemon ***) malloc(sizeof(Pokemon **) * BAG_NUM);
    if(pokemon_bag == NULL){
        printf("플레이어의 포켓몬 가방 할당 실패\n");
        exit(1);
    }
    else
        memset(pokemon_bag, 0x0, sizeof(Pokemon ***) * BAG_NUM);
    for(i = 0; i < 2; i++){
        pokemon_bag[i] = make_col_pokemon_bag(pokemon_bag[i]);
        for(j = 0; j < 4; j++){
            pokemon_bag[i][j] = make_row_pokemon_bag(pokemon_bag[i][j], j + 1);
        }
    }
    return pokemon_bag;
}

Player *make_player(Player **player_addr){
    *player_addr = (Player *) malloc(sizeof(Player) * PLAYER_NUM);
    if(*player_addr == NULL){
        printf("플레이어 공간 할당 실패\n");
        exit(1);
    }
    else{
        memset(*player_addr, 0x0, sizeof(Player) * PLAYER_NUM);
    }
    // 이름 공간 할당
    (*player_addr)->name = player_name_malloc((*player_addr)->name);
    // 배틀 포켓몬 공간 할당
    (*player_addr)->battle_pokemon = make_battle_pokemon_malloc((*player_addr)->battle_pokemon);
    // 인벤토리 공간 할당
    (*player_addr)->inventory = make_inventory((*player_addr)->inventory);
    // 가방 공간 할당
    (*player_addr)->pokemon_bag = make_pokemon_bag((*player_addr)->pokemon_bag);
    return *player_addr;
}


// (5) 플레이어 정보 입력
void set_pokemon_info(Player *player, Pokemon ***pokemon_book_addr){
    char tmp;
    int i;
    char print_temp[256];
    FILE *rfp = NULL;

    printf("(1)피츄 (2)꼬부기 (3)파이리\n");
    usleep(300000);
    while(1){
        printf("포켓몬을 선택하시오: ");
        scanf("%c", &tmp);
        if(tmp == '1' || tmp == '2' || tmp == '3'){
            i = ((int)tmp - 48);
            break;
        }
        else{
            printf("잘못 누르셨습니다. 다시 눌러주세요.\n");
            my_fflush();
        }
    }
    switch(i){
        case 1://피츄
            (*player).battle_pokemon[0] = (*pokemon_book_addr)[3][0];
            (*player).battle_pokemon[0].level = 1;
            pokemon_setting(&(*player).battle_pokemon[0]);
            system("clear");
            rfp = fopen("/Users/KDH/C_Programming/Pokemon/image/pichu.tmg", "rt");
            break;
        case 2://꼬부기
            (*player).battle_pokemon[0] = (*pokemon_book_addr)[0][0];
            (*player).battle_pokemon[0].level = 1;
            pokemon_setting(&(*player).battle_pokemon[0]);
            system("clear");
            rfp = fopen("/Users/KDH/C_Programming/Pokemon/image/ggobugi.tmg", "rt");
            break;
        case 3://파이리
            (*player).battle_pokemon[0] = (*pokemon_book_addr)[1][0];
            (*player).battle_pokemon[0].level = 1;
            pokemon_setting(&(*player).battle_pokemon[0]);
            system("clear");
            rfp = fopen("/Users/KDH/C_Programming/Pokemon/image/pairi.tmg", "rt");
            break;
    }
    if(rfp == NULL){
        printf("이미지 로딩 실패 \n");
        exit(1);
    }
    while(fgets(print_temp, 100, rfp) != NULL){
        printf("%s", print_temp);
    }
    puts("");
    usleep(1000000);
    fclose(rfp);
}

void set_inventory_info(Player *player){
    printf("[오박사] 여정에 필요한 캐시, 몬스터볼, 상처약을 주겠네\n");
    usleep(300000);
    player->cash = CASH;
    printf("캐시 %d원을 획득하였습니다.\n", player->cash);
    usleep(300000);
    player->inventory->monster_ball = MONSTER_BALL;
    usleep(300000);
    printf("포켓볼 %d개를 획득하였습니다.\n", player->inventory->monster_ball);
    usleep(300000);
    player->inventory->medicine = Medicine;
    usleep(300000);
    printf("상처약 %d개를 획득하였습니다.\n", player->inventory->medicine);
    usleep(300000);
}

void set_player_info(Player *player, Pokemon ***pokemon_book_addr){
    printf("[오박사] 자네의 이름은 무엇인가?: ");
    scanf("%s", player->name);
    my_fflush();
    printf("[오박사] 반갑네, %s!\n", player->name);
    usleep(300000);
    printf("[오박사] 자네의 모험과 함께할 포켓몬을 한 마리 고르시게나!\n");
    usleep(300000);
    set_pokemon_info(player, pokemon_book_addr);
    set_inventory_info(player);
}


// (6) 플레이어 정보 출력
int print_player(Player *player){
    int i = 0;
    int len;
    len = battle_pokemon_len(player);
    //플레이어 정보
    printf("=================== Player 정보 ===================\n");
    usleep(300000);
    printf("이름: %s\n", player->name);
    usleep(300000);
    printf("캐시: %d원\n", player->cash);
    usleep(300000);
    printf("뱃지: %d개\n", player->badge);
    usleep(300000);
    printf("몬스터볼: %d개\n", player->inventory->monster_ball);
    usleep(300000);
    printf("진화의 돌: %d개\n", player->inventory->evolution_stone);
    usleep(300000);
    printf("상처약: %d개\n", player->inventory->medicine);
    usleep(300000);
    printf("가방에는 %d마리의 포켓몬이 있습니다.\n", len);
    usleep(300000);
    //포켓몬 정보
    printf("=================== 동행 포켓몬 정보 ===================\n");
    usleep(300000);
    printf("이름: %s\n", player->battle_pokemon->name);
    usleep(300000);
    printf("HP: %d\n", player->battle_pokemon->hp);
    usleep(300000);
    printf("POWER: %d\n",player->battle_pokemon->power);
    usleep(300000);
    printf("EXP: %d\n", player->battle_pokemon->exp);
    usleep(300000);
    printf("[오박사] 즐거운 여행되기를 바라네!\n");
    usleep(300000);
    while(1){
        printf("마을로 이동하시겠습니까?\n");
        printf("(1)yes (2)no\n");
        printf("--> ");
        scanf("%d", &i);
        if(i == 1 || i == 2){
            break;
        }
        else{
            printf("잘못 누르셨습니다. 다시 눌러주세요.\n");
            my_fflush();
        }
    }
    return i;
}




// (7) 보스 공간 생성
Player *make_boss(Player **boss_addr){
    int i, j;
    *boss_addr = (Player *) malloc(sizeof(Player) * BOSS_NUM);
    if(*boss_addr == NULL){
        printf("보스공간 할당실패\n");
        exit(1);
    }
    else{
        memset(*boss_addr, 0x0, sizeof(Player) * BOSS_NUM);
    }
    for(i = 0; i < BOSS_NUM; i++){
        (*boss_addr)[i].name = player_name_malloc((*boss_addr)[i].name);
        (*boss_addr)[i].battle_pokemon = make_battle_pokemon_malloc((*boss_addr)[i].battle_pokemon);
    }
    return *boss_addr;
}


// (8) 보스 정보 입력
void set_boss_info2(Player *boss, Pokemon **pokemon_book){

    int i, j, k;
    char *boss_name[4] = {"속초마을 관장(물)", "태초마을 관장(불)", "상록시티 관장(풀)", "금빛시티 관장(전기)"};

    for(i = 0; i < 4; i++){
        boss[i].name = boss_name[i];
        for(j = 0; j <= i; j++){
            boss[i].battle_pokemon[i] = pokemon_book[i][j];
            boss[i].battle_pokemon[i].level = (i + 1) * 2;
            pokemon_setting(&(boss[i].battle_pokemon[i]));
        }

    }
    // 마지막 보스 설정
    boss[4].name = "새천년관 지배자";
    boss[4].battle_pokemon[0].name = "최윤정";
    boss[4].battle_pokemon[0].level = 15;
    boss[4].battle_pokemon[0].attribute = 5;
    boss[4].battle_pokemon[0].hp = 420;
    boss[4].battle_pokemon[0].max_hp = 420;
    boss[4].battle_pokemon[0].power = 95;
    boss[4].battle_pokemon[0].skill_info[0].skill_name = "소리치기";
    boss[4].battle_pokemon[0].skill_info[1].skill_name = "집에가";
    boss[4].battle_pokemon[0].skill_info[2].skill_name = "과제내기";
    boss[4].battle_pokemon[0].skill_info[0].type = 1;
    boss[4].battle_pokemon[0].skill_info[1].type = 1;
    boss[4].battle_pokemon[0].skill_info[2].type = 1;
    for(k = 0; k < 3; k++){
        boss[4].battle_pokemon[0].skill[k] = &attack_skill;
    }
}

void pokemon_setting(Pokemon *p_addr){
    double tmp;
    if((*p_addr).level < 5){
        (*p_addr).name = p_addr->name_list[0];
    }
    else if((*p_addr).level < 10){
        (*p_addr).name = p_addr->name_list[1];
    }
    else{
        (*p_addr).name = p_addr->name_list[2];
    }
    tmp = pow(1.1, (*p_addr).level - 1);
    (*p_addr).power *= tmp;
    (*p_addr).max_hp *= tmp;
    (*p_addr).hp = (*p_addr).max_hp;
}


// (9) 맵
void make_village(int village[VILLAGE_ROW][VILLAGE_COL], Player *player){
    int i, j, badge;
    char *city_name[5] = {"속초 마을(물)", "태초 마을(불)", "상록 시티(풀)", "금빛 시티(전기)", "새천년관"};

    badge = player->badge;
    printf("%s에 온 것을 환영합니다.\n", city_name[badge]);
    printf("♥: 포켓몬 센터  ◆: 사냥터  ★: 상점  ▲: 체육관\n");
    for(i = 0; i < VILLAGE_ROW; i++){
        for(j = 0; j < VILLAGE_COL; j++){
            gotoxy(11 + i * 2, 12 + j);
            if((i == 0) && (j == 1)){
                village[i][j] = 1;
                printf("▲");
            }
            else if(i == 1 && j == 2){
                village[i][j] = 2;
                printf("★");
            }
            else if(i == 1 && j == 0){
                village[i][j] = 3;
                printf("♥");
            }
            else if(i == 2 && j == 1){
                village[i][j] = 4;
                printf("◆");
            }
            else{
                village[i][j] = 5;
                printf("□");
            }
        }
        printf("\n");
    }
}

void pokemon_center(Player *player){
    int i, select;

    while(1){
        system("clear");
        printf("무엇을 도와드릴까요?\n");
        printf("1. 포켓몬 치료\n");
        printf("2. 나가기\n");
        printf("--> ");
        scanf("%d", &select);
        if(select == 1){
            for(i = 0; i < BATTLE_POKEMON_COUNT; i++){
                if(player->battle_pokemon[i].name != NULL){
                    printf("Lv.%d %s의 hp를 [%d] -> [%d]으로 치료했습니다\n", player->battle_pokemon[i].level,
                           player->battle_pokemon[i].name, player->battle_pokemon[i].hp,
                           player->battle_pokemon[i].max_hp);
                    player->battle_pokemon[i].hp = player->battle_pokemon[i].max_hp;
                }
            }
            break;
        }
        else if(select == 2){
            printf("마을로 돌아갑니다.\n");
            break;
        }
        else{
            printf("잘못 누르셨습니다. 다시 눌러주세요.\n");
            usleep(1000000);
        }
    }
    usleep(1000000);
}

void supermarket(Player *player){
    int select = 0;
    while(1){
        system("clear");
        printf("무엇을 고르시겠습니까?\n");
        printf("====== 인벤토리 ======\n");
        printf("몬스터볼: %d개, 상처약: %d개, 진화의돌: %d개\n", player->inventory->monster_ball,
               player->inventory->medicine, player->inventory->evolution_stone);
        printf("====== 가격표 ======\n");
        printf("1. 몬스터볼 3개(1,000원)  2. 상처약 3개(1,000원)  3. 진화의 돌 1개(30,000원)  4.나가기\n");
        printf("현재 돈: %d원\n", player->cash);
        printf("--> ");
        scanf("%d", &select);
        if(select == 1){
            if(player->cash < 1000){
                printf("잔액이 부족합니다.\n");
                usleep(1000000);
            }
            else{
                printf("몬스터볼 3개를 구입합니다.\n");
                printf("몬스터볼 [%d] -> ", player->inventory->monster_ball);
                player->inventory->monster_ball += 3;
                player->cash -= 1000;
                printf("[%d]\n", player->inventory->monster_ball);
                printf("남은 돈: %d원\n", player->cash);
                usleep(1000000);
            }
        }
        else if(select == 2){
            if(player->cash < 1000){
                printf("잔액이 부족합니다.\n");
                usleep(1000000);
            }
            else{
                printf("상처약 3개를 구입합니다.\n");
                printf("상처약 [%d] -> ", player->inventory->medicine);
                player->inventory->medicine += 3;
                player->cash -= 1000;
                printf("[%d]\n", player->inventory->medicine);
                printf("남은 돈: %d원\n", player->cash);
                usleep(1000000);
            }
        }
        else if(select == 3){
            if(player->cash < 30000){
                printf("잔액이 부족합니다.\n");
                usleep(1000000);
            }
            else{
                printf("진화의 돌 1개를 구입합니다.\n");
                printf("진화의 돌 [%d] -> ", player->inventory->evolution_stone);
                player->inventory->evolution_stone += 1;
                player->cash -= 30000;
                printf("[%d]\n", player->inventory->evolution_stone);
                printf("남은 돈: %d원\n", player->cash);
                usleep(1000000);
            }
        }
        else if(select == 4){
            printf("마을로 돌아갑니다.\n");
            usleep(1000000);
            break;
        }
        else{
            printf("잘못 누르셨습니다. 다시 눌러주세요.\n");
            usleep(1000000);
        }
    }
}

void make_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]){
    int i = 0, j = 0;
    int cnt = 0;
    for(i = 0; i < BATTLE_ROW; i++){
        for(j = 0; j < BATTLE_COL; j++){
            if(cnt < 6){
                if(i == 0 && j == 0){
                    continue;
                }
                hunt_field[i][j] = 1;
                cnt++;
            }
        }
    }
}

void mix_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]){
    int i = 0, j = 0, k = 0, l = 0, m = 0;
    int temp = 0;
    for(k = 0; k < MIX; k++){
        i = rand() % BATTLE_ROW;
        j = rand() % BATTLE_COL;
        l = rand() % BATTLE_ROW;
        m = rand() % BATTLE_COL;
        if((i != 0 && j != 0 && l != 0 && m != 0) && (i != 3 && j != 3 && l != 3 && m != 3)){
            temp = hunt_field[i][j];
            hunt_field[i][j] = hunt_field[l][m];
            hunt_field[l][m] = temp;
        }
    }
}

void show_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]){
    int i, j;
    for(i = 0; i < BATTLE_ROW; i++){
        for(j = 0; j < BATTLE_COL; j++){
            gotoxy(11 + i * 2, 11 + j);
            if(hunt_field[i][j] == 1) printf("포");
            else if(hunt_field[i][j] == 0) printf("■");
        }
        printf("\n");
    }
}


// (10) 필드 사냥
void hunt2(Player *player, int hunt_field[BATTLE_ROW][BATTLE_COL], Pokemon ***p_addr){
    int ch;
    int len;
    int result;
    int i = 0, j = 0;
    int x = 11, y = 11;
    Pokemon p1;
    while(1){
        len = battle_pokemon_len(player);
        gotoxy(x, y);
        ch = getch();
        if(ch == 'w'){
            if(y != 11){
                y -= 1;
                j -= 1;
            }
            else{
                printf("지정된 필드를 벗어났습니다.\n");
                printf("다시 누르세요.\n");
            }
        }
        if(ch == 's'){
            if(y != 14){
                y += 1;
                j += 1;
            }
            else{
                printf("지정된 필드를 벗어났습니다.\n");
                printf("다시 누르세요.\n");
            }
        }
        if(ch == 'a'){
            if(x != 11){
                x -= 2;
                i -= 1;
            }
            else{
                printf("지정된 필드를 벗어났습니다.\n");
                printf("다시 누르세요.\n");
            }
        }
        if(ch == 'd'){
            if(x != 14){
                x += 2;
                i += 1;
            }
            else{
                printf("지정된 필드를 벗어났습니다.\n");
                printf("다시 누르세요.\n");
            }
        }
        if(ch == 'q'){
            system("clear");
            printf("마을로 돌아갑니다.\n");
            usleep(1000000);
            break;
        }
        if(ch == '\t'){
            system("clear");
            printf("포켓몬의 현재 정보를 출력합니다.\n");
            print_battle_pokemon(player, len);
            show_hunt_field(hunt_field);
        }
        if(hunt_field[i][j] == 1){
            hunt_field[i][j] = 0;
            p1 = random_pokemon(player, p_addr);
            system("clear");
            result = battle(player, &p1);
            usleep(2000000);
            system("clear");
            if(result == 0){
                show_hunt_field(hunt_field);
            }
            else if(result == 1){
                break;
            }
        }
    }
}
Pokemon random_pokemon(Player *player_addr, Pokemon ***p_addr){
    int i, j;
    Pokemon pokemon = {0};
    srand((unsigned) time(NULL));
    i = rand() % 4;
    j = rand() % 4;
    pokemon = (*p_addr)[i][j];
    pokemon.level = ((*player_addr).badge) * 2 + 1;
    pokemon_setting(&pokemon);
    pokemon.power *= 0.7;
    pokemon.hp *= 0.7;
    return pokemon;
}
int battle(Player *p1, Pokemon *temp_pokemon){
    int menu;
    int poke_num = 0; // battle_pokemon index
    int total_poke = 0; // 가지고 있는 포켓몬 수
    int catch_or_not = 0; // 1이 되면 종료
    int win = 0; // 1이 되면 종료
    int exit = 0; // 1이 되면 종료
    total_poke = battle_pokemon_len(p1);
    printf("야생의 Lv.%d %s(이/가)등장했다!! \n", (*temp_pokemon).level, (*temp_pokemon).name);
    printf("가라!! %s!!\n", p1->battle_pokemon[poke_num].name);
    while(total_poke != 0 && win != 1 && exit != 1 && catch_or_not != 1){
        while(1){
            printf("어떤 행동을 하시겠습니까?\n");
            printf("1.공격한다 2.가방 3.도망간다\n");
            scanf("%d", &menu);
            if(menu == 1 || menu == 2 || menu == 3){
                break;
            }
            else{
                printf("잘못누르셨습니다. 다시 눌러주세요.\n");
                my_fflush();
            }
        }
        system("clear");
        switch(menu){
            case 1:
                // my pokemon attack
                my_poke_attack(&(p1->battle_pokemon[poke_num]), temp_pokemon);
                // enemy pokemon attack
                if((*temp_pokemon).hp > 0){
                    enemy_attack(temp_pokemon, &(p1->battle_pokemon[poke_num]));
                    // 포켓몬 교체
                    if(p1->battle_pokemon[poke_num].hp <= 0){
                        printf("%s가 기절했습니다.\n", p1->battle_pokemon[poke_num].name);
                        if(total_poke - 1 > 0){
                            poke_num = change_pokemon(p1, total_poke--);
                            printf("=================\n");
                            break;
                        }
                        else break;
                    }
                    else break;
                }
                    // enemy pokemon 죽었을 때
                else{
                    win = 1;
                    break;
                }
            case 2:
                printf("가방을 열어봅니다.\n");
                catch_or_not = using_backpack(p1, temp_pokemon, poke_num, 0);
                break;
            case 3:
                printf("도망갑니다.\n");
                exit = 1;
                break;
        }
        if(total_poke == 0){
            printf("모든 포켓몬이 전투 불능 상태가 되었습니다.\n");
            printf("마을로 돌아갑니다.\n");
            return 1;
        }
        if(win == 1){
            printf("전투에서 승리했습니다.\n");
            printf("%s이(가) 죽었습니다.\n", (*temp_pokemon).name);
            // 경험치 획득
            printf("경험치를 1 획득합니다.\n");
            printf("Lv.%d %s의 경험치: [%d] -> [%d]\n", p1->battle_pokemon[poke_num].level,
                   p1->battle_pokemon[poke_num].name,
                   p1->battle_pokemon[poke_num].exp, p1->battle_pokemon[poke_num].exp + 1);
            p1->battle_pokemon[poke_num].exp = p1->battle_pokemon[poke_num].exp + 1;
            // 레벨업 및 진화
            level_up(&p1->battle_pokemon[poke_num]);
            printf("전투를 종료하고 필드로 돌아갑니다.\n");
            break;
        }
        if(catch_or_not == 1){
            catch_pokemon(p1, temp_pokemon);
            printf("잡은 포켓몬: %s\n", p1->battle_pokemon[1].name);
            printf("hp: %d max_hp: %d\n", p1->battle_pokemon[1].hp, p1->battle_pokemon[1].max_hp);
            printf("전투를 종료하고 필드로 돌아갑니다.\n");
            break;
        }
    }
    return 0;
}
void my_poke_attack(Pokemon *my_addr, Pokemon *enemy_addr){
    int skill;
    while(1){
        printf("=====%s의 스킬 목록=====\n", (*my_addr).name);
        printf("1.%s\n", (*my_addr).skill_info[0].skill_name);
        if((*my_addr).level >= 5)
            printf("2.%s\n", (*my_addr).skill_info[1].skill_name);
        if((*my_addr).level >= 10)
            printf("3.%s\n", (*my_addr).skill_info[2].skill_name);
        while(1){
            scanf("%d", &skill);
            if(skill == 1 || skill == 2 || skill == 3){
                break;
            }
            else{
                printf("잘못누르셨습니다. 다시 눌러주세요.\n");
                my_fflush();
            }
        }
        if((*my_addr).level < 5){
            if(skill != 1){
                printf("잘못 누르셨습니다.\n");
                continue;
            }
        }
        else if((*my_addr).level < 10){
            if(skill != 1 && skill != 2){
                printf("잘못 누르셨습니다.\n");
                continue;
            }
        }
        else{
            if(skill != 1 && skill != 2 && skill != 3){
                printf("잘못 누르셨습니다.\n");
                continue;
            }
        }
        if(skill == 1)
            (*my_addr).skill[0]((*my_addr).skill_info[0].skill_name, 1,
                                my_addr, enemy_addr);
        else if(skill == 2)
            (*my_addr).skill[1]((*my_addr).skill_info[1].skill_name, 2,
                                my_addr, enemy_addr);
        else if(skill == 3)
            (*my_addr).skill[2]((*my_addr).skill_info[2].skill_name, 2,
                                my_addr, enemy_addr);
        break;
    }
}
void enemy_attack(Pokemon *enemy_addr, Pokemon *my_addr){
    int random;
    if((*enemy_addr).level < 5) random = rand() % 1;
    else if((*enemy_addr).level < 10) random = rand() % 2;
    else random = rand() % 3;
    (*enemy_addr).skill[random]((*enemy_addr).skill_info[random].skill_name, random + 1,
                                enemy_addr, my_addr);
}
int using_backpack(Player *p1, Pokemon *temp_pokemon, int poke_num, int mode){
    int choosen;
    int success_or_fail;
    int i = 0;
    int cnt = 0;
    srand((unsigned) time(NULL));
    while(1){
        printf("1.포켓볼 %d개  2.상처약 %d개  3.진화의 돌 %d개\n", p1->inventory->monster_ball, p1->inventory->medicine,
               p1->inventory->evolution_stone);
        scanf("%d", &choosen);
        if(choosen == 1 || choosen == 2 || choosen == 3){
            break;
        }
        else{
            printf("잘못누르셨습니다. 다시 눌러주세요.\n");
            my_fflush();
        }
    }
    switch(choosen){
        case 1:
            if(mode == 1){
                printf("사용이 불가능합니다.\n");
                return 0;
            }

            if(p1->inventory->monster_ball <= 0){
                printf("몬스터볼이 없습니다.\n");
                break;
            }
            p1->inventory->monster_ball -= 1;
            success_or_fail = monster_ball_fail_or_success(temp_pokemon);
            if(success_or_fail == 1){
                printf("포켓몬을 잡았습니다!!\n");
                for(i = 0; i < BATTLE_POKEMON_COUNT; i++){
                    if(p1->battle_pokemon[i].name == NULL){
                        *((*p1).battle_pokemon + i) = *temp_pokemon;
                    }
                    break;
                }
                if(i == 6){
                    cnt = 1;
                }
                if(cnt == 1){
                    store_pokemon2(p1, temp_pokemon);
                }
                return 1;
            }
            else{
                printf("포켓몬이 볼에서 튀어나왔습니다.\n");
                return 0;
            }
        case 2:
            p1->inventory->medicine -= 1;
            printf("HP를 30%% 회복 합니다,\n");
            healing(p1, poke_num);
            return 3;
        case 3:
            printf("사용이 불가능 합니다,");
            return 3;
    }
    return 0;
}
int monster_ball_fail_or_success(Pokemon *pokemon_addr){
    // hp가 30% 미만이면 잡습니다.
    int a;
    a = 100 * (float) pokemon_addr->hp / (float) pokemon_addr->max_hp;
    if(a < 30) return 1;
    else return 0;
}
void catch_pokemon(Player *p1, Pokemon *pokemon_addr){
    printf("포켓몬 정보를 등록합니다.\n");
    p1->battle_pokemon[1] = *pokemon_addr;
}
void store_pokemon2(Player *player, Pokemon *stored_pokemon){
    int i, j, k;
    printf("현재 battle pokemon이 6마리입니다.\n");
    printf("포켓몬을 가방에 저장합니다.\n");
    print_bag(player->pokemon_bag);
    printf("가방을 선택해주세요: ");
    scanf("%d", &i);
    printf("행을 선택해주세요: ");
    scanf("%d", &j);
    printf("열을 선택해주세요: ");
    scanf("%d", &k);

    if(player->pokemon_bag[i][j][k].level == 0){
        player->pokemon_bag[i][j][k] = *stored_pokemon;
    }
    else{
        printf("이미 포켓몬이 저장되어 있습니다.\n");
        printf("다른 곳을 선택해주세요.\n");
    }
}
void healing(Player *p1, int poke_num){
    int heal;

    heal = p1->battle_pokemon[poke_num].max_hp * 0.3;
    printf("hp: [%d] -> ", p1->battle_pokemon[poke_num].hp);

    p1->battle_pokemon[poke_num].hp += heal;
    if(p1->battle_pokemon[poke_num].hp > p1->battle_pokemon[poke_num].max_hp)
        p1->battle_pokemon[poke_num].hp = p1->battle_pokemon[poke_num].max_hp;
    printf("[%d]\n", p1->battle_pokemon[poke_num].hp);
}
void level_up(Pokemon *p_addr){
    while((*p_addr).exp >= (*p_addr).level * 2){
        printf("level up!\n");
        (*p_addr).level += 1;
        (*p_addr).exp -= ((*p_addr).level - 1) * 2;
        (*p_addr).power *= 1.1;
        (*p_addr).max_hp *= 1.1;
        if((*p_addr).level == 5){
            printf("어라... 포켓몬의 상태가!\n");
            printf("%s가(이) %s로 진화했습니다.\n", (*p_addr).name, (*p_addr).name_list[1]);
            (*p_addr).name = (*p_addr).name_list[1];
            //            if((*p_addr).name == "피카츄" || (*p_addr).name == "리자드" || (*p_addr).name == "어니부기");
            // 아스키아트 출력
        }
        if((*p_addr).level == 10){
            printf("어라... 포켓몬의 상태가!\n");
            printf("%s가(이) %s로 진화했습니다.\n", (*p_addr).name, (*p_addr).name_list[2]);
            (*p_addr).name = (*p_addr).name_list[2];
            //            if((*p_addr).name == "라이츄" || (*p_addr).name == "리자몽" || (*p_addr).name == "거북왕");
            // 아스키아트 출력
        }
    }
}
int change_pokemon(Player *p1, int total_poke){
    int i, num;
    for(i = 0; i < total_poke; i++){
        if((p1->battle_pokemon)[i].hp > 0){
            printf("교체할 수 있는 포켓몬입니다.\n");
            printf("%d. Lv.%d %s Hp: [%d / %d]\n", i + 1, (p1->battle_pokemon)[i].level, (p1->battle_pokemon)[i].name,
                   (p1->battle_pokemon)[i].hp, (p1->battle_pokemon)[i].max_hp);
        }
    }
        while(1){
            printf("바꿀 포켓몬 번호를 입력하세요: ");
            scanf("%d", &num);
            if(p1->battle_pokemon[num - 1].hp <= 0) printf("이 포켓몬은 기절했습니다!\n다른 포켓몬을 선택해주세요.\n");
            else{
                system("clear");
                printf("가라!! %s!!\n", p1->battle_pokemon[num - 1].name);
                break;
            }
        }
    return (num - 1);
}

// (11) 보스 사냥
void battle_gym(Player *p1, Player *boss){
    int stage;
    int menu;
    int my_num = 0; // battle_pokemon index
    int boss_poke_num = 0;
    int total_my = 0; // 가지고 있는 포켓몬 수
    int total_boss_num = 0; // 보스 남은 포켓몬 수
    int catch_or_not;
    int exit = 0;
    Player tmp_boss;

    stage = p1->badge;
    tmp_boss = boss[stage];
    total_my = battle_pokemon_len(p1);
    total_boss_num = battle_pokemon_len(&tmp_boss);

    printf("%s에게 도전합니다.\n", tmp_boss.name);
    printf("Lv.%d %s(이/가)등장했다!! \n", tmp_boss.battle_pokemon[boss_poke_num].level,
           tmp_boss.battle_pokemon[boss_poke_num].name);
    printf("가라!! %s!!\n", p1->battle_pokemon[my_num].name);
    printf("============================\n");
    while(total_my != 0 && total_boss_num != 0 && exit == 0){
        printf("어떤 행동을 하시겠습니까?\n");
        printf("1.공격한다 2.가방 3.포기한다.\n");
        scanf("%d", &menu);
        system("clear");
        switch(menu){
            case 1:
                // my pokemon attack
                my_poke_attack(&(p1->battle_pokemon[my_num]), &(tmp_boss.battle_pokemon[boss_poke_num]));
                // enemy pokemon attack
                if(tmp_boss.battle_pokemon[boss_poke_num].hp > 0){
                    enemy_attack(&(tmp_boss.battle_pokemon[boss_poke_num]), &(p1->battle_pokemon[my_num]));
                    // 포켓몬 교체
                    if(p1->battle_pokemon[my_num].hp <= 0){
                        printf("%s가 기절했습니다.\n", p1->battle_pokemon[my_num].name);
                        total_my--;
                        if(total_my != 0){
                            my_num = change_pokemon(p1, ++total_my);
                            printf("============================\n");
                            break;
                        }
                        else break;
                    }
                    else break;
                }
                    // enemy pokemon 죽었을 때
                else{
                    printf("%s(이/가) 기절했습니다.\n", tmp_boss.battle_pokemon[boss_poke_num].name);
                    // 경험치 획득
                    printf("경험치를 2 획득합니다.\n");
                    printf("Lv.%d %s의 경험치: [%d] -> [%d]\n", p1->battle_pokemon[my_num].level,
                           p1->battle_pokemon[my_num].name, p1->battle_pokemon[my_num].exp,
                           p1->battle_pokemon[my_num].exp + 2);
                    p1->battle_pokemon[my_num].exp = p1->battle_pokemon[my_num].exp + 2;
                    printf("=================\n");
                    // 레벨업 및 진화
                    level_up(&p1->battle_pokemon[my_num]);
                    total_boss_num--;
                    if(total_boss_num != 0){
                        boss_poke_num++;
                        printf("Lv.%d %s(이/가)등장했다!! \n", tmp_boss.battle_pokemon[boss_poke_num].level,
                               tmp_boss.battle_pokemon[boss_poke_num].name);
                        printf("=================\n");
                        break;
                    }
                    else break;
                }
            case 2:
                printf("가방을 열어봅니다.\n");
                catch_or_not = using_backpack(p1, &tmp_boss.battle_pokemon[boss_poke_num], my_num, 1);
                break;
            case 3:
                printf("도망갑니다.\n");
                exit = 1;
                break;
        }
    }
    if(total_my == 0){
        printf("모든 포켓몬이 전투 불능 상태가 되었습니다.\n");
        printf("포켓센터로 돌아갑니다.\n");
    }
    if(total_boss_num == 0){
        if(p1->badge == 4){
            printf("축하합니다.\n");
            printf("최윤정 교수님을 이겼습니다.\n");
            printf("종강입니다!!!!!!!!!!!!!!\n");
            usleep(2000000);
        }
        else{
            p1->badge++;
            printf("%s에게 승리했습니다.\n", tmp_boss.name);
            printf("전투를 종료하고 다음 마을로 갑니다.\n");
        }
    }
    usleep(1000000);
}

int searching_monster(int hunt_field[4][4]){
    int i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(hunt_field[i][j] == 1)
                return 1;
        }
    }
    return 0;
}

// ====== no use =======
//int get_num(int n){
//    int i = 0;
//    char num = '\0';
//    char num1 = '\0';
//    while(1){
//        while(1){
//            scanf("%c", &num1);
//
//            if(num1 == '\n') break;
//            else{
//                num = num1;
//                i++;
//            }
//            if(i == 1 && num >= '1' && num <= n)
//                return ((int) num - 48);
//            else{
//                printf("잘못된 입력입니다 1,2 중 하나를 입력해주세요 \n");
//                i = 0;
//            }
//
//        }
//    }
//}