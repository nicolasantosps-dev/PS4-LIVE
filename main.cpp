#include <SDL2/SDL.h>
#include <cstdio>
#include <cstring>

// PS4 LIVE UI prototype. Streaming/capture are intentionally not faked.
static const int W = 1920, H = 1080;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static void rect(int x, int y, int w, int h, SDL_Color c) {
    SDL_Rect r{x, y, w, h};
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &r);
}
static void line(int x1, int y1, int x2, int y2, SDL_Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// Tiny 5x7 bitmap font for a deliberately small ASCII label set.
static const unsigned char* glyph(char c) {
    static const unsigned char blank[7]={0,0,0,0,0,0,0};
    static const unsigned char A[7]={14,17,17,31,17,17,17}, B[7]={30,17,17,30,17,17,30};
    static const unsigned char C[7]={14,17,16,16,16,17,14}, D[7]={30,17,17,17,17,17,30};
    static const unsigned char E[7]={31,16,16,30,16,16,31}, F[7]={31,16,16,30,16,16,16};
    static const unsigned char G[7]={14,17,16,23,17,17,15}, Hh[7]={17,17,17,31,17,17,17};
    static const unsigned char I[7]={14,4,4,4,4,4,14}, J[7]={7,2,2,2,18,18,12};
    static const unsigned char L[7]={16,16,16,16,16,16,31}, M[7]={17,27,21,21,17,17,17};
    static const unsigned char N[7]={17,25,21,19,17,17,17}, O[7]={14,17,17,17,17,17,14};
    static const unsigned char P[7]={30,17,17,30,16,16,16}, R[7]={30,17,17,30,20,18,17};
    static const unsigned char S[7]={15,16,16,14,1,1,30}, T[7]={31,4,4,4,4,4,4};
    static const unsigned char U[7]={17,17,17,17,17,17,14}, V[7]={17,17,17,17,17,10,4};
    static const unsigned char X[7]={17,17,10,4,10,17,17}, Y[7]={17,17,10,4,4,4,4};
    static const unsigned char zero[7]={14,17,19,21,25,17,14}, one[7]={4,12,4,4,4,4,14};
    static const unsigned char two[7]={14,17,1,2,4,8,31}, three[7]={30,1,1,14,1,1,30};
    static const unsigned char colon[7]={0,4,4,0,4,4,0}, dash[7]={0,0,0,31,0,0,0};
    switch(c){case 'A':return A;case 'B':return B;case 'C':return C;case 'D':return D;case 'E':return E;case 'F':return F;case 'G':return G;case 'H':return Hh;case 'I':return I;case 'J':return J;case 'L':return L;case 'M':return M;case 'N':return N;case 'O':return O;case 'P':return P;case 'R':return R;case 'S':return S;case 'T':return T;case 'U':return U;case 'V':return V;case 'X':return X;case 'Y':return Y;case '0':return zero;case '1':return one;case '2':return two;case '3':return three;case ':':return colon;case '-':return dash;default:return blank;}
}
static void text5(const char* s, int x, int y, int scale, SDL_Color c) {
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
    for (size_t i=0;s[i];++i) {
        const unsigned char* g=glyph(s[i]);
        for(int row=0;row<7;++row) for(int col=0;col<5;++col)
            if(g[row] & (1 << (4-col))) { SDL_Rect p{x+(int)i*6*scale+col*scale,y+row*scale,scale,scale}; SDL_RenderFillRect(renderer,&p); }
    }
}
static void draw() {
    const SDL_Color bg{12,10,18,255}, panel{25,20,36,255}, purple{145,70,255,255}, white{245,241,255,255}, muted{164,153,181,255}, red{238,72,105,255};
    SDL_SetRenderDrawColor(renderer,bg.r,bg.g,bg.b,bg.a); SDL_RenderClear(renderer);
    rect(0,0,1920,12,purple); rect(0,12,430,1068,{18,14,27,255});
    text5("PS4 LIVE",72,70,9,white); text5("TWITCH BROADCAST",74,160,3,purple);
    rect(58,230,314,5,purple); text5("DASHBOARD",72,275,4,white);
    text5("STREAM STATUS",520,95,4,muted);
    rect(520,145,1320,250,panel); rect(520,145,10,250,purple);
    text5("NOT LIVE",575,205,8,white); text5("CAPTURE MODULE PENDING",578,300,4,red);
    rect(520,450,635,430,panel); rect(1185,450,655,430,panel);
    text5("STREAM SETTINGS",565,500,4,white); text5("PLATFORM",565,590,3,muted); text5("TWITCH",565,635,5,purple);
    text5("OUTPUT TARGET",565,720,3,muted); text5("1080P 30 FPS",565,765,4,white);
    text5("SYSTEM CHECK",1230,500,4,white); text5("GOLDHEN",1230,590,3,muted); text5("2.4B18.11",1230,630,4,white);
    text5("FIRMWARE TARGET",1230,710,3,muted); text5("13.52 - VERIFY",1230,750,3,white);
    rect(520,925,1320,86,{55,30,82,255}); rect(520,925,10,86,purple);
    text5("LIVE START DISABLED",565,960,4,white);
    text5("CAPTURE AND ENCODER NOT IMPLEMENTED",520,1035,2,muted);
    SDL_RenderPresent(renderer);
}
int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) return 1;
    window=SDL_CreateWindow("PS4 LIVE",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_SHOWN);
    if(!window){SDL_Quit();return 2;}
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer){SDL_DestroyWindow(window);SDL_Quit();return 3;}
    SDL_RenderSetLogicalSize(renderer,W,H);
    bool running=true; SDL_Event e;
    while(running){while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT)running=false;if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE)running=false;}draw();SDL_Delay(16);}
    SDL_DestroyRenderer(renderer);SDL_DestroyWindow(window);SDL_Quit();return 0;
}
