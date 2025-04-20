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
float lastUpdateTime = 0;


//Game variables
int score = 0;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

bool EventTriggered(float timePassed)
{
    float currentTime = GetTime();
    
    if(currentTime - lastUpdateTime >= timePassed)
    {
        lastUpdateTime = GetTime();
        return true;
    }
    return false;
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
        Vector2 direction = {1, 0};

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
        private:
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

        Vector2 GetPosition()
        {
            return position;
        }

        void SetPositionX(float value)
        {
            position.x = value;
        }

        void SetPositionY(float value)
        {
            position.y = value;
        }

        void SetPosition(Vector2 value)
        {
            position = value;
        }
    };

    class Game
    {
        public:

        Food food = Food();
        Snake snake = Snake();

        void Draw()
        {
            food.Draw();
            snake.Draw();
        }

        void Update()
        {
            snake.Update();
        }

        void CheckCollisionWithFood()
        {
            if(Vector2Equals(snake.body[0], food.GetPosition()))
            {
                food.SetPosition(food.GenerateRandomPos());
                score++;
            }
        }
    };

    Game game;



    //Setting the window refresh rate to 60 frames per second.
    SetTargetFPS(60);

    //Game loop
    while(!WindowShouldClose())
    {

        //Begin screen drawing
        BeginDrawing();
        ClearBackground(green);

        //Event handling
        /* std::cout << GetTime() << std::endl; */
         

        //Update positions
        if(EventTriggered(0.2f))
        {
            game.Update();
        } 
        
        if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0,-1};
            std::cout << "KEY PRESSED";
        }
        if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0,1};
            std::cout << "KEY PRESSED";
        }
        if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1,0};
            std::cout << "KEY PRESSED";
        }
        if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1,0};
            std::cout << "KEY PRESSED";
        }

        //Check collisions
        game.CheckCollisionWithFood();


        //Drawing
        DrawText(TextFormat("%i", score), screen_width/2, screen_heigth/10, 50, darkGreen);

        game.Draw();

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