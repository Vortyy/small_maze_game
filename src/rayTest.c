/********************************************************************************
 * The goal of this project was to remake an old school assignment (Dpt3.c) in a 
 * single day, using my newly created library, "dynamic_array.h", and experimenting 
 * with "raylib.h" – a truly user-friendly API for displaying graphics.
 *
 * The game is quite simple: it's a maze where you fight monsters (represented by red dots),
 * open doors (brown dots) with keys (yellow dots), and navigate through the environment. 
 * Additionally, I implemented a feature that allows you to easily create your own maps 
 * using a simple .txt file, which can be provided as a parameter.
 *
 * REF : 
 * - https://github.com/raysan5/raylib
 * - https://github.com/Vortyy/dynamic_array 
 *******************************************************************************/
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include "dynamic_array.h"

#define SCREENWIDTH 800
#define SCREENHEIGTH 450
#define CASESIZE 20
#define MAXMAPSIZE 40
#define MAXWORD 5

#define STARTINGENTITIES 32
#define PLAYER_ID 0

#define get_map(x, y) (map.values[(y) * (map.size_width + 1) + (x)])
#define CENTER_H (SCREENWIDTH/2 - (map.size_width/2 * CASESIZE))
#define CENTER_V (SCREENHEIGTH/2 - (map.size_height/2 * CASESIZE))

typedef struct _map { /* represent a map */
  int size_width;                       /* map width */
  int size_height;                      /* map height */
  char values[MAXMAPSIZE * MAXMAPSIZE]; /* values inside a square of position x,y */
} Map;

typedef struct _entity{ /* represent entities */
  int life;     /* life */
  int size[2];  /* size : 0->width, 1->heigth */
  int pos[2];   /* pos : 0->x, 1->y <=> correspond to square of map.txt*/
} Entity;

typedef enum _game_state {
  RUN,
  FIGHT
} GAME_STATE;

char *prog;                                                     /* program name */
Map map = {.size_width = 0, .size_height = 0, .values = {0}};   /* init empty map */
Entity entities[STARTINGENTITIES];                              /* allocation of a starting array of entities */
Vector entities_v;                                              /* dynamic_array of entities */
int key;                                                        /* key player posses */
GAME_STATE state;                                               /* current state of the game */

Color get_map_color(char c){
  switch (c) {
    case '4' : return BROWN;
    case '3' : return YELLOW;
    case '2' : return DARKGREEN;
    case '1' : return DARKGRAY;
    case '0' : return LIGHTGRAY;
    default:
      fprintf(stderr, "%s: there is no color at this char %c \n", prog, c);
      exit(1);
  }
}

Color get_entities_color(int id){
  if(id == PLAYER_ID)
    return BLACK;
  return RED;
}

/* check_collision : check with the char given is blocking return 1, else return 0 */ 
char check_collision(char c){
  switch (c) {
    case '4' : return 0; /* doors */
    case '2' : return 0; /* trees */
    case '1' : return 0; /* walls */
    default: return 1;
  }
}

/* checks : handle check with map elements such walls, key or door, return 1 if traversable, 0 otherwise  */
char checks(int x, int y){
  char c = get_map(x, y);
  if(c == '4' && key >= 1){ /* doors found and posses key */
    key--;
    get_map(x, y) = '0';
  }

  if(!check_collision(c))
    return 0;

  if(c == '3'){ /* key found */
    key++;
    get_map(x, y) = '0';
  }
  return 1;
}

int checks_entities_collision(){
  Entity player = vec_get(&entities_v, PLAYER_ID, Entity); 
  for(int i = 1; i < entities_v.cnt; i++){
    Entity entity_tmp = vec_get(&entities_v, i, Entity);
    if(player.pos[0] == entity_tmp.pos[0] && player.pos[1] == entity_tmp.pos[1])
      return i;
  }
  return 0;
}

char *get_word(char *buff, char *w){
  char *pw = w;
  while(*buff && *buff != '\n')
    *pw++ = *buff++;
  *pw = '\0'; 
  return ++buff;
}

/* read_map : reads map from a txt file and store inside map global var */
void read_map(char *filename){
  int fd; /* file descriptor */
  int n;

  if((fd = open(filename, O_RDONLY, 0)) == -1){ /* open file */
    fprintf(stderr, "%s: can't open %s\n", prog, filename);
    exit(1);
  }
 
  char buff[BUFSIZ];  /* read buffer */ 
  char word[MAXWORD]; /* meta content buffer */
  char *p = buff;            /* ptr on read buffer */

  if(BUFSIZ > MAXMAPSIZE * MAXMAPSIZE)  /* BUFSIZ is enough to single buffered the 4000 */
    n = read(fd, buff, BUFSIZ); 
  else {
    fprintf(stderr, "%s: single buffered BUFSIZ is too small -> %d bytes\n", prog, BUFSIZ);
    exit(1);
  }
 
  /* get Map metadata */
  p = get_word(p, word); /* get width size */
  map.size_width = atoi(word);

  p = get_word(p, word); /* get heigth size */
  map.size_height = atoi(word);

  /* get Map values */
  memcpy(map.values, p, MAXMAPSIZE * MAXMAPSIZE); /* get map layout */

  close(fd);
}

/* draw_map : draws map using layout and char to get color to fill the rectangle */
void draw_map(){
  if(map.size_width == 0 || map.size_height == 0){ /* check if there is a map buffered */ 
    fprintf(stderr, "%s : no map has been read ! you need to read_map before draw it \n", prog);
    exit(1);
  }

  int x = CENTER_H, y = CENTER_V;
  int dx = CASESIZE, dy = CASESIZE;

  for(int i = 0; i < MAXMAPSIZE * MAXMAPSIZE; i++){
    char c;
    if((c = map.values[i]) > 0){
      if(c == '\n'){
        y += dy;
        x = CENTER_H;
      } else {
        DrawRectangle(x, y, dx, dy, get_map_color(c));
        x += dx;
      } 
    }
  }
}

void fight_animation(int animation){
}

/* draw_entities : draw all the entity then the player */
void draw_entities(){
  Entity entity_tmp;
  for(int i = 1; i < entities_v.cnt; i++){
    entity_tmp = vec_get(&entities_v, i, Entity);
    if(entity_tmp.life > 0)
      DrawRectangle(entity_tmp.pos[0] * CASESIZE + CENTER_H, entity_tmp.pos[1] * CASESIZE + CENTER_V, entity_tmp.size[0], entity_tmp.size[1], get_entities_color(i)); /* draw entity_tmp */
  }
  entity_tmp = vec_get(&entities_v, PLAYER_ID, Entity);
  DrawRectangle(entity_tmp.pos[0] * CASESIZE + CENTER_H, entity_tmp.pos[1] * CASESIZE + CENTER_V, entity_tmp.size[0], entity_tmp.size[1], get_entities_color(PLAYER_ID)); /* draw PLAYER */
}

/* generate_entities : generates n entity at random on the map spawnable zone */
void generate_entities(int n){
  while(n-- > 0){
    int x, y;
    do {
      x = (rand() %(map.size_width - 2) + 1);
      y = (rand() %(map.size_height - 2) + 1);
    } while(!check_collision(get_map(x, y)));
    Entity entity = {.life = 5, .pos={x, y}, .size={20, 20}};
    vec_push_generic(&entities_v, entity, Entity);
  }
}

int main(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "error: missing arg !\n");
    return 1;
  }

  srand(time(NULL));
  state = RUN;
  int pause = 0;
  int animation = 0;

  float opacity = 249.f; /* pause text starting opacity */
  int delta = 2;         /* delta modifying the opacity */
  
  prog = *argv++;   /* get program name */
  read_map(*argv);  /* read the first map given as arg */

  vec_init(&entities_v, entities, STARTINGENTITIES);

  key = 0;
  Entity player = {.life = 9, .pos = {1, 1}, .size = {20, 20}}; /* init a players */
  vec_push_generic(&entities_v, player, Entity);

  generate_entities(3);

  Entity *player_ptr = ((Entity *) entities_v.values); /* allow us to move the player */
  Entity *monster_ptr = NULL;

  /* ------------------ RAYLIB ---------------------- */
  InitWindow(SCREENWIDTH, SCREENHEIGTH, "raylib testing example - basic window");

  SetTargetFPS(60);
  Vector2 pause_text_size = MeasureTextEx(GetFontDefault(), "PAUSE", 50.f, 5.f);
  int time_anim = 0;

  Vector3 player_fight_pos = (Vector3) {.x = SCREENWIDTH - 565, .y = SCREENHEIGTH/2, .z=255};   /* z is used to blink rectangle */
  Vector3 monster_fight_pos = (Vector3) {.x = SCREENWIDTH - 300, .y = SCREENHEIGTH/2, .z=255};  /* z is used to blink rectangle */

  while(!WindowShouldClose()){
    int entity_id = -1;

    if(state == RUN && !pause){
      if(IsKeyPressed(KEY_RIGHT) && checks(player_ptr->pos[0] + 1, player_ptr->pos[1])) player_ptr->pos[0] += 1;
      if(IsKeyPressed(KEY_LEFT) && checks(player_ptr->pos[0] - 1, player_ptr->pos[1])) player_ptr->pos[0] -= 1;
      if(IsKeyPressed(KEY_DOWN) && checks(player_ptr->pos[0], player_ptr->pos[1] + 1)) player_ptr->pos[1] += 1;
      if(IsKeyPressed(KEY_UP) && checks(player_ptr->pos[0], player_ptr->pos[1] - 1)) player_ptr->pos[1] -= 1;

      if((entity_id = checks_entities_collision())){
        state = FIGHT;
        monster_ptr = ((Entity *) entities_v.values + entity_id);
      }
    }

    if(state == FIGHT && !pause){
      if(IsKeyPressed(KEY_RIGHT) && animation == 0)
        animation = 1;

      if(animation == 4){
        monster_ptr->life -= 1;
        animation = 5;
      }
      
      if(animation == 8){
        player_ptr->life -= 1;
        animation = 0;
      }

      if(monster_ptr->life <= 0){
        animation = 0;
        state = RUN;
      }
     
      if(player_ptr->life <= 0)
        exit(0);
    }

    if(IsKeyPressed(KEY_SPACE)) pause = (pause) ? 0 : 1;
    
    if(animation)
      time_anim++;
    else
      time_anim = 0;
        
    BeginDrawing();
      ClearBackground(RAYWHITE);

      DrawText(TextFormat("PLAYER LIFE : %.2i", player_ptr->life), 10, 10, 10, DARKGRAY);
      DrawText(TextFormat("KEYS : %.2i", key), 10, 25, 10, DARKGRAY);
      DrawText(TextFormat("MAP SIZE : %.2ix%.2i", map.size_width, map.size_height), 10, 40, 10, DARKGRAY);

      if(state == RUN){
        DrawText("Use your arrow key to move or press escape to pause!", 110, 400, 20, LIGHTGRAY);
        draw_map();
        draw_entities();
      }

      if(state == FIGHT){
        if(animation){ /* awful animation for small fighting style */
          if(animation == 1) /* player attack */
            player_fight_pos.x += 5;
          if(animation == 2)
            player_fight_pos.x -= 5;
          if(animation == 3)
            monster_fight_pos.z = ((time_anim/30)%2) ? 0 : 255;

          if(animation == 5) /* monster attack */
            monster_fight_pos.x -= 5;
          if(animation == 6)
            monster_fight_pos.x += 5;
          if(animation == 7)
            player_fight_pos.z = ((time_anim/30)%2) ? 0 : 255;

          /* animation step updater */
          if(animation == 1 && player_fight_pos.x == (SCREENWIDTH - 525))
            animation = 2;
          if(animation == 2 &&player_fight_pos.x == (SCREENWIDTH - 565))
            animation = 3;
          if(animation == 3 && time_anim > 120)
            animation = 4;
          if(animation == 5 && monster_fight_pos.x == (SCREENWIDTH - 340))
            animation = 6;
          if(animation == 6 && monster_fight_pos.x == (SCREENWIDTH - 300))
            animation = 7;
          if(animation == 7 && time_anim > 240)
            animation = 8;
        }

        DrawText(TextFormat("life : %.2i", monster_ptr->life), SCREENWIDTH - 298, SCREENHEIGTH/2 - 15, 10, RED);
        DrawRectangle(player_fight_pos.x, player_fight_pos.y, 40, 40, CLITERAL(Color){ 0, 0, 0, player_fight_pos.z});
        DrawRectangle(monster_fight_pos.x, monster_fight_pos.y, 40, 40, CLITERAL(Color){ 230, 41, 55, monster_fight_pos.z});
        DrawLine(SCREENWIDTH - 625, SCREENHEIGTH/2 + 40, SCREENWIDTH - 200, SCREENHEIGTH/2 + 40, DARKGRAY);
        DrawText("Use your right arrow key to attack or press escape to pause!", 80, 400, 20, LIGHTGRAY);
      }

      if(pause){
        delta = (opacity > 50 && opacity < 250) ? delta : -delta;
        DrawTextEx(GetFontDefault(), "PAUSE", (Vector2){(float) SCREENWIDTH/2 - pause_text_size.x/2, (float) SCREENHEIGTH/2 - pause_text_size.y/2}, 50.f, 5.f, CLITERAL(Color){ 0, 0, 0, opacity});
        opacity += delta; 
      }
    EndDrawing();
  }

  vec_free(&entities_v);
  CloseWindow();
  return 0;
}
