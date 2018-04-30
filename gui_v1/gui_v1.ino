#include "Gui.h"
#include "SPI.h"
#include <Adafruit_GFX.h>
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
#define WHITE 0xFFFF
#define BLACK 0x0000

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Gui gui;

char* gui_output_ptr;

void setup() {
  Serial.begin(9600);
  Serial.println("gui start");
  tft.begin();
  tft.fillScreen(WHITE);
  tft.setRotation(3);
  tft.drawLine(0, 190, 320, 190, BLACK);
  tft.drawLine(24, 8, 24, 190, BLACK);
  tft.drawLine(0, 8, 320, 8, BLACK);
  for (int i = 34; i <= 164; i += 26) {
    tft.drawLine(0, i, 24, i, BLACK);
    delay(40);
  }



}

void loop() {
  if (gui.update_screen(gui_output_ptr)) { //gui_output_ptr
    Serial.println();
    Serial.println("gui output: ");
    while (*gui_output_ptr != '\0') {
      Serial.println(*gui_output_ptr);
      gui_output_ptr++;
    }

  }
}



