#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <thread>
#include "timer.h"

// Global Timer instance.
Timer timer;

// Declaration for hotkeyListener.
void hotkeyListener();

// New helper function to format the elapsed time.
std::string formatTime(double seconds) {
    std::ostringstream oss;
    int totalSec = static_cast<int>(seconds);
    
    if (seconds < 60.0) {
        // Under 60 seconds: show seconds with decimals.
        oss << std::fixed << std::setprecision(2) << seconds;
    } else if (seconds < 3600.0) {
        // Between 60 seconds and 1 hour: MM:SS.ss
        int minutes = totalSec / 60;
        double sec = seconds - minutes * 60;
        oss << minutes << ":"
            << std::setfill('0') << std::setw(2) << static_cast<int>(sec) << "."
            << std::setfill('0') << std::setw(2) << static_cast<int>((sec - static_cast<int>(sec)) * 100);
    } else if (seconds < 86400.0) {
        // Between 1 hour and 1 day: HH:MM:SS.ss
        int hours = totalSec / 3600;
        int minutes = (totalSec % 3600) / 60;
        double sec = seconds - hours * 3600 - minutes * 60;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << static_cast<int>(sec) << "."
            << std::setfill('0') << std::setw(2) << static_cast<int>((sec - static_cast<int>(sec)) * 100);
    } else {
        // 1 day or more: DD:HH:MM:SS.ss (max days considered is 99)
        int days = totalSec / 86400;
        int hours = (totalSec % 86400) / 3600;
        int minutes = (totalSec % 3600) / 60;
        double sec = seconds - days * 86400 - hours * 3600 - minutes * 60;
        oss << std::setfill('0') << std::setw(2) << days << ":"
            << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << static_cast<int>(sec) << "."
            << std::setfill('0') << std::setw(2) << static_cast<int>((sec - static_cast<int>(sec)) * 100);
    }
    return oss.str();
}

// Helper function to search for a system default font.
std::string getDefaultFontPath() {
    std::vector<std::string> possiblePaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf"
    };

    for (const auto &path : possiblePaths) {
        if (access(path.c_str(), F_OK) == 0) {
            return path;
        }
    }
    return "";
}

// Global variables for overlay sizing and margins.
int overlayWidth = 0;
int overlayHeight = 0;
const int horizontalMargin = 10;
const int verticalMargin = 10;

void renderTimer(SDL_Renderer* renderer, TTF_Font* font, double timeValue) {
    std::string timeText = formatTime(timeValue);
    int textW = 0, textH = 0;
    if (TTF_SizeText(font, timeText.c_str(), &textW, &textH) != 0) {
        std::cerr << "TTF_SizeText error: " << TTF_GetError() << "\n";
        return;
    }
    
    // Compute the destination rectangle so that text is right-aligned.
    // The text's right edge will be at overlayWidth - horizontalMargin.
    int dstX = overlayWidth - horizontalMargin - textW;
    int dstY = (overlayHeight - textH) / 2; // vertically centered
    SDL_Rect dstRect = { dstX, dstY, textW, textH };

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, timeText.c_str(), white);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
        return 1;
    }

    // Create an initial window (size will be adjusted below)
    SDL_Window* window = SDL_CreateWindow("SimpleFuckingTimer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          400, 200,
                                          SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return 1;
    }
    SDL_SetWindowOpacity(window, 0.5);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        return 1;
    }

    std::string fontPath = getDefaultFontPath();
    if (fontPath.empty()) {
        std::cerr << "No default system font found!\n";
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 48);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
        return 1;
    }

    // Determine a fixed overlay size that will fit the worst-case timer string.
    // We'll use "99:23:59:59.99" as the maximum possible string.
    int maxTextW = 0, maxTextH = 0;
    std::string maxString = "99:23:59:59.99";
    if (TTF_SizeText(font, maxString.c_str(), &maxTextW, &maxTextH) != 0) {
        std::cerr << "TTF_SizeText error: " << TTF_GetError() << "\n";
    }
    overlayWidth = maxTextW + 2 * horizontalMargin;
    overlayHeight = maxTextH + 2 * verticalMargin;
    SDL_SetWindowSize(window, overlayWidth, overlayHeight);

    // Launch the hotkey listener in a separate thread.
    std::thread hotkeyThread(hotkeyListener);

    // Variables for dragging the window.
    bool dragging = false;
    int dragOffsetX = 0, dragOffsetY = 0;

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        dragging = true;
                        dragOffsetX = e.button.x;
                        dragOffsetY = e.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        dragging = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (dragging) {
                        int globalX, globalY;
                        SDL_GetGlobalMouseState(&globalX, &globalY);
                        SDL_SetWindowPosition(window, globalX - dragOffsetX, globalY - dragOffsetY);
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        double currentTime = timer.getTime();
        renderTimer(renderer, font, currentTime);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approximately 60 FPS.
    }

    hotkeyThread.detach();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

