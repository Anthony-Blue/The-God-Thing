#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>       
#include "stb_image.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include <stdexcept>
#include <vector>
#include <random>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <algorithm>
#include <functional>
bool running, fullscreen;
float placeWidth, placeHeight;
int scale = 3;
bool Game_End_I_Want_A_Long_Variable_Name = false;
float WhiskerEncounterBefore = 0;
float domerTimer = 0;
float Wheight = 180 * scale;
float selectKey = 0;
        Mix_Chunk* beep;
        Mix_Chunk* bloop;
        Mix_Chunk* balls;
        Mix_Chunk* deathSound;
        Mix_Chunk* hit;
float KeyMove = false;
bool monkey = false;
float moveCoolDown = 1;
float Wlength = 270 * scale;
bool record = false;
bool title = true;
bool DeathTitleToggle = false;
bool death = false; 
bool Shop1 = false;
bool Shop3 = false;
float tutorial = 0;
int shopKey = 0;
bool TerminalEx_Answer = true;
bool pause = false;
float tutorialTarget = 0;
bool pauseScreen = false;
bool other = true;
int Width, Height;
SDL_Window *window = nullptr;
GLuint Base;
bool titleSelect = false; 
bool Terminal = false;
float rotation = 0.0f;
GLuint shaderProgram;
float deltaTime, lastFrame = 0.0f;
float speed = 150;
float playerX = 16000;
float playerY = 100;
float animationTimer = 0.0f;
bool Ruby = false;
bool Gold = false;
bool Blue = false;
bool Silver = false;
bool White = false;
int totalFrames = 3;
bool facingLeft = true;
bool rolling = false;
float realFrame = 0;
bool slashing = false;
float TerminalTimer = 0; 
bool slashed = false;
int frameIndex;
bool beginning = false;
int movementTotal;
bool tutorialToggle = false;
float cameraX = 16000 - 270 * 1.5 + 24;
float cameraY = 100 - 180 * 1.5 + 16;
float Relaps = 0 ;
float SlashRelaps = 0;
bool playerHit = false;
float hitX, hitY;
float tutorialPrev = 0;
float health = 1;
float barWidth = 75;
float barHeight = 75;
float margin = 30.0f;
float focusX, focusY;
bool outsideTheBox = true;
bool facingvert = false;
bool facingdown;
int shell = 21;
bool healing;
std::string playerName = "Zelda";
float damage = 1;
float pulse = 0;
float Tutpulse = 0;
bool TextBox = true;
float weaponSize = 0;
float orbitAngle = 0.0f;
float orbitRadius = 150.0f;
float orbitSpeed = 1.75f;
float shellDamage;
float sensitivity;
float pickCoolDown = -1;
bool healthSwing = false;
bool constSwing = false;
bool deathBoost = false;
bool pick;
bool Kkey;
bool IvineLaser;
float DialogStateNext = 0;
float DialogCap = 0;
std::vector<glm::vec2> playerHistory;
const std::string fontChars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789'.,!? ";

std::mt19937 rng(std::random_device{}());

int rand(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}
    
float Lerp(float a, float b, float t){
    return a + t * (b-a);
};

struct Animation
{
    GLuint texture;
    int totalFrames;
    float frameDuration;
};

struct Sentence{
    std::string sentence;
    float placeX;
    float placeY;
    float size;
    bool exception;
};

struct Gem {
    float r;
    float g;
    float b;
    std::string type;
    float life;
    bool still;
};

struct Item {
    float x;
    float y;
    GLuint texture;
    std::string type;
    bool expire;


};




Animation walk;
Animation walkdown;
Animation walkup;
Animation heal;
Animation roll;
Animation slash;
Animation flyDown;
Animation background;
Animation pjta;
Animation pkti;
Animation transparent;
Animation slashdown;
Animation StewMass;
Animation BudelariumDominicus;
Animation Whiskers;
Animation Cigarette;
Animation sign;
Animation house;
Animation trapHouse;
Animation Tree;
Animation train;
Animation Shadow;
Animation slashup;
Animation vinceWalk;
Animation vinceLunge;
Animation flyDownLeft;
Animation cocoon;
Animation titleTex;
Animation selectOpt;
Animation Shop1Tex;
Animation Shop3Tex;
Animation shop2texture;
Animation end_credit;
Animation laser;
Animation flyUp;
Animation enterLeftTexture;
Animation Obelisk;
Animation Marcus;
Animation flySide;
Animation ballAMN;
Animation domerOp;
Animation flyUpRight;
Animation chara;
Animation* player;

struct GameObject
{
    float x;
    float y;
    float hitboxWidth = 32;
    float hitboxHeight = 32;
    Animation* animation;
    int frameIndex;
    float animationTimer;
    bool facingLeft;
    float relaps;
    float health;
    bool alive;
    float knockback;
    std::string name;
    float height;
    float offsetX;
    float offsetY;
    bool up;
};


bool compareByLength(const GameObject &a, const GameObject &b)
{
    return a.y > b.y;
}

struct Particle {
    float x, y;
    float vx, vy;
    float life;      
    float maxLife;   
    float size;
};

void drawPixel(float x, float y, float size, GLuint texture, GLint modelLoc, GLint useTextureLoc, GLint colorLoc, float index, float steps)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(x - ((-index + steps) / steps) * 4, y - ((-index + steps) / steps) * 4, 0.0f));
    model = glm::scale(model, glm::vec3(size + ((-index + steps) / steps) * 8, size + ((-index + steps) / steps) * 8, 1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(useTextureLoc, 0);
        glUniform4f(colorLoc, 0.8f, 0.05f, 0.05f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

 
    
}

struct Wall {
    float hitboxX;
    float hitboxY;
    float x;
    float y;
    bool facingLeft;
    GLuint texture;
};

struct Box
{
    float x;
    float y;
    float sizeX;
    float sizeY;

    float r;
    float g;
    float b;
};

struct Pledge {
    std::string name;
    std::string type;
    std::string info;
};

std::vector<GameObject> objects;
std::vector<GameObject> backgroundObjects;
std::vector<Wall> Walls;
std::vector<Gem> currentGems;
std::vector<Box> groundBoxes;
std::vector<Sentence> Dialoge;
std::vector<Particle> particles;
std::vector<Item> Items;
std::vector<GameObject> burns;
std::vector<Pledge> inventory;
    GameObject playerObj;
    GameObject cocoonShell;
    GameObject vince;
    GameObject domer;
    GameObject ivine;

void spawnHitParticles(float x, float y)
{
    for (int i = 0; i < 12; i++)
    {
        float angle = rand(0.0f, 2.0f * M_PI);
        float speed = rand(50.0f, 150.0f);

        Particle p;
        p.x = x;
        p.y = y;
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;
        p.life = 0.3f;
        p.maxLife = 0.3f;
        p.size = rand(3.0f, 6.0f);

        particles.push_back(p);
    }
}

void spawnDeathParticles(float x, float y)
{
    for (int i = 0; i < 20; i++)
    {
        float angle = rand(0.0f, 2.0f * M_PI) + rand(0, 45);
        float speed = rand(100.0f, 200.0f);

        Particle p;
        p.x = x;
        p.y = y;
        p.vx = cos(angle) * speed;
        p.vy = sin(angle) * speed;
        p.life = 0.6f;
        p.maxLife = 0.6f;
        p.size = rand(4.0f, 8.0f);

        particles.push_back(p);
    }
}

void enemyRand(int seed, float x, float y, GameObject enemy1, GameObject enemy2, GameObject enemy3) {
    if (seed == 1) {
        enemy1.x = x;
        enemy1.y = y;
        objects.push_back(enemy1);
    }
    if (seed == 2) {
        enemy1.x = x - 150;
        enemy1.y = y + 200;
        objects.push_back(enemy1);
        enemy2.x = x + 150;
        enemy2.y = y + 200;
        objects.push_back(enemy2);
    }
        if (seed == 3) {
        enemy2.x = x;
        enemy2.y = y;
        objects.push_back(enemy2);
        enemy3.x = x + 150;
        enemy3.y = y + 200;
        objects.push_back(enemy3);
    }

        if (seed == 4) {
        enemy2.x = x;
        enemy2.y = y;
        objects.push_back(enemy2);
        enemy3.x = x - 150;
        enemy3.y = y + 200;
        objects.push_back(enemy3);
    }

    if (seed == 5) {
        enemy3.x = x;
        enemy3.y = y;
        objects.push_back(enemy3);
        enemy3.x = x - 150;
        enemy3.y = y + 200;
        objects.push_back(enemy3);
        enemy3.x = x + 150;
        enemy3.y = y + 200;
        objects.push_back(enemy3);
    }

    if (seed == 6) {
        enemy1.x = x;
        enemy1.y = y;
        objects.push_back(enemy1);
        enemy2.x = x - 150;
        enemy2.y = y + 200;
        objects.push_back(enemy2);
        enemy3.x = x + 150;
        enemy3.y = y + 200;
        objects.push_back(enemy3);
    }

    if (seed == 7) {
        enemy1.x = x;
        enemy1.y = y;
        objects.push_back(enemy1);
        enemy2.x = x - 150;
        enemy2.y = y + 200;
        objects.push_back(enemy2);
        enemy2.x = x + 150;
        enemy2.y = y + 200;
        objects.push_back(enemy2);
    }
};


bool Collision(const GameObject& a, const GameObject& b)
{
    return
        a.x < b.x + b.hitboxWidth &&
        a.x + a.hitboxWidth > b.x &&
        a.y < b.y + b.hitboxHeight &&
        a.y + a.hitboxHeight > b.y;
}

bool wallCollision(float x1, float y1, float w1, float h1,
                    float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

bool InBox(const GameObject& a, const Box& b)
{
    return
        a.x < b.x + b.sizeX &&
        a.x + a.hitboxWidth > b.x &&
        a.y < b.y + b.sizeY &&
        a.y + a.hitboxHeight > b.y;
}

GLuint loadTexture(const char* path)
{
    int w, h, channels;
        stbi_set_flip_vertically_on_load(true);
    unsigned char* data =
        stbi_load(path, &w, &h, &channels, 4);

    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);
        glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return tex;
}

float randHash(int x, int y)
{
    int n = x * 374761393 + y * 668265263;
    n = (n ^ (n >> 13)) * 1274126177;
    return (n & 0x7fffffff) / (float)0x7fffffff;
}

std::string loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}




GLuint compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
    }

    return shader;
} 


GLuint createProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = loadFile(vertexPath);
    std::string fragmentSource = loadFile(fragmentPath);

    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}





void update(){

                        auto Playss = std::find_if(objects.begin(), objects.end(), 
                    [](const GameObject& playera) {
                        return playera.name == playerName;
                    });
    IvineLaser = false;
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(fullscreen) {
        scale = 6;
        Wheight = 180 * scale;
        Wlength = 270 * scale;
        placeWidth = Width / 2 - Wlength / 2;
        placeHeight = Height / 2 - Wheight / 2;
    } else {
        scale = 3;
        Wheight = 180 * scale;
        Wlength = 270 * scale;
        placeWidth = placeHeight = 0;
    }
    
    realFrame += deltaTime;

    for (int i = 0; i < (int)particles.size(); i++)
{
    Particle& p = particles[i];

    p.life -= deltaTime;

    p.x += p.vx * deltaTime;
    p.y += p.vy * deltaTime;
    
    p.vy -= 300.0f * deltaTime;

    p.vx *= 0.98f;
    p.vy *= 0.98f;

    if (p.life <= 0.0f)
    {
        particles.erase(particles.begin() + i);
        i--;
    }
}    
    
    if (!TextBox) {
                orbitAngle += orbitSpeed * deltaTime;
            }
    glViewport(placeWidth, placeHeight, Wlength, Wheight);
    for(auto& obj : objects)
{

    if (title && TerminalTimer > 20) {
        Mix_ResumeMusic(); 
    }
    if (realFrame * 60 >= 1) { 
        domerTimer += 0.6;
        realFrame = 0;

        if (!Kkey){
        pickCoolDown--;
        }
        if (!monkey){
        moveCoolDown--;
        }
        TerminalTimer += 0.5;
        if (!TextBox) {
        playerHistory.push_back(glm::vec2(playerX, playerY));
        if (playerHistory.size() > 32) playerHistory.erase(playerHistory.begin());
        }
    }
    obj.animationTimer += deltaTime;
                float Espeed;
    if (obj.name == "ball") {
        if (obj.animationTimer >= obj.animation->frameDuration && !TextBox) {
        obj.animationTimer = 0;
        obj.relaps--;
        }
        obj.frameIndex = -1;

        if (obj.relaps == 0) {
            obj.alive = false;
        } else {
            if (!TextBox) {
            obj.x += obj.offsetX * 4.5 * deltaTime * 50;
            obj.y += obj.offsetY * 4.5 * deltaTime * 50;
            }
        }

        if (wallCollision(obj.x, obj.y, 24, 24, playerX + 9, playerY + 4, 12, 4 ) && !playerHit) {
                float dx, dy;
            dx = playerX - (obj.x + 12);
            dy = playerY - (obj.y + 12);

                        float length = sqrt(dx * dx + dy * dy);
                
                if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }

                    playerHit = true;
                    hitX = dx;
                    hitY = dy;
                    health -= obj.health * sensitivity;
                    if (outsideTheBox) {
                        health = 0;
                    }
                }
    }
    if (obj.name == "domer") {
    float dx, dy;

                    float JohnX = playerX + focusX * 24;
            float JohnY = playerY + focusY * 24;
    
            dx = JohnX - (obj.x + 16);
            dy = JohnY - (obj.y + 16);
            float Objlength = sqrt(dx * dx + dy * dy);
            dx = playerX - (obj.x + 16);
            dy = playerY - (obj.y + 16);
                float length = sqrt(dx * dx + dy * dy);

            if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }
    
            if (Objlength < 64 && Objlength < length && slashed) {
                slashed = false;
                obj.knockback = 7.5;
                obj.health -= damage;
                    Mix_PlayChannel(-1, hit, 0);
                                if (currentGems.size() != 0) {
                for (auto& gem : currentGems) {
                    gem.life -= 1;
                };
                }
                std::cout << obj.health << "\n";
                obj.hitboxWidth = 24;
                obj.hitboxHeight = 32;
                obj.relaps = 15;
                spawnHitParticles(obj.x + 12, obj.y + 16);
            }
            
        
        
        
            float hitBackX = 0, hitBackY = 0;   
            
            if  (obj.knockback > 0) {
                hitBackX -= dx * obj.knockback * 100 * deltaTime;
                hitBackY -= dy * obj.knockback * 100 * deltaTime;
                obj.knockback -= 30 * deltaTime;
                
            } else {
                obj.knockback = 0;
            }
            
    
            if (obj.health <= 0){
                obj.alive = false;
                if (deathBoost) {
                    health += 100;
                }
                spawnDeathParticles(obj.x + 12, obj.y + 16);
            }


        

                    if (((wallCollision(obj.x, obj.y, 64, 64, playerX + 9, playerY + 4, 12, 4 )&& !playerHit) && !playerHit)) {
                    playerHit = true;
                    hitX = dx;
                    hitY = dy;
                    health -= obj.health * sensitivity;
                    if (outsideTheBox) {
                        health = 0;
                    }
                }

        

        float angle = atan2(dy, dx);
if (obj.relaps > -40 && obj.relaps < 3) {
    dy = sin(angle + (obj.relaps / 5) + 0.2);
    dx = cos(angle + (obj.relaps / 5) + 0.2);
    angle = atan2(dy, dx);
}


        
float degrees = angle * (180.0f / 3.14159265f);

if (degrees < 0)
    degrees += 360.0f;

        obj.offsetY = 0;
        obj.offsetX = 0;

if (degrees >= 337.5f || degrees < 22.5f) {
obj.frameIndex = 5;
    obj.animationTimer = 0;
}
else if (degrees < 67.5f) {
obj.frameIndex = 4;
    obj.animationTimer = 0;
}
else if (degrees < 112.5f) {
obj.frameIndex = 3;
    obj.animationTimer = 0;
}
else if (degrees < 157.5f) {
obj.frameIndex = 2;
    obj.animationTimer = 0;
}
else if (degrees < 202.5f) {
obj.frameIndex = 1;
    obj.animationTimer = 0;
}
else if (degrees < 247.5f) {
obj.frameIndex = 0;
    obj.animationTimer = 0;
}
else if (degrees < 292.5f) {
obj.frameIndex = -1;
    obj.animationTimer = 0;
}
else {
obj.frameIndex = 6;
obj.animationTimer = 0;
}

        if (obj.relaps == -40) {
            obj.relaps = 90;
        }
        float prodx = playerX - 12 - obj.x;
        float prody = playerY - 32 - obj.y; 
        
        float prolength = sqrt(prodx * prodx + prody * prody);

        if (domerTimer >= 1 && !TextBox) {
            if (obj.name == "domer") obj.relaps--;
        }



        
        if (!TextBox) {

        if (prolength < 400) {
            if (obj.relaps > 20){
                if (length < 250) {
                    obj.x += dx * (-length / 3) * deltaTime + hitBackX;
                    obj.y += dy * (-length / 3) * deltaTime + hitBackY;
        
                } else if (length > 300) {
                    obj.x += dx * (75) * deltaTime + hitBackX;
                    obj.y += dy * (75) * deltaTime + hitBackY;
                } else {
                    obj.x += hitBackX;
                    obj.y += hitBackY;
                }
            } else if (obj.relaps > -40 && obj.relaps < 0 && (int)obj.relaps % 2 == 0) {
                obj.relaps--;
                GameObject ball;
                ball.x = obj.x + 8;
                ball.y = obj.y + 8;
                ball.hitboxWidth = 24;
                ball.hitboxHeight = 24;
                ball.animation = &ballAMN;
                ball.frameIndex = 0;
                ball.animationTimer = 0.0f;
                ball.facingLeft = true;
                ball.relaps = 20.0f;
                ball.health = 5;
                ball.alive = true;
                ball.knockback = 0;
                ball.offsetX = dx;
                ball.offsetY = dy;
                ball.name = "ball";
                objects.push_back(ball);
                    obj.x += hitBackX;
                    obj.y += hitBackY;
            } else {
                    obj.x += hitBackX;
                    obj.y += hitBackY;
                }
        
        } else {
            obj.relaps = 90;
        }
}
    }
    obj.up = false;
    if (obj.name == "ivine") {
        if (IvineLaser) {
            if (obj.relaps > 0){
            obj.relaps = 10;
            } else {
            obj.relaps = -1;      
            }
        }
        float dx, dy;
        if (obj.relaps < 85 && obj.relaps > 0) {
            float objectX = playerX - 12 - 40 + cos(orbitAngle) * orbitRadius;
            float objectY = playerY + sin(orbitAngle) * orbitRadius;
            dx = objectX - obj.x;
            dy = objectY - obj.y;
            
            obj.animation->frameDuration = 0.092;
                        if (obj.height < 30.0) {
                obj.height += deltaTime * 30;
                obj.y += deltaTime * 30;
                    }
        } else if (obj.relaps < -20){
            dx = playerHistory[0].x - 12 - obj.x;
            dy = playerHistory[0].y - 32 - obj.y;  
            IvineLaser = true;
            obj.animation->frameDuration = 0.092;


        } else {
            dx = playerX - 12 - obj.x;
            dy = playerY - 32 - obj.y;

            float length = sqrt(dx * dx + dy * dy);
                
                if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }

            obj.animation->frameDuration = 0.080;
            if (obj.height > 0 && obj.relaps > 0) {
                obj.height -= deltaTime * 12;
                obj.y -= deltaTime * 12;


                
            } else if (!(obj.relaps > 0)){
                dx = playerHistory[0].x - 12 - obj.x;
                dy = playerHistory[0].y - 32 - obj.y;

            } 

            if (obj.relaps > 0) {
                       
                obj.x += 16;
                obj.y += 20;

    
        
                if ((Collision(*Playss, obj) && !playerHit)) {
                    playerHit = true;
                    hitX = dx;
                    hitY = dy;
                    health -= obj.health * sensitivity;
                    if (outsideTheBox) {
                        health = 0;
                    }
                }
                    

                
                GameObject& playerObject = *Playss;
                        
                dx = playerObject.x - 12 - obj.x;
                dy = playerObject.y - 32 - obj.y; 
                float length = sqrt(dx * dx + dy * dy);
                
                playerObject.x += focusX * 34;
                playerObject.y += focusY * 34;
        
                dx = playerObject.x - 12 - obj.x;
                dy = playerObject.y - 32 - obj.y; 
                float Objlength = sqrt(dx * dx + dy * dy);
                if (Objlength != 0)
                {
                    dx /= Objlength;
                    dy /= Objlength;
                }
                
            if (Objlength < 64 && Objlength < length && slashed) {
                slashed = false;
                obj.knockback = 7.5;
                obj.health -= damage;
                    Mix_PlayChannel(-1, hit, 0);
                if (currentGems.size() != 0) {
                for (auto& gem : currentGems) {
                    gem.life -= 1;
                };
                }
                std::cout << obj.health << "\n";
                spawnHitParticles(obj.x + 16, obj.y + 42);
            }
                        
                obj.x -= 16;
                obj.y -= 20;
            

            if (obj.health <= 0){
                obj.alive = false;
                if (deathBoost) {
                    health += 100;
                }
                spawnDeathParticles(obj.x + 16, obj.y + 50);
            }
                }   

            
        }


            if  (obj.knockback > 0) {
                obj.x -= dx * obj.knockback * 100 * deltaTime;
                obj.y -= dy * obj.knockback * 100 * deltaTime;
                obj.knockback -= 30 * deltaTime;
                
            } else {
                obj.knockback = 0;
  
    
            }

            if (obj.relaps > 85) {
                dx = playerHistory[0].x - 12 - obj.x;
                dy = playerHistory[0].y - 32 - obj.y;
            }

        
        float length = sqrt(dx * dx + dy * dy);
                
                if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }

        
float angle = atan2(dy, dx);


                
float degrees = angle * (180.0f / 3.14159265f);

if (!TextBox) {
if (degrees < 0)
    degrees += 360.0f;

        obj.offsetY = 0;
        obj.offsetX = 0;

if (degrees >= 337.5f || degrees < 22.5f) {
    obj.animation = &flySide;
    obj.offsetX = 27;
    obj.offsetY = 12;
    obj.facingLeft = false;
    obj.up = false;
}
else if (degrees < 67.5f) {
    obj.animation = &flyUpRight;
    obj.facingLeft = false;
    obj.offsetX = 24;
    obj.offsetY = 23;
    obj.up = true;
}
else if (degrees < 112.5f) {
    obj.animation = &flyUp;
    obj.offsetX = -3;
    obj.offsetY = 27;
    obj.facingLeft = true;
    
    obj.up = true;
}
else if (degrees < 157.5f) {
    obj.animation = &flyUpRight;
    obj.offsetX = -17;
    obj.offsetY = 19;
    obj.up = true;
    obj.facingLeft = true;
}
else if (degrees < 202.5f) {
    obj.animation = &flySide;
    obj.offsetX = -18;
    obj.offsetY = 9;
    obj.facingLeft = true;
    obj.up = false;
}
else if (degrees < 247.5f) {
    obj.animation = &flyDownLeft;
    obj.offsetX = -6;
    obj.facingLeft = true;
    obj.up = false;
}
else if (degrees < 292.5f) {
    obj.animation = &flyDown;
    obj.offsetX = -3;
    obj.offsetY = 0;
    obj.facingLeft = true;
    obj.up = false;
}
else {
    obj.animation = &flyDownLeft;
    obj.offsetX = 15;
    obj.facingLeft = false;
    obj.up = false;
}

}
        float prodx = playerX - 12 - obj.x;
        float prody = playerY - 32 - obj.y; 
        
        float prolength = sqrt(prodx * prodx + prody * prody);
                
if (!TextBox) {

        if (prolength < 400) {
        obj.offsetY += (-obj.frameIndex - 1) * 3;
        if (obj.frameIndex == 4) {
            obj.offsetY += 10;
        }
        if (obj.relaps > 0 && obj.relaps < 85) {
            obj.x += dx * (75 + 100) * deltaTime;
            obj.y += dy * (75 + 100) * deltaTime;
        } else if (length < 100) {
            if (obj.height > 0) {
                obj.x += dx * (-75) * deltaTime;
                obj.y += dy * (-75) * deltaTime;
            }
        } else if (length > 150) {
            obj.x += dx * (75) * deltaTime;
            obj.y += dy * (75) * deltaTime;
        }
        
        } else {
            obj.relaps = 85;
        }
    
}
    }





    
    if (obj.animation == &vinceWalk || obj.animation == &vinceLunge) {
                float dx = playerX - obj.x;
                float dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
                
                if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }
            
            GameObject& playerObject = *Playss;
           
            obj.x += 0;
            playerObject.x += 4;
            if (wallCollision(obj.x + 9, obj.y + 4, 12, 4, playerX + 9, playerY + 4, 12, 4 )&& !playerHit) {
                playerHit = true;
                hitX = dx;
                hitY = dy;
            
                std::cout << health << "\n";
                health -= obj.health * sensitivity;
                if (outsideTheBox) {
                    health = 0;
                }
            if (obj.animation == &vinceLunge) {
                obj.hitboxWidth = 24;
                obj.hitboxHeight = 32;
                obj.relaps = 15;
                obj.animation = &vinceWalk;
                Espeed = 0;
                obj.frameIndex = -1;
                obj.animationTimer = 0;
                }
            } 
            float JohnX = playerObject.x + focusX * 24;
            float JohnY = playerObject.y + focusY * 24;
    
            dx = JohnX - obj.x;
            dy = JohnY - obj.y;
            float Objlength = sqrt(dx * dx + dy * dy);
            dx = playerX - obj.x;
            dy = playerY - obj.y;
    
            
            if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                }
    
            if (Objlength < 32 && Objlength < length && slashed) {
                slashed = false;
                obj.knockback = 7.5;
                obj.health -= damage;
                
                    Mix_PlayChannel(-1, hit, 0);
                                if (currentGems.size() != 0) {
                for (auto& gem : currentGems) {
                    gem.life -= 1;
                };
                }
                std::cout << obj.health << "\n";
                obj.hitboxWidth = 24;
                obj.hitboxHeight = 32;
                obj.relaps = 15;
                spawnHitParticles(obj.x + 12, obj.y + 16);
                obj.animation = &vinceWalk;
                Espeed = 0;
                obj.frameIndex = -1;
                obj.animationTimer = 0;
            }
            
        
        
        
            float hitBackX = 0, hitBackY = 0;   
            
            if  (obj.knockback > 0) {
                //object move plp
                hitBackX -= dx * obj.knockback * 100 * deltaTime;
                hitBackY -= dy * obj.knockback * 100 * deltaTime;
                obj.knockback -= 30 * deltaTime;
                
            } else {
                obj.knockback = 0;
            }
            
    
            if (obj.health <= 0){
                obj.alive = false;
                if (deathBoost) {
                    health += 100;
                }
                spawnDeathParticles(obj.x + 12, obj.y + 16);
            }
    
                
            obj.x -= 0;
    
                
            
            if (length > 400) {
                obj.frameIndex = -1;
                obj.animationTimer = 0;
    
            } else {
    
                if (length < 200){
    
                    if (obj.relaps < 0) {
                        if (obj.animation == &vinceWalk) {
                            obj.animationTimer = 0;
                            obj.frameIndex = -1;
                            obj.animation = &vinceLunge;
                            obj.hitboxWidth = 32;
                            obj.hitboxHeight = 32;
                        }
    
                    }
    
                } 
                
                Espeed = 0;
                
                if (obj.animation == &vinceLunge) Espeed = 150;
    if (!TextBox) {
                //object move plp
                obj.x += dx * (75 + Espeed) * deltaTime + hitBackX;
                obj.y += dy * (75 + Espeed) * deltaTime + hitBackY;
            
}
        
        for (auto& wall : Walls)
        {
            if (wallCollision(obj.x + 9, obj.y + 4, 12, 4,
                               wall.x, wall.y, wall.hitboxX, wall.hitboxY))
            {
                obj.x -= dx * (75 + Espeed) * deltaTime + hitBackX;
            }
        }
        
        
        for (auto& wall : Walls)
        {
            if (wallCollision(obj.x + 9, obj.y + 4, 12, 4,
                               wall.x, wall.y, wall.hitboxX, wall.hitboxY))
            {
                obj.y -= dy * (75 + Espeed) * deltaTime + hitBackY;
            }
        }

                if (dx > 0) {
                    obj.facingLeft = false;
                } else {
                    obj.facingLeft = true;
                }
        }
        
    }


    if(obj.animationTimer >= obj.animation->frameDuration)
    {
        obj.animationTimer -= obj.animation->frameDuration;


        if (!TextBox) {
        obj.frameIndex++;
        Relaps--;
        SlashRelaps--;
        obj.relaps--;
        }


        if(obj.frameIndex >= obj.animation->totalFrames)
        {
            obj.frameIndex = 0;
            if (obj.animation == &vinceLunge){
                obj.relaps = 15;
                obj.animation = &vinceWalk;
                Espeed = 0;
                obj.hitboxWidth = 24;
                obj.hitboxHeight = 32;

            }
            
        if (obj.name == "ivine" && obj.relaps < -80){
            obj.relaps = 185;
        }


            
            if(rolling && obj.animation == &roll)
            {
                rolling = false;
                obj.animation = &walk;
                speed = 150;
                Relaps = 5;
            }
            
            if(healing && obj.health == 2000000000)
                {
                    healing = false;
                    obj.animation = &walk;
                    if (health > 80){
                        health = 80;
                    }
                    health += 20;
                    speed = 150;
                    Relaps = 3;
                }
            
            if(slashing && obj.health == 2000000000)
            {
                slashing = false;
                slashed = false;
                obj.animation = &walk;
                speed = 150;
                SlashRelaps = -1;
            }
        }
    }

    
    
}

    
    if (health <= 0){
        
        health = 0;
        other = true;
if (!death) {
        DeathTitleToggle = false;
}
        TextBox = true;
        death = true;
    }

    objects.erase(
    std::remove_if(objects.begin(), objects.end(),
        [](const GameObject& obj)
        {
            return !obj.alive;
        }),
    objects.end());

    backgroundObjects.erase(
    std::remove_if(backgroundObjects.begin(), backgroundObjects.end(),
        [](const GameObject& obj)
        {
            return !obj.alive;
        }),
    backgroundObjects.end());
    

    Items.erase(
    std::remove_if(Items.begin(), Items.end(),
        [](const Item& i)
        {
            return i.expire;
        }),
        
    Items.end());

    currentGems.erase(
    std::remove_if(currentGems.begin(), currentGems.end(),
        [](const Gem& i)
        {
            return i.still;
        }),
    currentGems.end());

if (playerY > 590.0f && !beginning) {
    beginning = !beginning;
    playerX = 0;
    playerY = -800;
    cameraX = -400;
    if (false) {
    playerX = 1850;
    playerY = 4800;
    tutorialPrev = 2;
    }
    cameraY = -1170;
}

        if (playerY >= -400 && tutorialPrev == 0 && beginning) {
            tutorialToggle = false;
                tutorial = 1;
                tutorialPrev = 1;
                TextBox = true;
                other = true;    
                Sentence sen;
                sen.sentence = "When outside of a box your health";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "becomes fragile, causing any slight";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "attack to kill you instantly.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "Anytime spent outside of the box";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "will cause your health bar to blink.";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
        }
    
        if (domerTimer >= 1) {
            domerTimer = 0;
        }
    


if (playerY >= 150 && tutorialPrev == 1 && beginning) {
                tutorial = 2;
            tutorialToggle = false;
                tutorialPrev = 2;
                TextBox = true;
                other = true;    
                Sentence sen;
                sen.sentence = "When you see a colored shape on the";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "ground press K to pick it up.";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "These little items are called Gems.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "These Gems will give you special ";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "effects that make combat easier.";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
        }
    if (tutorialTarget == 4) {
                tutorialTarget = 0;
                tutorialToggle = false;
                tutorial = 4;
                TextBox = true;
                other = true;    
                Sentence sen;
                sen.sentence = "The Red Gem you can find on the";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "ground deals extra damage for each";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "hit.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "Each swing heals 10 HP.";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
}
if (tutorialTarget == 5) {
                tutorialTarget = 0;
                tutorial = 5;
                tutorialToggle = false;
                TextBox = true;
                other = true;    
                Sentence sen;
                sen.sentence = "";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "The Gold Gem lets you attack";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "outside of the box.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "You'll also be unable to heal.";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "  ";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
}
    if (tutorialTarget == 6) {
                tutorialTarget = 0;
                tutorial = 6;
                tutorialToggle = false;
                TextBox = true;
                other = true;    
                Sentence sen;
                sen.sentence = "The Blue Gem deals 3 times ";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "damage with each swing.";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "you also subsequently take 3";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "times as much damage.";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
}
    if (tutorialTarget == 7) {
                tutorialTarget = 0;
                tutorial = 7;
                TextBox = true;
                tutorialToggle = false;
                other = true;    
                Sentence sen;
                sen.sentence = "";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "The Silver Gem deals extra damge";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "at no aditional cost.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "  ";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = " ";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
}
    if (tutorialTarget == 8) {
                tutorialTarget = 0;
                tutorial = 8;
                TextBox = true;
                other = true;    
                tutorialToggle = false;
                Sentence sen;
                sen.sentence = "The Pure Gem deals maximum damage";
                sen.placeX = 225;
                sen.placeY = 236;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "and instantly kills any foe you.";
                sen.placeX = 225;
                sen.placeY = 218;
                Dialoge.push_back(sen);
                sen.sentence = "attack it with.";
                sen.placeX = 225;
                sen.placeY = 200;
                Dialoge.push_back(sen);
                sen.sentence = "Downside is that it breaks on ";
                sen.placeX = 225;
                sen.placeY = 146;
                Dialoge.push_back(sen);
                sen.sentence = "impact.";
                sen.placeX = 225;
                sen.placeY = 128;
                Dialoge.push_back(sen);
                sen.sentence = "Press K to close";
                sen.placeX = 455;
                sen.placeY = 60;
                Dialoge.push_back(sen);
}
    
        if (domerTimer >= 1) {
            domerTimer = 0;
        }
    
}


void input(std::vector<GameObject>& objects){
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) running=false;
        if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) fullscreen = !fullscreen;
    }
        auto Playss = std::find_if(objects.begin(), objects.end(), 
        [](const GameObject& playera) {
            return playera.name == playerName;
        });
        GameObject& playerObject = *Playss;
        movementTotal = 0;

        float dx = 0;
        float dy = 0;

        
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(state[SDL_SCANCODE_ESCAPE] && !pause && !(TextBox && !pauseScreen)) {
            pause = true;
            pauseScreen = !pauseScreen;

            if(pauseScreen) {
                TextBox = true;
                other = true;
                Sentence sen;
                sen.sentence = "Paused";
                sen.placeX = 338;
                sen.placeY = 260;
                sen.size = 32;
                Dialoge.push_back(sen);
            } else {
                TextBox = false;
                Dialoge.erase(Dialoge.begin() + 2);
                other = false;
            }
        }



        if(!state[SDL_SCANCODE_ESCAPE]) {
            pause = false;
        }
            if (!TextBox) {

        if (state[SDL_SCANCODE_W]) {
            dy += 1;
            movementTotal += 3;
            facingvert = true;
            facingdown = false;

        }
        if (state[SDL_SCANCODE_S]) {
            dy -= 1;
            movementTotal -= 3;
            facingdown = true;
            facingvert = true;
        }
        if (state[SDL_SCANCODE_A]) {
            dx -= 1;
            facingLeft = false;
            movementTotal -= 1;
            facingvert = false;
            
        }
        if (state[SDL_SCANCODE_D]) {
            dx += 1;
            facingLeft = true;
            movementTotal += 1;
            facingvert = false;
        }
        if (false) {
            record = true;
        }
        if (state[SDL_SCANCODE_J]) {
            if (!outsideTheBox || constSwing){
                
            if (SlashRelaps < 0) {
                if (!slashing) {
                    playerObject.frameIndex = -1;
                    slashed = true;
                if (healthSwing) {
                    health += 5;
                    }
                }
                slashing = true;
                }
                } else {
            if (Relaps < 0) {
                    playerObject.frameIndex = 0;
                    healing = true;
                
                Mix_PlayChannel(-1, balls, 0);
                }
            }
        }
    pick = false;
    Kkey = false;
    if (state[SDL_SCANCODE_K]){
        if (pickCoolDown < 0) {
        pick = true;
        }
        Kkey = true;
    }
    KeyMove = false;
    monkey = false;
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_DOWN]){
        if (moveCoolDown < 0) {
        KeyMove = true;
        }
        monkey = true;
    }
        if (state[SDL_SCANCODE_SPACE]) {
            if (Relaps < 0) {
                if (!slashing && !rolling) {
                playerObject.frameIndex = 0;
                
                }
                rolling = true;
            }

        }

            } else {
                    if (state[SDL_SCANCODE_K]){
                        if (!pick) {
            pick = true;
                            if (!Terminal) {
            DialogStateNext++;
                                }
                            }
                    } else {
                        pick = false;
                    }
                                    
                
            if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_DOWN]){
                if (!KeyMove) {
                    KeyMove = true;
                    if (state[SDL_SCANCODE_UP]) {
                        selectKey++;
                    } else {
                        selectKey--;
                    }               
                }
            } else {
                KeyMove = false;
            }
                if (title) {
                    if (state[SDL_SCANCODE_SPACE]) {
                        if (!titleSelect) {
                        title = false;
                        TextBox = false;
                        other = false;
                        pickCoolDown = 1;
                        Mix_PauseMusic();
                        } else {
                            if (TerminalEx_Answer) {
                            std::cout << "Hold... \n";
                            Mix_PauseMusic();
                            TerminalEx_Answer = false;
                            }
                        }
                    }
                    if (state[SDL_SCANCODE_UP]) {
                        titleSelect = false;
                    }
                    if (state[SDL_SCANCODE_DOWN]) {
                        titleSelect = true;
                    }
                }
            }
        if (!pick) {
            tutorialToggle = true;
        }
        if (state[SDL_SCANCODE_K] && tutorial != 0 && tutorialToggle) {
            TextBox = false;
            Dialoge.erase(Dialoge.begin() + 2);
            Dialoge.erase(Dialoge.begin() + 2);
            Dialoge.erase(Dialoge.begin() + 2);
            Dialoge.erase(Dialoge.begin() + 2);
            Dialoge.erase(Dialoge.begin() + 2);
            Dialoge.erase(Dialoge.begin() + 2);
            other = false;
            tutorial = 0;
            DialogStateNext = 0;
            pickCoolDown = 1;
        }

        if (!TerminalEx_Answer && title) {
            Mix_PauseMusic();
        } else if (!TerminalEx_Answer) {
            std::cout << "- .... .   --. --- -..   - .... .. -. --. (II) (6372) \n";
            TerminalEx_Answer = true;
        }
        float length = sqrt(dx*dx + dy*dy);

        if(length != 0)
        {
            dx /= length;
            dy /= length;
        }

        if (!(dx == 0) || !(dy == 0)){
            focusX = dx;
            focusY = dy;
        }
        float smoothing = 5 * deltaTime;    
        float playerHitX = 0, playerHitY = 0;
    
        if (playerHit) {
            if (abs(hitX) + abs(hitY) < 0.05) {
                playerHit = false;
            }
            playerHitX += hitX * deltaTime * 600;
            playerHitY += hitY * deltaTime * 600;
            hitX = Lerp(hitX, 0.0f, smoothing);
            hitY = Lerp(hitY, 0.0f, smoothing);
        }
    
        playerX += dx * speed * deltaTime + playerHitX;
        
        for (auto& wall : Walls)
        {
            if (wallCollision(playerX + 9, playerY + 4, 12, 4,
                               wall.x, wall.y, wall.hitboxX, wall.hitboxY))
            {
                playerX -= dx * speed * deltaTime + playerHitX;
            }
        }
        
        playerY += dy * speed * deltaTime + playerHitY;
        
        for (auto& wall : Walls)
        {
            if (wallCollision(playerX + 9, playerY + 4, 12, 4,
                               wall.x, wall.y, wall.hitboxX, wall.hitboxY))
            {
                playerY -= dy * speed * deltaTime + playerHitY;
            }
        }

    if (beginning){
        cameraX = Lerp(cameraX, playerX + dx * 35 - 270 * 1.5 + 24, smoothing);
    }
    if (!beginning) {
        cameraY = std::clamp(Lerp(cameraY, playerY + dy * 35 - 180 * 1.5 + 16, smoothing), (float)(100.0f - 180 * 1.5 + 16), (float)(330.0f - 180 * 1.5 + 16));
    } else {
         cameraY =(Lerp(cameraY, playerY + dy * 35 - 180 * 1.5 + 16, smoothing));
    }
        if (rolling) {
            player = &roll; 
            speed = 1000;
            movementTotal = 247581;
        }

        if (slashing) {
            if (!facingvert){
                player = &slash;
            } else {
                if (facingdown) {
                    player = &slashdown;
                } else {
                    player = &slashup;
                }
            }
            speed = 100;
            movementTotal = 247581;
        }

    if (healing) {
            player = &heal;
            speed = 50;
            Relaps = 5;
            movementTotal = 247581;
        }


        if(movementTotal == 0) {
            playerObject.frameIndex = -1;

        }
                if (!rolling && !slashing && !healing) {
            if (!facingvert){
                player = &walk;
            } else {
                if (facingdown) {
                    player = &walkdown;
                } else {
                    player = &walkup;
                }
            }
            }
    
        playerObject.facingLeft = facingLeft;
        playerObject.x = playerX;
        playerObject.y = playerY;
        playerObject.animation = player;
}




int main(int argc, char* argv[]) {


lastFrame = SDL_GetTicks() / 1000.0f;

    
    running = 1;
    fullscreen = 0;


    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL was not initialized: " << SDL_GetError() << "\n";
    }else{
        std::cout << "Up and running, Captain \n";
    }

        
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    Width = DM.w;
    Height = DM.h;

    
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Wlength, Wheight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_SetWindowTitle(window, "SDL2");
    SDL_ShowCursor(1);


    SDL_GLContext context = SDL_GL_CreateContext(window); 
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
    }
    SDL_GL_SetSwapInterval(1); 

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);

    SDL_Init(SDL_INIT_AUDIO);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Music* music = Mix_LoadMUS("title.mp3");
    if (!music) {
        std::cout << "Load Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Music* shop = Mix_LoadMUS("shop.mp3");
    if (!shop) {
        std::cout << "Load Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    shaderProgram = createProgram("Base.vert", "Base.frag");

    glUseProgram(shaderProgram);
    
    glm::mat4 projection = glm::ortho(0.0f, Wlength, 0.0f, Wheight);
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "projection"),
        1, GL_FALSE,
        glm::value_ptr(projection)
    );

 float quadVertices[16] =
{
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f
};

    unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
    
 };


    
GLuint VAO, VBO, EBO;

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_DYNAMIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
glEnableVertexAttribArray(1);

glBindVertexArray(0);


    transparent.texture = 0;
    walk.texture = loadTexture("Images/a.png");
    walk.totalFrames = 3;
    walk.frameDuration = 0.1f;
    walkdown.texture = loadTexture("Images/walkdown.png");
    walkdown.totalFrames = 3;
    walkdown.frameDuration = 0.1f;
    walkup.texture = loadTexture("Images/walkup.png");
    walkup.totalFrames = 3;
    walkup.frameDuration = 0.1f;
    roll.texture = loadTexture("Images/roll.png");
    roll.totalFrames = 5;
    roll.frameDuration = 0.04f;
    heal.texture = loadTexture("Images/roll.png");
    heal.totalFrames = 5;
    heal.frameDuration = 0.1f;
    slash.texture = loadTexture("Images/slash.png");
    slash.totalFrames = 4;
    slash.frameDuration = 0.08f;
    slashdown.texture = loadTexture("Images/slashdown.png");
    slashdown.totalFrames = 4;
    slashdown.frameDuration = 0.08f;
    slashup.texture = loadTexture("Images/slashup.png");
    slashup.totalFrames = 4;
    slashup.frameDuration = 0.08f;
    vinceWalk.texture = loadTexture("Images/vinceWalk.png");
    vinceWalk.totalFrames = 5;
    vinceWalk.frameDuration = 0.12;
    domerOp.texture = loadTexture("Images/domer.png");
    domerOp.totalFrames = 7;
    domerOp.frameDuration = 0.12;
    vinceLunge.texture = loadTexture("Images/vinceLunge.png");
    vinceLunge.totalFrames = 9;
    vinceLunge.frameDuration = 0.075;
    chara.texture = loadTexture("Images/chara.png");
    chara.totalFrames = 68;
    chara.frameDuration = -1;
    background.texture = loadTexture("Images/background.png");
    background.totalFrames = 1;
    background.frameDuration = -1;
    enterLeftTexture.texture = loadTexture("Images/enterLeft.png");
    enterLeftTexture.totalFrames = 1;
    enterLeftTexture.frameDuration = -1;
    flyDownLeft.texture = loadTexture("Images/flyDownLeft.png");
    flyDownLeft.totalFrames = 5;
    flyDownLeft.frameDuration = 0.092;
    flyDown.texture = loadTexture("Images/flyDown.png");
    flyDown.totalFrames = 5;
    flyDown.frameDuration = 0.092;
    flySide.texture = loadTexture("Images/flySide.png");
    flySide.totalFrames = 5;
    flySide.frameDuration = 0.092;
    flyUpRight.texture = loadTexture("Images/flyUpRight.png");
    flyUpRight.totalFrames = 5;
    flyUpRight.frameDuration = 0.092;
    player = &walk;
    ballAMN.texture = loadTexture("Images/ball.png");
    ballAMN.totalFrames = 1;
    ballAMN.frameDuration = 0.2;
    Marcus.texture = loadTexture("Images/Marcus.png");
    flyUp.texture = loadTexture("Images/flyUp.png");
    flyUp.totalFrames = 5;
    StewMass.texture = loadTexture("Images/Stewart.png");
    titleTex.texture = loadTexture("Images/title.png");
    selectOpt.texture = loadTexture("Images/select.png");
    end_credit.texture = loadTexture("Images/BadGameIdea.png");
    Shop1Tex.texture = loadTexture("Images/BudDome.png");
    shop2texture.texture = loadTexture("Images/SmilingGirl.png");
    Shop3Tex.texture = loadTexture("Images/Mairzy.png");
    Obelisk.texture = loadTexture("Images/Null:IsNotReady.png");
    pjta.texture = loadTexture("Images/pjta.png");
    pkti.texture = loadTexture("Images/pkti.png");
    Whiskers.texture = loadTexture("Images/Whiskers.png");
    Cigarette.texture = loadTexture("Images/PetiteCigarette.png");
    sign.texture = loadTexture("Images/sign.png");
    house.texture = loadTexture("Images/house.png");
    trapHouse.texture = loadTexture ("Images/TrapHouse.png");
    train.texture = loadTexture("Images/Train.png");
    Tree.texture = loadTexture("Images/tree.png");
    BudelariumDominicus.texture = loadTexture("Images/BudelariumDominicus.png");
    flyUp.frameDuration = 0.092;
    Shadow.texture = loadTexture("Images/shadow.png");
    laser.texture = loadTexture("Images/White.png");
    flyUp.totalFrames = 5;
    flyUp.frameDuration = 0.092;
 
    playerObj.x = playerX;
    playerObj.y = playerY;
    playerObj.hitboxWidth = 8;
    playerObj.hitboxHeight = 16;
    playerObj.animation = player;
    playerObj.frameIndex = 0;
    playerObj.animationTimer = 0.0f;
    playerObj.facingLeft = false;
    playerObj.relaps = 0.0f;
    playerObj.health = 2000000000;
    playerObj.alive = true;
    playerObj.knockback = 0;
    playerObj.name = "Zelda";

    vince.x = 200;
    vince.y = 50;
    vince.hitboxWidth = 32;
    vince.hitboxHeight = 32;
    vince.animation = &vinceWalk;
    vince.frameIndex = 0;
    vince.animationTimer = 0.0f;
    vince.facingLeft = true;
    vince.relaps = 0.0f;
    vince.health = 4;
    vince.alive = true;
    vince.knockback = 0;
    vince.name = "vince";
    
    ivine.x = 100;
    ivine.y = 800;
    ivine.hitboxWidth = 32;
    ivine.hitboxHeight = 32;
    ivine.animation = &flyDownLeft;
    ivine.frameIndex = 0;
    ivine.animationTimer = 0.0f;
    ivine.facingLeft = true;
    ivine.relaps = 85.0f;
    ivine.health = 16;
    ivine.alive = true;
    ivine.knockback = 0;
    ivine.name = "ivine";
    ivine.height = 30;

    domer.x = 100;
    domer.y = 800;
    domer.hitboxWidth = 32;
    domer.hitboxHeight = 32;
    domer.animation = &domerOp;
    domer.frameIndex = 100;
    domer.animationTimer = 0.0f;
    domer.facingLeft = true;
    domer.relaps = 85.0f;
    domer.health = 4.0f;
    domer.alive = true;
    domer.knockback = 0;
    domer.name = "domer";
    
    objects.push_back(playerObj);
    objects.push_back(vince);
    vince.health += 18;
    

    groundBoxes.push_back({-200.0f, -350.0f, 410.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f, -345.0f, 400.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({-200.0f - 200, -350.0f + 800, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f - 200, -345.0f + 800, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(1, -200, 700, ivine, vince, domer);
    
    groundBoxes.push_back({-200.0f + 800, -350.0f + 300, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f + 800, -345.0f + 300, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({-200.0f + 1000, -350.0f + 1000, 510.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f + 1000, -345.0f + 1000, 500.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(2,3), 1200, 825, ivine, vince, domer);
    
    groundBoxes.push_back({-200.0f + 300, -350.0f + 1300, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f + 300, -345.0f + 1300, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({-200.0f - 800, -350.0f + 300, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f - 800, -345.0f + 300, 300.0f, 300.0f, 0, 0, 0});
    
    groundBoxes.push_back({-200.0f - 900, -350.0f + 900, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({-195.0f - 900, -345.0f + 900, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(4, -1050, 725, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f - 750, 0.0f + 1100, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 750, 5.0f + 1100, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({0.0f - 200, 0.0f + 1500, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 200, 5.0f + 1500, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({0.0f + 700, 0.0f + 1300, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 700, 5.0f + 1300, 300.0f, 300.0f, 0, 0, 0});
    
    groundBoxes.push_back({0.0f - 600, 0.0f + 1650, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 600, 5.0f + 1650, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({0.0f + 400, 0.0f + 2000, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 400, 5.0f + 2000, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(4,7), 400, 2000, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f + 140, 0.0f + 2400, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 140, 5.0f + 2400, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(4,7), 140, 2400, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f - 1350, 0.0f + 1700, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 1350, 5.0f + 1700, 300.0f, 300.0f, 0, 0, 0});
    
    groundBoxes.push_back({0.0f - 850, 0.0f + 2200, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 850, 5.0f + 2200, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(4,7), -850, 2200, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f - 750, 0.0f + 1100, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f - 750, 5.0f + 1100, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({0.0f + 900, 0.0f + 2600, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 900, 5.0f + 2600, 300.0f, 300.0f, 0, 0, 0});
    
    groundBoxes.push_back({0.0f + 450, 0.0f + 2950, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 450, 5.0f + 2950, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(4,7), 450, 2950, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f + 1200, 0.0f + 3000, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 1200, 5.0f + 3000, 300.0f, 300.0f, 0, 0, 0});
    enemyRand(rand(4,7), 1200, 3000, ivine, vince, domer);
    
    groundBoxes.push_back({0.0f + 1550, 0.0f + 3500, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 1550, 5.0f + 3500, 300.0f, 300.0f, 0, 0, 0});

    groundBoxes.push_back({0.0f + 1000, 0.0f + 2000, 310.0f, 310.0f, 1, 1, 1});
    groundBoxes.push_back({5.0f + 1000, 5.0f + 2000, 300.0f, 300.0f, 0, 0, 0});

    Item gemma;
    gemma.x = -410;
    gemma.y = 400;
    gemma.type = "Ruby";
    gemma.texture = loadTexture("Images/Ruby.png");
    gemma.expire = false;
    Items.push_back(gemma);
    gemma.x = -380;
    gemma.y = 360;
    gemma.type = "Silver";
    gemma.texture = loadTexture("Images/Silver.png");
    Items.push_back(gemma);
    
    float barX = margin;
    float barY = 180 * 3 - barHeight - margin;
    Dialoge.push_back({"Health", barX + 4, barY - 20, 16.0f});
    Dialoge.push_back({"Glove", barX + 8, barY - 100, 16.0f});
    float objectSize;

    for (objectSize = 1; objectSize < 2048; objectSize++) {
    GameObject object;
    object.x = (((int)objectSize % 32) * 320 + (32 * rand(0 , 9))) - 16 * 320;
    object.y = ((int)(objectSize / 32) * 320 + (32 * rand(0 , 9))) - 16 * 320;
    object.hitboxWidth = 32;
    object.hitboxHeight = 32;
    object.animation = &background;
    object.frameIndex = -1;
    object.animationTimer = 0.0f;
    object.facingLeft = rand(0, 1);
    object.relaps = 0.0f;
    object.health = 0;
    object.alive = true;
    object.knockback = 0;
    object.name = "";
    backgroundObjects.push_back(object);
    }
    Wall wall;
    GameObject enterLeft;
    enterLeft.x = -32;
    enterLeft.y = -864;
    enterLeft.frameIndex = -1;
    enterLeft.animationTimer = 0.0f;
    enterLeft.hitboxWidth = 32;
    enterLeft.hitboxHeight = 32;
    enterLeft.relaps = 0.0f;
    enterLeft.health = 0;
    enterLeft.animation = &enterLeftTexture;
    enterLeft.facingLeft = true;
    enterLeft.alive = true;

    
    GameObject Npc;
    Npc.x = playerX + 32 * 1.5 * -2.75;
    Npc.y = playerY + 32 * 1.5 * 7.85;
    Npc.hitboxWidth = 48;
    Npc.hitboxHeight = 96;
    Npc.animation = &StewMass;
    Npc.frameIndex = 0;
    Npc.animationTimer = 0.0f;
    Npc.facingLeft = true;
    Npc.alive = true;
    Npc.name = "Stewart";
    backgroundObjects.push_back(Npc);
                wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);

        Item object;
    object.x = Npc.x + 16 * 8 * 2;
    object.y = Npc.y + 16 * 1.5 * 2;
    object.type = "Glove";
    object.texture = loadTexture("Images/Glove.png");
    object.expire = false;
    Items.push_back(object);
    
    Npc.x = -1500;
    Npc.y = 1300;
    Npc.hitboxWidth = 25 * 3;
    Npc.hitboxHeight = 44 * 3;
    Npc.animation = &Obelisk;
    Npc.name = "NULL";
    backgroundObjects.push_back(Npc);    
                wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);

    Npc.x = 2500;
    Npc.y = 3300;
    Npc.hitboxWidth = 16 * 2.5;
    Npc.hitboxHeight = 16 * 2.5;
    Npc.animation = &Marcus;
    Npc.name = "Marcus";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth - 15;
            wall.hitboxY = Npc.hitboxHeight - 3;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = - 110;
    Npc.y = - 325;
    Npc.hitboxWidth = 75 * 3;
    Npc.hitboxHeight = 8 * 3;
    Npc.animation = &pjta;
    Npc.name = "pjta";
    backgroundObjects.push_back(Npc); 
    
    Npc.x = playerX - 100;
    Npc.y = playerY + 300;
    Npc.hitboxWidth = 82 * 3;
    Npc.hitboxHeight = 8 * 3;
    Npc.animation = &pkti;
    Npc.name = "pkti";
    backgroundObjects.push_back(Npc); 

    Npc.x = 2265;
    Npc.y = 5050;
    Npc.hitboxWidth = 25 * 3;
    Npc.hitboxHeight = 29 * 3;
    Npc.animation = &Whiskers;
    Npc.name = "Whiskers";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);

    Npc.x = 3600;
    Npc.y = 3400;
    Npc.hitboxWidth = 6 * 3;
    Npc.hitboxHeight = 11 * 3;
    Npc.animation = &Cigarette;
    Npc.name = "Petite Cigarette";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    
    Npc.x = -260;
    Npc.y = 1000;
    Npc.hitboxWidth = 6 * 6;
    Npc.hitboxHeight = 11 * 6;
    Npc.animation = &sign;
    Npc.name = "sign";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);

    groundBoxes.push_back({2000.0f, 2800, 2000.0f, 1900.0f, 1, 1, 1});
    groundBoxes.push_back({2000.0f + 5, 5.0f + 2800, 2000.0f - 10, 1900.0f - 10, 0, 0, 0});

    Npc.x = 2520;
    Npc.y = 3200;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 64;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2800;
    Npc.y = 3200;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 64;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3080;
    Npc.y = 3200;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 64;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2150;
    Npc.y = 4800;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2350;
    Npc.y = 4800;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2150;
    Npc.y = 4608;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2350;
    Npc.y = 4608;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2054;
    Npc.y = 4992;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2446;
    Npc.y = 4992;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2054;
    Npc.y = 5184;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2446;
    Npc.y = 5184;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2150;
    Npc.y = 5184;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2350;
    Npc.y = 5184;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 0;
    Npc.y = -2000;
    Npc.hitboxWidth = 24 * 6;
    Npc.hitboxHeight = 24 * 6;
    Npc.animation = &trapHouse;
    Npc.name = "Trap";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2250;
    Npc.y = 5184;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2054;
    Npc.y = 4800;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2446;
    Npc.y = 4800;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);

    Npc.x = 2520;
    Npc.y = 4300;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 64;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 2800;
    Npc.y = 4300;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 64;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3360;
    Npc.y = 4300;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 70;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3360;
    Npc.y = 4020;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 70;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3360;
    Npc.y = 3740;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 70;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3360;
    Npc.y = 3460;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &house;
    Npc.name = "house";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 70;
            wall.y = Npc.y + 88;
            wall.hitboxX = Npc.hitboxWidth - 166;
            wall.hitboxY = Npc.hitboxHeight - 108;
            wall.texture = 0;
            Walls.push_back(wall);

    Npc.x = -3300;
    Npc.y = 4000;
    Npc.hitboxWidth = 64 * 3;
    Npc.hitboxHeight = 64 * 3;
    Npc.animation = &train;
    Npc.name = "Train";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 6;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth - 12;
            wall.hitboxY = Npc.hitboxHeight - 50;
            wall.texture = 0;
            Walls.push_back(wall);

    Npc.x = 2900;
    Npc.y = 3940;
    Npc.hitboxWidth = 16 * 6;
    Npc.hitboxHeight = 32 * 6;
    Npc.animation = &Tree;
    Npc.name = "tree";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x;
            wall.y = Npc.y;
            wall.hitboxX = Npc.hitboxWidth;
            wall.hitboxY = Npc.hitboxHeight;
            wall.texture = 0;
            Walls.push_back(wall);
    
    Npc.x = 3080;
    Npc.y = 4300;
    Npc.hitboxWidth = 64 * 6;
    Npc.hitboxHeight = 64 * 6;
    Npc.animation = &BudelariumDominicus;
    Npc.name = "Budelarium Dominicus House";
    backgroundObjects.push_back(Npc);    
            wall.x = Npc.x + 100;
            wall.y = Npc.y + 88;
            wall.hitboxX = 12 * 6;
            wall.hitboxY = 3 * 6;
            wall.texture = 0;
            Walls.push_back(wall);
            wall.x = Npc.x + 154 + 72;
            wall.y = Npc.y + 88;
            wall.hitboxX = 12 * 6;
            wall.hitboxY = 3 * 6;
            wall.texture = 0;
            Walls.push_back(wall);
            wall.x = Npc.x + 108;
            wall.y = Npc.y + 188 + 5 * 3;
            wall.hitboxX = 14 * 12;
            wall.hitboxY = 14 * 6;
            wall.texture = 0;
            Walls.push_back(wall);
            wall.x = Npc.x + 100 + 72;
            wall.y = Npc.y + 188 + -4 * 3;
            wall.hitboxX = 52;
            wall.hitboxY = 8 * 6;
            wall.texture = 0;
            Walls.push_back(wall);
    Npc.x = 3080 + 100 + 72;
    Npc.y = 4300 + 188 + -4 * 3;
    Npc.hitboxWidth = 0;
    Npc.hitboxHeight = 0;
    Npc.animation = &house;
    Npc.name = "Budelarium Dominicus";
    backgroundObjects.push_back(Npc);   

    wall.x = -4030;
    wall.y = -4000;
    wall.hitboxX = 30;
    wall.hitboxY = 16000;
    wall.texture = 0;
    Walls.push_back(wall);

    
    wall.x =  3970;
    wall.y = -4000;
    wall.hitboxX = 30;
    wall.hitboxY = 16000;
    wall.texture = 0;
    Walls.push_back(wall);
    
    wall.x = -4000;
    wall.y = -4030;
    wall.hitboxX = 16000;
    wall.hitboxY = 30;
    wall.texture = 0;
    Walls.push_back(wall);
    
    wall.x = -4000;
    wall.y =  6000;
    wall.hitboxX = 16000;
    wall.hitboxY = 30;
    wall.texture = 0;
    Walls.push_back(wall);
    
    wall.facingLeft = false;
    float wallSide = -4;
    for (int p = 0; p < 2; p++){
    for (int i = -10; i < 10; i++){
    wall.x = playerX;
    wall.y = playerY;
    wall.x += -16 * 3 * wallSide;
    wall.y += (16 * 3) * i;
    wall.hitboxX = 16  * 3;
    wall.hitboxY = 16 * 3;
    wall.texture = loadTexture("Images/wall.png");
    Walls.push_back(wall);
    }
        wall.facingLeft = true;
        wallSide = 4;
    }
    wall.facingLeft = false;
    wallSide = -7;
    for (int p = 0; p < 2; p++){
    for (int i = -18; i < 2; i++){
    wall.x = -16;
    wall.y = 0;
    wall.x += -16 * 3 * wallSide;
    wall.y += (16 * 3) * i;
    wall.hitboxX = 16  * 3;
    wall.hitboxY = 16 * 3;
    wall.texture = loadTexture("Images/wall.png");
    Walls.push_back(wall);
    }
        wall.facingLeft = true;
        wallSide = 7;
    }

    for (int i = -5; i < 8; i++) {
            wall.x = -64;
            wall.y = 0;
            wall.x += 16 * 3 * i;
            wall.y += (16 * 3) * -19;
            wall.hitboxX = 16  * 3;
            wall.facingLeft = false;
            wall.hitboxY = 16 * 3;
            wall.texture = loadTexture("Images/wallUp.png");
            Walls.push_back(wall);
        }

            wall.x = -64;
            wall.y = 0;
            wall.x += 16 * 3 * -6;
            wall.y += (16 * 3) * -19;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.facingLeft = false;
            wall.texture = loadTexture("Images/wallUpRight.png");
            Walls.push_back(wall);
            wall.x = -64;
            wall.x += 16 * 3 * 8;
            wall.facingLeft = true;
            wall.texture = loadTexture("Images/wallUpRight.png");
            Walls.push_back(wall);


        for (int i = -5; i < 8; i++){
            if (i != -4) {
            wall.x = -64;
            wall.y = 0;
            wall.x += 16 * 3 * i;
            wall.y += (16 * 3) * 2;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.texture = loadTexture("Images/wallDown.png");
            Walls.push_back(wall);
        }
            }

            wall.x = -64;
            wall.y = 0;
            wall.x += 16 * 3 * -6;
            wall.y += (16 * 3) * 2;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.texture = loadTexture("Images/wallDownRight.png");
            Walls.push_back(wall);
            wall.x = -64;
            wall.x += 16 * 3 * 8;
            wall.facingLeft = false;
            wall.texture = loadTexture("Images/wallDownRight.png");
            Walls.push_back(wall);
    
    for (int i = -3; i < 4; i++)
        if (i != 0) {
            wall.x = playerX;
            wall.y = playerY;
            wall.x += 16 * 3 * i;
            wall.y += (16 * 3) * 10;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.texture = loadTexture("Images/wallDown.png");
            Walls.push_back(wall);
        }
    wall.facingLeft = true;
    for (int i = -3; i < 4; i++) {
            wall.x = playerX;
            wall.y = playerY;
            wall.x += 16 * 3 * i;
            wall.y += (16 * 3) * -6;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.texture = 0;
            Walls.push_back(wall);
        }
    
            wall.x = playerX;
            wall.y = playerY;
            wall.x += 16 * 3 * -4;
            wall.y += (16 * 3) * 10;
            wall.hitboxX = 16  * 3;
            wall.hitboxY = 16 * 3;
            wall.texture = loadTexture("Images/wallDownRight.png");
            Walls.push_back(wall);
            wall.x = playerX;
            wall.x += 16 * 3 * 4;
            wall.facingLeft = false;
            wall.texture = loadTexture("Images/wallDownRight.png");
            Walls.push_back(wall);


    
    for (objectSize = 1; objectSize < 1024; objectSize++) {
    Item object;
    float gemtype = rand(1,5);
    object.x = (((int)objectSize % 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    object.y = ((int)(objectSize / 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    if (gemtype == 1) {
        object.texture = loadTexture("Images/Gold.png");
        object.type = "Gold";
    } else if (gemtype == 2) {
        object.texture = loadTexture("Images/Silver.png");
        object.type = "Silver";
    } else if (gemtype == 3) {        
        object.texture = loadTexture("Images/Ruby.png");
        object.type = "Ruby";
    } else if (gemtype == 4) { 
        object.texture = loadTexture("Images/Blue.png");
        object.type = "Blue";
    } else if (gemtype == 5) {        
        object.texture = loadTexture("Images/Parmesan.png");
        object.type = "Parmesan";
    }
            object.expire = false;
    Items.push_back(object);
    }
    

    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
    GLint spriteLoc = glGetUniformLocation(shaderProgram, "sprite");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
        Mix_PlayMusic(music, -1); 
        beep = Mix_LoadWAV("Beep.wav");
        bloop = Mix_LoadWAV("Bloop.wav");
        balls = Mix_LoadWAV("balls.wav");
        deathSound = Mix_LoadWAV("Death.wav");
        hit = Mix_LoadWAV("Hit.wav");

    for (auto i = Items.begin(); i != Items.end(); ) {
        bool removed = false;
    
        for (auto& wall : Walls) {
            if (wallCollision(i->x, i->y, 4, 4, wall.x, wall.y, wall.hitboxX, wall.hitboxY)) {
                i = Items.erase(i); 
                removed = true;
                break;
            }
        }
    
        if (!removed) {
            ++i;
        }
    }
    Mix_PauseMusic();
    while(running){
        if (title && TerminalTimer == 0) {
                    Mix_PauseMusic();
        }
        float currentTime = SDL_GetTicks() / 1000.0f;
        deltaTime = currentTime - lastFrame;
lastFrame = currentTime;


if (deltaTime > 0.05f) 
    deltaTime = 0.05f;
if (deltaTime < 0.02f){
    deltaTime = 0.02f;
}
if (record) {
    deltaTime = 0.02;
}

       // std::cout << deltaTime << "\n";
        
        update();
        input(objects);
        if (health > 100) {
        health = 100;
        }

        glUseProgram(shaderProgram);
        glm::mat4 backmodel = glm::mat4(1.0f);
        backmodel = glm::scale(backmodel, glm::vec3(270 * 3, 180 * 3, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(backmodel));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);

            glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        outsideTheBox = true;
        std::sort(objects.begin(), objects.end(), compareByLength);

        
        for (auto& box : groundBoxes)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,
                glm::vec3(box.x - cameraX, box.y - cameraY, 0.0f));
            model = glm::scale(model,
                glm::vec3(box.sizeX, box.sizeY, 1.0f));
            glUniform4f(glGetUniformLocation(shaderProgram, "color"), box.r, box.g, box.b, 1);
            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram, "model"),
                1, GL_FALSE,
                glm::value_ptr(model)
            );
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            auto Playss = std::find_if(objects.begin(), objects.end(), 
        [](const GameObject& playera) {
            return playera.name == playerName;
        });
    
            if (box.r == 0 && InBox(*Playss, box)) {
                outsideTheBox = false;
                pulse = 0;
            }
        }

        if (pauseScreen || tutorial != 0) {
            pickCoolDown = 1;
            pulse -= deltaTime * 40;
        }
        
        glUseProgram(shaderProgram);
    
        glActiveTexture(GL_TEXTURE0);
        for(auto& obj : backgroundObjects) {

        if (obj.name == "Stewart"){
            float dx, dy;
                dx = playerX - obj.x - 22;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                sen.sentence = "Stewart";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 2;
                sen.sentence = "Hi.";
                sen.placeX = 395; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);

            }
                                TextBox = true;
                }
            if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
            }
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Leave me alone.";
                sen.placeX = 340; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
        }

        if (obj.name == "Marcus"){
            float dx, dy;
                dx = playerX - obj.x + 5;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                sen.sentence = "Marcus";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 3;
                sen.sentence = ". . .";
                sen.placeX = 395; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);

            }

            
                                TextBox = true;
                }
            if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
            }
            
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "You've got your own goals, and I've got mine.";
                sen.placeX = 185; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (length < 32 && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Don't get in my way and I won't get in yours.";
                sen.placeX = 190; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            
        }

        if (obj.name == "Whiskers"){
            float dx, dy;
                dx = playerX - obj.x - 9;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                DialogStateNext = 0;
                sen.sentence = "Whiskers";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 6;
            if (!(WhiskerEncounterBefore == 2)) {
                sen.sentence = "Ow";
                sen.placeX = 395; 
                sen.placeY = 120;
                sen.size = 16;
                } else {
                    sen.sentence = "Thank you.";
                    sen.placeX = 365; 
                    sen.placeY = 120;
                    sen.size = 16;
                }
                Dialoge.push_back(sen);

            }

            
                                TextBox = true;
                }


            if (WhiskerEncounterBefore == 0) {
                
            if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
                WhiskerEncounterBefore = 1;
            }
                
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Why, is there no god to comfort me in my woes.";
                sen.placeX = 180; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (length < 32 && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "What? You want to leave the island?";
                sen.placeX = 250; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (length < 32 && DialogStateNext == 3 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "no no no no no no no no no no no no no no no no no no no no no no no no no no no no";
                sen.placeX = 140; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }            
            
            if (length < 32 && DialogStateNext == 4 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Sorry, man. I'd know nothing about that.";
                sen.placeX = 225; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (length < 32 && DialogStateNext == 5 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "All I've got is this ticket.";
                sen.placeX = 280; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

        } else if (WhiskerEncounterBefore == 1) {
                if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
                WhiskerEncounterBefore = 2;
            }
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "You want my ticket?";
                sen.placeX = 330; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (length < 32 && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "But it's all I have...";
                sen.placeX = 327; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (length < 32 && DialogStateNext == 3 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "";
                sen.placeX = 140; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }            
            
            if (length < 32 && DialogStateNext == 4 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Please, man, I need this.";
                sen.placeX = 300;
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (length < 32 && DialogStateNext == 5 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                sen.sentence = "You got a ticket.";
                sen.placeX = 340; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "";
                Dialoge.push_back(sen);
            }
                
        } else {
                DialogCap = 3;

                                if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
            }
                
                if (length < 32 && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "I didn't realize the pain it caused me.";
                sen.placeX = 225; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
        }
                if (length < 32 && DialogStateNext == 2 && TextBox) {
                    Sentence sen;
                    Dialoge.erase(Dialoge.begin() + 3);
                    sen.sentence = "I apologize for the anguish you're about to endure.";
                    sen.placeX = 160; 
                    sen.placeY = 120;
                    sen.size = 16;
                    Dialoge.push_back(sen);
            }
            
        }

    }
        if (obj.name == "Petite Cigarette"){
            float dx, dy;
                dx = playerX - obj.x - 5;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                DialogStateNext = 0;
                sen.sentence = "Petite Cigarette";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 1;
                sen.sentence = "I wanna pee with the fishes.";
                sen.placeX = 300; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);

            }

            
                                TextBox = true;
                }
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
            }
            
        }

            if (obj.name == "sign"){
            float dx, dy;
                dx = playerX - obj.x - 5;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                DialogStateNext = 0;
                sen.sentence = "Sign";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 1;
                sen.sentence = "Frog Place ''northeast''";
                sen.placeX = 300; 
                sen.placeY = 142.5;
                sen.size = 16;
                Dialoge.push_back(sen);
                sen.sentence = "Train Station ''northwest''";
                sen.placeX = 274; 
                sen.placeY = 95;
                sen.size = 16;
                Dialoge.push_back(sen);

            }

            
                                TextBox = true;
                }
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
            }
            
        }

                if (obj.name == "Train" && !Game_End_I_Want_A_Long_Variable_Name){
            float dx, dy;
                dx = playerX - obj.x - 32;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                Sentence sen;
                DialogStateNext = 0;
                sen.sentence = "Written Message";
                sen.placeX = 62.5;
                sen.placeY = 170;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 1;
                sen.sentence = "Purchased passes, pass 18820 'Whiskers GoodBoy' ";
                sen.placeX = 170; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);

            }

            
                                TextBox = true;
                }
            if (length < 32 && DialogStateNext == 1 && TextBox) {
                if (WhiskerEncounterBefore == 2) {
                    DialogStateNext = 2;
                } else {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
                }
            }

            if (length < 32 && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Wait? Are you Whiskers GoodBoy?";
                sen.placeX = 265; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }            
            if (length < 32 && DialogStateNext == 3 && TextBox) {
                Sentence sen;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "Very well then, climb aboard.";
                sen.placeX = 260; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }            

            if (length < 32 && DialogStateNext == 4 && TextBox) {
                other = true;
                Game_End_I_Want_A_Long_Variable_Name = true;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                Sentence sen;
                sen.sentence = "Press K to Play Again";
                sen.exception = true;
                sen.placeX = 290; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            
        }
            if (obj.name == "Budelarium Dominicus"){
            float dx, dy;
                dx = playerX - obj.x + 16;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick && !TextBox) {
                
                Mix_PlayMusic(shop, -1);
                Sentence sen;
                sen.exception = true;
                sen.sentence = "Bud-Dome";
                sen.placeX = 62.5;
                sen.placeY = 110;
                sen.size = 16;
                Dialoge.push_back(sen);
                TextBox = true;
                other = true;
                Shop1 = true;
                shopKey = 3;
                DialogCap = 746774;
                sen.sentence = "Welcome!";
                sen.placeX = 395; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
                //std::cout << length;
            
        }

        if (obj.name == "Trap"){
            float dx, dy;
                dx = playerX - obj.x - 12 * 6;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);
            if (length < 32 && pick && !TextBox) {
                
                Mix_PlayMusic(shop, -1);
                Sentence sen;
                sen.exception = true;
                sen.sentence = "Mrs Mairzy Rocksworth";
                sen.placeX = 85;
                sen.placeY = 134;
                sen.size = 16;
                Dialoge.push_back(sen);
                TextBox = true;
                other = true;
                Shop3 = true;
                shopKey = 0;
                DialogCap = 746774;
                sen.sentence = "Hiiiiii!";
                sen.placeX = 385; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
                //std::cout << length;
            
        }

        if (obj.name == "NULL"){
            float dx, dy;
                dx = playerX - obj.x - 22;
                dy = playerY - obj.y;
                float length = sqrt(dx * dx + dy * dy);

            if (Terminal) {
                if(TerminalTimer > 20 && TerminalTimer < 70) {
                std::string textRoll =       "                         ";
                std::string terminalString = "... Null is not ready ...";
                for (int i = 0; i != (int)TerminalTimer - 20 && i <= 25; i++) {
                    textRoll[i] = terminalString[i]; 
                }

                std::cout << TerminalTimer << "\n";
                
                Sentence sen;
                sen.sentence = textRoll;
                sen.placeX = 24; 
                sen.placeY = (180 * 3) - 30;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 1;

            } else if (TerminalTimer > 85 && TerminalTimer < 200){
                std::string textRoll =       "                 ";
                std::string terminalString = "... rebooting ...";
                for (int i = 0; i != (int)TerminalTimer - 85 && i <= 17; i++) {
                    textRoll[i] = terminalString[i]; 
                }

                std::cout << TerminalTimer << "\n";
                
                Sentence sen;
                sen.sentence = textRoll;
                sen.placeX = 24; 
                sen.placeY = (180 * 3) - 30;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 1;
            } else {
            Sentence sen;
                sen.sentence = "";
                sen.placeX = 20; 
                sen.placeY = (180 * 3) - 30;
                sen.size = 16;
                DialogCap = 1;
                Dialoge.push_back(sen); 
                if (TerminalTimer > 220) {
                    DialogStateNext++;
                }
            }
            }
            if (length < 32 && pick) {

            
            if (length < 32 && !TextBox) {
                DialogCap = 1;
                Sentence sen;
                sen.sentence = "";
                sen.placeX = 20; 
                sen.placeY = (180 * 3) - 30;
                sen.size = 16;
                DialogStateNext = 0;
                DialogCap = 1;
                Dialoge.push_back(sen);
                TerminalTimer = 0;
            }
                TextBox = true;
                Terminal = true;
                
                }
            if (length < 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                Dialoge.erase(Dialoge.begin() + 2);
                Terminal = false;
                DialogStateNext = 0;
                obj.alive = false;
                obj.animation = &transparent;
                pickCoolDown = 1;
            }
        }
                    glBindTexture(GL_TEXTURE_2D, obj.animation->texture);
            float u0 = 0;
        float u1 = u0 + 1;
        
        float v0 = 0.0f;
        float v1 = 1.0f;

        if (!obj.facingLeft)
        {
            std::swap(u0, u1);
        }

        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        
        glm::mat4 model = glm::mat4(1.0f);
    
        model = glm::translate(model, glm::vec3(obj.x - cameraX, obj.y - cameraY, 0.0f));
    
        model = glm::scale(model, glm::vec3(obj.hitboxWidth, obj.hitboxHeight, 1.0f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
            glUniform1i(spriteLoc, 0);

            glUniform1i(useTextureLoc, 1);
    
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
        for (auto& burn : burns) {
 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(burn.x - cameraX, burn.y - 4 - cameraY, 0.0f));
        model = glm::scale(model, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f *(burn.health / 100), 1.0f * ((burn.health - 50) / 50), 1.0f * ((burn.health - 50)  / 100), (burn.health / 100));
        burn.health -= 60 * deltaTime;
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        }

        for (auto& wall : Walls) {
        glBindTexture(GL_TEXTURE_2D, wall.texture);
        
        float u0 = 0;
        float u1 = 1;

        if (!wall.facingLeft)
        {
            std::swap(u0, u1);
        }

        quadVertices[2]  = u0; 
        quadVertices[6]  = u1; 
        quadVertices[10] = u1; 
        quadVertices[14] = u0; 

            
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(wall.x - cameraX, wall.y - cameraY, 0.0f));
        model = glm::scale(model, glm::vec3(wall.hitboxX, wall.hitboxY, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glBindVertexArray(VAO);
                    
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);

        if (wall.texture != 0) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        }
        
        for (auto& i : Items) {
        glBindTexture(GL_TEXTURE_2D, i.texture);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i.x - cameraX, i.y - cameraY, 0.0f));
        model = glm::scale(model, glm::vec3(24, 24, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glBindVertexArray(VAO);
                    
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        float dx = playerX - i.x;
        float dy = playerY - i.y;
        float length = sqrt(dx*dx + dy*dy);
        if (length <= 32 && pick ) {
            if (!(weaponSize <= currentGems.size()) && i.type != "Glove") {
            i.expire = true;
            Gem gem;
            if (i.type == "Ruby" || i.type == "Parmesan" || i.type == "Gold") {
            gem.r = 1.0f;
            } else if (i.type == "Silver") {
            gem.r = 0.3f ;
            } else {
            gem.r = 0.0f;
            }
            if (i.type == "Gold" || i.type == "Parmesan") {
            gem.g = 1.0f;
            } else if (i.type == "Silver") {
            gem.g = 0.3f;   
            } else {
            gem.g = 0.0f;
            }
            if (i.type == "Blue" || i.type == "Parmesan") {
            gem.b = 1.0f;
            } else if (i.type == "Silver") {
            gem.b = 0.3f;   
            } else {
            gem.b = 0.0f;
            }
            if (i.type == "Parmesan") {
                gem.life = 1;
            } else {
            gem.life = 20;
            }
            gem.type = i.type;
            currentGems.push_back(gem);
            if (i.type == "Ruby" && !Ruby){
                tutorialTarget = 4;
                Ruby = true; 
            } else if (i.type == "Gold" && !Gold){
                tutorialTarget = 5;
                Gold = true;
            }else if (i.type == "Blue" && !Blue){
                tutorialTarget = 6;
                Blue = true;
            }else if (i.type == "Silver" && !Silver){
                tutorialTarget = 7;
                Silver = true;
            }else if (i.type == "Parmesan" && !White){
                tutorialTarget = 8;
                White = true;
            }



                
        } else if (i.type == "Glove") {

            if (!TextBox) {
                Sentence sen;
                sen.sentence = "You collected a Glove.";
                sen.placeX = 290; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
                DialogCap = 2;
            }
                TextBox = true;

        }

            
            }

            if (i.type == "Glove" && length <= 32 && DialogStateNext == DialogCap && TextBox) {
                TextBox = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 2);
                pickCoolDown = 1;
                shell = 1;
                i.expire = true;
            }
                        if (i.type == "Glove" && length <= 32 && DialogStateNext == 1 && TextBox) {
                Dialoge.erase(Dialoge.begin() + 2);
                Sentence sen;
                sen.sentence = "Press the J KEY to HEAL.";
                sen.placeX = 283; 
                sen.placeY = 120;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            
        }
        
        for(auto& obj : objects) {
        if (obj.up == true) {
       
        glm::mat4 model = glm::mat4(1.0f); 
        glBindTexture(GL_TEXTURE_2D, Shadow.texture);
        
        if (obj.facingLeft) {
        model = glm::translate(model, glm::vec3(obj.x + 28 - cameraX, obj.y - obj.height - cameraY, 0.0f));
        } else  {
            model = glm::translate(model, glm::vec3(obj.x + 38 - cameraX, obj.y - obj.height - cameraY, 0.0f));
        }
            
        if  ((obj.height <= 0)) {
        model = glm::translate(model, glm::vec3(0, 10, 0.0f));
        }
                
        model = glm::scale(model, glm::vec3(64.0f, 64.0f, 1.0f));    
            
    
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "model"),
            1, GL_FALSE,
            glm::value_ptr(model)
        );


        float u0 = 0;
        float u1 = 1;
        
        float v0 = 0.0f;
        float v1 = 1.0f;

        if (!obj.facingLeft)
        {
            std::swap(u0, u1);
        }

        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;


        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    

        if (obj.relaps < -20) {
            float dx = playerHistory[0].x + 10 - (obj.x + 60 + obj.offsetX);
            float dy = playerHistory[0].y - (obj.y + 60 + obj.offsetY);
            
            float length = sqrt(dx*dx + dy*dy);
            
                int steps = length;

            float playobjdx = playerHistory[0].x - (obj.x + 60);
            float playobjdy = playerHistory[0].y - (obj.y + 60);
            float polength = sqrt(playobjdx*playobjdx + playobjdy*playobjdy);
              float limit = 0;
                        
            if (-obj.relaps < 5) {
                limit = -obj.relaps * (length / 5); 
            }
            if (!(polength < 16)) {
                for (int i = 0; i < (steps + limit); i += 4) {
                    float t = i / length;
                
                    float px = obj.x + obj.offsetX + 60 + dx * t;
                    float py = obj.y + obj.offsetY + 60 + dy * t;
                
                    drawPixel(((int)(px - cameraX) / 4) * 4, ((int)(py - cameraY) / 4) * 4, 4, laser.texture, modelLoc, useTextureLoc, colorLoc, i, steps + limit);
                }
            }

            dx = playerHistory[0].x - playerX;
            dy = playerHistory[0].y - playerY;
            
            length = sqrt(dx*dx + dy*dy);

            if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                } else {
                    dx = 1;
                    dy = 0;
                }

            if (length < 16  && !playerHit) {
                playerHit = true;
                hitX = -dx;
                hitY = -dy;
                health -= obj.health * sensitivity;
                obj.relaps = 80;
                if (outsideTheBox) {
                    health = 0;
                }
        }
            }
        }
        glBindTexture(GL_TEXTURE_2D, obj.animation->texture);
        float frameWidth = 1.0f / (obj.animation->totalFrames + 1);
        float u0 = (obj.frameIndex + 1) * frameWidth;
        float u1 = u0 + frameWidth;
        if (obj.name == "ball") {
            frameWidth = 1.0f / (obj.animation->totalFrames);
            u0 = (obj.frameIndex + 1) * frameWidth;
            u1 = u0 + frameWidth;
        }
        
        float v0 = 0.0f;
        float v1 = 1.0f;

        if (!obj.facingLeft)
        {
            std::swap(u0, u1);
        }

        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;

        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
    
        glm::mat4 model = glm::mat4(1.0f);
    
        model = glm::translate(model, glm::vec3(obj.x - cameraX, obj.y - cameraY, 0.0f));
            if (obj.name == "ivine"){

if (obj.relaps < -20) {
            if (!playerHistory.empty())
        {
            GameObject burn;
            burn.x = playerHistory[0].x;
            burn.y = playerHistory[0].y;
            burn.health = 100;
            burns.push_back(burn);

        }
}
                
        model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
            } else if (obj.name == "cocoon"){
        model = glm::scale(model, glm::vec3(112.0f * 3, 112.0f * 3, 1.0f));                  
            } else if (obj.name == "domer"){
                model = glm::scale(model, glm::vec3(64, 64, 1.0f));   
            } else {
        model = glm::scale(model, glm::vec3(32.0f, 32.0f, 1.0f));    
            }


        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "model"),
            1, GL_FALSE,
            glm::value_ptr(model)
        );
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
        if (obj.name == "ivine" && obj.up == false) {

        
       
        model = glm::mat4(1.0f);
        glBindTexture(GL_TEXTURE_2D, Shadow.texture);

        if (obj.facingLeft) {
        model = glm::translate(model, glm::vec3(obj.x + 28 - cameraX, obj.y - obj.height - cameraY, 0.0f));
        } else  {
            model = glm::translate(model, glm::vec3(obj.x + 38 - cameraX, obj.y - obj.height - cameraY, 0.0f));
        }
            
        if  ((obj.height <= 0)) {
        model = glm::translate(model, glm::vec3(0, 10, 0.0f));
        }
                
        model = glm::scale(model, glm::vec3(64.0f, 64.0f, 1.0f));    
            
    
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "model"),
            1, GL_FALSE,
            glm::value_ptr(model)
        );

        u0 = 0;
        u1 = u0 + 1;
        
        v0 = 0.0f;
        v1 = 1.0f;

        if (!obj.facingLeft)
        {
            std::swap(u0, u1);
        }

        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;


        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
        
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    

        if (obj.relaps < -20) {
            float dx = playerHistory[0].x + 10 - (obj.x + 60 + obj.offsetX);
            float dy = playerHistory[0].y - (obj.y + 60 + obj.offsetY);
            
            float length = sqrt(dx*dx + dy*dy);
            
                int steps = length;

            float playobjdx = playerHistory[0].x - (obj.x + 60);
            float playobjdy = playerHistory[0].y - (obj.y + 60);
            float polength = sqrt(playobjdx*playobjdx + playobjdy*playobjdy);
            float limit = 0;
            
            if (-obj.relaps < 5) {
                limit = -obj.relaps * (length / 5); 
            }
            if (!(polength < 16)) {
                for (int i = 0; i < (steps + limit); i += 4) {
                    float t = i / length;
                
                    float px = obj.x + obj.offsetX + 60 + dx * t;
                    float py = obj.y + obj.offsetY + 60 + dy * t;
                
                    drawPixel(((int)(px - cameraX) / 4) * 4, ((int)(py - cameraY) / 4) * 4, 4, laser.texture, modelLoc, useTextureLoc, colorLoc, i, steps + limit);
                }
            }

            dx = playerHistory[0].x - playerX;
            dy = playerHistory[0].y - playerY;
            
            length = sqrt(dx*dx + dy*dy);

            if (length != 0)
                {
                    dx /= length;
                    dy /= length;
                } else {
                    dx = 1;
                    dy = 0;
                }

            if (length < 16  && !playerHit) {
                playerHit = true;
                hitX = -dx;
                hitY = -dy;
                health -= obj.health * 3 * sensitivity;
                obj.relaps = 80;
                if (outsideTheBox) {
                    health = 0;
                }
        }
            }
            }
}


for (auto& p : particles)
{
    float alpha = p.life / p.maxLife;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(p.x - cameraX, p.y - cameraY, 0));
    model = glm::scale(model, glm::vec3(p.size, p.size, 1));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

 
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, alpha * 0.9f, alpha * 0.3f, alpha);
    
        glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


            

        glDisable(GL_DEPTH_TEST);
        glm::mat4 model = glm::mat4(1.0f);
                
if (outsideTheBox){
    
        pulse += deltaTime * 40;
        if (pulse >= 40) {
            pulse = 0;
        }
        
        model = glm::translate(model, glm::vec3(barX - pulse, barY - pulse, 0.0f));
        model = glm::scale(model, glm::vec3(10, barHeight + pulse * 2, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX + barWidth - 10 + pulse, barY - pulse, 0.0f));
        model = glm::scale(model, glm::vec3(10, barHeight + pulse * 2, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX - pulse + 10, barY + barHeight - 10 + pulse, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth + pulse * 2 - 20 , 10, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX - pulse + 10, barY - pulse, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth + pulse * 2 - 20 , 10, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1, (-pulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}




        

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX, barY, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth, barHeight, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.2f, 0.2f, 0.2f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(barX, barY, 0.0f));
        model2 = glm::scale(model2, glm::vec3(barWidth, barHeight * health / 100, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



        if (shell == 1) {
            Dialoge[1].sentence = "Glove";
            Dialoge[1].placeX = barX + 8;
            weaponSize = 2;
            shellDamage = 1;
        }

            if (shell == 2) {
            Dialoge[1].sentence = "Blunt Crowbar";
            Dialoge[1].placeX = barX + -16;
            weaponSize = 1;
            shellDamage = 5;
        }

            if (shell == 21) {
            Dialoge[1].sentence = "Sheer Will";
            Dialoge[1].placeX = barX + -14;
            weaponSize = 0;
            shellDamage = 8;
        }


        

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX, barY - 106, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth, -barHeight * weaponSize, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.2f, 0.2f, 0.2f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        damage = shellDamage;
        sensitivity = 1;
        float mode = 0;
        healthSwing = false;
        constSwing = false;
        deathBoost = false;
        for (auto& gem : currentGems) {
            
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(barX + barWidth / 10, (barY - 106) - barHeight / 10 - (barHeight * mode), 0.0f));
        model = glm::scale(model, glm::vec3(barWidth / 1.25, -barHeight / 1.25, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), gem.r, gem.g, gem.b, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (gem.life <= 0) {
            gem.still = true;
        } else {
            gem.still = false;
        }


        if (gem.type == "Silver") { 
            damage += 2;
        } else if (gem.type == "Parmesan") {
            damage += 40;
        } else if (gem.type == "Blue") {
            damage += 8;
            sensitivity = 3;
        } else {
            damage += 1;
        }
        if (gem.type == "Ruby") {
            healthSwing = true;
        } else if (gem.type == "Gold") {
            constSwing = true;
        } else if (gem.type == "Silver") {
            sensitivity = 0.8;
        }

        mode++;
            
        }


        if (shell == 21) {
            constSwing = true;
        }








        if (TextBox && !Terminal && !other) {
        model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(45, 45, 0.0f));
        model2 = glm::scale(model2, glm::vec3((270 * 3) - 90, 160, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(50, 50, 0.0f));
        model = glm::scale(model, glm::vec3((270 * 3) - 100, 150, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        
        }   else if (Terminal) {
                    model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0, 0, 0.0f));
        model2 = glm::scale(model2, glm::vec3((270 * 3), 180 * 3, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5, 5, 0.0f));
        model = glm::scale(model, glm::vec3((270 * 3) - 10, (180 * 3) - 10, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        } else if (title) {
        glBindTexture(GL_TEXTURE_2D, titleTex.texture);
        float u0 = 0;
        float u1 = 1;
        float v0 = 0.0f;
        float v1 = 1.0f;
        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
                
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (titleSelect) {
            glBindTexture(GL_TEXTURE_2D, ballAMN.texture);
    
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(160, 107, 0.0f));
            model = glm::scale(model, glm::vec3(8 * 3, 8 * 3, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        } else {
            glBindTexture(GL_TEXTURE_2D, ballAMN.texture);
    
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(187, 176, 0.0f));
            model = glm::scale(model, glm::vec3(8 * 3, 8 * 3, 1.0f));;
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
            
    }    else if (pauseScreen) {

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 0.5f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

         model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270, 180, 0.0f));
        model2 = glm::scale(model2, glm::vec3(270, 180, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        
        model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(280, 190, 0.0f));
        model2 = glm::scale(model2, glm::vec3(250, 160, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        } else if (tutorial != 0) {

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 0.5f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

         model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 - 80, 180 - 100, 0.0f));
        model2 = glm::scale(model2, glm::vec3(270 + 160, 180 + 200, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        
        model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(280 - 80, 190 - 100, 0.0f));
        model2 = glm::scale(model2, glm::vec3(250 + 160, 160 + 200, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        } else if (Shop1) {

                    glBindTexture(GL_TEXTURE_2D, Shop1Tex.texture);
        float frameWidth = 1.0f / 6.0f;
        float u0 = shopKey * frameWidth;
        float u1 = u0 + frameWidth;
        float v0 = 0.0f;
        float v1 = 1.0f;
        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
                
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            if (pick && DialogStateNext == 67 && TextBox) {
                TextBox = false;
                other = false;
                Shop1 = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                Mix_HaltMusic();
                pickCoolDown = 1;
            }
            
            if (pick && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "The name's Budelarium Dominicus the III.";
                sen.placeX = 205; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 4;
                sen.sentence = "But you may call me Bud Dome.";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 3 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Yes, I've got plenty of wares.";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 4 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "From the cold skull's of Bulldogs.";
                sen.placeX = 230; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 5 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 5;
                sen.sentence = "To a collection of boardgames beloved by frogs.";
                sen.placeX = 168; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 6 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = " Yes, truly I do have it all.";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 7 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = " What      might you be in for today?";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (DialogStateNext == 8 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;

                selectKey = std::clamp(selectKey, 1.0f, 4.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "talk";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "buy";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 3) {
                    sen.sentence = "leave";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "sell";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }

                Dialoge.push_back(sen);
                    if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }
                
                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }

                if (DialogStateNext == 9){
                
                    if (selectKey == 1) {
                        DialogStateNext = 10;
                    } else if (selectKey == 2) {
                        DialogStateNext = 20;
                    }  else if (selectKey == 3) {
                        DialogStateNext = 66;
                    } else {
                        DialogStateNext = 40;
                    }
                        
                }

             if (pick && DialogStateNext == 66 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "I'll see you when kingdom come, my friend.";
                sen.placeX = 220; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 10 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "yeah, what you got to say?";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }



            if (DialogStateNext == 11 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;

                selectKey = std::clamp(selectKey, 1.0f, 4.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "What happend to your house?";
                    sen.placeX = 290; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "Getting out of here.";
                    sen.placeX = 310; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 3) {
                    sen.sentence = "The God Thing.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "Back";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;

                    
                }

                if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }
                
                Dialoge.push_back(sen);

                
                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }


            if (DialogStateNext == 12){
                
                    if (selectKey == 1) {
                        DialogStateNext = 13;
                    } else if (selectKey == 2) {
                        DialogStateNext = 23;
                    }  else if (selectKey == 3) {
                        DialogStateNext = 33;
                    } else {
                        DialogStateNext = 8;
                    }
                        
                }

            if (pick && DialogStateNext == 13 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Microwave.";
                sen.placeX = 360; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 23 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "You want to leave, huh?";
                sen.placeX = 320; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 24 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Well, you'd have to leave through the train station to get anywhere.";
                sen.placeX = 70; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 25 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 2;
                sen.sentence = "Tickets are beyond expensive though.";
                sen.placeX = 240; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 26 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "If you could find a ticket second-hand,";
                sen.placeX = 220; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 27 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "which is the only realistic way of getting one.";
                sen.placeX = 180; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 28 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 4;
                sen.sentence = "You could take the next train to orbital eight.";
                sen.placeX = 170; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 33 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "The TetraTrinty.";
                sen.placeX = 320; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 34 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Yeah, I don't really believe in the whole god thing, yknow.";
                sen.placeX = 140; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 35 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "And if the king really was ruling under their glory,";
                sen.placeX = 160; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 36 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "There wouldn't be as many lynchings as there are now.";
                sen.placeX = 160; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 20 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "take whatever you want, money doesn't mean much to me anymore.";
                sen.placeX = 100; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 40 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 4;
                sen.sentence = "respectfully, buddy, I don't want any of your stuff.";
                sen.placeX = 160; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (DialogStateNext == 21 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;

                selectKey = std::clamp(selectKey, 1.0f, 6.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "Ruby Gem";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "Gold Gem.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 3) {
                    sen.sentence = "Blue Gem.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 4) {
                    sen.sentence = "Metal Gem.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 5) {
                    sen.sentence = "White Gem.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "Back";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }

                if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }

                Dialoge.push_back(sen);

                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);                
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }


            if (DialogStateNext == 22){
                Gem gem;

                             
                    if (!(selectKey == 6)) {

                        DialogStateNext = 21;
                    if (!(weaponSize <= currentGems.size())) {

                    gem.life = 20;
                        gem.still = false;
                    if (selectKey == 1) {
                                    gem.r = 1.0f;
                                    gem.g = 0.0f;
                                    gem.b = 0.0f;
                                    gem.type = "Ruby";
                    }  else if (selectKey == 2) {
                                    gem.r = 1.0f;
                                    gem.g = 1.0f;
                                    gem.b = 0.0f;
                                    gem.type = "Gold";
                    }  else if (selectKey == 3) {
                                    gem.b = 1.0f;
                                    gem.r = 0.0f;
                                    gem.g = 0.0f;
                                    gem.type = "Blue";
                    }  else if (selectKey == 4) {
                                    gem.r = 0.3f;
                                    gem.b = 0.3f; 
                                    gem.g = 0.3f; 
                                    gem.type = "Silver";
                    }  else if (selectKey == 5) {
                                    gem.b = 1.0f;
                                    gem.r = 1.0f;
                                    gem.g = 1.0f;
                                    gem.life = 1;
                                    gem.type = "Parmesan";
                    } 
                    currentGems.push_back(gem);
                    std::cout << gem.type;
                } else {
                    DialogStateNext = 44;
                }
                    
                    } else {
                        DialogStateNext = 8;
                    }
                        
                } 

            if (pick && DialogStateNext == 44 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 4;
                sen.sentence = "sorry, I don't think you have the space.";
                sen.placeX = 220; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (DialogStateNext == 45){
                        DialogStateNext = 21;
                }
            
            if (DialogStateNext == 14 || DialogStateNext == 29 ||  DialogStateNext == 37){
                        DialogStateNext = 11;
                }
            if (DialogStateNext == 41){
                        DialogStateNext = 8;
                }



        
            
            
    } else if (death) {

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
std::cout << DeathTitleToggle;
        if (!pick) {
            DeathTitleToggle = true;
        }
    if (pick && DeathTitleToggle){
    hitX = 0;
    hitY = 0;
    objects.erase(objects.begin(), objects.end());
    Items.erase(Items.begin(), Items.end());
    currentGems.erase(currentGems.begin(), currentGems.end());
    shell = 21;
    WhiskerEncounterBefore = 0;
    playerObj.x = playerX;
    playerObj.y = playerY;
    playerObj.hitboxWidth = 8;
    playerObj.hitboxHeight = 16;
    playerObj.animation = player;
    playerObj.frameIndex = 0;
    playerObj.animationTimer = 0.0f;
    playerObj.facingLeft = false;
    playerObj.relaps = 0.0f;
    playerObj.health = 2000000000;
    playerObj.alive = true;
    playerObj.knockback = 0;
    playerObj.name = "Zelda";

    vince.x = 200;
    vince.y = 50;
    vince.hitboxWidth = 32;
    vince.hitboxHeight = 32;
    vince.animation = &vinceWalk;
    vince.frameIndex = 0;
    vince.animationTimer = 0.0f;
    vince.facingLeft = true;
    vince.relaps = 0.0f;
    vince.health = 4;
    vince.alive = true;
    vince.knockback = 0;
    vince.name = "vince";
    
    ivine.x = 100;
    ivine.y = 800;
    ivine.hitboxWidth = 32;
    ivine.hitboxHeight = 32;
    ivine.animation = &flyDownLeft;
    ivine.frameIndex = 0;
    ivine.animationTimer = 0.0f;
    ivine.facingLeft = true;
    ivine.relaps = 85.0f;
    ivine.health = 16;
    ivine.alive = true;
    ivine.knockback = 0;
    ivine.name = "ivine";
    ivine.height = 30;

    domer.x = 100;
    domer.y = 800;
    domer.hitboxWidth = 32;
    domer.hitboxHeight = 32;
    domer.animation = &domerOp;
    domer.frameIndex = 100;
    domer.animationTimer = 0.0f;
    domer.facingLeft = true;
    domer.relaps = 85.0f;
    domer.health = 4.0f;
    domer.alive = true;
    domer.knockback = 0;
    domer.name = "domer";
    
    objects.push_back(playerObj);
    objects.push_back(vince);
    vince.health += 18;
    


    enemyRand(1, -200, 700, ivine, vince, domer);
    enemyRand(rand(2,3), 1200, 825, ivine, vince, domer);
    enemyRand(4, -1050, 725, ivine, vince, domer);
    enemyRand(rand(4,7), 400, 2000, ivine, vince, domer);
    enemyRand(rand(4,7), 140, 2400, ivine, vince, domer);
    enemyRand(rand(4,7), -850, 2200, ivine, vince, domer);
    enemyRand(rand(4,7), 450, 2950, ivine, vince, domer);
    enemyRand(rand(4,7), 1200, 3000, ivine, vince, domer);
    
    Item gemma;
    gemma.x = -410;
    gemma.y = 400;
    gemma.type = "Ruby";
    gemma.texture = loadTexture("Images/Ruby.png");
    gemma.expire = false;
    Items.push_back(gemma);
    gemma.x = -380;
    gemma.y = 360;
    gemma.type = "Silver";
    gemma.texture = loadTexture("Images/Silver.png");
    Items.push_back(gemma);

    

    
    for (objectSize = 1; objectSize < 1024; objectSize++) {
    Item object;
    float gemtype = rand(1,5);
    object.x = (((int)objectSize % 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    object.y = ((int)(objectSize / 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    if (gemtype == 1) {
        object.texture = loadTexture("Images/Gold.png");
        object.type = "Gold";
    } else if (gemtype == 2) {
        object.texture = loadTexture("Images/Silver.png");
        object.type = "Silver";
    } else if (gemtype == 3) {        
        object.texture = loadTexture("Images/Ruby.png");
        object.type = "Ruby";
    } else if (gemtype == 4) { 
        object.texture = loadTexture("Images/Blue.png");
        object.type = "Blue";
    } else if (gemtype == 5) {        
        object.texture = loadTexture("Images/Parmesan.png");
        object.type = "Parmesan";
    }
            object.expire = false;
    Items.push_back(object);
    }
    

    for (auto i = Items.begin(); i != Items.end(); ) {
        bool removed = false;
    
        for (auto& wall : Walls) {
            if (wallCollision(i->x, i->y, 4, 4, wall.x, wall.y, wall.hitboxX, wall.hitboxY)) {
                i = Items.erase(i); 
                removed = true;
                break;
            }
        }
    
        if (!removed) {
            ++i;
        }
    }


            death = false;
            other = false;
            TextBox = false;
            health = 1;
            playerX = 16000;
            playerY = 100;
            cameraX = 16000 - 270 * 1.5 + 24;
            cameraY = 100 - 180 * 1.5 + 16;
            beginning = false;

    GameObject Npc;
    Npc.x = playerX + 32 * 1.5 * -2.75;
    Npc.y = playerY + 32 * 1.5 * 7.85;
        Item object;
    object.x = Npc.x + 16 * 8 * 2;
    object.y = Npc.y + 16 * 1.5 * 2;
    object.type = "Glove";
    object.texture = loadTexture("Images/Glove.png");
    object.expire = false;
    Items.push_back(object);

        }


        } else if (Game_End_I_Want_A_Long_Variable_Name) {
        glBindTexture(GL_TEXTURE_2D, end_credit.texture);
        float u0 = 0;
        float u1 = 1;
        float v0 = 0.0f;
        float v1 = 1.0f;
        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
                
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (!pick) {
            DeathTitleToggle = true;
        }
        if (pick && DeathTitleToggle){
            Dialoge.erase(Dialoge.begin() + 2);
            Game_End_I_Want_A_Long_Variable_Name = false;
    objects.erase(objects.begin(), objects.end());
    Items.erase(Items.begin(), Items.end());
    currentGems.erase(currentGems.begin(), currentGems.end());
    shell = 21;
    WhiskerEncounterBefore = 0;
    playerObj.x = playerX;
    playerObj.y = playerY;
    playerObj.hitboxWidth = 8;
    playerObj.hitboxHeight = 16;
    playerObj.animation = player;
    playerObj.frameIndex = 0;
    playerObj.animationTimer = 0.0f;
    playerObj.facingLeft = false;
    playerObj.relaps = 0.0f;
    playerObj.health = 2000000000;
    playerObj.alive = true;
    playerObj.knockback = 0;
    playerObj.name = "Zelda";

    vince.x = 200;
    vince.y = 50;
    vince.hitboxWidth = 32;
    vince.hitboxHeight = 32;
    vince.animation = &vinceWalk;
    vince.frameIndex = 0;
    vince.animationTimer = 0.0f;
    vince.facingLeft = true;
    vince.relaps = 0.0f;
    vince.health = 4;
    vince.alive = true;
    vince.knockback = 0;
    vince.name = "vince";
    
    ivine.x = 100;
    ivine.y = 800;
    ivine.hitboxWidth = 32;
    ivine.hitboxHeight = 32;
    ivine.animation = &flyDownLeft;
    ivine.frameIndex = 0;
    ivine.animationTimer = 0.0f;
    ivine.facingLeft = true;
    ivine.relaps = 85.0f;
    ivine.health = 16;
    ivine.alive = true;
    ivine.knockback = 0;
    ivine.name = "ivine";
    ivine.height = 30;

    domer.x = 100;
    domer.y = 800;
    domer.hitboxWidth = 32;
    domer.hitboxHeight = 32;
    domer.animation = &domerOp;
    domer.frameIndex = 100;
    domer.animationTimer = 0.0f;
    domer.facingLeft = true;
    domer.relaps = 85.0f;
    domer.health = 4.0f;
    domer.alive = true;
    domer.knockback = 0;
    domer.name = "domer";
    
    objects.push_back(playerObj);
    objects.push_back(vince);
    vince.health += 18;
    


    enemyRand(1, -200, 700, ivine, vince, domer);
    enemyRand(rand(2,3), 1200, 825, ivine, vince, domer);
    enemyRand(4, -1050, 725, ivine, vince, domer);
    enemyRand(rand(4,7), 400, 2000, ivine, vince, domer);
    enemyRand(rand(4,7), 140, 2400, ivine, vince, domer);
    enemyRand(rand(4,7), -850, 2200, ivine, vince, domer);
    enemyRand(rand(4,7), 450, 2950, ivine, vince, domer);
    enemyRand(rand(4,7), 1200, 3000, ivine, vince, domer);
    
    Item gemma;
    gemma.x = -410;
    gemma.y = 400;
    gemma.type = "Ruby";
    gemma.texture = loadTexture("Images/Ruby.png");
    gemma.expire = false;
    Items.push_back(gemma);
    gemma.x = -380;
    gemma.y = 360;
    gemma.type = "Silver";
    gemma.texture = loadTexture("Images/Silver.png");
    Items.push_back(gemma);

    

    
    for (objectSize = 1; objectSize < 1024; objectSize++) {
    Item object;
    float gemtype = rand(1,5);
    object.x = (((int)objectSize % 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    object.y = ((int)(objectSize / 32) * 32 * 30 + (32 * rand(0 , 29))) - 16 * 32 * 30;
    if (gemtype == 1) {
        object.texture = loadTexture("Images/Gold.png");
        object.type = "Gold";
    } else if (gemtype == 2) {
        object.texture = loadTexture("Images/Silver.png");
        object.type = "Silver";
    } else if (gemtype == 3) {        
        object.texture = loadTexture("Images/Ruby.png");
        object.type = "Ruby";
    } else if (gemtype == 4) { 
        object.texture = loadTexture("Images/Blue.png");
        object.type = "Blue";
    } else if (gemtype == 5) {        
        object.texture = loadTexture("Images/Parmesan.png");
        object.type = "Parmesan";
    }
            object.expire = false;
    Items.push_back(object);
    }
    

    for (auto i = Items.begin(); i != Items.end(); ) {
        bool removed = false;
    
        for (auto& wall : Walls) {
            if (wallCollision(i->x, i->y, 4, 4, wall.x, wall.y, wall.hitboxX, wall.hitboxY)) {
                i = Items.erase(i); 
                removed = true;
                break;
            }
        }
    
        if (!removed) {
            ++i;
        }
    }


            death = false;
            other = false;
            TextBox = false;
            health = 1;
            playerX = 16000;
            playerY = 100;
            cameraX = 16000 - 270 * 1.5 + 24;
            cameraY = 100 - 180 * 1.5 + 16;
            beginning = false;

    GameObject Npc;
    Npc.x = playerX + 32 * 1.5 * -2.75;
    Npc.y = playerY + 32 * 1.5 * 7.85;
        Item object;
    object.x = Npc.x + 16 * 8 * 2;
    object.y = Npc.y + 16 * 1.5 * 2;
    object.type = "Glove";
    object.texture = loadTexture("Images/Glove.png");
    object.expire = false;
    Items.push_back(object);
        }

        }else if (Shop3) {

        glBindTexture(GL_TEXTURE_2D, Shop3Tex.texture);
        float frameWidth = 1.0f / 4.0f;
        float u0 = shopKey * frameWidth;
        float u1 = u0 + frameWidth;
        float v0 = 0.0f;
        float v1 = 1.0f;
        quadVertices[2]  = u0; 
        quadVertices[3]  = v0;
        quadVertices[6]  = u1; 
        quadVertices[7]  = v0;
        quadVertices[10] = u1; 
        quadVertices[11] = v1;
        quadVertices[14] = u0; 
        quadVertices[15] = v1;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, 0.0f));
        model = glm::scale(model, glm::vec3(270 * 3, 180 * 3, 1.0f));
                
        glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            if (pick && DialogStateNext == 67 && TextBox) {
                TextBox = false;
                other = false;
                Shop3 = false;
                DialogStateNext = 0;
                Dialoge.erase(Dialoge.begin() + 3);
                Dialoge.erase(Dialoge.begin() + 2);
                Mix_HaltMusic();
                pickCoolDown = 1;
            }
            
            if (pick && DialogStateNext == 1 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                sen.sentence = "I never get vistors around here.";
                sen.placeX = 255; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 2 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "Sorry if I'm a little scatterbrained.";
                sen.placeX = 220; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 3 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "I'm Mairzy, the proprietar of this establishment.";
                sen.placeX = 150; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 4 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 3;
                sen.sentence = "I sever cakes and deserts.";
                sen.placeX = 290; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 5 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Anything you can see is yours. Go crazy.";
                sen.placeX = 218; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 6 && TextBox) {
                DialogStateNext = 8;
            }
            if (pick && DialogStateNext == 7 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = " What      might you be in for today?";
                sen.placeX = 270; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (DialogStateNext == 8 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;

                selectKey = std::clamp(selectKey, 1.0f, 4.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "talk";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "buy";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 3) {
                    sen.sentence = "leave";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "sell";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }

                                if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }

                Dialoge.push_back(sen);

                
                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }

                if (DialogStateNext == 9){
                
                    if (selectKey == 1) {
                        DialogStateNext = 10;
                    } else if (selectKey == 2) {
                        DialogStateNext = 20;
                    }  else if (selectKey == 3) {
                        DialogStateNext = 66;
                    } else {
                        DialogStateNext = 40;
                    }
                        
                }

             if (pick && DialogStateNext == 66 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "I'll see you around!";
                sen.placeX = 320; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 10 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "sure, what's up?";
                sen.placeX = 310; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }



            if (DialogStateNext == 11 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;

                selectKey = std::clamp(selectKey, 1.0f, 4.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "Who are you?";
                    sen.placeX = 355; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "Getting out of here.";
                    sen.placeX = 310; 
                    sen.placeY = 70;
                    sen.size = 16;
                }  else if (selectKey == 3) {
                    sen.sentence = "The God Thing.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "Back";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }
                                if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }

                Dialoge.push_back(sen);

                
                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }


            if (DialogStateNext == 12){
                
                    if (selectKey == 1) {
                        DialogStateNext = 13;
                    } else if (selectKey == 2) {
                        DialogStateNext = 23;
                    }  else if (selectKey == 3) {
                        DialogStateNext = 33;
                    } else {
                        DialogStateNext = 8;
                    }
                        
                }

            if (pick && DialogStateNext == 13 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "awww, little 'ol me?";
                sen.placeX = 290; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 14 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "shucks, well...";
                sen.placeX = 335; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 15 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "I'm not much, single mother of a few hundered.";
                sen.placeX = 180; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }
            if (pick && DialogStateNext == 16 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "Baker, Painter, I'm just a normal old lady.";
                sen.placeX = 200; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 23 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "oh, getting out is very much tricky.";
                sen.placeX = 245; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 24 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "I guess you'd leave through the trainstation to orbital eight.";
                sen.placeX = 90; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 25 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 2;
                sen.sentence = "But, I wouldn't recommend it.";
                sen.placeX = 260; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 26 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "The tickets are... steep.";
                sen.placeX = 280; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 27 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "The only man with that much money is Whiskers GoodBoy.";
                sen.placeX = 115; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 28 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 2;
                sen.sentence = "Honestly, it'd probably do him better with the ticket anyway.";
                sen.placeX = 100; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 33 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "The God thing.";
                sen.placeX = 340; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 34 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "They're nice, I met them when I was six.";
                sen.placeX = 190; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 35 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 2;
                sen.sentence = "Seeing the way things are going...";
                sen.placeX = 230; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 36 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 2;
                sen.sentence = "I think the king's lying about actually serving their word.";
                sen.placeX = 110; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 20 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 0;
                sen.sentence = "Again, you can just take whatever you want.";
                sen.placeX = 200; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (pick && DialogStateNext == 40 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;
                sen.sentence = "That wouldn't be too sanitary.";
                sen.placeX = 240; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (DialogStateNext == 21 && TextBox) {

                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 1;

                selectKey = std::clamp(selectKey, 1.0f, 3.0f);
                Sentence sen;
                sen.exception = true;

                if (selectKey == 1) {
                    sen.sentence = "Strawberry Lovers Desert.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else if (selectKey == 2) {
                    sen.sentence = "Instant death noodles.";
                    sen.placeX = 360; 
                    sen.placeY = 70;
                    sen.size = 16;
                } else {
                    sen.sentence = "Back";
                    sen.placeX = 390; 
                    sen.placeY = 70;
                    sen.size = 16;
                }

                                if (KeyMove) {
                    Mix_PlayChannel(-1, beep, 0);
                    }

                Dialoge.push_back(sen);

                    glBindTexture(GL_TEXTURE_2D, selectOpt.texture);
                    float u0 = 0.0f;
                    float u1 = 1.0f;
                    float v0 = 0.0f;
                    float v1 = 1.0f;
                    quadVertices[2]  = u0; 
                    quadVertices[3]  = v0;
                    quadVertices[6]  = u1; 
                    quadVertices[7]  = v0;
                    quadVertices[10] = u1; 
                    quadVertices[11] = v1;
                    quadVertices[14] = u0; 
                    quadVertices[15] = v1;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
                    model = glm::mat4(1.0f);                
                    model = glm::translate(model, glm::vec3(405, 100, 0.0f));
                    model = glm::scale(model, glm::vec3(16, 32, 1.0f));
                            
                    glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
                    glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
                    
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
             }


            if (DialogStateNext == 22){

                             
                    if (!(selectKey == 3)) {
                        DialogStateNext = 21;
                    if (selectKey == 1) {
                        health = 100;
                    }  else if (selectKey == 2) {
                        health = 0;
                        TextBox = false;
                        other = false;
                        Shop3 = false;
                        DialogStateNext = 0;
                        Dialoge.erase(Dialoge.begin() + 3);
                        Dialoge.erase(Dialoge.begin() + 2);
                        Mix_HaltMusic();
                        pickCoolDown = 1;
                    } 
                } else {
                        DialogStateNext = 8;
                    }
                        
                } 

            if (pick && DialogStateNext == 44 && TextBox) {
                Sentence sen;
                sen.exception = true;
                Dialoge.erase(Dialoge.begin() + 3);
                shopKey = 4;
                sen.sentence = "sorry, I don't think you have the space.";
                sen.placeX = 220; 
                sen.placeY = 70;
                sen.size = 16;
                Dialoge.push_back(sen);
            }

            if (DialogStateNext == 45){
                        DialogStateNext = 21;
                }
            
            if (DialogStateNext == 17 || DialogStateNext == 29 ||  DialogStateNext == 37){
                        DialogStateNext = 11;
                }
            if (DialogStateNext == 41){
                        DialogStateNext = 8;
                }



        
            
            
    }
        


        for(auto& sen : Dialoge) {
if ((!title && !Terminal && !Shop1 && !death && !Game_End_I_Want_A_Long_Variable_Name && !Shop3) || sen.placeX == 24 || sen.exception == true) {
        float kernAmount = 0;
        float kern = 0;
        glBindTexture(GL_TEXTURE_2D, chara.texture);
        float SenI = 0;
        for (char c : sen.sentence) {
            size_t index = fontChars.find(c);
            float frameWidth = 1.0f / chara.totalFrames;
            float u0 = index * frameWidth;
            float u1 = u0 + frameWidth;
            float v0 = 0.0f;
            float v1 = 1.0f;
            quadVertices[2]  = u0; 
            quadVertices[3]  = v0;
            quadVertices[6]  = u1; 
            quadVertices[7]  = v0;
            quadVertices[10] = u1; 
            quadVertices[11] = v1;
            quadVertices[14] = u0; 
            quadVertices[15] = v1;
    
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
            
            if (!kernAmount == 0){
                kern += kernAmount;
                kernAmount = 0;
            }

            if (c == '!' || c == '.' || c == '?' || index == 62) {
                kern -= sen.size / 10;
                kernAmount -= sen.size / 5;
            } else if (c == ',' ) {
                kernAmount -= sen.size / 4;
            }
            if (c == 'm' || c == 'M' || c == 'W' || c == 'w' ){
                kern += sen.size / 6;  
                kernAmount = sen.size / 6;
            }
            

            
            glm::mat4 model = glm::mat4(1.0f);
    
            model = glm::translate(model,
                glm::vec3(sen.placeX + kern + SenI * sen.size / 1.6, sen.placeY, 0.0f));
    
            model = glm::scale(model,
                glm::vec3(sen.size, sen.size, 1.0f));
    
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
            glUniform1i(spriteLoc, 0);

            glUniform1i(useTextureLoc, 1);
    
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
            SenI++;

        }
}

}

        if (Terminal) {
            Dialoge.erase(Dialoge.end());
        }

                if (tutorial == 1){
        Tutpulse += deltaTime * 40;
        if (Tutpulse >= 40) {
            Tutpulse = 0;
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(270 * 1.5 - Tutpulse - 37.5, 320 - Tutpulse, 0.0f));
        model = glm::scale(model, glm::vec3(10, barHeight + Tutpulse * 2, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(270 * 1.5 + barWidth - 37.5 - 10 + Tutpulse, 320 - Tutpulse, 0.0f));
        model = glm::scale(model, glm::vec3(10, barHeight + Tutpulse * 2, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(270 * 1.5 - Tutpulse - 37.5 + 10, 320 + barHeight - 10 + Tutpulse, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth + Tutpulse * 2 - 20 , 10, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(270 * 1.5 - Tutpulse - 37.5 + 10, 320 - Tutpulse, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth + Tutpulse * 2 - 20 , 10, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1, (-Tutpulse / 40) + 1);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(270 * 1.5 - 37.5, 320, 0.0f));
        model = glm::scale(model, glm::vec3(barWidth, barHeight, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.2f, 0.2f, 0.2f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 37.5, 320, 0.0f));
        model2 = glm::scale(model2, glm::vec3(barWidth, barHeight * 0 / 100, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


} else if (tutorial == 2) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



} else if (tutorial == 4) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 0.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



} else if (tutorial == 5) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 0.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



} else if (tutorial == 6) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



} else if (tutorial == 7) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.3f, 0.3f, 0.3f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



} else if (tutorial == 8) {
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(270 * 1.5  - 4, 360, 0.0f));
        model2 = glm::scale(model2, glm::vec3(8, 8, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
        
        
                glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



}
        
        if (record) {

        int width = 270 * 3;   
        int height = 180 * 3;   
        
        std::vector<unsigned char> pixels(width * height * 3);
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        for (int y = 0; y < height / 2; y++)
        {
            for (int x = 0; x < width * 3; x++)
            {
                std::swap(
                    pixels[y * width * 3 + x],
                    pixels[(height - 1 - y) * width * 3 + x]
                );
            }
}
            


            

            static int frameNumber = 0;

            char filename[64];
            sprintf(filename, "Frames/frame_%05d.png", frameNumber++);
            
            stbi_write_png(filename, width, height, 3, pixels.data(), width * 3);
                    }


// ffmpeg -framerate 50 -i Frames/frame_%05d.png -c:v libx264 -pix_fmt yuv420p output.mp4
            //std::cout << playerX << ", " << playerY << "\n"; 
        
                
        SDL_GL_SwapWindow(window);


    }


    
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    glDeleteProgram(Base);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}