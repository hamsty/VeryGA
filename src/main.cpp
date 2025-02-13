// Verilated -*- C++ -*-
// DESCRIPTION: main() calling loop, created with Verilator --main

#include "verilated.h"
#include "Vveryga.h"
#include <fstream>
#include <stdio.h>
#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "emscripten/html5.h"
#endif

#define GLFW_INCLUDE_NONE
#define GLM_FORCE_RADIANS
// #include <GLFW/glfw3.h>
// #include <GLES3/gl3.h>
// #include <glad/glad.h>
#include <SDL2/SDL.h>

#define Mhz 50

#define width 640
#define height 480

#define tick 20

uint32_t mov;
int rst = 2;

// static void error_callback(int error, const char *description)
// {
//     fprintf(stderr, "Error: %s\n", description);
// }

const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};

const std::unique_ptr<Vveryga> topp{new Vveryga{contextp.get(), ""}};

// static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_UP && action == GLFW_PRESS)
//         mov = 4;
//     if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
//         mov = 2;
//     if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
//         mov = 8;
//     if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
//         mov = 1;
//     if (action == GLFW_RELEASE)
//         mov = 0;
// }

// void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
// {
//     if (button == GLFW_MOUSE_BUTTON_LEFT)
//     {
//         if (action == GLFW_PRESS)
//         {
//             rst = 0;
//         }
//         if (action == GLFW_RELEASE)
//         {
//             rst = 2;
//         }
//     }
// }

// GLFWwindow *window;

// int set_window()
// {

//     glfwSetErrorCallback(error_callback);

//     if (!glfwInit())
//         exit(EXIT_FAILURE);

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

//     window = glfwCreateWindow(width, height, "VeryGA", NULL, NULL);
//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }

//     glfwSetKeyCallback(window, key_callback);
//     glfwSetMouseButtonCallback(window, mouse_button_callback);

//     glfwMakeContextCurrent(window);
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         // std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

//     float theta = 0;
//     return 0;
// }

SDL_Window *window;
SDL_Renderer *renderer;

#ifdef EMSCRIPTEN

bool key_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
    {
        if (!strcmp(keyEvent->code, "ArrowUp"))
        {
            mov = 4;
        }
        else if (!strcmp(keyEvent->code, "ArrowDown"))
        {
            mov = 2;
        }
        else if (!strcmp(keyEvent->code, "ArrowLeft"))
        {
            mov = 8;
        }
        else if (!strcmp(keyEvent->code, "ArrowRight"))
        {
            mov = 1;
        }
        else if (!strcmp(keyEvent->code, "KeyA"))
        {
            rst = 0;
        }
        else if (!strcmp(keyEvent->code, "KeyB"))
        {
            if (!contextp->gotFinish())
            {
                VL_DEBUG_IF(VL_PRINTF("+ Exiting without $finish; no events left\n"););
            }

            // Execute 'final' processes
            topp->final();

            // Print statistical summary report
            contextp->statsPrintSummary();

            // glfwDestroyWindow(window);

            // glfwTerminate();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();

            exit(EXIT_SUCCESS);
        }
    }
    else if (eventType == EMSCRIPTEN_EVENT_KEYUP)
    {
        mov = 0;
        rst = 2;
    }
    // printf("%s %d %d %d\n", keyEvent->code, mov, rst, eventType);
    return true;
}

#else
int event_listener()
{
    int code = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                mov = 4;
                break;
            case SDLK_DOWN:
                mov = 2;
                break;
            case SDLK_LEFT:
                mov = 8;
                break;
            case SDLK_RIGHT:
                mov = 1;
                break;
            case SDLK_a:
                rst = 0;
                break;
            }
            break;
        case SDL_KEYUP:
            mov = 0;
            rst = 2;
            break;
        case SDL_QUIT:
            if (!contextp->gotFinish())
            {
                VL_DEBUG_IF(VL_PRINTF("+ Exiting without $finish; no events left\n"););
            }

            // Execute 'final' processes
            topp->final();

            // Print statistical summary report
            contextp->statsPrintSummary();

            // glfwDestroyWindow(window);

            // glfwTerminate();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();

            exit(EXIT_SUCCESS);
            break;
        }
    }

    return 0;
}
#endif

int set_window()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
    window = SDL_CreateWindow("VeryGA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    return 0;
}

double timestamp = 0;
uint64_t out = 0;
// int offset = 0;
int vcount = 0;
int hcount = 0;
int x = 0;
int y = 0;
// uint8_t pixels[width * height * 3];
int old_clk = 0;
int old_hsync = 0;
int old_vsync = 0;
int hsync = 1;
int vsync = 1;
int clk = 0;
int en_hcount = true;

void loop()
{

    while (true)
    {

        // Evaluate model
        contextp->timeInc(tick);

        topp->in = !clk;
        clk = (topp->in & 1);

        topp->in += mov << 2;
        topp->in += rst;

        topp->eval();

        out = topp->out;

        hsync = (out & 0x2000) >> 13;
        vsync = (out & 0x1000) >> 12;

        // VL_PRINTF("%lu %lu\n", out, topp->in);

        if (clk == 1 && old_clk == 0)
        {
            if (rst == 2)
            {
                if (hsync == 0 && old_hsync == 1)
                {
                    hcount = 0;
                }
                else
                {
                    if (true)
                    {
                        hcount++;

                        if (hcount > 144 && hcount < 784)
                        {
                            x++;
                        }
                        else
                        {
                            x = 0;
                        }
                        en_hcount = false;
                    }
                    else
                    {
                        en_hcount = true;
                    }
                }

                if (vsync == 0 && old_vsync == 1)
                {

// glClear(GL_COLOR_BUFFER_BIT);

// glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

// glfwSwapBuffers(window);

// glfwPollEvents();
#ifndef EMSCRIPTEN
                    event_listener();
#endif
                    SDL_RenderPresent(renderer);
                    if (vcount > 0)
                    {
                        vcount = 0;
                        // printf("quadro\n");
                        return;
                    }
                    vcount = 0;
                }
                else
                {
                    if (hsync == 0 && old_hsync == 1)
                    {
                        vcount++;
                        if (vcount > 35 && vcount < 515)
                        {
                            y++;
                        }
                        else
                        {
                            y = 0;
                        }
                    }
                }
                old_vsync = vsync;
                old_hsync = hsync;

                if (vcount > 35 && hcount > 144 && vcount < 515 && hcount < 784)
                {
                    // offset = ((479 - y) * 640 + x) * 3;
                    // pixels[offset + 0] = ((out & 0xF00) >> 8) * 17;
                    // pixels[offset + 1] = ((out & 0xF0) >> 4) * 17;
                    // pixels[offset + 2] = (out & 0xF) * 17;
                    uint8_t R = ((out & 0xF00) >> 8) * 17;
                    uint8_t G = ((out & 0xF0) >> 4) * 17;
                    uint8_t B = (out & 0xF) * 17;
                    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            else
            {
#ifndef EMSCRIPTEN
                event_listener();
#endif
                return;
                // glfwPollEvents();
            }
        }
        old_clk = clk;
    }
}

//======================

int main()
{
    // Setup context, defaults, and parse command line

    if (set_window() != 0)
        return -1;

    Verilated::debug(0);

    // contextp->traceEverOn(true);
    // contextp->commandArgs(argc, argv);

    // Construct the Verilated model, from Vtop.h generated from Verilating
    mov = 0;
    topp->in = 0;
    topp->out = 0;

    rst = 2;
#ifdef EMSCRIPTEN
    emscripten_set_keydown_callback("#canvas", 0, 1, key_callback);
    emscripten_set_keyup_callback("#canvas", 0, 1, key_callback);
    emscripten_set_main_loop(loop, -1, true);
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, 1000);

#else
    while (true)
    {
        loop();
    }

    return 0;
#endif
}
