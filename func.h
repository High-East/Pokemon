#pragma once
#include "data_type.h"


// - UI
int main_UI();
void init();
void gotoxy(int x, int y);
//void gotoxy_menu(int x, int y);
int getX();
int getY();
//void setcolor(int color, int bgcolor);
void setcolor(char color[]);
int get_only_num(char arr[ARSIZE]);
int str2num(char i[ARSIZE]);
int get_num(int n);


// - 맵 관련
void make_village(int village[VILLAGE_ROW][VILLAGE_COL], Player *player);
void make_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]);
void mix_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]);
void show_hunt_field(int hunt_field[BATTLE_ROW][BATTLE_COL]);


// - 맵 이동
// 포켓몬 센터
void pokemon_center(Player *player);
// 마을에서 상점
void supermarket(Player *player);



// - 포켓몬 도감 관련
// 포켓몬 도감 만들기
int makeRow(Pokemon **p_addr, int row);
int makeCol(Pokemon ***p_addr, int col);
int make_pokemon_book(Pokemon ***p_addr);
// 공격 스킬
void attack_skill(char skill_name[], int i, Pokemon *x, Pokemon *y);
// 힐 스킬
void heal_skill(char skill_name[], int i, Pokemon *x, Pokemon *y);
// skill setting
void skill_setting(Pokemon ***p_addr);
//포켓몬 공간 생성
Pokemon* make_pokemon(Pokemon* pokemon);
//포켓몬 정보 설정
void set_pokemon_info(Player* player, Pokemon ***pokemon_book_addr);




// - 플레이어 관련
// 플레이어 공간 생성
void make_bag_group(Pokemon**** group_addr, int size);
Player *make_player(Player **player);
//인벤토리 공간 생성
Inventory* make_inventory(Inventory* inventory);
//인벤토리 정보 설정
void set_inventory_info(Player *player);
//플레이어 정보 설정
void set_player_info(Player *player, Pokemon ***pokemon_book_addr);


// 가방 3차원중 행 할당
void make_bag_row(Pokemon**** group_addr, int size);
// 가방 3차원 중 열 할당
void make_bag_col(Pokemon**** group_addr);
// 플레이어 정보 전부 출력
int print_player(Player *player);




// - 보스 관련
// 보스 공간 생성
Player *make_boss(Player **boss_addr);
// 보스 정보 세팅
void set_boss_info(Player *boss, Pokemon ***pokemon_book_addr);
// 보스 포켓몬 정보 세팅
void set_boss_pokemon_info(Player *boss, Pokemon ***pokemon_book_addr);


// - 배틀 관련
// 필드 사냥
void hunt(Player *player, int hunt_field[BATTLE_ROW][BATTLE_COL], Pokemon ***p_addr);
// 필드 이동
void moving(int hunt_field[BATTLE_ROW][BATTLE_COL], Player* player, Pokemon** pokemon_book, int *i, int *j, int *x, int *y);
// 포켓몬이랑 싸우기
int battle(Player *p1, Pokemon *temp_pokemon);
// 내 포켓몬 공격
void my_poke_attack(Pokemon *my_addr, Pokemon *enemy_addr);
// 포켓몬 스킬 발동
void skill(char skill_name[], int i, Pokemon *x, Pokemon *y);
// 포켓몬 hunt시 잡을때 저장 함수
void store_pokemon(Player* player, Pokemon* stored_pokemon);
// 가방 보기
//int using_backpack(Player *p1, Pokemon *temp_pokemon, int poke_num);
// 관장이랑 싸우기
void battle_gym(Player *p1, Player *boss);
// 포켓몬 랜덤 생성
Pokemon random_pokemon(Player *player_addr, Pokemon ***p_addr);
// 배틀 포켓몬 수 확인
int battle_pokemon_len(Player *p);
// 적 포켓몬 공격
void enemy_attack(Pokemon *enemy_addr, Pokemon *my_addr);
// 포켓몬 교체
int change_pokemon(Player *p1, int total_poke);
// 포켓몬 잡기
int monster_ball_fail_or_success(Pokemon *pokemon_addr);
// 잡은 포켓몬 등록
void catch_pokemon(Player *p1, Pokemon *pokemon_addr);
// 치료약 사용
void healing(Player *p1, int poke_num);
// 가방 사용
int using_backpack(Player *p1, Pokemon *temp_pokemon, int poke_num, int mode);
// 레벨업
void level_up(Pokemon *p_addr);

int getch(void);
void pokemon_setting(Pokemon *p_addr);

/* 잡동사니
void fighting(Player *p1, Pokemon *temp_pokemon);
// 포켓몬 전부 사망했는지 여부
int check_pokemon_hp(Player *p1, int n);
int searching_monster(int hunt_field[4][4]);
void set_gym_pokemon(Player *boss);
// 포켓몬 도감에 존해하는 변수중에 할당이 필요한 것들 세팅 -> 조장님 확인 바람
void make_book_col_element(Pokemon ***pokemon_book_addr);
show pokemon book
void show_pokemon_book(Pokemon ***p_addr);
 */


Pokemon *make_battle_pokemon_malloc(Pokemon *battle_pokemon);
char *player_name_malloc(char *name);
Pokemon ***make_pokemon_bag(Pokemon ***pokemon_bag);
Pokemon *make_row_pokemon_bag(Pokemon *pokemon_bag, int num);
Pokemon **make_col_pokemon_bag(Pokemon **pokemon_bag);
void print_bag(Pokemon ***pokemon_bag); // 가방에 있는 포켓몬 보는 함수
void set_boss_info2(Player *boss, Pokemon **pokemon_book);
void store_pokemon2(Player *player, Pokemon *stored_pokemon);
int searching_monster(int hunt_field[4][4]);
void print_battle_pokemon(Player *p1, int len);
void hunt2(Player *player, int hunt_field[BATTLE_ROW][BATTLE_COL], Pokemon ***p_addr);
void cheat(Player *p1, Pokemon **pokemon_book);
void my_fflush();