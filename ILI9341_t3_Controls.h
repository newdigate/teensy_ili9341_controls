/*
The MIT License (MIT)

library writen by Kris Kasprzak

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

On a personal note, if you develop an application or product using this library
and make millions of dollars, I'm happy for you!

rev		date			author				change
1.0		9/2019			kasprzak			initial code
2.0		9/2020			kasprzak			added shapes and sizes for handles
3.0		10/2020			kasprzak			fixed some repaint issues in CGraph, added marker support, added Button class with tons of methods
4.0		11/2020			kasprzak			fixed bugs added Button, Checkbox, OptionButton classes
5.0		11/2020			kasprzak			modified sliders, option and check to return true/false if pressed, and actual value stored in value property
5.1		11/2020			kasprzak			added automatic "blank out" old handle support insided draw() method in sliderH and SliderV (really needed when a slide is redrawn based on input other than a finger slide (encoder)
5.4		12/2021			kasprzak			added ring sliders 
*/



#ifndef ILI9341_T3_CONTROLS_H
#define ILI9341_T3_CONTROLS_H

#if ARDUINO >= 100

#include "Arduino.h"
#include "Print.h"
#include "nonstd.h"

#else
#include "WProgram.h"
#endif

#ifdef __cplusplus

#include "Arduino.h"

#endif

//#include <ILI9341_t3.h>

#define G_REPAINT 0
#define G_DRAWOVER 1
#define BELOW 0
#define ABOVE 1

#define SLIDER_HANDLE_SIZE 16
#define HANDLE_NONE 0
#define HANDLE_CIRCLE 1
#define HANDLE_SQUARE 2
#define HANDLE_TRIANGLE_1 3
#define HANDLE_TRIANGLE_2 4
#define HANDLE_TRIANGLE_3 5
#define HANDLE_RECTANGLE 6

#define LOCATION_TOP 0
#define LOCATION_BOTTOM 1

#define DISABLED 0
#define ENABLED 1


#define    C_BLACK        0x0000
#define    C_BLUE            0x001F
#define    C_RED            0xF800
#define    C_GREEN        0x07E0
#define C_CYAN            0x07FF
#define C_MAGENTA        0xF81F
#define C_YELLOW        0xFFE0
#define C_WHITE        0xFFFF

#define C_LTBLUE        0xB6DF
#define C_LTTEAL        0xBF5F
#define C_LTGREEN        0xBFF7
#define C_LTCYAN        0xC7FF
#define C_LTRED        0xFD34
#define C_LTMAGENTA    0xFD5F
#define C_LTYELLOW        0xFFF8
#define C_LTORANGE        0xFE73
#define C_LTPINK        0xFDDF
#define C_LTPURPLE        0xCCFF
#define C_LTGREY        0xE71C

#define C_TEAL            0x0438
#define C_ORANGE        0xFD20
#define C_PINK            0xF81F
#define C_PURPLE        0x801F
#define C_GREY        0xC618

#define C_DKBLUE        0x000D
#define C_DKTEAL        0x020C
#define C_DKGREEN        0x03E0
#define C_DKCYAN        0x03EF
#define C_DKRED        0x6000
#define C_DKMAGENTA    0x8008
#define C_DKYELLOW        0x8400
#define C_DKORANGE    0x8200
#define C_DKPINK        0x9009
#define C_DKPURPLE        0x4010
#define C_DKGREY        0x3186

#define C_MDGREY        0x7BCF


#define B_PRESSED true
#define B_RELEASED false
#define TFT_DEBOUNCE 100  // debounce delay to minimize screen repress

#define CORNER_AUTO   -1
#define CORNER_SQUARE  0

#define  MAX_OPTION 20
#define OPTION_BUTTON_RADIUS 10

#define CHECKBOX_SIZE 20

#define C_DISABLE_LIGHT 0xC618
#define C_DISABLE_MED    0x7BCF
#define C_DISABLE_DARK    0x3186

const float degtorad = .0174532778;

class PRTime {
public:
    PRTime() {}

    void startTime() {
        starttime = millis();
        startetime = starttime;
    }

    void resetStart() {
        starttime = millis();
    }

    unsigned long getElapsedTimeS() {
        return ((millis() - startetime) / 1000);
    }

    unsigned long getTotalTimeS() {
        return ((millis() - starttime) / 1000);
    }

    unsigned long getElapsedTimeMS() {
        return (millis() - startetime);
    }

    unsigned long getTotalTimeMS() {
        return (millis() - starttime);
    }

    void restartElapsedTime() {
        startetime = millis();
    }

    unsigned long starttime;
    unsigned long startetime;
};

template<typename TDisplay, typename TFont>
class BarChartH {

public:

    BarChartH(TDisplay *Display) : d(Display) {
    }

    void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh,
              float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor,
              uint16_t BarBColor, uint16_t BackColor, const TFont &TitleFont, const TFont &ScaleFont) {
        Low = ScaleLow;
        High = ScaleHigh;
        Inc = ScaleInc;
        gx = GraphXLoc;
        gy = GraphYLoc;
        gw = GraphWidth;
        gh = GraphHeight;
        strncpy(ti, Title, 20);

        tf = TitleFont;
        sf = ScaleFont;
        tc = TextColor;
        oc = BorderColor;
        rc = BarColor;
        ac = BarBColor;
        bc = BackColor;

        redraw = true;
    }

    void setBarColor(uint16_t val = 0xF800) {
        rc = val;
    }

    void draw(float val) {

        if (redraw == true) {
            redraw = false;


            // step val basically scales the hival and low val to the height
            // deducting a small value to eliminate round off errors
            // this val may need to be adjusted
            if (ss) {
                d->setFont(sf);
                stepval = MapFloat(Inc, Low, High, 0, gw);

                // paint over previous y scale
                tHi = sf.cap_height * 2 + 8;
                d->fillRect(gx - 10, gy + gh + 1, gw + 30, tHi, bc);
                d->setTextColor(tc, bc);

                for (i = 0; i <= gw; i += stepval) {

                    d->drawFastVLine(i + gx, gy + gh + 1, 5, tc);
                    // draw lables
                    if (High < .1) {
                        Dec = 3;
                    } else if (High <= 1) {
                        Dec = 2;
                    } else if (High <= 10) {
                        Dec = 1;
                    } else {
                        Dec = 0;
                    }
                    data = i * (Inc / stepval);

                    dtostrf(data, 0, Dec, text);
                    tLen = d->strPixelLen(text) * 1.2;
                    tHi = sf.cap_height;
                    d->setCursor(i + gx - (tLen / 2), gy + gh + 10);

                    d->print(text);
                }
            }

            if (st) {
                d->setTextColor(tc, bc);
                d->setFont(tf);
                tHi = sf.cap_height * 2 + 8;
                d->setCursor(gx, gy - tHi);
                d->print(ti);
            }

        }
        // compute level of bar graph that is scaled to the  height and the hi and low vals

        if (val >= High) {
            val = High;
        }
        if (val <= Low) {
            val = Low;
        }

        level = MapFloat(val, Low, High, gx, gx + gw);

        // draw the bar graph
        // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
        d->fillRect(level, gy + 1, gx + gw - level, gh - 2, ac);
        d->fillRect(gx, gy + 1, level - gx, gh - 2, rc);

        d->drawRect(gx, gy, gw, gh, oc);

    }

    void setScale(float ScaleLow, float ScaleHigh, float ScaleInc) {

        Low = ScaleLow;
        High = ScaleHigh;
        Inc = ScaleInc;
        redraw = true;

    }

    void showTitle(bool val) {
        st = val;
    }

    void showScale(bool val) {
        ss = val;
    }


private:
    TDisplay *d;
    TFont tf;
    TFont sf;
    bool st = true, ss = true;
    char ti[20];
    char sc[20];
    char cc[2] = "D";
    char text[30];
    float XLow;
    float XHigh;
    float XInc;
    int Dec, tLen, tHi;
    float Low;
    float High;
    float Inc;

    float gx;
    float gy;
    float gw;
    float gh;

    uint16_t tc;
    uint16_t oc;
    uint16_t rc;
    uint16_t bc;
    uint16_t ac;
    bool redraw;
    float stepval, range, TempY, level, i, data;

    float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

        if (in_min < 0) {
            in_max = in_max + abs(in_min);
            in_min = 0.0;
        }

        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

template<typename TDisplay, typename TFont>
class BarChartV {

public:

    BarChartV(TDisplay *Display) : d(Display) {

    }

    void init(float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float ScaleLow, float ScaleHigh,
              float ScaleInc, const char *Title, uint16_t TextColor, uint16_t BorderColor, uint16_t BarColor,
              uint16_t BarBlankColor, uint16_t BackgroundColor, const TFont &TitleFont, const TFont &ScaleFont) {

        Low = ScaleLow;
        High = ScaleHigh;
        Inc = ScaleInc;
        gx = GraphXLoc;
        gy = GraphYLoc;
        gw = GraphWidth;
        gh = GraphHeight;
        strncpy(ti, Title, 20);

        tf = TitleFont;
        sf = ScaleFont;
        tc = TextColor;
        oc = BorderColor;
        rc = BarColor;
        ac = BarBlankColor;
        bc = BackgroundColor;
        redraw = true;
    }

    void setBarColor(uint16_t val = 0xF800) {

        rc = val;

    }

    void draw(float val) {

        if (redraw == true) {
            redraw = false;


            // step val basically scales the hival and low val to the height
            // deducting a small value to eliminate round off errors
            // this val may need to be adjusted

            if (ss) {
                d->setFont(sf);
                stepval = MapFloat(Inc, Low, High, gh - gh, gh);

                // paint over previous y scale
                d->fillRect(gx + gw, gy - gh - 15, 70, gh + 30, bc);
                d->setTextColor(tc, bc);
                for (i = 0; i <= gh; i += stepval) {
                    TempY = gy - gh + i;
                    d->drawFastHLine(gx + gw, TempY, 5, tc);
                    // draw lables

                    if (High < .1) {
                        Dec = 3;
                    } else if (High <= 1) {
                        Dec = 2;
                    } else if (High <= 10) {
                        Dec = 1;
                    } else {
                        Dec = 0;
                    }
                    data = High - (i * (Inc / stepval));
                    dtostrf(data, 0, Dec, text);
                    tLen = d->strPixelLen(text) * 1.2;
                    tHi = sf.cap_height;
                    d->setCursor(gx + gw + 12, TempY - (tHi / 2));
                    d->print(text);
                }
            }
            if (st) {
                d->setTextColor(tc, bc);
                d->setFont(tf);
                tHi = sf.cap_height * 2 + 5;
                d->setCursor(gx, gy - gh - tHi);
                d->print(ti);
            }
        }
        // compute level of bar graph that is scaled to the  height and the hi and low vals
        // this is needed to accompdate for +/- range
        if (val >= High) {
            val = High;
        }
        if (val <= Low) {
            val = Low;
        }
        level = (gh * (((val - Low) / (High - Low))));


        // draw the bar graph
        // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update

        d->fillRect(gx + 1, gy - gh, gw - 2, gh - level, ac);
        d->fillRect(gx + 1, gy - level, gw - 2, level, rc);
        d->drawRect(gx, gy - gh - 1, gw, gh + 2, oc);

    }

    void setScale(float ScaleLow, float ScaleHigh, float ScaleInc) {


        Low = ScaleLow;
        High = ScaleHigh;
        Inc = ScaleInc;
        redraw = true;
    }

    void showTitle(bool val) {

        st = val;

    }

    void showScale(bool val) {

        ss = val;

    }

private:
    TDisplay *d;
    TFont tf;
    TFont sf;
    bool st = true, ss = true;
    char ti[20];
    char sc[20];
    char cc[2] = "D";
    char text[30];
    float XLow;
    float XHigh;
    float XInc;
    int Dec, tLen, tHi;
    float Low;
    float High;
    float Inc;

    float gx;
    float gy;
    float gw;
    float gh;

    uint16_t tc;
    uint16_t oc;
    uint16_t rc;
    uint16_t bc;
    uint16_t ac;
    bool redraw;
    float stepval, range, TempY, level, i, data;

    float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {

        if (in_min < 0) {
            in_max = in_max + abs(in_min);
            in_min = 0.0;
        }

        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

template<typename TDisplay, typename TFont>
class CGraph {

public:

    CGraph(TDisplay *disp, float GraphXLoc, float GraphYLoc, float GraphWidth, float GraphHeight, float XAxisLow,
           float XAxisHigh, float XAxisInc, float YAxisLow, float YAxisHigh, float YAxisInc) {

        d = disp;

        XLow = XAxisLow;
        XHigh = XAxisHigh;
        XInc = XAxisInc;

        YLow = YAxisLow;
        YHigh = YAxisHigh;
        YInc = YAxisInc;

        gx = GraphXLoc;
        gy = GraphYLoc;
        gw = GraphWidth;
        gh = GraphHeight;

    }

    void init(const char *Title, const char *XAxis, const char *YAxis, uint16_t TextColor, uint16_t GridColor,
              uint16_t AxisColor, uint16_t BackColor, uint16_t PlotColor, const TFont &TitleFont,
              const TFont &AxisFont) {

        strncpy(t, Title, 30);
        strncpy(xa, XAxis, 30);
        strncpy(ya, YAxis, 30);

        tf = TitleFont;
        af = AxisFont;
        tc = TextColor;

        gc = GridColor;
        ac = AxisColor;
        bc = BackColor;
        pc = PlotColor;

        st = true;
        sl = true;
        sal = true;
        sxs = true;
        sys = true;

        Delta = XHigh - XLow;

        RedrawGraph = true;

        TextHeight = tf.cap_height;

    }

    int add(const char *name, uint16_t color) {

        // max number of plots is 10
        if (ID >= 10) {
            return -1;
        }
        pdia[ID] = 0;
        linet[ID] = 1;
        strncpy(dl[ID], name, 30);
        dc[ID] = color;
        HaveFirstPoint[ID] = false;
        ID++;
        return ID - 1;

    }

    void setX(float xpoint) {

        x = xpoint;

    }

    void showAxisLabels(bool val) {

        sal = val;

    }

    void plot(int cID, float y) {

        if (RedrawGraph) {
            RedrawGraph = false;
            drawGraph();
        }

        // plot the data
        XPoint = MapFloat(x, XLow, XHigh, gx, gx + gw);
        YPoint = MapFloat(y, YHigh, YLow, gy - gh, gy);

        if ((YPoint > gy)) {
            YPoint = gy;
        }
        if (YPoint < (gy - gh)) {
            YPoint = gy - gh;
        }

        if ((XPoint > gx) && (XPoint < gx + gw)) {
            if (HaveFirstPoint[cID]) {
                for (j = 0; j < linet[cID]; j++) {
                    d->drawLine(oXPoint[cID], oYPoint[cID] + j, XPoint, YPoint + j, dc[cID]);

                }

                // d->drawLine(oXPoint[cID], oYPoint[cID], XPoint, YPoint, dc[cID]);
                if (pdia[cID] > 1) {
                    d->fillCircle(XPoint, YPoint, pdia[cID], dc[cID]);
                    // d->fillRect(XPoint -  pdia[cID]/2, YPoint-  pdia[cID]/2, pdia[cID],   pdia[cID], dc[cID]);
                }
            }
        }

        HaveFirstPoint[cID] = true;
        oYPoint[cID] = YPoint;
        oXPoint[cID] = XPoint;

        // test to see if we need to redraw

        if (XPoint + 2 > gx + gw) {
            Delta = XHigh - (XLow);
            XLow = XHigh;
            XHigh = XHigh + Delta;
            RedrawGraph = true;
            HaveFirstPoint[cID] = false;

        }

    }

    void setMarkerSize(int cID, byte val) {

        pdia[cID] = val;

    }

    void setLineThickness(int cID, byte val) {

        linet[cID] = val;

    }

    void setTitle(const char *Title) {

        strncpy(t, Title, 30);

    }

    void setXAxisName(const char *XAxis) {

        strncpy(xa, XAxis, 30);

    }


    void setYAxisName(const char *YAxis) {

        strncpy(ya, YAxis, 30);

    }


    void drawLegend(byte Location) {

        tl = Location;

    }


    void drawGraph() {


        RedrawGraph = false;
        byte to = 0;

        float xDiv = ((XHigh - XLow) / XInc);
        float yDiv = ((YHigh - YLow) / YInc);


        float ylen = gh / yDiv;
        float xlen = gw / xDiv;

        d->setTextColor(tc, bc);

        // draw title
        if (st) {
            d->setFont(tf);
            d->setCursor(gx, gy - gh - TextHeight - 10);
            d->print(t);
        }

        // draw grid lines
        // first blank out xscale for redrawing
        d->fillRect(gx - 10, gy + 2, gw + 20, 25, bc);
        d->setFont(af);
        d->fillRect(gx, gy - gh - 4, gw, gh + 8, bc);
        d->fillRect(gx, gy - gh, gw, gh, pc);

        // draw vertical lines
        for (j = 0; j <= xDiv; j++) {
            if (j > 0) {
                d->drawFastVLine(gx + ((0 + j) * xlen), gy - gh, gh, gc);
            }
            if (xDiv < .1) {
                XDec = 2;
            } else if (xDiv < 1) {
                XDec = 1;
            } else {
                XDec = 0;
            }

            //get text offsets


            if (XLow + (XInc * j) < 10) {
                to = 3;
            } else if (XLow + (XInc * j) < 100) {
                to = 7;
            } else if (XLow + (XInc * j) < 1000) {
                to = 11;
            }

            if (sxs) {
                dtostrf(XLow + (XInc * j), 0, XDec, text);
                d->setCursor(gx + (j * xlen) - to, gy + 5);
                d->print(text);
            }
        }

        d->fillRect(gx - 30, gy - gh - TextHeight + 4, 27, gh + TextHeight, bc);

        // draw horizontal lines
        for (i = 0; i <= yDiv; i++) {

            d->drawFastHLine(gx, gy - (ylen * (0 + i)), gw, gc);

            if (YInc < .1) {
                YDec = 2;
            } else if (YInc < 1) {
                YDec = 1;
            } else {
                YDec = 0;
            }
            if (sys) {

                dtostrf(YLow + (YInc * i), 0, YDec, text);
                d->setCursor(gx - 40, gy - (ylen * i) - 8);
                d->print(text);
            }
        }

        // put the y axis at the zero point
        if ((YLow < 0) && (YHigh > 0)) {
            YPoint = MapFloat(0, YHigh, YLow, gy - gh, gy);
        } else {
            YPoint = MapFloat(YLow, YHigh, YLow, gy - gh, gy);
        }


        d->drawFastHLine(gx, YPoint, gw, ac);
        d->drawFastHLine(gx, YPoint - 1, gw, ac);

        d->drawFastVLine(gx - 1, gy - gh, gh + 1, ac);
        d->drawFastVLine(gx - 2, gy - gh, gh + 1, ac);


        // draw legend
        if (sal) {
            // draw y label
            oOrientation = d->getRotation();
            d->setTextColor(tc, bc);
            d->setRotation(oOrientation - 1);
            d->setCursor(d->width() - gy, gx - 44);
            d->print(ya);
            d->setRotation(oOrientation);


            // draw x label
            d->setTextColor(tc, bc);
            d->setCursor(gx, gy + TextHeight + 5);
            d->print(xa);
        }
        if (sl) {
            // draw legend
            StartPointX = gx - 20;

            if (tl == LOCATION_TOP) {
                StartPointY = gy - gh;
            } else if (tl == LOCATION_BOTTOM) {
                StartPointY = d->getCursorY() + 13;
            }

            for (k = 0; k <= ID; k++) {
                d->setCursor(StartPointX, StartPointY);
                d->print(dl[k]);
                StartPointX = d->getCursorX();

                for (j = 0; j < pdia[k]; j++) {
                    d->drawFastHLine(StartPointX + 3, StartPointY + TextHeight / 4 + j, 20, dc[k]);
                    //d->drawFastHLine(StartPointX+3, StartPointY+TextHeight/4 , 20, dc[k]);
                    //d->drawFastHLine(StartPointX+3, StartPointY+TextHeight/4 + 1, 20, dc[k]);

                }
                StartPointX += 30;
            }

        }

    }

    void setYAxis(float Ylow, float Yhigh, float Yinc) {

        YLow = Ylow;
        YHigh = Yhigh;
        YInc = Yinc;
        RedrawGraph = true;
    }

    void setXAxis(float XAxisLow, float XAxisHigh, float XAxisInc) {

        XLow = XAxisLow;
        XHigh = XAxisHigh;
        XInc = XAxisInc;
        RedrawGraph = true;
    }

    void showTitle(bool val) {
        st = val;
    }

    void showLegend(bool val) {
        sl = val;
    }

    void showXScale(bool val) {
        sxs = val;
    }

    void showYScale(bool val) {
        sys = val;
    }

private:

    TDisplay *d;
    TFont tf;
    TFont af;
    int ID = 0;
    float x, y;
    float i, j;
    bool st, sl, sal, sxs, sys;
    float Delta;
    int k;
    float XLow, XHigh, XInc;
    float YLow, YHigh, YInc;
    bool RedrawGraph = true;
    bool HaveFirstPoint[10];
    float XPoint, YPoint, oXPoint[10], oYPoint[10], TextHeight;
    float XDec = 0.0, YDec = 0.0;
    char text[30];
    byte oOrientation = 0;
    float gx, gy, gw, gh;
    int StartPointX, StartPointY;
    char buf0[20], buf1[20], buf2[20], buf3[20], buf4[20], buf5[20], buf6[20], buf7[20], buf8[20], buf9[20];
    char *dl[20] = {buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf6, buf8, buf9};
    char t[20];
    byte tl = 0; // title location
    char xa[20];
    char ya[20];
    uint16_t tc;
    uint16_t dc[10];
    uint16_t ac;
    uint16_t gc;
    uint16_t bc;
    uint16_t pc;
    byte pdia[20];
    byte linet[20];

    float MapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


};

template<typename TDisplay, typename TFont>
class Dial {
public:

    Dial(TDisplay *disp, int CenterX, int CenterY, int DialRadius, float LowVal, float HiVal, float ValInc,
         float SweepAngle) {

        degtorad = .0174532778;

        d = disp;

        cx = CenterX;
        cy = CenterY;
        dr = DialRadius;
        lv = LowVal;
        hv = HiVal;
        inc = ValInc;
        sa = SweepAngle;

        // store initial values
        // this looks silly but we don't have needle size calculated yet but to keep needle from first draw being weird, just make a dot at the center
        px = CenterX;
        py = CenterY;
        pix = CenterX;
        piy = CenterY;
        plx = CenterX;
        ply = CenterY;
        prx = CenterX;
        pry = CenterY;

        Redraw = true;

    }

    void init(uint16_t NeedleColor, uint16_t DialColor, uint16_t TextColor, uint16_t TickColor, const char *Title,
              const TFont &TitleFont, const TFont &DataFont) {

        tf = TitleFont;
        df = DataFont;
        strncpy(t, Title, 10);
        nc = NeedleColor;
        dc = DialColor;
        tc = TextColor;
        ic = TickColor;

    }

    void draw(float Val) {


        // draw the dial only one time--this will minimize flicker
        if (Redraw == true) {
            Redraw = false;
            d->fillCircle(cx, cy, dr, dc);
            d->drawCircle(cx, cy, dr, ic);
            d->drawCircle(cx, cy, dr - 1, ic);
        }

        // draw the current value
        d->setFont(df);
        d->setTextColor(tc, dc);
        d->setCursor(cx - 25, cy + 20);
        //disp.println(Format(curval, dig, dec));
        // center the scale about the vertical axis--and use this to offset the needle, and scale text
        offset = (270 + (sa / 2)) * degtorad;
        // find hte scale step value based on the hival low val and the scale sweep angle
        // deducting a small value to eliminate round off errors
        // this val may need to be adjusted
        stepval = (inc) * (sa / (float(hv - lv)));
        // draw the scale and numbers
        // note draw this each time to repaint where the needle was

        // first draw the previous needle in dial color to hide it
        d->fillTriangle(pix, piy, plx, ply, prx, pry, dc);

        for (i = 0.0; i <= (sa + 0.0001); i += stepval) {

            angle = (i * degtorad);
            angle = offset - angle;
            ox = (float) (dr - 2) * cos(angle) + cx;
            oy = (float) (dr - 2) * sin(angle) + cy;
            ix = (float) (dr - 10) * cos(angle) + cx;
            iy = (float) (dr - 10) * sin(angle) + cy;
            tx = (float) (dr - 30) * cos(angle) + cx;
            ty = (float) (dr - 30) * sin(angle) + cy;
            dx = (float) (dr - 20) * cos(angle) + cx;
            dy = (float) (dr - 20) * sin(angle) + cy;

            d->drawLine(ox, oy, ix, iy, ic);


            if (hv < .1) {
                dec = 3;
            } else if (hv <= 1) {
                dec = 2;
            } else if (hv <= 10) {
                dec = 1;
            } else {
                dec = 0;
            }

            data = hv - (i * (inc / stepval));
            dtostrf(data, 0, dec, buf);
            tLen = d->strPixelLen(buf);
            tHi = df.cap_height;
            d->setCursor(dx - (tLen / 2), dy - (tHi / 2));

            d->print(buf);


        }

        // compute and draw the needle
        angle = (sa * (1.0 - (((Val - lv) / (hv - lv)))));
        angle = angle * degtorad;
        angle = offset - angle;

        // draw a triangle for the needle (compute and store 3 vertiticies)
        // 5.0 is width of needle at center
        ix = (float) (dr - 10.0) * cos(angle) + cx;
        iy = (float) (dr - 10.0) * sin(angle) + cy;
        lx = 6.0 * cos(angle - 90.0 * degtorad) + cx;
        ly = 6.0 * sin(angle - 90.0 * degtorad) + cy;
        rx = 6.0 * cos(angle + 90.0 * degtorad) + cx;
        ry = 6.0 * sin(angle + 90.0 * degtorad) + cy;

        // then draw the new needle in need color to display it
        d->fillTriangle(ix, iy, lx, ly, rx, ry, nc);

        // draw a cute little dial center
        d->fillCircle(cx, cy, 8, tc);

        // print the title
        d->setTextColor(tc, dc);
        d->setFont(tf);
        tLen = d->strPixelLen(t);

        d->setCursor(cx - tLen / 2, cy + 10);
        d->println(t);

        //save all current to old so the previous dial can be hidden
        pix = ix;
        piy = iy;
        plx = lx;
        ply = ly;
        prx = rx;
        pry = ry;

    }

private:

    bool Redraw = true;
    TDisplay *d;
    TFont tf;
    TFont df;
    char t[10];
    int cx;
    int cy;
    int dr;
    float lv;
    float hv;
    float inc;
    float sa;
    uint16_t nc;
    uint16_t dc;
    uint16_t tc;
    uint16_t ic;
    float degtorad;
    float offset, stepval, angle, data;
    float i;

    //variables to track new needle values
    float ix;
    float iy;
    float ox;
    float oy;
    float tx;
    float ty;
    float dx;
    float dy;
    float lx;
    float rx;
    float ly;
    float ry;
    int tLen, tHi;
    char buf[38];
    int dec;
    // variables to track previous needle values
    float px;
    float py;
    float pix;
    float piy;
    float plx;
    float ply;
    float prx;
    float pry;

};

template<typename TDisplay>
class SliderH {

public:
    SliderH(TDisplay *Display) {
        // map arguements to class variables
        d = Display;
    }

    void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, float ScaleLow, float ScaleHi, float ScaleSize,
              float SnapSize, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {
        // map arguements to class variables
        sl = ScaleLow;
        sh = ScaleHi;
        sc = 0.0;
        sn = 0.0;
        l = SliderX;
        t = SliderY;
        w = SliderW;
        bt = 3;
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;
        debounce = TFT_DEBOUNCE;
        dsColor = C_DISABLE_MED;
        dhColor = C_DISABLE_LIGHT;
        enabled = true;
        visible = true;
        ox = -1;
        colorscale = true;
        handlesize = SLIDER_HANDLE_SIZE;
        handlewidth = handlesize / 2;
        handleshape = HANDLE_CIRCLE;

        if (ScaleSize != 0) {
            sc = (sh - sl) / ScaleSize;
            ce = abs(sl / ScaleSize);
        }
        if (SnapSize != 0) {
            sn = (sh - sl) / SnapSize;
        }

    }

    void resetScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize) {

        sl = ScaleLow;
        sh = ScaleHi;
        sc = 0.0;
        sn = 0.0;
        if (ScaleSize != 0) {
            sc = (sh - sl) / ScaleSize;
            ce = abs(sl / ScaleSize);
        }
        if (SnapSize != 0) {
            sn = (sh - sl) / SnapSize;
        }

    }

    void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

        // map arguements to class variables
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;

    }

    void setHandleColor(uint16_t HandleColor) {

        hColor = HandleColor;

    }

    void setBarThickness(byte Thickness) {

        bt = Thickness;

    }

    void setDisableColor(uint16_t DisableHandleColor, uint16_t DisableSliderColor) {

        dsColor = DisableSliderColor;
        dhColor = DisableHandleColor;
    }

    void enable() {

        enabled = true;

    }

    void disable() {

        enabled = false;


    }

    void show() {

        visible = true;

    }

    void hide() {

        visible = false;

    }

    void draw(float val) {

        if (!visible) {

            if (!visible) {
                d->fillRect(l - handlesize, t - handlesize, w + (2 * handlesize), handlesize * handlesize, bColor);
            }
            // no need to draw anything
            return;
        }

        value = val;

        if (enabled) {
            tsColor = sColor;
            thColor = hColor;
            ssColor = sColor;
        } else {
            tsColor = dsColor;
            thColor = dhColor;
            ssColor = dsColor;
        }




        ////////////////////////

        // draw the slider
        pos = MapFloat(value, (float) sl, (float) sh, (float) l, (float) (w + l));
        //////////////////////////
        // seems odd to blank out old but of draw is called in .ino, need to clean up old stuff
        if ((ox != pos) && (ox > 0)) {
            if (handleshape == HANDLE_CIRCLE) {
                d->fillCircle(ox, t, handlesize / 2, bColor);
            } else if (handleshape == HANDLE_SQUARE) {
                d->fillRect(ox - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
            } else if (handleshape == HANDLE_TRIANGLE_1) {
                d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox, t,
                                bColor);
            } else if (handleshape == HANDLE_TRIANGLE_2) {
                d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox, t,
                                bColor);
            } else if (handleshape == HANDLE_TRIANGLE_3) {
                d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize, ox, t,
                                bColor);
                d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize, ox, t,
                                bColor);
            } else if (handleshape == HANDLE_RECTANGLE) {
                d->fillRect(ox - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
            }
        }


        if (colorscale) {
            d->fillRect(l, t - (bt / 2), abs(pos - l), bt, thColor);
            d->fillRect(pos, t - (bt / 2), abs(w - pos + l), bt, tsColor);
        } else {
            d->fillRect(l, t - (bt / 2), w + 1, bt, ssColor);
        }

        // draw any tick marks
        if (sc != 0.0) {
            for (i = 0; i <= sc; i++) {

                d->fillRect((i * (w / sc)) + l, t - 3, 1, 7, tsColor);

                if ((i == ce) | (i == 0) | (i == sc)) {
                    d->fillRect((i * (w / sc)) + l - 1, t - 3, 3, 7, tsColor);
                }
            }
        }


        // draw new handle
        if (handleshape == HANDLE_CIRCLE) {
            d->fillCircle(pos, t, handlesize / 2, thColor);
            d->drawCircle(pos, t, handlesize / 2, tsColor);
            d->drawCircle(pos, t, (handlesize / 2) - 1, tsColor);
        } else if (handleshape == HANDLE_SQUARE) {
            d->fillRect(pos - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, thColor);
            d->drawRect(pos - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, tsColor);
            d->drawRect(pos - (handlesize / 2) + 1, t - (handlesize / 2) + 1, handlesize - 2, handlesize - 2, tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_1) {
            d->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos, t,
                            thColor);
            d->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos, t,
                            tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_2) {
            d->fillTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos, t,
                            thColor);
            d->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos, t,
                            tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_3) {
            d->fillTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos, t,
                            thColor);
            d->fillTriangle(ox - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos, t,
                            thColor);

            d->drawTriangle(pos - (handlesize / 2), t - handlesize, pos + (handlesize / 2), t - handlesize, pos, t,
                            tsColor);
            d->drawTriangle(pos - (handlesize / 2), t + handlesize, pos + (handlesize / 2), t + handlesize, pos, t,
                            tsColor);

        } else if (handleshape == HANDLE_RECTANGLE) {
            d->fillRect(pos - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, thColor);
            d->drawRect(pos - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, tsColor);
            d->drawRect(pos - (handlewidth / 2) + 1, t - (handlesize / 2) + 1, handlewidth - 2, handlesize - 2,
                        tsColor);
        }
        ox = pos;
        pos = value;

    }

    bool slide(float ScreenX, float ScreenY) {

        bool pressed = false;

        x = ScreenX;
        y = ScreenY;

        if (!enabled) {
            return pressed;
        }

        if (enabled) {
            tsColor = sColor;
            thColor = hColor;
            ssColor = sColor;
        } else {
            tsColor = dsColor;
            thColor = dhColor;
            ssColor = dsColor;
        }

        // Serial.print("sn "); Serial.println(sn);
        if (sn != 0.0) {

            x = x - l;
            // Serial.print("x b "); Serial.println(x);
            x = round(x / (w / sn));
            //Serial.print("x a "); Serial.println(x);
            x = x * (w / sn);
            x = x + l;
        } else {
            x = x;
        }

        // draw ball and scale
        if (x != ox) {

            if ((x >= l) & (x <= (l + w))) {

                if ((abs(y - t)) <= handlesize) {

                    pressed = true;
                    if (handleshape == HANDLE_CIRCLE) {
                        d->fillCircle(ox, t, handlesize / 2, bColor);
                    } else if (handleshape == HANDLE_SQUARE) {
                        d->fillRect(ox - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_1) {
                        d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize,
                                        ox, t, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_2) {
                        d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize,
                                        ox, t, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_3) {
                        d->fillTriangle(ox - (handlesize / 2), t - handlesize, ox + (handlesize / 2), t - handlesize,
                                        ox, t, bColor);
                        d->fillTriangle(ox - (handlesize / 2), t + handlesize, ox + (handlesize / 2), t + handlesize,
                                        ox, t, bColor);
                    } else if (handleshape == HANDLE_RECTANGLE) {
                        d->fillRect(ox - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, bColor);
                    }

                    if (colorscale) {
                        d->fillRect(l, t - (bt / 2), x - l, bt, hColor);
                        d->fillRect(x, t - (bt / 2), w - x + l, bt, sColor);
                    } else {
                        d->fillRect(l, t - (bt / 2), w + 1, bt, ssColor);
                    }


                    if (sc != 0.0) {
                        for (i = 0; i <= sc; i++) {

                            d->fillRect((i * (w / sc)) + l, t - 3, 1, 7, sColor);

                            if ((i == ce) | (i == 0) | (i == sc)) {
                                d->fillRect((i * (w / sc)) + l - 1, t - 3, 3, 7, sColor);
                            }
                        }
                    }

                    if (handleshape == HANDLE_CIRCLE) {
                        d->fillCircle(x, t, handlesize / 2, hColor);
                        d->drawCircle(x, t, handlesize / 2, sColor);
                        d->drawCircle(x, t, (handlesize / 2) - 1, sColor);
                    } else if (handleshape == HANDLE_SQUARE) {
                        d->fillRect(x - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, hColor);
                        d->drawRect(x - (handlesize / 2), t - (handlesize / 2), handlesize, handlesize, sColor);
                        d->drawRect(x - (handlesize / 2) + 1, t - (handlesize / 2) + 1, handlesize - 2, handlesize - 2,
                                    sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_1) {
                        d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,
                                        t, hColor);
                        d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,
                                        t, sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_2) {
                        d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,
                                        t, hColor);
                        d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,
                                        t, sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_3) {
                        d->fillTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,
                                        t, hColor);
                        d->fillTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,
                                        t, hColor);

                        d->drawTriangle(x - (handlesize / 2), t - handlesize, x + (handlesize / 2), t - handlesize, x,
                                        t, sColor);
                        d->drawTriangle(x - (handlesize / 2), t + handlesize, x + (handlesize / 2), t + handlesize, x,
                                        t, sColor);

                    } else if (handleshape == HANDLE_RECTANGLE) {
                        d->fillRect(x - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, hColor);
                        d->drawRect(x - (handlewidth / 2), t - (handlesize / 2), handlewidth, handlesize, sColor);
                        d->drawRect(x - (handlewidth / 2) + 1, t - (handlesize / 2) + 1, handlewidth - 2,
                                    handlesize - 2, sColor);
                    }
                    ox = x;

                    // get scaled val and pass back and store in the public variable in case anyone wants it

                    pos = MapFloat(x, l, l + w, sl, sh);
                    //Serial.print("pos "); Serial.println(pos);
                    value = pos;
                    //Serial.print("value "); Serial.println(value);
                    delay(debounce);
                }
            }

        }

        return pressed;
    }

    void drawSliderColor(bool color) {

        colorscale = color;

    }

    void setHandleSize(int size) {

        if (size < 4) {
            handlesize = 4;
        } else if (size > 40) {
            handlesize = 40;
        } else {
            handlesize = size;
        }

    }

    void setHandleSize(int size, int width) {

        if (size < 4) {
            handlesize = 4;
        } else if (size > 40) {
            handlesize = 40;
        } else {
            handlesize = size;
        }

        if (width < 4) {
            handlewidth = 4;
        } else if (width > 40) {
            handlewidth = 40;
        } else {
            handlewidth = width;
        }


    }

    void setHandleShape(byte shape) {

        if (shape < 0) {
            handleshape = HANDLE_CIRCLE;
        } else if (shape > 6) {
            handleshape = HANDLE_CIRCLE;
        } else {
            handleshape = shape;
        }

    }

    void setPressDebounce(byte Debounce) {

        debounce = Debounce;

    }

    float value;

private:

    TDisplay *d;            // the display object
    uint16_t sColor;        // the slider color
    uint16_t bColor;        // the slider background color
    uint16_t hColor;        // the sliders drag handle
    uint16_t dsColor;
    uint16_t dhColor;
    uint16_t tsColor;
    uint16_t ssColor;
    uint16_t thColor;
    float x;
    float y;
    uint16_t l;            // the left coordinate of the scale
    uint16_t t;            // the top coordinate of the scale
    uint16_t w;            // the width of the scale
    byte bt;            // the thockness of the bar
    float ox;            // the old screen x value where user pressed
    bool enabled;
    float sl;                // the scale lower value
    float sh;                // the scale upper value
    float pos;                // the position on the scale
    float sn;                // the snap increment
    float sc;                // the scale increment
    float ce;                // the tick mark where zero is (for drawing heavy line on +/- scales
    float i;                // loop counter
    int handlesize;
    int handlewidth;
    byte handleshape;
    bool visible;
    bool colorscale;        // flag to draw slider in handle color
    float MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
        return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    }

    byte debounce;

};

template<typename TDisplay>
class SliderV {
public:
    SliderV(TDisplay *Display) : d(Display) {}

    void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderH, float ScaleLow, float ScaleHi, float ScaleSize,
              float SnapSize, uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

        // map arguements to class variables
        sl = ScaleLow;
        sh = ScaleHi;
        sc = 0.0;
        sn = 0.0;
        oy = -1;
        colorscale = true;
        handlesize = SLIDER_HANDLE_SIZE;
        handlewidth = handlesize / 2;
        handleshape = HANDLE_CIRCLE;
        enabled = true;
        visible = true;
        l = SliderX;
        t = SliderY;
        h = SliderH;
        bt = 3;
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;
        debounce = TFT_DEBOUNCE;
        dsColor = C_DISABLE_MED;
        dhColor = C_DISABLE_LIGHT;

        // compute scale increments and snap increments
        if (ScaleSize != 0) {
            sc = (sh - sl) / ScaleSize;
            ce = abs(sh / ScaleSize);
        }
        if (SnapSize != 0) {
            sn = (sh - sl) / SnapSize;
        }

    }

    void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

        // map arguements to class variables
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;

    }

    void draw(float val) {


        if (!visible) {

            if (!visible) {
                d->fillRect(l - handlesize, t - (handlesize / 2), handlesize * 2, h + handlesize, bColor); // erase it
            }
            return;
        }

        value = val;

        if (enabled) {
            tsColor = sColor;
            thColor = hColor;
        } else {
            tsColor = dsColor;
            thColor = dhColor;
        }

        pos = MapFloat(value, (float) sl, (float) sh, (float) (t + h), (float) t);

        // blannk ouut any previous--note draw can be called w/o slide in cases where sliders are tied together

        if ((pos != oy) && (oy >= 0)) {
            // blank out the old one
            if (handleshape == HANDLE_CIRCLE) {
                d->fillCircle(l, oy, handlesize / 2, bColor);
            } else if (handleshape == HANDLE_SQUARE) {
                d->fillRect(l - (handlesize / 2), oy - (handlesize / 2), handlesize, handlesize, bColor);
            } else if (handleshape == HANDLE_TRIANGLE_1) {
                // weird i know but need to draw the black out slightly larger due to round off errors
                d->fillTriangle(l - handlesize, oy - (handlesize / 2) - 1, l - handlesize, oy + (handlesize / 2) + 1,
                                l + 1, oy, bColor);
            } else if (handleshape == HANDLE_TRIANGLE_2) {
                // weird i know but need to draw the black out slightly larger due to round off errors
                d->fillTriangle(l + handlesize, oy - (handlesize / 2) - 1, l + handlesize, oy + (handlesize / 2) + 1,
                                l - 1, oy, bColor);
            } else if (handleshape == HANDLE_TRIANGLE_3) {
                // weird i know but need to draw the black out slightly larger due to round off errors
                d->fillTriangle(l - handlesize, oy - (handlesize / 2) - 1, l - handlesize, oy + (handlesize / 2) + 1,
                                l + 1, oy, bColor);
                d->fillTriangle(l + handlesize, oy - (handlesize / 2) - 1, l + handlesize, oy + (handlesize / 2) + 1,
                                l - 1, oy, bColor);
            } else if (handleshape == HANDLE_RECTANGLE) {
                // weird i know but need to draw the black out slightly larger due to round off errors
                d->fillRect(l - (handlesize / 2), oy - (handlewidth / 2), handlesize, handlewidth, bColor);
            }

        }

        if (colorscale) {
            d->fillRect(l - (bt / 2), t, bt, pos - t, tsColor); // draw new slider
            d->fillRect(l - (bt / 2), pos, bt, h - pos + t, thColor); // draw new slider
        } else {
            d->fillRect(l - (bt / 2), t, bt, h, tsColor);
        }

        if (sc != 0.0) {
            for (i = 0; i <= sc; i++) {

                d->fillRect(l - 3, (i * (h / sc)) + t, 7, 1, tsColor);

                if ((i == ce) || (i == 0) || (i == sc)) {
                    d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, tsColor);
                }
            }
        }


        if (handleshape == HANDLE_CIRCLE) {
            d->fillCircle(l, pos, handlesize / 2, thColor);
            d->drawCircle(l, pos, handlesize / 2, tsColor);
            d->drawCircle(l, pos, (handlesize / 2) - 1, tsColor);
        } else if (handleshape == HANDLE_SQUARE) {
            d->fillRect(l - (handlesize / 2), pos - (handlesize / 2), handlesize, handlesize, thColor);
            d->drawRect(l - (handlesize / 2), pos - (handlesize / 2), handlesize, handlesize, tsColor);
            d->drawRect(l - (handlesize / 2) + 1, pos - (handlesize / 2) + 1, handlesize - 2, handlesize - 2, tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_1) {
            d->fillTriangle(l - handlesize, pos - (handlesize / 2), l - handlesize, pos + (handlesize / 2), l, pos,
                            thColor);
            d->drawTriangle(l - handlesize, pos - (handlesize / 2), l - handlesize, pos + (handlesize / 2), l, pos,
                            tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_2) {
            d->fillTriangle(l + handlesize, pos - (handlesize / 2), l + handlesize, pos + (handlesize / 2), l, pos,
                            thColor);
            d->drawTriangle(l + handlesize, pos - (handlesize / 2), l + handlesize, pos + (handlesize / 2), l, pos,
                            tsColor);
        } else if (handleshape == HANDLE_TRIANGLE_3) {
            d->fillTriangle(l - handlesize, pos - (handlesize / 2), l - handlesize, pos + (handlesize / 2), l, pos,
                            thColor);
            d->fillTriangle(l + handlesize, pos - (handlesize / 2), l + handlesize, pos + (handlesize / 2), l, pos,
                            thColor);
            d->drawTriangle(l - handlesize, pos - (handlesize / 2), l - handlesize, pos + (handlesize / 2), l, pos,
                            tsColor);
            d->drawTriangle(l + handlesize, pos - (handlesize / 2), l + handlesize, pos + (handlesize / 2), l, pos,
                            tsColor);
        } else if (handleshape == HANDLE_RECTANGLE) {
            d->fillRect(l - (handlesize / 2), pos - (handlewidth / 2), handlesize, handlewidth, thColor);
            d->drawRect(l - (handlesize / 2), pos - (handlewidth / 2), handlesize, handlewidth, tsColor);
            d->drawRect(l - (handlesize / 2) + 1, pos - (handlewidth / 2) + 1, handlesize - 2, handlewidth - 2,
                        tsColor);
        }
        oy = pos;

        pos = value;


    }

    bool slide(uint16_t ScreenX, uint16_t ScreenY) {

        bool pressed = false;

        x = ScreenX;
        y = ScreenY;

        if ((!enabled) || (!visible)) {
            //return pos;
            return pressed;
        }

        if (sn != 0.0) {
            y = y - t;
            y = (y / (h / sn));
            y = (y * (h / sn)) + t;
        }

        if (y != oy) {

            if (abs(x - l) <= handlesize) {

                if ((y >= t) & (y <= (t + h))) {
                    pressed = true;

                    // it's in rage of ball

                    // blank out the old one
                    if (handleshape == HANDLE_CIRCLE) {
                        d->fillCircle(l, oy, handlesize / 2, bColor);
                    } else if (handleshape == HANDLE_SQUARE) {
                        d->fillRect(l - (handlesize / 2), oy - (handlesize / 2), handlesize, handlesize, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_1) {
                        // weird i know but need to draw the black out slightly larger due to round off errors
                        d->fillTriangle(l - handlesize, oy - (handlesize / 2) - 1, l - handlesize,
                                        oy + (handlesize / 2) + 1, l + 1, oy, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_2) {
                        // weird i know but need to draw the black out slightly larger due to round off errors
                        d->fillTriangle(l + handlesize, oy - (handlesize / 2) - 1, l + handlesize,
                                        oy + (handlesize / 2) + 1, l - 1, oy, bColor);
                    } else if (handleshape == HANDLE_TRIANGLE_3) {
                        // weird i know but need to draw the black out slightly larger due to round off errors
                        d->fillTriangle(l - handlesize, oy - (handlesize / 2) - 1, l - handlesize,
                                        oy + (handlesize / 2) + 1, l + 1, oy, bColor);
                        d->fillTriangle(l + handlesize, oy - (handlesize / 2) - 1, l + handlesize,
                                        oy + (handlesize / 2) + 1, l - 1, oy, bColor);
                    } else if (handleshape == HANDLE_RECTANGLE) {
                        d->fillRect(l - (handlesize / 2), oy - (handlewidth / 2), handlesize, handlewidth, bColor);
                    }
                    // draw slider
                    if (colorscale) {
                        d->fillRect(l - (bt / 2), t, bt, y - t, sColor); // draw new slider
                        d->fillRect(l - (bt / 2), y, bt, h - y + t, hColor); // draw new slider
                    } else {
                        d->fillRect(l - (bt / 2), t, bt, h, sColor);
                    }

                    // draw tick marks
                    if (sc != 0.0) {
                        for (i = 0; i <= sc; i++) {

                            d->fillRect(l - 3, (i * (h / sc)) + t, 7, 1, sColor);

                            if ((i == ce) | (i == 0) | (i == sc)) {
                                d->fillRect(l - 3, (i * (h / sc)) + t, 7, 4, sColor);
                            }
                        }
                    }
                    // draw new handle
                    if (handleshape == HANDLE_CIRCLE) {
                        d->fillCircle(l, y, handlesize / 2, hColor);
                        d->drawCircle(l, y, handlesize / 2, sColor);
                        d->drawCircle(l, y, (handlesize / 2) - 1, sColor);
                    } else if (handleshape == HANDLE_SQUARE) {
                        d->fillRect(l - (handlesize / 2), y - (handlesize / 2), handlesize, handlesize, hColor);
                        d->drawRect(l - (handlesize / 2), y - (handlesize / 2), handlesize, handlesize, sColor);
                        d->drawRect(l - (handlesize / 2) + 1, y - (handlesize / 2) + 1, handlesize - 2, handlesize - 2,
                                    sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_1) {
                        d->fillTriangle(l - handlesize, y - (handlesize / 2), l - handlesize, y + (handlesize / 2), l,
                                        y, hColor);
                        d->drawTriangle(l - handlesize, y - (handlesize / 2), l - handlesize, y + (handlesize / 2), l,
                                        y, sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_2) {
                        d->fillTriangle(l + handlesize, y - (handlesize / 2), l + handlesize, y + (handlesize / 2), l,
                                        y, hColor);
                        d->drawTriangle(l + handlesize, y - (handlesize / 2), l + handlesize, y + (handlesize / 2), l,
                                        y, sColor);
                    } else if (handleshape == HANDLE_TRIANGLE_3) {
                        d->fillTriangle(l - handlesize, y - (handlesize / 2), l - handlesize, y + (handlesize / 2), l,
                                        y, hColor);
                        d->fillTriangle(l + handlesize, y - (handlesize / 2), l + handlesize, y + (handlesize / 2), l,
                                        y, hColor);
                        d->drawTriangle(l - handlesize, y - (handlesize / 2), l - handlesize, y + (handlesize / 2), l,
                                        y, sColor);
                        d->drawTriangle(l + handlesize, y - (handlesize / 2), l + handlesize, y + (handlesize / 2), l,
                                        y, sColor);
                    } else if (handleshape == HANDLE_RECTANGLE) {
                        d->fillRect(l - (handlesize / 2), y - (handlewidth / 2), handlesize, handlewidth, hColor);
                        d->drawRect(l - (handlesize / 2), y - (handlewidth / 2), handlesize, handlewidth, sColor);
                        d->drawRect(l - (handlesize / 2) + 1, y - (handlewidth / 2) + 1, handlesize - 2,
                                    handlewidth - 2, sColor);
                    }
                    oy = y;

                    // get scaled val and pass back and store in the public variable--in case anyone needs easy access...

                    pos = MapFloat(y, t, (t + h), sh, sl);
                    value = pos;
                    delay(debounce);
                }
            }
        }
        //return pos;
        return pressed;
    }

    void setHandleColor(uint16_t HandleColor) {

        hColor = HandleColor;

    }

    void drawSliderColor(bool state) {

        colorscale = state;

    }

    void setBarThickness(byte Thickness) {

        bt = Thickness;


    }

    void setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

        dsColor = SliderColor;
        dhColor = HandleColor;

    }

    void setScale(float ScaleLow, float ScaleHi, float ScaleSize, float SnapSize) {

        // map arguements to class variables
        sl = ScaleLow;
        sh = ScaleHi;
        sc = 0.0;
        sn = 0.0;
        // compute scale increments and snap increments
        if (ScaleSize != 0) {
            sc = (sh - sl) / ScaleSize;
            ce = abs(sh / ScaleSize);
        }
        if (SnapSize != 0) {
            sn = (sh - sl) / SnapSize;
        }

    }

    void enable() {
        enabled = true;
    }

    void disable() {
        enabled = false;
    }

    void show() {
        visible = true;
    }

    void hide() {
        visible = false;
    }

    void setHandleSize(int value) {
        if (value < 4) {
            handlesize = 4;
        } else if (value > 100) {
            handlesize = 100;
        } else {
            handlesize = value;
        }
    }

    void setHandleSize(int value, int width) {

        if (value < 4) {
            handlesize = 4;
        } else if (value > 40) {
            handlesize = 40;
        } else {
            handlesize = value;
        }

        if (width < 4) {
            handlewidth = 4;
        } else if (width > 100) {
            handlewidth = 100;
        } else {
            handlewidth = width;
        }

    }

    void setHandleShape(byte value) {
        if (value < 0) {
            handleshape = HANDLE_CIRCLE;
        } else if (value > 6) {
            handleshape = HANDLE_CIRCLE;
        } else {
            handleshape = value;
        }

    }

    void setPressDebounce(byte Debounce) {
        debounce = Debounce;
    }

    float value;

private:
    TDisplay *d;            // the display object
    uint16_t sColor;        // the slider color
    uint16_t bColor;        // the slider background color
    uint16_t hColor;        // the sliders drag handle
    uint16_t dsColor;
    uint16_t dhColor;
    uint16_t tsColor;
    uint16_t thColor;
    uint16_t x;            // the left coordinate of the scale
    uint16_t y;            // the top coordinate of the scale
    uint16_t l;            // the left coordinate of the scale
    uint16_t t;            // the top coordinate of the scale
    uint16_t w;            // the with of the scale
    uint16_t h;            // the with of the scale
    byte bt;            // the thockness of the bar
    float oy;            // the old screen y value where user pressed
    float sl;                // the scale lower value
    float sh;                // the scale upper value
    float pos;                // the screen coordinate position
    float sn;                // the snap increment
    float sc;                // the scale increment
    float ce;                // the tick mark where zero is (for drawing heavy line on +/- scales
    float i;                // loop counter
    byte tl;
    bool colorscale;        // flag to draw slider in handle color
    float MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
        return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    }

    int tLen, tHi;
    int handlesize;
    int handlewidth;
    byte handleshape;
    bool enabled;
    bool visible;
    byte debounce;
};

template<typename TDisplay>
class SliderOnOff {

public:

    SliderOnOff(TDisplay *display, uint16_t SliderX, uint16_t SliderY, uint16_t SliderW, uint16_t SliderH,
                uint16_t SliderColor, uint16_t BackColor, uint16_t OnColor, uint16_t OffColor) {
        // map arguements to class variables
        _d = display;
        _l = SliderX;
        _t = SliderY;
        _w = SliderW;
        _h = SliderH;
        _sColor = SliderColor;
        _bColor = BackColor;
        _onColor = OnColor;
        _offColor = OffColor;

    }

    bool slide(float ScreenX, float ScreenY) {

        _changed = false;

        if ((ScreenX >= _l) && (ScreenX <= (_l + _w))) {


            if (abs(ScreenY - (_t + (_h / 2))) < _h) {

                _changed = true;

                // it's in range of slider ball
                if (ScreenX < (_l + (_w / 2))) {
                    // press in the off range
                    if (_pos) {

                        // clear on button
                        _d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
                        // draw off button
                        _d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);
                        _pos = false;
                    }
                } else {
                    // it's in the on range
                    if (!_pos) {
                        // clear on button
                        _d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _bColor);
                        // draw off button
                        _d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);

                        _pos = true;
                    }
                }

            }
        }

        return _pos;

    }

    void draw(bool state) {

        _pos = state;

        _d->fillRoundRect(_l, _t, _w, _h, _h / 2, _bColor);
        _d->drawRoundRect(_l, _t, _w, _h, _h / 2, _sColor);
        _d->drawRoundRect(_l + 1, _t + 1, _w - 2, _h, _h / 2, _sColor);


        if (state) {
            // draw on button
            _d->fillCircle(_l + _w - (_h / 2), _t + (_h / 2), (_h / 2) - 2, _onColor);
        } else {
            // draw off button
            _d->fillCircle(_l + (_h / 2), _t + (_h / 2), (_h / 2) - 2, _offColor);

        }

    }

    bool changed() {

        return _changed;

    }

    bool getValue() {

        return _pos;

    }

private:

    TDisplay *_d;            // the display object
    uint16_t _sColor;        // the slider color
    uint16_t _bColor;        // the slider background color
    uint16_t _onColor;        // the sliders on color
    uint16_t _offColor;        // the sliders on color
    uint16_t _l;                // the left coordinate of the scale
    uint16_t _t;                // the top coordinate of the scale
    uint16_t _w;                // the with of the scale
    uint16_t _h;                // the with of the scale
    bool _pos;                // the screen coordinate position
    bool _changed;            //flag to track if button was just changed
};

template<typename TDisplay, typename TFont>
class Button {
public:
    Button(TDisplay *Display) { d = Display; }

    void init(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonWidth, uint8_t ButtonHeight,
              uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor,
              const char *ButtonText, int TextOffsetX, int TextOffsetY, const TFont &TextFont) {

        x = ButtonX;
        y = ButtonY;
        w = ButtonWidth;
        h = ButtonHeight;
        outlinecolor = OutlineColor;
        fillcolor = ButtonColor;
        textcolor = TextColor;
        backcolor = BackgroundColor;
        disablecolorfill = C_DISABLE_LIGHT;
        disablecolortext = C_DISABLE_DARK;
        x_offset = TextOffsetX;
        y_offset = TextOffsetY;
        strncpy(label, ButtonText, 20);
        f = TextFont;
        ct = CORNER_AUTO;
        drawit = true;
        enabled = true;
        visible = true;
        debounce = TFT_DEBOUNCE;
        value = 0; // user controlled for whatever....
        newcorner = false;

    }

    void draw(bool inverted = false) {

        uint16_t fill, outline, text;

        if (!visible) {
            return;
        }

        if (!inverted) {
            drawit = true;
            fill = fillcolor;
            outline = outlinecolor;
            text = textcolor;
        } else {

            if (drawit == false) {

                return;
            }
            drawit = false;
            fill = disablecolorfill;
            outline = outlinecolor;
            text = fillcolor;
        }

        if (newcorner) {
            newcorner = false;
            // d->fillRect(x - (w/2), y - (h/2), w, h, backcolor);
        }


        if (!enabled) {

            if (ct == CORNER_AUTO) {
                d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, min(w, h) / 4, disablecolorfill);
                d->drawRoundRect(x - (w / 2), y - (h / 2), w, h, min(w, h) / 4, disablecolortext);
            } else if (ct == CORNER_SQUARE) {
                d->fillRect(x - (w / 2), y - (h / 2), w, h, disablecolorfill);
                d->drawRect(x - (w / 2), y - (h / 2), w, h, disablecolortext);
            } else {
                d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, ct, disablecolorfill);
                d->drawRoundRect(x - (w / 2), y - (h / 2), w, h, ct, disablecolortext);
            }

            d->setCursor(x + x_offset - (strlen(label) * 3), y + y_offset);
            d->setFont(f);
            d->setTextColor(disablecolortext);
            d->print(label);
        } else {
            if (ct == CORNER_AUTO) {
                d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, min(w, h) / 4, fill);
                d->drawRoundRect(x - (w / 2), y - (h / 2), w, h, min(w, h) / 4, outline);
            } else if (ct == CORNER_SQUARE) {
                d->fillRect(x - (w / 2), y - (h / 2), w, h, fill);
                d->drawRect(x - (w / 2), y - (h / 2), w, h, outline);
            } else {
                d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, ct, fill);
                d->drawRoundRect(x - (w / 2), y - (h / 2), w, h, ct, outline);
            }

            d->setCursor(x + x_offset - (strlen(label) * 3), y + y_offset);
            d->setFont(f);
            d->setTextColor(text);
            d->print(label);
        }

    }

    bool press(int16_t ScreenX, int16_t ScreenY) {

        if ((!enabled) || (!visible)) {
            delay(debounce);
            return false;
        }

        if ((ScreenX < (x - w / 2)) || (ScreenX > (x + w / 2))) {
            return false;
        }
        if ((ScreenY < (y - h / 2)) || (ScreenY > (y + h / 2))) {
            return false;
        }
        delay(debounce);
        return true;

    }

    void show() {
        visible = true;
        draw();
    }

    void hide() {

        if (ct == CORNER_AUTO) {
            d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, min(w, h) / 4, backcolor);
        } else if (ct == CORNER_SQUARE) {
            d->fillRect(x - (w / 2), y - (h / 2), w, h, backcolor);
        } else {
            d->fillRoundRect(x - (w / 2), y - (h / 2), w, h, ct, backcolor);
        }

        visible = false;
    }

    void disable() {
        enabled = false;

    }

    void enable() {
        enabled = true;

    }

    void resize(int16_t ButtonX, int16_t ButtonY, uint8_t ButtonW, uint8_t ButtonH) {
        //hide();
        //draw();
        //show();
        x = ButtonX;
        y = ButtonY;
        w = ButtonW;
        h = ButtonH;

    }

    void setColors(uint16_t OutlineColor, uint16_t ButtonColor, uint16_t TextColor, uint16_t BackgroundColor,
                   uint16_t DisabledTextColor, uint16_t DisabledButtonColor) {

        outlinecolor = OutlineColor;
        fillcolor = ButtonColor;
        textcolor = TextColor;
        backcolor = BackgroundColor;
        disablecolorfill = DisabledButtonColor;
        disablecolortext = DisabledTextColor;

    }

    void setFont(int TextOffsetX, int TextOffsetY, const TFont &TextFont) {
        x_offset = TextOffsetX;
        y_offset = TextOffsetY;
        f = TextFont;
    }

    void setText(const char *ButtonText) {
        strncpy(label, ButtonText, 20);
    }

    void setCornerRadius(int radius) {
        newcorner = true;
        ct = radius;
    }

    bool isEnabled() {
        return enabled;
    }

    bool isVisible() {
        return visible;
    }

    void setPressDebounce(byte Debounce) {
        debounce = Debounce;
    }

    int value;

private:
    TDisplay *d;
    TFont f;
    int16_t x, y;
    uint16_t w, h;
    int x_offset, y_offset;
    bool redraw;
    uint16_t outlinecolor, fillcolor, textcolor, backcolor, disablecolorfill, disablecolortext;
    char label[20];
    boolean drawit;
    bool enabled;
    int ct;
    bool visible;
    byte debounce;
    bool newcorner;

};

template<typename TDisplay, typename TFont>
class CheckBox {
public:
    CheckBox(TDisplay *Display) { d = Display; }

    void init(int16_t ButtonX, uint16_t ButtonY, uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor,
              uint16_t TextColor, uint16_t BackgroundColor, int TextOffsetX, int TextOffsetY, const char *Text,
              const TFont &TextFont) {

        x = ButtonX;
        y = ButtonY + CHECKBOX_SIZE;
        s = CHECKBOX_SIZE;
        ct = 3;
        oc = OutlineColor;
        uc = UPColor;
        dc = DownColor;
        tc = TextColor;
        bc = BackgroundColor;
        doc = C_DISABLE_LIGHT;
        duc = C_DISABLE_MED;
        ddc = C_DISABLE_DARK;
        dtc = C_DISABLE_MED;
        strncpy(label, Text, 60);
        tox = TextOffsetX;
        toy = TextOffsetY;
        f = TextFont;
        enabled = true;
        state = true; // true=up, false=down
        visible = true;
        value = -1;
        debounce = TFT_DEBOUNCE;

    }

    void draw(bool val) {

        uint16_t fill, outline, tcolor;

        if (!visible) {
            d->fillRoundRect(x, y - s, s, s, ct, bc);
            return;
        }
        if (val) {
            // last exit was no press so restore val from internal saved state
            val = state;
        }
        state = val;
        // store the variable in the public variable
        value = val;
        if (state) {
            fill = uc;
            outline = oc;
            tcolor = tc;
        } else {
            fill = dc;
            outline = oc;
            tcolor = tc;
        }

        if (!enabled) {
            if (state) {
                fill = duc;
                outline = doc;
                tcolor = dtc;
            } else {
                fill = ddc;
                outline = doc;
                tcolor = dtc;
            }
        }

        d->fillRoundRect(x, y - s, s, s, ct, fill);
        d->drawRoundRect(x, y - s, s, s, ct, outline);

        d->setCursor(x + tox + (s / 2), y - s + toy);
        d->setFont(f);
        d->setTextColor(tcolor);
        d->print(label);
    }

    bool press(int16_t SceenX, int16_t ScreenY) {
        bool pressed = false;

        if ((!visible) || (!enabled)) {
            return pressed;
        }

        if (((SceenX >= x) && (SceenX <= (x + s))) && ((ScreenY >= y - s) && (ScreenY <= (y)))) {
            state = !state;
            draw(state);
            delay(debounce);
            // store the variable in the public variable
            value = state;
            pressed = true;
        }
        return pressed;
    }

    void show() {
        visible = true;
    }


    void hide() {
        visible = false;
    }

    void disable() {
        enabled = false;
    }

    void enable() {
        enabled = true;
    }


    void resize(int16_t ButtonX, int16_t ButtonY, uint8_t Size) {
        hide();
        draw(state);
        x = ButtonX;
        y = ButtonY;
        s = Size;
        show();
        draw(state);

    }

    void setColors(uint16_t OutlineColor, uint16_t UPColor, uint16_t DownColor, uint16_t BackgroundColor,
                   uint16_t DisableOutlineColor, uint16_t DisableTextColor, uint16_t DisableUPColor,
                   uint16_t DisableDownColor) {

        oc = OutlineColor;
        uc = UPColor;
        dc = DownColor;
        bc = BackgroundColor;
        doc = DisableOutlineColor;
        duc = DisableUPColor;
        ddc = DisableDownColor;
        dtc = DisableTextColor;

    }

    void setText(int TextOffsetX, int TextOffsetY, const char *Text, const TFont &TextFont) {

        tox = TextOffsetX;
        toy = TextOffsetY;
        strncpy(label, Text, 60);
        f = TextFont;

    }

    void setCornerRadius(int val) {
        ct = val;

    }

    bool isEnabled() {
        return enabled;
    }

    bool isVisibled() {
        return visible;
    }

    void setPressDebounce(byte Debounce) {
        debounce = Debounce;
    }


    bool value;

private:
    TDisplay *d;
    char label[60];
    TFont f;
    int16_t x, y;
    uint16_t s, ct;
    uint16_t oc, uc, dc, bc, doc, duc, ddc, dtc, tc;
    bool state;
    int tox, toy;
    bool enabled;
    bool visible;
    byte debounce;
};

template<typename TDisplay, typename TFont>
class OptionButton {
public:
    OptionButton(TDisplay *Display) { d = Display; }

    void init(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, int16_t TextColor,
              uint16_t BackgroundColor, int TextOffsetX, int TextOffsetY, const TFont &TextFont) {

        r = OPTION_BUTTON_RADIUS;
        oc = OutlineColor;
        sc = SelectedColor;
        uc = UnSelectedColor;
        bc = BackgroundColor;
        tc = TextColor;
        dsc = C_DISABLE_MED;
        duc = C_DISABLE_DARK;
        doc = C_DISABLE_LIGHT;
        tox = TextOffsetX;
        toy = TextOffsetY;
        dtc = C_DISABLE_MED;
        f = TextFont;
        enabled = true;
        current = 0;
        ID = 0;
        visible = true;
        value = current;
        debounce = TFT_DEBOUNCE;
    }

    int add(uint16_t ButtonX, uint16_t ButtonY, const char *Text, float OptionValue = -32001) {

        strncpy(label[ID], Text, 60);

        x[ID] = ButtonX;
        y[ID] = ButtonY;
        // this next part gives users to specify what the value will be
        // you can defaul to the ID,or specify it, good use is if option ID=3 is pressed and you want value to be 24
        // very odd value but chances of someone wanting this val?
        if (OptionValue == -32001) {
            rv[ID] = ID;
        } else {
            rv[ID] = OptionValue;
        }

        ID++;
        return (ID - 1);

    }


    void draw(int OptionID) {

        uint16_t toc, tsc, tuc, tcolor;

        current = OptionID;

        if (!visible) {

            for (i = 0; i < ID; i++) {
                d->fillCircle(x[i] + r / 2, y[i] + r / 2, r, bc);
                d->setCursor(x[i] + tox, y[i] + toy);
                d->setFont(f);
                d->setTextColor(bc);
                d->print(label[i]);
            }
            return;
        }

        toc = oc;
        tsc = sc;
        tuc = uc;
        tcolor = tc;

        if (!enabled) {
            toc = doc;
            tsc = dsc;
            tuc = duc;
            tcolor = dtc;
        }

        for (i = 0; i < ID; i++) {
            if (i == current) {
                d->fillCircle(x[i] + r / 2, y[i] + r / 2, r, tsc);
                value = rv[i];
                option = i;
            } else {
                d->fillCircle(x[i] + r / 2, y[i] + r / 2, r, tuc);
            }
            d->drawCircle(x[i] + r / 2, y[i] + r / 2, r, toc);
            d->setCursor(x[i] + tox, y[i] + toy);
            d->setFont(f);
            d->setTextColor(tcolor);
            d->print(label[i]);
        }

        // store the variable in the public variable


    }

    bool press(uint16_t ScreenX, uint16_t ScreenY) {
        bool pressed = false;
        if ((!visible) || (!enabled)) {
            return pressed;
        }

        for (i = 0; i < ID; i++) {
            if (((ScreenX >= (x[i] - 2 * r)) && (ScreenX <= (x[i] + 2 * r))) &&
                ((ScreenY >= (y[i] - 2 * r)) && (ScreenY <= (y[i] + 2 * r)))) {
                current = i;
                // store the variable in the public variable
                value = rv[i];
                option = i;
                pressed = true;
                break;
            }
        }
        draw(current);
        delay(debounce);

        return pressed;

    }

    void select(int val) {
        value = rv[val];
        option = val;
        current = val;
    }

    void show() {
        visible = true;
    }

    void hide() {
        visible = false;
    }

    void disable() {
        enabled = false;
    }

    void enable() {
        enabled = true;
    }

    void resize(byte radius) {
        // caution, only all this before first draw--code will not remember old size
        r = radius;
    }

    void setColors(uint16_t OutlineColor, uint16_t SelectedColor, uint16_t UnSelectedColor, uint16_t TextColor,
                   uint16_t BackgroundColor, uint16_t DisableOutlineColor, uint16_t DisableSelColor,
                   uint16_t DisableUnSelColor, uint16_t DisableTextColor) {

        oc = OutlineColor;
        sc = SelectedColor;
        uc = UnSelectedColor;
        tc = TextColor;
        bc = BackgroundColor;
        doc = DisableOutlineColor;
        dsc = DisableSelColor;
        duc = DisableUnSelColor;
        dtc = DisableTextColor;

    }

    void setFont(int TextOffsetX, int TextOffsetY, const TFont &TextFont) {

        tox = TextOffsetX;
        toy = TextOffsetY;
        f = TextFont;

    }

    void setText(int ID, const char *Text) {
        strncpy(label[ID], Text, 60);
    }

    int selected() {
        return current;
    }

    bool isEnable() {
        return enabled;
    }

    bool isVisible() {
        return visible;
    }

    void setPressDebounce(byte Debounce) {
        debounce = Debounce;
    }

    float value;
    int option;

private:
    TDisplay *d;
    char label[MAX_OPTION][60];
    TFont f;
    uint16_t x[MAX_OPTION], y[MAX_OPTION];
    float rv[MAX_OPTION];
    uint16_t r;
    int i, tox, toy;
    int ID;
    int current;
    uint16_t oc, sc, uc, bc, tc, doc, dsc, duc, dtc;
    bool enabled;
    bool visible;
    byte debounce;
};

template<typename TDisplay>
class SliderD {

public:

    SliderD(TDisplay *Display) {
        // map arguements to class variables
        d = Display;


    }

    void init(uint16_t SliderX, uint16_t SliderY, uint16_t SliderR, float SweepAngle, float ScaleLow, float ScaleHi,
              uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

        // map arguements to class variables
        x = SliderX;
        y = SliderY;
        r = SliderR;
        sl = ScaleLow;
        sh = ScaleHi;
        sa = SweepAngle;
        as = 180.0 + 90.0 - ((360.0 - sa) / 2.0);
        ae = -(90.0 - (360.0 - sa) / 2.0);
        colorscale = true;
        handlesize = SLIDER_HANDLE_SIZE;
        enabled = true;
        visible = true;
        dt = 3;
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;
        debounce = TFT_DEBOUNCE;
        dsColor = C_DISABLE_LIGHT;
        dhColor = C_DISABLE_MED;
        redraw = true;
        state = true;
    }

    void setColors(uint16_t SliderColor, uint16_t BackgroundColor, uint16_t HandleColor) {

        // map arguements to class variables
        sColor = SliderColor;
        bColor = BackgroundColor;
        hColor = HandleColor;

    }

    void draw(float val) {

        pressed = false;

        if (!redraw) {
            redraw = !redraw;
            return;
        }

        if ((val < sl) || (val > sh)) {
            return;
        }
        if (!visible) {
            if (!visible) {
                DrawRing(as, ae, bColor);
                DrawHandle(angle, bColor, bColor);
            }
            return;
        }

        value = val;

        if (enabled) {
            tsColor = sColor;
            thColor = hColor;
        } else {
            tsColor = dsColor;
            thColor = dhColor;
        }
        // get position angle
        angle = MapFloat(value, sl, sh, (float) as, (float) ae);

        if (colorscale) {
            DrawRing(as, angle, thColor);
        } else {
            DrawRing(as, angle, tsColor);
        }

        DrawRing(angle, ae, tsColor);

        DrawHandle(angle, thColor, tsColor);

        oangle = angle;

        value = val;

    }

    bool slide(uint16_t ScreenX, uint16_t ScreenY) {

        pressed = false;

        if ((!enabled) || (!visible)) {
            //return pos;
            return pressed;
        }

        // is press in range

        dist = sqrt(((ScreenX - x) * (ScreenX - x)) + ((ScreenY - y) * (ScreenY - y)));

        angle = (atan((abs(y - (float) ScreenY)) / (abs(x - (float) ScreenX)))) / degtorad;
        // cases

        // quad 1
        if ((ScreenX >= x) && (ScreenY <= y)) {
            angle = angle;
        }
            //quad 4
        else if ((ScreenX > x) && (ScreenY > y)) {
            angle = -angle;
        }
            // quad 2
        else if ((ScreenX <= x) && (ScreenY <= y)) {
            angle = 180 - angle;
        }
            // quad 3
        else if ((ScreenX <= x) && (ScreenY >= y)) {
            angle = 180 + angle;
        }

        if ((abs(angle - oangle) > 0) && (abs(dist - r)) < (handlesize / 2) && (angle <= as) && (angle >= ae)) {

            pressed = true;
            redraw = true;
            DrawHandle(oangle, bColor, bColor);

            if (colorscale) {
                DrawRing(as, angle, hColor);
            } else {
                DrawRing(as, angle, sColor);
            }
            DrawRing(angle, ae, sColor);

            DrawHandle(angle, hColor, sColor);

            oangle = angle;

            value = MapFloat(angle, as, ae, sl, sh);

        }
        return pressed;
    }

    void setHandleColor(uint16_t HandleColor) {

        hColor = HandleColor;

    }

    void drawSliderColor(bool state) {

        colorscale = state;

    }

    void setRingThickness(byte Thickness) {

        dt = Thickness;

    }

    void setDisableColor(uint16_t HandleColor, uint16_t SliderColor) {

        dsColor = SliderColor;
        dhColor = HandleColor;

    }

    void setScale(float ScaleLow, float ScaleHi) {

        sl = ScaleLow;
        sh = ScaleHi;

    }

    void enable() {

        enabled = true;
        redraw = false;
        if (!state) {
            state = true;
            redraw = true;
        }
    }

    void disable() {
        enabled = false;
        redraw = false;
        if (state) {
            state = false;
            redraw = true;
        }
    }

    void show() {

        visible = true;

    }

    void hide() {

        visible = false;

    }

    void setHandleSize(int value) {

        if (value < 4) {
            handlesize = 4;
        } else if (value > 100) {
            handlesize = 100;
        } else {
            handlesize = value;
        }

    }

    void setPressDebounce(byte Debounce) {
        debounce = Debounce;
    }

    float value;
    bool state;

private:

    TDisplay *d;            // the display object
    uint16_t sColor;        // the slider color
    uint16_t bColor;        // the slider background color
    uint16_t hColor;        // the sliders drag handle
    uint16_t dsColor;
    uint16_t dhColor;
    uint16_t tsColor;
    uint16_t thColor;
    uint16_t x;            // the left coordinate of the scale
    uint16_t y;            // the top coordinate of the scale
    uint16_t r;            // the left coordinate of the scale
    float sa, as, ae; // sweep angle, start angle, end angle
    byte dt;
    float sl;                // the scale lower value
    float sh;                // the scale upper value
    float angle, oangle, hx, hy;                // the screen coordinate position
    float i;                // loop counter
    float dist;
    bool colorscale;        // flag to draw slider in handle color
    bool pressed = false;

    float MapFloat(float val, float fromLow, float fromHigh, float toLow, float toHigh) {
        return (val - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
    }

    int handlesize;
    bool enabled;
    bool visible, redraw;
    byte debounce;

    void DrawRing(float Start, float End, uint16_t color) {

        for (i = Start; i > End; i--) {
            hx = (r * (cos(i * degtorad))) + x;
            hy = -(r * (sin(i * degtorad))) + y;

            d->fillCircle(hx, hy, dt, color);

            if (color == bColor) {
                d->fillCircle(hx, hy, dt + 1, color);
            }
        }
    }

    void DrawHandle(float angle, uint16_t hColor, uint16_t sColor) {

        hx = (r * (cos(angle * degtorad))) + x;
        hy = -(r * (sin(angle * degtorad))) + y;

        if (hColor == bColor) {
            d->fillCircle(hx, hy, handlesize, bColor); // erase it
        } else {
            d->fillCircle(hx, hy, handlesize, hColor);
            d->drawCircle(hx, hy, handlesize - 1, sColor);
            d->drawCircle(hx, hy, handlesize - 2, sColor);
        }
    }
};

#endif
