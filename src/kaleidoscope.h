#include <SDL2/SDL.h>
#include <string>

class Kaleidoscope
{
public:
    int shutdown = false;

private:
    // Constants
    const std::string WINDOW_TITLE = "Kaleidoscope";

    // SCALE_FACTOR is the number you divide the picture width and height to get the canvas'. 
    // So, if the picture you loaded is 500x1000 and the SCALE_FACTOR is 5, the canvas size would be 100x200. 
    const int SCALE_FACTOR = 5;
    const double X_VELOCITY = 0.11;
    const double Y_VELOCITY = 0.11;

    // Properties
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int pictureWidth = 0;
    int pictureHeight = 0;

    SDL_Surface* imageSurface = NULL;
    SDL_Texture* imageTexture = NULL;

    SDL_Texture* canvasTexture = NULL;
    SDL_Texture* lastCanvasTexture = NULL;

    int canvasWidth = 0;
    int canvasHeight = 0;

    int windowWidth = 0;
    int windowHeight = 0;

    // Methods
    void init(SDL_WindowFlags windowFlags);
    bool getNextPosition(Uint32 t, SDL_Point &point);

public:
    Kaleidoscope();
    ~Kaleidoscope();

    bool loadImage();

    bool render();
    bool handleEvents();

};




