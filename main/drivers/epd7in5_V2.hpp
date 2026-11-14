/**
 *  @filename   :   epd7in5.h
 *  @brief      :   Header file for e-paper library epd7in5.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPD7IN5_V2_H
#define EPD7IN5_V2_H

#include "epdif.hpp"
#include "lvgl.h"

// Display resolution
#define EPD_WIDTH       800
#define EPD_HEIGHT      480
// #define EPD_WIDTH       480
// #define EPD_HEIGHT      800

#define FRAME_BUFFER_SIZE_IN_BITS   EPD_HEIGHT * EPD_WIDTH
#define FRAME_BUFFER_SIZE_IN_BYTES  EPD_HEIGHT * EPD_WIDTH / 8

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int Init(void);
    int InitFullUpdate(void);
    int EPD_7IN5_V2_Init_Part(void);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetLut(void);
    void DisplayFrame(const unsigned char* frame_buffer);

    // A partial update can contain several LVGL flushes. Start the controller
    // once, write every rendered region, and refresh the visible panel once.
    bool BeginPartialUpdate();
    bool IsValidPartialArea(const lv_area_t &area) const;
    bool WriteRegion(const lv_area_t &area, const uint8_t *pixels);
    void EndPartialUpdate();

    void DisplayFrameBuffer(unsigned char* frame_buffer, size_t len);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);

    void SendDataBuffer(unsigned char *data, size_t len);

    void Sleep(void);
    void Clear(void);
    void Displaypart(const unsigned char* pbuffer, unsigned long Start_X, unsigned long Start_Y,unsigned long END_X,unsigned long END_Y);
 
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long widthInBytes;
    unsigned long height;
    unsigned long heightInBytes;
    bool partial_mode_initialized = false;
    bool partial_update_active = false;

	void SetLut(unsigned char *lut);
    void SetLut_by_host(unsigned char *lut_vcom, unsigned char *lut_ww, unsigned char *lut_bw, unsigned char *lut_wb, unsigned char *lut_bb);
};

#endif /* EPD7IN5_H */

/* END OF FILE */
