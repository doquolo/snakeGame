#include <Arduino.h>
#include <U8g2lib.h>
#include <vector>
#include "ezButton.h"

// import game
struct coord {
  int x, y;
};

#include "snake.h"
#include "food.h"

// declaring screen
const int screenLight = PB0;
int brightness = 254;
U8G2_UC1601_128X64_F_4W_SW_SPI u8g2(U8G2_R0, PA5, PA7, PA4, PA3, PA2);
const int w = 128, h = 64;
const int conversionFactor = 2;
coord gameCoord = {62, 30};

// declaring buttons
ezButton btnup(PA8);
ezButton btndown(PA10);
ezButton btnleft(PA9);
ezButton btnright(PB3);
ezButton btnsel(PB4);

// updates and intervals
uint64_t prevUpdateTime = 0;
uint16_t updateTime = 200;

// drawing
void drawFrame() { 
  u8g2.setDrawColor(1);
  u8g2.drawBox(0, 0, 128, 64);
  u8g2.setDrawColor(0);
  u8g2.drawBox(2, 2, 122, 60);
}

void drawParts(int x, int y) {
  u8g2.setDrawColor(1);
  u8g2.drawBox(y*conversionFactor, x*conversionFactor, conversionFactor, conversionFactor);
}

void setup() {

  // begin serial for debug
  Serial.begin(115200);

  pinMode(PC13, OUTPUT);

  // init and set brightness
  pinMode(screenLight, OUTPUT);
  analogWrite(screenLight, brightness);

  // init screen
  u8g2.begin();
  u8g2.setContrast(120);

  // draw frame
  u8g2.clearBuffer();
  drawFrame();
  u8g2.sendBuffer();

  // init game
  // add starting node
  for (int i = 0; i < 3; i++) {
      coord snakeStart = {((int)((gameCoord.y-2))/2)+i, ((int)((gameCoord.x-2))/2)};  
      addNode(snakeStart);
  }
  for (int i = 0; i < 3; i++) {
      createRandomFood(1, 1, gameCoord.y-2, gameCoord.x-2, snake);
  }
}

void loop() {
  // process button
  btnup.loop();
  btndown.loop();
  btnleft.loop();
  btnright.loop();
  btnsel.loop();
  if (btnup.isPressed()) processMovement(-1);
  else if (btndown.isPressed()) processMovement(+1);
  else if (btnleft.isPressed()) processMovement(-2);
  else if (btnright.isPressed()) processMovement(+2);

  // process game
  if ((millis() - prevUpdateTime) > updateTime) {
    prevUpdateTime = millis();
    u8g2.firstPage();
    do {
      // Process game logic
      moveSnake();
      if (checkWallColision(gameCoord.x, gameCoord.y, snake) || checkSelfColision(snake)) {
          while (true) {
            digitalWrite(PC13, HIGH);
            delay(500);
            digitalWrite(PC13, LOW);
            delay(500);
          }
      };
      if (checkFoodColision(gameCoord.x, gameCoord.y, snake)) {
          // increase speed
          updateTime = (int)((float)updateTime * .85);
      };
      // draw game
      drawFrame();
      for (auto snakePart : snake) {
        drawParts(snakePart.x, snakePart.y);
      }
      for (auto foodPart : foodList) {
        drawParts(foodPart.x, foodPart.y);
      }
      // draw score 
      u8g2.setFont(u8g2_font_4x6_tr);
      char buffer[256];
      sprintf(buffer, "%d", score);
      u8g2.drawStr(4, 10, buffer);
    } while ( u8g2.nextPage() );
  }
}