#include "Screen.h"

// Screen
Screen::Screen() {
  this->screen = new Adafruit_ILI9341(SCREEN_TFTCS_PIN, SCREEN_TFTDC_PIN);
}

// Init
void Screen::init() {
  if (DEBUG)
    Serial.print(F("[GLIMT][Screen] Initializing... "));

  this->screen->begin();
  this->screen->fillScreen(ILI9341_BLACK);

  if (!SD.begin(SCREEN_SDCS_PIN)) {
    if (DEBUG)
      Serial.println(F("FAIL (SD)"));

    return;
  }

  //this->bmpDraw("all.bmp", 0, 0);
  //this->simpleBmpDraw("1234.bmp", 0, 0);

  this->screen->setRotation(45);
  this->simpleBmpDraw("fb.bmp", 80, 41);

  //this->snap(true);
  //this->homecare(true);
  //this->menu(true);
  //this->activity(true);

  if (DEBUG)
    Serial.println(F("DONE"));
}

void Screen::homecare(bool on) {
  this->screen->setRotation(135);
  if (on == true) {
    this->simpleBmpDraw("grn.bmp", 77, 0);
  } else {
    this->screen->fillRect(77, 0, 166, 41, ILI9341_BLACK);  
  }
}
void Screen::snap(bool on) {
  this->screen->setRotation(45);
  if (on == true) {
    this->simpleBmpDraw("ylw.bmp", 77, 0);
    this->simpleBmpDraw("cat.bmp", 80, 41);
  } else {
    this->screen->fillRect(77, 0, 166, 41, ILI9341_BLACK);
    this->simpleBmpDraw("fb.bmp", 80, 41);
  }
}
void Screen::menu(bool on) {
  this->screen->setRotation(90);
  if (on == true) {
    this->simpleBmpDraw("red.bmp", 37, 40);
  } else {
    this->screen->fillRect(37, 40, 166, 41, ILI9341_BLACK);  
  }
}
void Screen::activity(bool on) {
  this->screen->setRotation(0);
  if (on == true) {
    this->simpleBmpDraw("blu.bmp", 37, 40);
  } else {
    this->screen->fillRect(37, 40, 166, 41, ILI9341_BLACK);  
  }
}

// BMP Draw
/*
void Screen::simpleBmpDraw(char *filename, uint8_t x, uint16_t y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;         // W+H in pixels
  uint8_t  bmpDepth;                    // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;              // Start of image data in file
  uint32_t rowSize;                     // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*SCREEN_PIXELBUFF];// pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer);  // Current position in sdbuffer
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    if (DEBUG)
      Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    (void)read32(bmpFile); // Read & ignore file size
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    (void)read32(bmpFile); // Read & ignore DIB header

    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= this->screen->width())  w = this->screen->width()  - x;
        if((y+h-1) >= this->screen->height()) h = this->screen->height() - y;

        // Set TFT address window to clipped image bounds
        this->screen->setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) {
          pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;

          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            this->screen->pushColor(this->screen->color565(r,g,b));
          }
        }
      } else { if (DEBUG) { Serial.println(F("DEPTH NOT 24 OR COMPRESSED")); } }
    } else { if (DEBUG) { Serial.println(F("# planes != 1")); } }
  } else { if (DEBUG) { Serial.println(F("NOT BMP")); } }

  // Close file
  bmpFile.close();
}
*/


void Screen::simpleBmpDraw(char *filename, uint8_t x, uint16_t y) {
  File     bmpFile;
  int      bmpWidth, bmpHeight;         // W+H in pixels
  uint8_t  bmpDepth;                    // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;              // Start of image data in file
  uint32_t rowSize;                     // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*20];       // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer);  // Current position in sdbuffer
  boolean  goodBmp = false;             // Set to true on valid header parse
  boolean  flip    = true;              // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= this->screen->width()) || (y >= this->screen->height())) return;

  if (DEBUG) {
    Serial.println();
    Serial.print(F("Loading image '"));
    Serial.print(filename);
    Serial.println('\'');
  }

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    if (DEBUG)
      Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    if (DEBUG) {
      Serial.print(F("File size: ")); 
      Serial.println(read32(bmpFile));
    }
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    if (DEBUG) {
      Serial.print(F("Image Offset: ")); 
      Serial.println(bmpImageoffset, DEC);
    }
    // Read DIB header
    if (DEBUG) {
      Serial.print(F("Header size: ")); 
      Serial.println(read32(bmpFile));
    }
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      if (DEBUG) {
        Serial.print(F("Bit Depth: ")); 
        Serial.println(bmpDepth);
      }
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        if (DEBUG) {
          Serial.print(F("Image size: "));
          Serial.print(bmpWidth);
          Serial.print('x');
          Serial.println(bmpHeight);
        }

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= this->screen->width())  w = this->screen->width()  - x;
        if((y+h-1) >= this->screen->height()) h = this->screen->height() - y;

        // Set TFT address window to clipped image bounds
        this->screen->setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            this->screen->pushColor(this->screen->color565(r,g,b));
          } // end pixel
        } // end scanline
        if (DEBUG) {
          Serial.print(F("Loaded in "));
          Serial.print(millis() - startTime);
          Serial.println(" ms");
        }
      } // end goodBmp
    }
  }
  Serial.println(freeMemory());
  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// Read 16/32
uint16_t Screen::read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Screen::read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
