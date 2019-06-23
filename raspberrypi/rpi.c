/*
 * gpSP - Raspberry Pi port
 *
 * Copyright (C) 2013 DPR <pribyl.email@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "../common.h"
#include <sys/types.h>
#include <sys/stat.h>
// #include "gles_video.h"
// #include "bcm_host.h"
#include "time.h"


//Default setting of gamepad
u32 gamepad_config_map[PLAT_BUTTON_COUNT] =
{
  JOY_ASIX_YM,      // Up
  JOY_ASIX_YP,      // Down
  JOY_ASIX_XM,      // Left
  JOY_ASIX_XP,      // Right
  JOY_BUTTON_1,     // Button A
  JOY_BUTTON_2,     // Button B
  JOY_BUTTON_5,     // Button L
  JOY_BUTTON_6,     // Button R
  JOY_BUTTON_3,     // Button Select
  JOY_BUTTON_4      // Button Start
};

//Default setting of keyboard
u32 keyboard_config_map[PLAT_KEY_COUNT] =
{
  SDLK_UP,          // Up
  SDLK_DOWN,        // Down
  SDLK_LEFT,        // Left
  SDLK_RIGHT,       // Right
  SDLK_k,           // Button A
  SDLK_j,           // Button B
  SDLK_u,           // Button L
  SDLK_i,           // Button R
  SDLK_RETURN,    // Button Start
  SDLK_SPACE      // Button Select
};


#define MAX_VIDEO_MEM (480*270*2)
static int video_started=0;
static uint16_t * video_buff;
static uint video_buff_size;

static SDL_Surface* screen;
static SDL_Surface* gba_screen;
#define GBA_RED_MASK     0x1F
#define GBA_GREEN_MASK  0x3E0
#define GBA_BLUE_MASK  0x7C00


void gpsp_plat_init(void)
{
//    int ret;

// //   bcm_host_init();
//    printf("gpsp_plat_init\n");

//    ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE);
//    if (ret != 0) {
//       fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
//       exit(1);
//    }

//    screen=NULL;
//    screen = SDL_SetVideoMode(320,240, 16,  SDL_SWSURFACE);
//    // Print out some information about the video surface
//    if (screen == NULL) {
//       fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
//       exit(1);
//    }

//    SDL_WM_SetCaption( "gpsp", NULL );
//    // gba_screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16,  GBA_RED_MASK,GBA_GREEN_MASK,GBA_BLUE_MASK,0);

//    SDL_ShowCursor(0);
//    fb_set_mode(240, 160, 0, 0, 0, 0);
//    screen_scale = 1;
}

void gpsp_plat_quit(void)
{
   //  if (video_started) {
   //    //  video_close();
   //     free(video_buff);
   //     video_started=0;
   //  }
//   SDL_Quit();
}


void *fb_flip_screen(void)
{
   // glClearColor(1.0, 0.0, 0.0, 1.0);
   // glClear(GL_COLOR_BUFFER_BIT);
//   video_draw(video_buff);
   // glfwSwapBuffers(window);
   // glfwPollEvents();
   // memset(gba_screen->pixels,0,video_buff_size);
   // memcpy(gba_screen->pixels,video_buff,video_buff_size);
   // gba_screen = SDL_CreateRGBSurfaceFrom(video_buff,240,160,16,240*2,GBA_RED_MASK,GBA_GREEN_MASK,GBA_BLUE_MASK,0);
   // SDL_BlitSurface(gba_screen, NULL, screen, NULL );

   // if( SDL_Flip( screen ) == -1 )
   // {
   //    printf("sdl flip fail:%s\n",SDL_GetError());
   //    return 1;    
   // }
  return video_buff;
}

void fb_wait_vsync(void)
{
}

void fb_set_mode(int w, int h, int buffers, int scale,int filter, int filter2)
{
    if (video_started) {
      //  video_close();
       free(video_buff);
    }
    video_buff_size =w*h*sizeof(uint16_t);
    video_buff=malloc(video_buff_size);
    memset(video_buff,0,video_buff_size);
   //  video_init(w,h,filter);
    video_started=1;
}


int get_joystick(void) {
   SDL_Event event;
   int Back=-2;
   long timer;

   timer=time(NULL)+10;
   while(Back==-2) {
      if (time(NULL)>timer) {
         Back=-1;
      }
      if (SDL_PollEvent(&event)) {
         switch(event.type) {
            case SDL_QUIT:
                quit();
            break;

            case SDL_KEYDOWN:
               if(event.key.keysym.sym == SDLK_ESCAPE){
                 Back=-1;
               };
            break;

            case SDL_JOYBUTTONDOWN:
              if (event.jbutton.button<16) Back=event.jbutton.button+JOY_BUTTON_1;
            break;

           case SDL_JOYAXISMOTION: {
              if (event.jaxis.axis==0) { //Left-Right
                 if (event.jaxis.value < -TRESHOLD)  Back = JOY_ASIX_XM;
                    else if (event.jaxis.value > TRESHOLD) Back = JOY_ASIX_XP;
              }
              if (event.jaxis.axis==1) {  //Up-Down
                 if (event.jaxis.value < -TRESHOLD)  Back = JOY_ASIX_YM;
                    else if (event.jaxis.value > TRESHOLD)  Back = JOY_ASIX_YP;
             }
          break;
          }
        }
      }
   }
   return Back;
}

int get_keyboard(void) {
   SDL_Event event;
   int Back=-2;
   long timer;

   timer=time(NULL)+10;

   while(Back==-2) {

     if (time(NULL)>timer) {
        Back=-1;
     }

     if (SDL_PollEvent(&event)) {
       if (event.type==SDL_QUIT) {
          quit();
       }
       if (event.type==SDL_KEYDOWN) {
          Back=event.key.keysym.sym;
         //  printf("keydown:%d\n",Back);
          if (Back == SDLK_ESCAPE) Back=-1;
          //Reserved key
          if ((Back == SDLK_F10) || (Back == SDLK_F5) ||
               (Back == SDLK_F7) || (Back == SDLK_BACKQUOTE)) {
                  Back=-2;
          }
       }
     }
  }

  return Back;
}

int button_map[] = {
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_A,
  BUTTON_B,
  BUTTON_L,
  BUTTON_R,
  BUTTON_START,
  BUTTON_SELECT
};

int key_map(SDLKey key_sym) {
   int i;

   for(i=0;i<PLAT_KEY_COUNT;i++) {
      if (keyboard_config_map[i]==key_sym) return button_map[i];
   }
   return BUTTON_NONE;
}

int joy_map(u32 button) {
   int i;

   for(i=0;i<PLAT_BUTTON_COUNT;i++) {
      if (gamepad_config_map[i]==button) return button_map[i];
   }
   return BUTTON_NONE;
}

