#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include <unistd.h>
#include <string.h>

#define malloc_error(x) if(x == 1) exit(1);


int main(){
    int village[VILLAGE_ROW][VILLAGE_COL] = {0}; // 마을
    int hunt_field[BATTLE_ROW][BATTLE_COL] = {0}; // 사냥터
    int start = 0;
    int start_point = 0;
    int error; // 할당이 안될 때 1
    int ch;
    int x = 13, y = 13; // 터미널 초기값
    int len;

    Player *player = NULL; // 플레이어
    Player *boss = NULL; // 보스
    Pokemon **pokemon_book = NULL; // 포켓몬 도감

    // Intro
    start_point = main_UI(); // (1) intro

    // Game Setting
    if(start_point == 1){
        system("clear");
        error = make_pokemon_book(&pokemon_book); // (2) 포켓몬 도감 생성
        malloc_error(error); // 할당 실패시 종료
        skill_setting(&pokemon_book); // (3) 포켓몬 도감 스킬 세팅
        player = make_player(&player); // (4) 플레이어 공간 생성
        set_player_info(player, &pokemon_book); // (5) 플레이어 정보 입력
        start = print_player(player); // (6) 플레이어 정보 출력
        boss = make_boss(&boss); // (7) 보스 공간 생성
        set_boss_info2(boss, pokemon_book); // (8) 보스 정보 입력
    }
    else{
        printf("아직 개발되지 않았습니다.\n");
        exit(1);
    }

    // Game Start
    if(start != 1){
        printf("게임을 종료합니다.\n");
        exit(1);
    }
    while(1){
        len = battle_pokemon_len(player);
        system("clear");
        make_village(village, player); // (9) 맵
        gotoxy(x, y); // 커서 이동
        ch = getch();
        if(ch == 'w'){
            pokemon_center(player); // (9) 맵
        }
        if(ch == 's'){
            supermarket(player); // (9) 맵
        }
        if(ch == 'd'){
            make_hunt_field(hunt_field);
            mix_hunt_field(hunt_field);
            show_hunt_field(hunt_field);
            hunt2(player, hunt_field, &pokemon_book); // (10) 필드 사냥
        }
        if(ch == 'a'){
            system("clear");
            battle_gym(player, boss); // (11) 보스 사냥
        }
        if(ch == '\t'){
            system("clear");
            printf("포켓몬의 현재 정보를 출력합니다.\n");
            print_battle_pokemon(player, len);
            usleep(1000000);
        }
        if(ch == 'c'){
            system("clear");
            printf("치트키를 사용합니다.\n");
            cheat(player, pokemon_book);
            usleep(1000000);
        }
        if(ch == 'z'){
            system("clear");
            printf("게임을 종료합니다.\n");
            break;
        }
        usleep(1000000);
    }
}