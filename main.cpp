#include <raylib.h>
#include <raymath.h>
#include <deque>
#include <iostream>


const int screen_width = 750;
const int screen_heigth = 750;

// creating a grid to "simplify" the game, making it like a retro snake.
//Also i don't want to deal with the whole plane, its just easier this way.
float cellSize = 30.0f;
float cellCount = 25.0f;

bool renderGrid = false;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

void await(float time)
{
    int timeDifference = GetTime();
    bool goOut = false;

    while(goOut = false)
    {
        float b = GetTime();

        if(b - timeDifference >= time)
        {
            timeDifference = GetTime();
            goOut = true;
        }
    }
}



int main()
{
    //Initializing the window
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Raylib Snake Game");

    class Snake
    {
        public:
        //Defining the starting body of the snake, using deque so we can operate on both the start and the end
        //of the snake.
        std::deque<Vector2> body = {Vector2{5,9}, Vector2{6,9}, Vector2{7,9}};
        Vector2 direction = {0, 0};

        void Draw()
        {
            //Cycling through the body to draw each cell.
            for(unsigned int i = 0; i < body.size(); i++)
            {
                //Defining a rectangle to use inside the DrawRectangleRounded function.
                Rectangle rectangle = Rectangle(body[i].x * cellSize, body[i].y * cellSize, cellSize, cellSize);
                DrawRectangleRounded(rectangle, 0.5, 1, darkGreen);
            }
        }

        void Update()
        {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }

    };

    class Food
    {
        //Vector2 position, variable used for the position of the object, since  working on a 2d space
        //Using a Vector2 which contains an X and Y coord.
        Vector2 position;

        //Drawing a texture to represent the Food object instead of using a square.
        Texture2D texture;

        public:

        Food()
        {
            //Loading the image from the resources into an Image variable
            //Image image = LoadImage("graphics/food.png");
            //Creating a texture from the loaded image
            //texture = LoadTextureFromImage(image);
            //Unloading the image from memory to save resources
            //UnloadImage(image);

            //The code above is unnecessary since now there's a more convenient way of loading textures.
            //This basically does the same thing, you just dont have to remember to unload the image.
            texture = LoadTexture("graphics/food.png");
            //Generating a random position for the object
            position = GenerateRandomPos();
        }

        //Deconstructor used to free resourced that the object used during his life time.
        ~Food()
        {
            UnloadTexture(texture);
        }

        void Draw()
        {
            //DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, BLACK);
            //DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);

            //Using DrawTexturePro to stretch the texture to the grid size since it's lower in pixel size(texture)
            DrawTexturePro(texture,
                           Rectangle{0, 0, (float)texture.width, (float)texture.height},
                           Rectangle{position.x * cellSize, position.y * cellSize, cellSize, cellSize},
                           Vector2{0,0}, 0, WHITE);
        }

        //Function used to generate a random position for the Object, by randomizing X and Y.
        Vector2 GenerateRandomPos()
        {
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x,y};
        }
    };

    Food food;

    Snake snake;



    //Setting the window refresh rate to 60 frames per second.
    SetTargetFPS(60);

    //Game loop
    while(!WindowShouldClose())
    {

        //Begin screen drawing
        BeginDrawing();
        ClearBackground(green);

        //Event handling
        std::cout << GetTime() << std::endl;
         

        //Update positiong / Collisions 
        snake.Update();


        //Drawing
        food.Draw();   
        snake.Draw();
        if(renderGrid == true)
        {
            for(int i = 0; i < cellCount; i++)
            {
                DrawLine(0, i * cellSize, cellCount * cellSize, i * cellSize, BLACK);
                DrawLine(i * cellSize, 0, i * cellSize, cellCount * cellSize, BLACK);
            }
        }   
        //End screen drawing
        EndDrawing();
    }

    //Closing the window
    CloseWindow();

    return 0;
}