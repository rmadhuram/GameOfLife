#include <Colorduino.h>
char cells[8][8];
char nextSet[8][8];
int r, g, b, iteration;

/********************************************************
Name: ColorFill
Function: Fill the frame with a color
Parameter:R: the value of RED.   Range:RED 0~255
          G: the value of GREEN. Range:RED 0~255
          B: the value of BLUE.  Range:RED 0~255
********************************************************/
void ColorFill(unsigned char R,unsigned char G,unsigned char B)
{
  PixelRGB *p = Colorduino.GetPixel(0,0);
  for (unsigned char y=0;y<ColorduinoScreenWidth;y++) {
    for(unsigned char x=0;x<ColorduinoScreenHeight;x++) {
      p->r = R;
      p->g = G;
      p->b = B;
      p++;
    }
  }
  
}

void drawColony() {
  ColorFill(0, 0, 0);
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      if (cells[i][j] == 1) {
        Colorduino.SetPixel(i, j, r, g, b);
      }      
    }
  }
  Colorduino.FlipPage();  
}

int nextGen() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      
      int current = cells[i][j];
      int neighbors = 0;
      int next = 0;
      
      /* count the neighbors */
      for (int p = -1; p<=1; p++) {
        for (int q = -1; q <=1; q++) {
          int x = i+p;
          int y = j+q;
          if (x>=0 && x<8 && y>=0 && y<8) {
            if (p != 0 || q != 0 ) {
              if (cells[x][y] == 1) {
                neighbors ++;
              }
            }
          }
        } 
      }
      
      if (current == 1) {
        next = 1;
        if (neighbors < 2 || neighbors > 3) {
          next = 0;
        }
      } else {
        if (neighbors == 3) {
          next = 1;
        }
      }
      
      nextSet[i][j] = next;
    }
  }
 
  int nLive = 0;
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      cells[i][j] = nextSet[i][j];
      if (cells[i][j] == 1) {
        nLive ++;
      }
    }
  }
  
  iteration++;
  
  return nLive;
  
}

void initCells() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      int rnd = random(10);
      if (rnd < 5) {
        cells[i][j] = 1;
      } else {
        cells[i][j] = 0;
      }
    }
  }  
  
  r = random(0, 255);
  g = random(0, 255);
  b = random(0, 255);
  
  iteration = 0;
}

void setup()
{
  Colorduino.Init();
  // compensate for relative intensity differences in R/G/B brightness
  // array of 6-bit base values for RGB (0~63)
  // whiteBalVal[0]=red
  // whiteBalVal[1]=green
  // whiteBalVal[2]=blue
  unsigned char whiteBalVal[3] = {36,63,63}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  Serial.begin(9600);
  randomSeed(1234);
  
  initCells();  
  drawColony();
}

void loop()
{
  int live = nextGen();
  drawColony();
  if (live == 0 || iteration > 30) {
    randomSeed(random(5000));
    initCells();
  }
  delay(500);
}
