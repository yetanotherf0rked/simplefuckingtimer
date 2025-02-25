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

// Helper function to format elapsed time.
std::string formatTime(double seconds) {
    int totalSec = static_cast<int>(seconds);
    int days = totalSec / 86400;
    int hours = (totalSec % 86400) / 3600;
    int minutes = (totalSec % 3600) / 60;
    // Calculate the seconds (with fraction) that remain after extracting days, hours, and minutes.
    double secWithFraction = seconds - (days * 86400 + hours * 3600 + minutes * 60);
    int secInt = static_cast<int>(secWithFraction);
    int secFrac = static_cast<int>((secWithFraction - secInt) * 100); // two decimals

    std::ostringstream oss;
    if (days > 0) {
        oss << days << "d "
            << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << secInt << "."
            << std::setfill('0') << std::setw(2) << secFrac;
    } else if (hours > 0) {
        oss << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << secInt << "."
            << std::setfill('0') << std::setw(2) << secFrac;
    } else if (minutes > 0) {
        oss << minutes << ":"
            << std::setfill('0') << std::setw(2) << secInt << "."
            << std::setfill('0') << std::setw(2) << secFrac;
    } else {
        // For times less than a minute, show seconds with two decimals.
        oss << std::fixed << std::setprecision(2) << seconds;
    }
    return oss.str();
}

// Common Linux font lookup.
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

void renderTimer(SDL_Renderer* renderer, TTF_Font* font, double timeValue) {
    std::string timeText = formatTime(timeValue);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, timeText.c_str(), white);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textW = 0, textH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstRect = {100, 100, textW, textH};

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

    std::thread hotkeyThread(hotkeyListener);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        double currentTime = timer.getTime();
        renderTimer(renderer, font, currentTime);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    hotkeyThread.detach();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

