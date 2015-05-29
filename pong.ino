#include <TVout.h>
#include <fontALL.h>
#include "schematic.h"
#include "TVOlogo.h"

TVout TV;

// select the input pin for the potentiometer
int sensor1Pin = A0;
int sensor2Pin = A1;

// ball coord in pixel. VP is [120, 96]
float ballPos[2] = {60, 48};
float ballSpeed[2] = {1, 1};

// player pad pos in pixel
float player1Pos[2] = {2, 48};
int player1Score = 0;

float player2Pos[2] = {117, 48};
int player2Score = 0;

// pad size in pixel
const int c_padSize = 7;
const int c_ballSpeed = 1;

void setup() {
  TV.begin(PAL,120,96);
  TV.select_font(font6x8);
  TV.println("Pong\n");
  TV.delay(1000);
}

void loop() 
{
  //-------------------------------------------------
  ballPos[0] += ballSpeed[0];
  ballPos[1] += ballSpeed[1];

  // manage pad intersection
  if ( ballPos[0] <= player1Pos[0]
      && player1Pos[1] - c_padSize - 1 <= ballPos[1] && ballPos[1] <= player1Pos[1] + c_padSize + 1 )
      {
        ballPos[0] = player1Pos[0] + 1;
        ballSpeed[0] *= -1;
      }

  if ( ballPos[0] >= player2Pos[0]
      && player2Pos[1] - c_padSize <= ballPos[1] && ballPos[1] <= player2Pos[1] + c_padSize )
      {
        ballPos[0] = player2Pos[0]-1;
        ballSpeed[0] *= -1;
      }

  // Manage lef and right border of terrain
  if ( ballPos[0] <= 0 )
  {
    ballPos[0] = 60;
    ballPos[1] = 48;
    player2Score += 1;
  }
  else if ( ballPos[0] >= 118 )
  {
    ballPos[0] = 60;
    ballPos[1] = 48;
    player1Score += 1;
 //   ballPos[0] = 118;
 //   ballSpeed[0] *= -1;
  }

  if ( ballPos[1] <= 1 )
  {
    ballPos[1] = 1;
    ballSpeed[1] *= -1;
  }
  else if ( ballPos[1] >= 94 )
  {
    ballPos[1] = 94;
    ballSpeed[1] *= -1;
  }

  //-------------------------------------------------
  float sensor1 = (float)analogRead(sensor1Pin) / 1024.0f;
  player1Pos[1] = sensor1 * 96.0f;
  if ( player1Pos[1] <= c_padSize ) player1Pos[1] = c_padSize;
  if ( player1Pos[1] >= 95 - c_padSize ) player1Pos[1] = 95 - c_padSize;

  float sensor2 = (float)analogRead(sensor2Pin) / 1024.0f;
  player2Pos[1] = sensor2 * 96.0f;
  if ( player2Pos[1] <= c_padSize ) player2Pos[1] = c_padSize;
  if ( player2Pos[1] >= 95 - c_padSize ) player2Pos[1] = 95 - c_padSize;

  //-------------------------------------------------
  TV.delay_frame(1);
  TV.clear_screen();

  // draw terrain
  TV.draw_line(0, 0, 119, 0, WHITE);
  TV.draw_line(119, 0, 119, 95, WHITE);
  TV.draw_line(119, 95, 0, 95, WHITE);
  TV.draw_line(0, 95, 0, 0, WHITE);

  for ( int i = 0; i < 96; i+= 2 )
    TV.set_pixel(60, i, WHITE);


  // draw scores
  TV.select_font(font4x6);
  TV.print(55, 6, player1Score);
  TV.print(63, 6, player2Score);

  // draw pads
  TV.draw_line((int)player1Pos[0], (int)player1Pos[1] - c_padSize, (int)player1Pos[0], (int)player1Pos[1] + c_padSize, WHITE);
  TV.draw_line((int)player2Pos[0], (int)player2Pos[1] - c_padSize, (int)player2Pos[0], (int)player2Pos[1] + c_padSize, WHITE);

  // draw ball
  TV.set_pixel(ballPos[0], ballPos[1], WHITE);
}

