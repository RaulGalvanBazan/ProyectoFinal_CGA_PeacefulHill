#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <array>
#include <sstream>
#include <iterator>
#include <fstream>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

Shader shaderPrueba;
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth, shaderDepth2;
//Shader para las particulas de fuego
Shader shaderParticlesFire;

//Cámaras
std::shared_ptr<FirstPersonCamera> FPcamera(new FirstPersonCamera());
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
//Para seleccionar el tipo de camara
bool cameraType = false; //false = TPCamera, true = FPCamera
bool cameraLock = false;
bool keyLock = false;
float distanceFromTarget = 5.0;
float fpcYaw, fpcPitch;

float fx = 0, fy = 0, fz = 0;

Box boxCollider;
Sphere sphereCollider(10, 10);
Sphere skyboxSphere(20, 20);
Box boxViewDepth;

ShadowBox* shadowBox;

// Models complex instances
Model modelRock;
Model modelGate;
Model modelGate2;
Model modelFence00, modelFence01, modelFence02, modelFence03, modelFence04;
Model modelTree00, modelTree01, modelTree02, modelPine00, modelPine01;
Model modelStump;
Model modelHealth;
Model modelRock00, modelRock01;
Model modelLog00;
Model modelGrass;
Model modelCube;

int healtPicks = 0;

// Model animate instance

Model wendigoAnimate;
bool wendigoAttack = false;
bool wendigoRun = false;
bool wendigoHit = false;
bool wendigoRender = true;
Model heatherAnimate;
//Variable de selección de animación
int heatherAnimation = 0;
//Vida de heather
int heatherHealth = 100;
//Munición
int municion = 0.0;

bool pickUp = false;

float nieblaLL, nieblaD;
float screenView = 100.0;

// Terrain model instance
Terrain terrain(-1, -1, 200, 24, "../Textures/heightmap4.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureParticleFireID, texId;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

//std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
//		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
//		"../Textures/mp_bloodvalley/blood-valley_up.tga",
//		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
//		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
//		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

//std::string fileNames[6] = { "../Textures/NightPath/posx.jpg",
//        "../Textures/NightPath/negx.jpg",
//        "../Textures/NightPath/posy.jpg",
//        "../Textures/NightPath/negy.jpg",
//        "../Textures/NightPath/posz.jpg",
//        "../Textures/NightPath/negz.jpg" };

std::string fileNames[6] = { "../Textures/night/grimmnightft.tga",
        "../Textures/night/grimmnightbk.tga",
        "../Textures/night/grimmnightup.tga",
        "../Textures/night/grimmnightdn.tga",
        "../Textures/night/grimmnightrt.tga",
        "../Textures/night/grimmnightlf.tga" };



bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;
//glm::vec3 lightPos;

// Model matrix definitions
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 matrixModelGate = glm::mat4(1.0);
glm::mat4 matrixModelFence00 = glm::mat4(1.0);
glm::mat4 matrixModelFence01 = glm::mat4(1.0);
glm::mat4 matrixModelFence02 = glm::mat4(1.0);
glm::mat4 matrixModelFence03 = glm::mat4(1.0);
glm::mat4 matrixModelFence04 = glm::mat4(1.0);
glm::mat4 matrixModelTree00 = glm::mat4(1.0);
glm::mat4 matrixModelStump = glm::mat4(1.0);
glm::mat4 matrixModelRock00 = glm::mat4(1.0);
glm::mat4 matrixModelRock01 = glm::mat4(1.0);
glm::mat4 matrixModelLog00 = glm::mat4(1.0);
glm::mat4 matrixModelGrass = glm::mat4(1.0f);

glm::mat4 modelMatrixWendigo = glm::mat4(1.0f);
glm::mat4 modelMatrixHeather = glm::mat4(1.0f);
glm::mat4 modelMatrixHeatherBody;
bool SaveFrame = false, aviableSave = true, modelChange = false;

int wendigoRunning = 0;
int enableChange = 0;

int modelSelected = 1;
bool enableCountSelected = true;

unsigned int amount, pine00Amount, pine01Amount;
bool fogOn = false, enableFog = true;
bool showWall = false;
float oriz = 0.0;
float orix = 0.0;
float orixH = 0.0;

bool openGate = true;
bool unlockGate = false;
int gateAnimation = 0;
// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

/********************************************************************
Posicion, escala y orientacion de los diferentes objetos de la escena
*********************************************************************/

// Fence positions
float fenceHeight;
glm::vec3 fenceScale = glm::vec3(0.028, 0.0165, 0.04);
std::vector< glm::vec2>fence00Position = { 
    glm::vec2(1.86485, -1.0511), glm::vec2(-1.50622, 25.8365), glm::vec2(3.30794, 38.7327), glm::vec2(-2.79038, 64.2511), glm::vec2(20.128, 80.08),
    glm::vec2(32.1414, 57.7366), glm::vec2(41.3933, 18.0601), glm::vec2(39.5491, -12.9148)
    };

std::vector< glm::vec2>fence01Position = {
    glm::vec2(-0.652102, 4.05797), glm::vec2(2.71087, 29.3977), glm::vec2(-0.514344, 41.1714), glm::vec2(0.666232, 75.1669), glm::vec2(24.6944 , 76.6091),
    glm::vec2(29.5509, 52.6162), glm::vec2(27.543, 27.6974), glm::vec2(46.2644, -4.36723), glm::vec2(33.9819, -14.1432), glm::vec2(14.8981, -11.4186)
};

std::vector< glm::vec2>fence02Position = {
    glm::vec2(-0.849043, 9.68018), glm::vec2(-3.51202, 51.8872), glm::vec2(4.2834, 79.6492), glm::vec2(14.9357, 81.9379), glm::vec2(26.4688, 47.9437),
    glm::vec2(32.0711, 24.3876), glm::vec2(9.80979, -8.94707)
};

std::vector< glm::vec2>fence03Position = {
    glm::vec2(-0.774681, 15.0816), glm::vec2(-2.40572, 46.4177), glm::vec2(9.33416, 81.9427), glm::vec2(28.7167, 72.4988), glm::vec2(23.8459, 42.8985),
    glm::vec2(23.1814, 37.562), glm::vec2(46.9228, 6.63597), glm::vec2(43.6721, -9.44977)
};

std::vector< glm::vec2>fence04Position = {
    glm::vec2(-1.83894, 20.2343), glm::vec2(-1.61441, 69.7806), glm::vec2(31.3878, 67.7861), glm::vec2(24.3505, 32.3623), glm::vec2(36.91, 21.5855),
    glm::vec2(47.1732, 1.17982), glm::vec2(5.43382, -5.51681)
};

float fence00Orientation[] = { -124.5, -60.4, -185.943, -78.9134, 34.7114, 114.1, 43.8209, 155.642};
float fence01Orientation[] = { -103.0, -12.3, -106.029, -56.7808, 40.0226, 119.878, 45.682, 111.669, 177.613, 200.9 };
float fence02Orientation[] = { -78.1, -90.2302, -40.3273, 5.56088, 126.234, 29.3751, -149.932};
float fence03Orientation[] = { -91.9, -105.926, -5.1798, 50.5955, 112.845, 89.5456, 86.515, 121.844 };
float fence04Orientation[] = { -107, -71.7087, 71.2682, 67.0549, 31.8903, 88.3046, -134.437 };

float treeHeight;

std::vector<glm::vec2>tree00Position = {
    glm::vec2(-74.4418, -23.8715)
};
std::vector<glm::vec3>tree00Scale = {
    glm::vec3(0.13958)
};
std::vector<float>tree00Orientation = { 211.305 };


std::vector<glm::vec2>tree01Position = {
    glm::vec2(-0.052217, 24.374), glm::vec2(45.2101, -55.9837)
};

std::vector<glm::vec3>tree01Scale = {
    glm::vec3(0.148011), glm::vec3(0.167767)
};

std::vector<float>tree01Orientation = { 80.1517, 122.794 };

std::vector<glm::vec2>tree02Position = {
    //glm::vec2(15.2902, 3.38775)
};

std::vector<glm::vec3>tree02Scale = {
    glm::vec3(0.146165)
};

std::vector<float>tree02Orientation = { 122.794};

float logH;
std::vector<glm::vec3>log00Position = {
    glm::vec3(32.6602, 0, 62.9065), glm::vec3(31.5247, 0, 62.9065), glm::vec3(31.4078, 1.01501, 61.8505), glm::vec3(31.4078, 1.01501, 63.1775),
};

std::vector<glm::vec3>log00Scale = {
    glm::vec3(0.022157, 0.021735, 0.016859), glm::vec3(0.022157, 0.021735, 0.016859), glm::vec3(0.016774, 0.016454, 0.012763), glm::vec3(0.016774, 0.016454, 0.012763)
};

std::vector<glm::vec3>log00Orientation = {
    glm::vec3(0.0), glm::vec3(0.0), glm::vec3(90-133.976, 88.1801, -1.75684), glm::vec3(90-133.976, 90.1801, -1.35684)
};

std::vector<glm::vec3>rock00Position = {
    glm::vec3(29.0776, 0, -11.7502), glm::vec3(25.2882, 0.567088, -10.7507), glm::vec3(20.5452, 0.567088 , -11.0072),
    glm::vec3(-83.8232, 0, -99.4412), glm::vec3(-72.1543, 0, -99.4594), glm::vec3(-60.9811, 0, -99.8741),
    glm::vec3(-63.5823, 16.055, -101.971), glm::vec3(-77.4625, 16.5559, -102.243), glm::vec3(-85.9521, 6.26651, -100.052)
};

std::vector<glm::vec3>rock00Orientation = {
    glm::vec3(0, -250.313, 0), glm::vec3(-28.7718, -327.913, -23.7253 ), glm::vec3(-28.7718, -19.6365, -23.7253),
    glm::vec3(0, -220.313, 0), glm::vec3(0, -137.557, 0), glm::vec3(0, -304.068, 0),
    glm::vec3(-198.049, -411.391, -6.33), glm::vec3(-198.049, -288.681, -6.32585), glm::vec3(-90-331.32, -274.542, -26.6814)
};

std::vector<glm::vec3>rock00Scale = {
    glm::vec3(2.12353), glm::vec3(2.41885), glm::vec3(2.41885), glm::vec3(6.86234), glm::vec3(6.86234), glm::vec3(6.86234),
    glm::vec3(6.86234), glm::vec3(6.86234), glm::vec3(7.04065)
};

std::vector<glm::vec3> pine00Position = {
    glm::vec3(-12.9699 , 0, 93.7264), glm::vec3(-3.24272, 0, 94.2337), glm::vec3(7.27469, 0, 97.4994), glm::vec3(17.9623, 0, 95.5667),
    glm::vec3(29.8612, 0, 97.06), glm::vec3(43.4792, 0, 95.9041), glm::vec3(-8.43674, 0, 99.3748), glm::vec3(54.0759, 0, 93.8451), 
    glm::vec3(63.5917, 0, 98.0441), glm::vec3(70.7295, 0, 95.4438), glm::vec3(79.0203, 0, 99.0305), glm::vec3(87.7411, 0, 93.2516), 
    glm::vec3(90.6727, 0, 78.2788), glm::vec3(91.1834, 0, 58.7951), glm::vec3(89.7945, 0, 67.8261), glm::vec3(90.9228, 0, -29.3374), 
    glm::vec3(92.5365, 0, 10.8291), glm::vec3(90.188, 0, -55.7001), glm::vec3(90.188 , 0, 32.7666), glm::vec3(-60.7611 , 0, 12.0447), 
    glm::vec3(-59.7816 , 0, 22.7754), glm::vec3(-62.3228 , 0, 29.8715), glm::vec3(-62.3661 , 0, 55.1593), glm::vec3(-59.4373 , 0, 39.8111), 
    glm::vec3(-62.2339 , 0, 45.6151), glm::vec3(-88.8031 , 0, 61.9732), glm::vec3(-90.6806 , 0, 75.5413), glm::vec3(-89.1637 , 0, 29.4611),
    glm::vec3(-61.8051 , 0, 94.0391), glm::vec3(-88.7627 , 0, 11.9085), glm::vec3(-85.7378 , 0, 97.6864), glm::vec3(-66.022 , 0, 99.9965),
    glm::vec3(-72.9099 , 0, 94.1122 )
};
std::vector<float> pine00Orientation = {
    0, -51.1007, -108.673, -156.524, 164.552, 207.208, 207.208, 296.237, 18.844, 60.0859, 165.404, 9.75213, 164.552, 33.052, 78.951,
    60.5087, 108.903, 149.372, 149.372, 149.372, 191.516, 97.7631, 191.516, 149.741, 27.1449, 1.6549, 7.5219, 7.5219, 7.5219, 13.5304,
    141.362, 83.8955, 27.1307
};
std::vector<float> pine00Scale = {
    1.6, 1.75, 1.55, 1.65, 1.95, 1.7, 1.55, 1.75, 1.6, 1.8, 1.95, 1.6, 1.6, 1.406, 1.54422, 1.64085, 1.77449, 1.87, 2.02343, 2.02344,
    1.65498, 1.45568, 1.36804, 1.54485, 1.66532, 1.37798, 1.73855, 1.49399, 1.6014, 2.12063, 2.12063, 1.78394, 1.95531, 1.56576
};

std::vector<glm::vec3> pine01Position = {
    glm::vec3(-12.3211, 0, 99.5775), glm::vec3(-0.122257, 0, 99.5775), glm::vec3(11.7447, 0, 99.5775), glm::vec3(24.6871 , 0, 99.5775), 
    glm::vec3(37.111 , 0, 99.5775), glm::vec3(50.1395 , 0, 99.5775), glm::vec3(60.3369 , 0, 97.2415), glm::vec3(72.6479 , 0, 99.5775), 
    glm::vec3(85.3402 , 0, 99.5775), glm::vec3(92.24 , 0, 99.5775), glm::vec3(-20.1814, 0, 110.059), glm::vec3(-5.1814 , 0, 110.059),
    glm::vec3(10.1814 , 0, 110.059), glm::vec3(25.1814 , 0, 110.059), glm::vec3(40.1814 , 0, 110.059), glm::vec3(55.1814 , 0, 110.059), 
    glm::vec3(70.1814 , 0, 110.059), glm::vec3(85.1814 , 0, 110.059), glm::vec3(100.1814 , 0, 110.059)
};

std::vector<float> pine01Orientation = {
    45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 0, 0, 0, 0, 0, 0, 0, 0
};

std::vector<float> pine01Scale = {
    1.896, 2.196, 1.996, 1.996, 2.196, 1.896, 1.996, 2.156, 1.996, 2.196, 1.896, 1.796, 1.86, 1.906, 2.196, 1.896, 1.696, 1.896, 1.996
};

std::vector<glm::vec3> wallPosition = {
    glm::vec3(-17.839, 0.0, 35.5717), glm::vec3(39.957, 0.0, 93.3496), glm::vec3(90.1064, 0.0, 0.0), glm::vec3(50.3413, 0.0, -93.066),
    glm::vec3(-0.469382, 0.0, -80.7942), glm::vec3(-3.02573, 0.0, -67.6448), glm::vec3(-3.95038, 0.0, -58.44), glm::vec3(-15.1186, 0.0, -56.0558), 
    glm::vec3(-30.6915, 0.0, -60.0411), glm::vec3(-49.8387, 0.0, -59.6905), glm::vec3(-57.2818, 0.0, -68.5883), glm::vec3(-57.4733, 0.0, -88.9536), 
    glm::vec3(-87.831, 0.0, 0.0), glm::vec3(-62.8381, 0.0, 32.3806), glm::vec3(-57.8511, 0.0, -39.3821), glm::vec3(-46.6937, 0.0, -45.8592),
    glm::vec3(-32.2281, 0.0, -40.6566), glm::vec3(-22.6251, 0.0, -31.2376), glm::vec3(-75.3321, 0.0, 76.5),
    glm::vec3(-72.0858, 0.0, -91.9448),
    glm::vec3(-3.02138, 0.0, 57.9526), glm::vec3(32.6501, 0.0, 62.9555), glm::vec3(45.0907, 0.0, 12.8559), glm::vec3(24.3218, 0.0, -13.474),
    //arboles
    glm::vec3(-74.6786, 0.0, -24.023), glm::vec3(-0.06056, 0.0, 24.2005), glm::vec3(45.0175, 0.0, -56.1638)
};

std::vector<glm::vec3> wallScale = {
    glm::vec3(0.422308, 8, 64.2349), glm::vec3(0.349767, 8, 56.611), glm::vec3(0.349767, 8, 92.3093), glm::vec3(0.349767, 8, 44.2837),
    glm::vec3(0.349767, 8, 13.7763), glm::vec3(0.349767, 8, 4.58778), glm::vec3(0.349767, 8, 8.06477), glm::vec3(0.349767, 8, 7.55969), 
    glm::vec3(0.349767, 8, 9.34796), glm::vec3(0.349767, 8, 9.70394), glm::vec3(0.349767, 8, 9.35838), glm::vec3(0.349767, 8, 11.0363), 
    glm::vec3(0.349767, 8, 100.0), glm::vec3(0.349767, 8, 66.8038), glm::vec3(0.349767, 8, 5.05141), glm::vec3(0.349767, 8, 9.01047), 
    glm::vec3(0.349767, 8, 8.49928), glm::vec3(0.349767, 8, 4.98028), glm::vec3(0.349767, 8, 12.0046), 
    glm::vec3(0.349767, 8, 15.2092),
    glm::vec3(0.349767, 4, 3.31086), glm::vec3(0.349767, 4, 2.25026), glm::vec3(0.349767, 4, 3.5486), glm::vec3(0.349767, 4, 6.72889),
    //arboles
    glm::vec3(0.910376, 6.6995, 1.05417), glm::vec3(0.887673, 6.53243, 1.02788), glm::vec3(0.936939, 6.53243, 1.02788)
};

std::vector<float> wallOrientation = {
    0, 90, 0, 90, 146.964, 232.842, 142.273, 54.1401, 88.3979, 93.8601, -18.5669, 3.32544, 0, 0, 156.574, 100.146, 37.8525, 58.2515, 90,
    90,
    4.19419, -3.80371, 153.992, 97.384,
    //arboles
    0, 0, 0
};

std::vector<glm::vec3> healthPosition = {
    glm::vec3(-75.457, 0.665707, -22.0684), glm::vec3(13.4959, 0.665707, 74.2962), glm::vec3(88.0835, 0.665707, -88.7199)
};

std::vector<glm::vec3> healthOrientation = {
    glm::vec3(141.498-90, 0, 0), glm::vec3(141.498 - 90, 0, 0), glm::vec3(0, 90, 0)
};

std::vector<bool> healthRender = {
    true, true, true
};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
        {"fire", glm::vec3(0.0, 0.0, 0.0)}
};

double deltaTime;
double currTime, lastTime;
double stepCurrTime, stepLastTime;
double gateCurrTime, gateLastTime;
GLuint depthMap, depthMapFBO;

GLuint VAO;
GLuint buffer;

GLuint pine00_VAO;
GLuint pine_buffer;

GLuint pine01_VAO;
GLuint pine01_buffer;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2500;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.15, particleLifetime = 0.45;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;
glm::vec3 accel = glm::vec3(0.0f, 0.0f, -35.0f);
bool accelUp = true;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 7
#define NUM_SOURCES 7
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -40.2621, 0.0, 49.776 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 87.9458, 0.0 , -77.7962 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 85.8691 , 0.0, 109.578 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
//Source 3
ALfloat source3Pos[] = { 76.8517, 0.0, -98.7569 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
//Source 3
ALfloat source4Pos[] = { 76.8517, 0.0, -98.7569 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
ALfloat source5Pos[] = { 76.8517, 0.0, -98.7569 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };

ALfloat source6Pos[] = { 76.8517, 0.0, -98.7569 };
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint soundBuffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true, false, false, false, true };
//std::vector<bool> sourcesPlay = { false, false, false, false, false, false, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
    int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareDepthScene();
void prepareScene();
void renderScene(bool renderParticles = true);

void initParticleBuffersFire() {
    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, velBuf);    // velocity buffers
    glGenBuffers(2, age);       // age buffers

    // Allocate space for all buffers
    int size = nParticlesFire * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, age[1]);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

    // Fill the first age buffer
    std::vector<GLfloat> initialAge(nParticlesFire);
    float rate = particleLifetime / nParticlesFire;
    for (unsigned int i = 0; i < nParticlesFire; i++) {
        int index = i - nParticlesFire;
        float result = rate * index;
        initialAge[i] = result;
    }
    // Shuffle them for better looking results
    //Random::shuffle(initialAge);
    auto rng = std::default_random_engine{};
    std::shuffle(initialAge.begin(), initialAge.end(), rng);
    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);

    // Set up particle array 0
    glBindVertexArray(particleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, age[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // Set up particle array 1
    glBindVertexArray(particleArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, age[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    screenWidth = width;
    screenHeight = height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (bFullScreen)
        window = glfwCreateWindow(width, height, strTitle.c_str(),
            glfwGetPrimaryMonitor(), nullptr);
    else
        window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
            nullptr);

    if (window == nullptr) {
        std::cerr
            << "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
            << std::endl;
        destroy();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetWindowSizeCallback(window, reshapeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Init glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Failed to initialize glew" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Inicialización de los shaders
   /* shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
    shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
    shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
    shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");*/

    // Inicialización de los shaders
    shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
    shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
    shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
    shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
    shaderPrueba.initialize("../Shaders/prueba.vs", "../Shaders/multipleLights_shadow.fs");
    shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
    shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
    shaderDepth2.initialize("../Shaders/shadow_mapping_depth_instancing.vs", "../Shaders/shadow_mapping_depth.fs");
    shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });

    // Inicializacion de los objetos.
    skyboxSphere.init();
    skyboxSphere.setShader(&shaderSkybox);
    skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

    boxCollider.init();
    boxCollider.setShader(&shader);
    boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

    sphereCollider.init();
    sphereCollider.setShader(&shader);
    sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

    //boxViewDepth.init();
    //boxViewDepth.setShader(&shaderViewDepth);

    modelRock.loadModel("../models/rock/rock.obj");
    modelRock.setShader(&shaderMulLighting);
    
    modelGate.loadModel("../models/Gate/g6.fbx");
    modelGate.setShader(&shaderMulLighting);

    modelGate2.loadModel("../models/Gate/Gate.obj");
    modelGate2.setShader(&shaderMulLighting);

    modelFence00.loadModel("../models/fence/fence00.obj");
    modelFence00.setShader(&shaderMulLighting);

    modelFence01.loadModel("../models/fence/fence01.obj");
    modelFence01.setShader(&shaderMulLighting);

    modelFence02.loadModel("../models/fence/fence02.obj");
    modelFence02.setShader(&shaderMulLighting);

    modelFence03.loadModel("../models/fence/fence03.obj");
    modelFence03.setShader(&shaderMulLighting);

    modelFence04.loadModel("../models/fence/fence04.obj");
    modelFence04.setShader(&shaderMulLighting);

    //modelTree00.loadModel("../models/tree2/tree_mango_var01.obj");
    modelTree00.loadModel("../models/tree2/tree_mango_var02.obj");
    modelTree00.setShader(&shaderMulLighting);

    modelTree01.loadModel("../models/tree2/prueba2.obj");
    modelTree01.setShader(&shaderMulLighting);

    modelTree02.loadModel("../models/tree2/prueba2.obj");
    modelTree02.setShader(&shaderMulLighting);

    modelPine00.loadModel("../models/pine/pine_tree_model2.obj");
    modelPine00.setShader(&shaderPrueba);

    modelPine01.loadModel("../models/pine/pine_tree_fan.obj");
    modelPine01.setShader(&shaderPrueba);

    modelStump.loadModel("../models/stump/stump.obj");
    modelStump.setShader(&shaderMulLighting);

    modelLog00.loadModel("../models/logs/log.obj");
    modelLog00.setShader(&shaderMulLighting);

    modelRock00.loadModel("../models/rocks/rock000.obj");
    modelRock00.setShader(&shaderMulLighting);

    modelRock01.loadModel("../models/rocks/rock001.obj");
    modelRock01.setShader(&shaderMulLighting);

    modelGrass.loadModel("../models/grass/Grass_fbx/grass06.obj");
    modelGrass.setShader(&shaderPrueba);

    modelHealth.loadModel("../models/health_kit/health_kit.obj");
    modelHealth.setShader(&shaderMulLighting);

    modelCube.loadModel("../models/cube/cubo.obj");

    terrain.init();
    terrain.setShader(&shaderTerrain);
    terrain.setPosition(glm::vec3(100, 0, 100));

    //Wendigo
    wendigoAnimate.loadModel("../models/Wendigo/wendigoAnimado.fbx");
    wendigoAnimate.setShader(&shaderMulLighting);

    heatherAnimate.loadModel("../models/heather/heatherAnimate.fbx");
    heatherAnimate.setShader(&shaderMulLighting);

    camera->setPosition(glm::vec3(0.0, 0.2, 20.0));
    camera->setDistanceFromTarget(distanceFromTarget);
    camera->setSensitivity(0.25f);

    FPcamera->setPosition(glm::vec3(0.0, 1.0, 0.0));
    FPcamera->setDistanceFromTarget(distanceFromTarget);
    FPcamera->setSensitivity(5.0f);
    
    // Definimos el tamanio de la imagen
    int imageWidth, imageHeight;
    FIBITMAP *bitmap;
    unsigned char *data;

    // Carga de texturas para el skybox
    Texture skyboxTexture = Texture("");
    glGenTextures(1, &skyboxTextureID);
    // Tipo de textura CUBE MAP
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
        skyboxTexture = Texture(fileNames[i]);
        FIBITMAP *bitmap = skyboxTexture.loadImage(true);
        unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
            imageHeight);
        if (data) {
            glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, data);
        }
        else
            std::cout << "Failed to load texture" << std::endl;
        skyboxTexture.freeImage(bitmap);
    }

    // Definiendo la textura a utilizar
    Texture textureCesped("../Textures/cesped.jpg");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureCesped.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureCesped.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureCespedID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureCespedID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureCesped.freeImage(bitmap);

    
    // Definiendo la textura a utilizar
    Texture textureTerrainBackground("../Textures/forest2.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainBackground.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainBackground.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBackgroundID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainBackground.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainR("../Textures/grass3.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainR.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainR.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainRID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainR.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainG("../Textures/moss.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainG.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainG.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainGID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainG.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainB("../Textures/rockb.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainB.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainB.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainB.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainBlendMap("../Textures/blendMap2.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainBlendMap.loadImage(true);
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBlendMapID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainBlendMap.freeImage(bitmap);

    /*******************************************
     * Inicializacion del framebuffer para
     * almacenar el buffer de profunidadad
     *******************************************/
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    //Se leen las coordenadas para el pasto
    std::vector<glm::vec2> coordenadas;
    //std::ifstream infile("../models/grass/coords3.txt");
    std::ifstream infile("../textures/coords3.txt");
    float coordX, coordZ;
    while (infile >> coordX >> coordZ) {
        coordenadas.push_back(glm::vec2(coordX, coordZ));
    }

    std::cout << "size " << coordenadas.size() << std::endl;

    glm::mat4 model;
    amount = coordenadas.size();
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime());

    /*********************************************************************
    //Se generan las matrices de transformacion para los objetos de pasto
    *********************************************************************/
    int i = 0;
    for (std::vector<glm::vec2>::iterator it = coordenadas.begin(); it != coordenadas.end(); it++) {
        model = glm::mat4(1.0);
        //float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float xCoord = it->x;
        if (xCoord < -100) xCoord = -99.9;
        if (xCoord > 100) xCoord = 99.9;
        //r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float zCoord = it->y;
        if (zCoord < -100) zCoord = -99.9;
        if (zCoord > 100) zCoord = 99.9;
        float yCoord = terrain.getHeightTerrain(xCoord, zCoord);
        glm::vec3 grassNormal = terrain.getNormalTerrain(xCoord, zCoord);
        glm::vec3 xaxis = glm::vec3(model[0]);
        glm::vec3 zaxis = glm::normalize(glm::cross(xaxis, grassNormal));
        model = glm::translate(model, glm::vec3(xCoord, yCoord - 0.1, zCoord));
        //float r3 = 0.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        model[1] = glm::vec4(grassNormal, 0.0);
        model[2] = glm::vec4(zaxis, 0.0);
        float orientation = rand() % 360;
        model = glm::scale(model, glm::vec3(0.65, 3.0, 0.65));
        //model = glm::scale(model, glm::vec3(0.015, 0.019, 0.015));
        model = glm::rotate(model, glm::radians(orientation), glm::vec3(0, 1, 0));
        modelMatrices[i] = model;
        i++;
    }
    //Binding para los objetos de pasto
    glGenBuffers(1, &buffer); 
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < modelGrass.getMeshes().size(); i++)
    {
        VAO = modelGrass.getMeshes()[i]->getVAO();
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }

    /********************************************************
    **********************************************************/

    pine00Amount = pine00Position.size();
    //glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[pine00Amount];
    //srand(glfwGetTime());

    //Se generan las matrices de transformacion para los objetos de pino
    i = 0;
    for (std::vector<glm::vec3>::iterator it = pine00Position.begin(); it != pine00Position.end(); it++) {
        model = glm::mat4(1.0);
        float xCoord = it->x;
        float zCoord = it->z;
        float yCoord = terrain.getHeightTerrain(xCoord, zCoord);
        model = glm::translate(model, glm::vec3(xCoord, yCoord - 0.5, zCoord));
        model = glm::scale(model, glm::vec3(pine00Scale[i]));
        model = glm::rotate(model, glm::radians(pine00Orientation[i]), glm::vec3(0, 1, 0));
        modelMatrices[i] = model;
        i++;
    }
    //Binding para los objetos de pino
    glGenBuffers(1, &pine_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, pine_buffer);
    glBufferData(GL_ARRAY_BUFFER, pine00Amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < modelPine00.getMeshes().size(); i++)
    {
        pine00_VAO = modelPine00.getMeshes()[i]->getVAO();
        glBindVertexArray(pine00_VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }

    /********************************************************
    **********************************************************/

    pine01Amount = pine01Position.size();
    //glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[pine01Amount];
    //srand(glfwGetTime());

    //Se generan las matrices de transformacion para los objetos de pino
    i = 0;
    for (std::vector<glm::vec3>::iterator it = pine01Position.begin(); it != pine01Position.end(); it++) {
        model = glm::mat4(1.0);
        float xCoord = it->x;
        if (xCoord < -100) xCoord = -99.9;
        if (xCoord > 100) xCoord = 99.9;
        float zCoord = it->z;
        if (zCoord < -100) zCoord = -99.9;
        if (zCoord > 100) zCoord = 99.9;
        float yCoord = 0.0;
        model = glm::translate(model, glm::vec3(xCoord, yCoord, zCoord));
        model = glm::scale(model, glm::vec3(pine01Scale[i]));
        model = glm::rotate(model, glm::radians(pine01Orientation[i]), glm::vec3(0, 1, 0));
        modelMatrices[i] = model;
        i++;
    }
    //Binding para los objetos de pino
    glGenBuffers(1, &pine01_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, pine01_buffer);
    glBufferData(GL_ARRAY_BUFFER, pine01Amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < modelPine01.getMeshes().size(); i++)
    {
        pine01_VAO = modelPine01.getMeshes()[i]->getVAO();
        glBindVertexArray(pine01_VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }

    Texture textureParticleFire("../Textures/fire.png");
    bitmap = textureParticleFire.loadImage();
    data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
    glGenTextures(1, &textureParticleFireID);
    glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    textureParticleFire.freeImage(bitmap);

    std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
    std::mt19937 generator;
    std::random_device rd;
    generator.seed(rd());
    int size = nParticlesFire * 2;
    std::vector<GLfloat> randData(size);
    for (int i = 0; i < randData.size(); i++) {
        randData[i] = distr01(generator);
    }

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_1D, texId);
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    shaderParticlesFire.setInt("Pass", 1);
    shaderParticlesFire.setInt("ParticleTex", 0);
    shaderParticlesFire.setInt("RandomTex", 1);
    shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
    shaderParticlesFire.setFloat("ParticleSize", particleSize);
    //shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(1.0f,-0.5f,1.0f)));
    shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(accel));
    shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

    glm::mat3 basis;
    glm::vec3 u, v, n;
    v = glm::vec3(0, 1, 0);
    n = glm::cross(glm::vec3(1, 0, 0), v);
    if (glm::length(n) < 0.00001f) {
        n = glm::cross(glm::vec3(0, 1, 0), v);
    }
    u = glm::cross(v, n);
    basis[0] = glm::normalize(u);
    basis[1] = glm::normalize(v);
    basis[2] = glm::normalize(n);
    shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

    /*******************************************
     * Inicializacion de los buffers del fuego
     *******************************************/
    initParticleBuffersFire();

    /*******************************************
     * OpenAL init
     *******************************************/
    alutInit(0, nullptr);
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);
    alGetError(); // clear any error messages
    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else {
        printf("init() - No errors yet.");
    }
    // Config source 0
    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, soundBuffer);
    soundBuffer[0] = alutCreateBufferFromFile("../sounds/nightForest3.wav");
    soundBuffer[1] = alutCreateBufferFromFile("../sounds/nightForest3.wav");
    soundBuffer[2] = alutCreateBufferFromFile("../sounds/cricketsMix01.wav");
    soundBuffer[3] = alutCreateBufferFromFile("../sounds/step00.wav");
    soundBuffer[4] = alutCreateBufferFromFile("../sounds/pickUpItem.wav");
    soundBuffer[5] = alutCreateBufferFromFile("../sounds/heatherHit.wav");
    soundBuffer[6] = alutCreateBufferFromFile("../sounds/music.wav");


    int errorAlut = alutGetError();
    if (errorAlut != ALUT_ERROR_NO_ERROR) {
        printf("- Error open files with alut %d !!\n", errorAlut);
        exit(2);
    }


    alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if (alGetError() != AL_NO_ERROR) {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else {
        printf("init - no errors after alGenSources\n");
    }
    alSourcef(source[0], AL_PITCH, 0.8f);
    alSourcef(source[0], AL_GAIN, 12.0f);
    alSourcefv(source[0], AL_POSITION, source0Pos);
    alSourcefv(source[0], AL_VELOCITY, source0Vel);
    alSourcei(source[0], AL_BUFFER, soundBuffer[0]);
    alSourcei(source[0], AL_LOOPING, AL_TRUE);
    alSourcef(source[0], AL_MAX_DISTANCE, 600);

    alSourcef(source[1], AL_PITCH, 1.2f);
    alSourcef(source[1], AL_GAIN, 10.0f);
    alSourcefv(source[1], AL_POSITION, source1Pos);
    alSourcefv(source[1], AL_VELOCITY, source1Vel);
    alSourcei(source[1], AL_BUFFER, soundBuffer[1]);
    alSourcei(source[1], AL_LOOPING, AL_TRUE);
    alSourcef(source[1], AL_MAX_DISTANCE, 600);

    alSourcef(source[2], AL_PITCH, 1.0f);
    alSourcef(source[2], AL_GAIN, 16.5f);
    alSourcefv(source[2], AL_POSITION, source2Pos);
    alSourcefv(source[2], AL_VELOCITY, source2Vel);
    alSourcei(source[2], AL_BUFFER, soundBuffer[2]);
    alSourcei(source[2], AL_LOOPING, AL_TRUE);
    alSourcef(source[2], AL_MAX_DISTANCE, 600);

    alSourcef(source[3], AL_PITCH, 1.0f);
    alSourcef(source[3], AL_GAIN, 0.25f);
    alSourcefv(source[3], AL_POSITION, source3Pos);
    alSourcefv(source[3], AL_VELOCITY, source3Vel);
    alSourcei(source[3], AL_BUFFER, soundBuffer[3]);
    alSourcei(source[3], AL_LOOPING, false);
    alSourcef(source[3], AL_MAX_DISTANCE, 5);

    alSourcef(source[4], AL_PITCH, 1.0f);
    alSourcef(source[4], AL_GAIN, 3.0f);
    alSourcefv(source[4], AL_POSITION, source4Pos);
    alSourcefv(source[4], AL_VELOCITY, source4Vel);
    alSourcei(source[4], AL_BUFFER, soundBuffer[4]);
    alSourcei(source[4], AL_LOOPING, false);
    alSourcef(source[4], AL_MAX_DISTANCE, 5);

    alSourcef(source[5], AL_PITCH, 1.0f);
    alSourcef(source[5], AL_GAIN, 3.0f);
    alSourcefv(source[5], AL_POSITION, source5Pos);
    alSourcefv(source[5], AL_VELOCITY, source5Vel);
    alSourcei(source[5], AL_BUFFER, soundBuffer[5]);
    alSourcei(source[5], AL_LOOPING, false);
    alSourcef(source[5], AL_MAX_DISTANCE, 5);

    alSourcef(source[6], AL_PITCH, 1.0f);
    alSourcef(source[6], AL_GAIN, 0.5f);
    alSourcefv(source[6], AL_POSITION, source6Pos);
    alSourcefv(source[6], AL_VELOCITY, source6Vel);
    alSourcei(source[6], AL_BUFFER, soundBuffer[6]);
    alSourcei(source[6], AL_LOOPING, false);
    alSourcef(source[6], AL_MAX_DISTANCE, 5);

    //AbstractModel::OBB treeCollider00;
    //glm::mat4 modelMatrixColliderTree00 = glm::mat4(1.0f);
    //modelMatrixColliderTree00 = glm::translate(modelMatrixColliderTree00, glm::vec3(tree00Position[0].x, 0, tree00Position[0].y));
    //modelMatrixColliderTree00 = glm::rotate(modelMatrixColliderTree00, glm::radians(tree00Orientation[0]), glm::vec3(0, 1, 0));
    //treeCollider00.u = glm::quat_cast(modelMatrixColliderTree00);
    //modelMatrixColliderTree00 = glm::scale(modelMatrixColliderTree00, tree00Scale[0]);
    //modelMatrixColliderTree00 = glm::translate(modelMatrixColliderTree00, modelTree00.getObb().c);
    //treeCollider00.c = glm::vec3(modelMatrixColliderTree00[3]);
    //treeCollider00.e = modelTree00.getObb().e * glm::vec3(0.1, 0.1, 0.1);
    //addOrUpdateColliders(collidersOBB, "tree00", treeCollider00, modelMatrixColliderTree00);

    //Cercas
    for (int i = 0; i < fence00Position.size(); i++) {
        AbstractModel::OBB fenceCollider00;
        glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence00Position[i].x, 0, fence00Position[i].y));
        modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence00Orientation[i]), glm::vec3(0, 1, 0));
        fenceCollider00.u = glm::quat_cast(modelMatrixColliderFence);
        modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence00.getObb().c);
        fenceCollider00.c = glm::vec3(modelMatrixColliderFence[3]);
        fenceCollider00.e = modelFence00.getObb().e * fenceScale;
        addOrUpdateColliders(collidersOBB, "fence00-" + std::to_string(i), fenceCollider00, modelMatrixColliderFence);
    }

    for (int i = 0; i < fence01Position.size(); i++) {
        AbstractModel::OBB fenceCollider01;
        glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence01Position[i].x, 0, fence01Position[i].y));
        modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence01Orientation[i]), glm::vec3(0, 1, 0));
        fenceCollider01.u = glm::quat_cast(modelMatrixColliderFence);
        modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence01.getObb().c);
        fenceCollider01.c = glm::vec3(modelMatrixColliderFence[3]);
        fenceCollider01.e = modelFence00.getObb().e * fenceScale;
        addOrUpdateColliders(collidersOBB, "fence01-" + std::to_string(i), fenceCollider01, modelMatrixColliderFence);
    }

    for (int i = 0; i < fence02Position.size(); i++) {
        AbstractModel::OBB fenceCollider02;
        glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence02Position[i].x, 0, fence02Position[i].y));
        modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence02Orientation[i]), glm::vec3(0, 1, 0));
        fenceCollider02.u = glm::quat_cast(modelMatrixColliderFence);
        modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence02.getObb().c);
        fenceCollider02.c = glm::vec3(modelMatrixColliderFence[3]);
        fenceCollider02.e = modelFence00.getObb().e * fenceScale;
        addOrUpdateColliders(collidersOBB, "fence02-" + std::to_string(i), fenceCollider02, modelMatrixColliderFence);
    }

    for (int i = 0; i < fence03Position.size(); i++) {
        AbstractModel::OBB fenceCollider03;
        glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence03Position[i].x, 0, fence03Position[i].y));
        modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence03Orientation[i]), glm::vec3(0, 1, 0));
        fenceCollider03.u = glm::quat_cast(modelMatrixColliderFence);
        modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence03.getObb().c);
        fenceCollider03.c = glm::vec3(modelMatrixColliderFence[3]);
        fenceCollider03.e = modelFence00.getObb().e * fenceScale;
        addOrUpdateColliders(collidersOBB, "fence03-" + std::to_string(i), fenceCollider03, modelMatrixColliderFence);
    }

    for (int i = 0; i < fence04Position.size(); i++) {
        AbstractModel::OBB fenceCollider04;
        glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence04Position[i].x, 0, fence04Position[i].y));
        modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence04Orientation[i]), glm::vec3(0, 1, 0));
        fenceCollider04.u = glm::quat_cast(modelMatrixColliderFence);
        modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
        modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence04.getObb().c);
        fenceCollider04.c = glm::vec3(modelMatrixColliderFence[3]);
        fenceCollider04.e = modelFence00.getObb().e * fenceScale;
        addOrUpdateColliders(collidersOBB, "fence04-" + std::to_string(i), fenceCollider04, modelMatrixColliderFence);
    }

    for (int i = 0; i < wallPosition.size(); i++) {
        AbstractModel::OBB wallCollider;
        glm::mat4 modelMatrixColliderWall = glm::mat4(1.0f);
        modelMatrixColliderWall = glm::translate(modelMatrixColliderWall, wallPosition[i]);
        modelMatrixColliderWall = glm::rotate(modelMatrixColliderWall, glm::radians(wallOrientation[i]), glm::vec3(0, 1, 0));
        wallCollider.u = glm::quat_cast(modelMatrixColliderWall);
        modelMatrixColliderWall = glm::scale(modelMatrixColliderWall, wallScale[i]);
        modelMatrixColliderWall = glm::translate(modelMatrixColliderWall, modelCube.getObb().c);
        wallCollider.c = glm::vec3(modelMatrixColliderWall[3]);
        wallCollider.e = modelCube.getObb().e * wallScale[i];
        addOrUpdateColliders(collidersOBB, "wall-" + std::to_string(i), wallCollider, modelMatrixColliderWall);
    }


}

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
    // --------- IMPORTANTE ----------
    // Eliminar los shader y buffers creados.

    // Shaders Delete
    shader.destroy();
    shaderMulLighting.destroy();
    shaderSkybox.destroy();
    shaderTerrain.destroy();
    shaderPrueba.destroy();

    // Basic objects Delete
    skyboxSphere.destroy();

    boxCollider.destroy();
    sphereCollider.destroy();
    boxViewDepth.destroy();

    // Terrains objects Delete
    terrain.destroy();
    modelRock.destroy();
    modelGate.destroy();
    modelGate2.destroy();
    modelFence00.destroy();
    modelFence01.destroy();
    modelFence02.destroy();
    modelFence03.destroy();
    modelFence04.destroy();
    modelTree00.destroy();
    modelTree01.destroy();
    modelTree02.destroy();
    modelPine00.destroy();
    modelPine01.destroy();
    modelLog00.destroy();
    modelStump.destroy();
    modelRock00.destroy();
    modelRock01.destroy();
    modelGrass.destroy();
    modelCube.destroy();
    modelHealth.destroy();

    // Custom objects animate
    wendigoAnimate.destroy();
    heatherAnimate.destroy();

    // Textures Delete
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureCespedID);
    glDeleteTextures(1, &textureTerrainBackgroundID);
    glDeleteTextures(1, &textureTerrainRID);
    glDeleteTextures(1, &textureTerrainGID);
    glDeleteTextures(1, &textureTerrainBID);
    glDeleteTextures(1, &textureTerrainBlendMapID);
    glDeleteTextures(1, &textureParticleFireID);

    // Cube Maps Delete
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDeleteTextures(1, &skyboxTextureID);

    // Remove the buffer of the grass models
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &buffer);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);

    // Remove the buffer of the pine00 models
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &pine_buffer);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &pine00_VAO);

    // Remove the buffer of the pine01 models
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &pine01_buffer);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &pine01_VAO);

    // Remove the buffer of the fire particles
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, posBuf);
    glDeleteBuffers(2, velBuf);
    glDeleteBuffers(2, age);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glDeleteTransformFeedbacks(2, feedback);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
    screenWidth = widthRes;
    screenHeight = heightRes;
    glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
    int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            exitApp = true;
            break;
        }
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    offsetX = xpos - lastMousePosX;
    offsetY = ypos - lastMousePosY;
    lastMousePosX = xpos;
    lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    distanceFromTarget -= yoffset;
    if (distanceFromTarget > 10)
        distanceFromTarget = 10;
    if (distanceFromTarget < 1.5)
        distanceFromTarget = 1.5;
    camera->setDistanceFromTarget(distanceFromTarget);
};

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
    if (state == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
            std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
            break;
        }
    }
}

bool processInput(bool continueApplication) {
    if (exitApp || glfwWindowShouldClose(window) != 0) {
        return false;
    }

    if (cameraType) {
        orixH = 0;
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            FPcamera->mouseMoveCamera(offsetX, 0.0, deltaTime);
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            FPcamera->mouseMoveCamera(0.0, offsetY, deltaTime);
            orixH -= offsetX * 0.001;
    }
    else {
        orixH = 0;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
            camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
            camera->mouseMoveCamera(0.0, offsetY, deltaTime);
        }
        else {
            oriz += offsetY * 0.001;
            if (oriz > M_PI / 5)
                oriz = M_PI / 5;
            if (oriz < -M_PI / 5)
                oriz = -M_PI / 5;

            orix -= offsetX * 0.001;
            if (orix > M_PI / 5)
                orix = M_PI / 5;
            if (orix < -M_PI / 5)
                orix = -M_PI / 5;
        }
    }

    offsetX = 0;
    offsetY = 0;

    // Seleccionar modelo
    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        enableCountSelected = false;
        modelSelected++;
        if (modelSelected > 3)
            modelSelected = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
        enableCountSelected = true;

    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && cameraLock == false) {
        cameraLock = true;
        cameraType = !cameraType;
        if (cameraType) {
            FPcamera->setFront(modelMatrixHeather[2]);
            FPcamera->setYaw(90.0f);
            keyLock = true;
        }
        else {
            keyLock = false;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE && cameraLock == true) {
        cameraLock = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        camera->resetCamera();
    }
        

    if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && keyLock == false) {
        modelMatrixHeather = glm::rotate(modelMatrixHeather, 0.045f, glm::vec3(0, 1, 0));
        
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && keyLock == false) {
        modelMatrixHeather = glm::rotate(modelMatrixHeather, -0.045f, glm::vec3(0, 1, 0));
    }
    if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, 0.08));
        heatherAnimation = 1;
        stepCurrTime = TimeManager::Instance().GetTime();
        if (stepCurrTime - stepLastTime > 0.62) {
            sourcesPlay[3] = true;
            stepLastTime = stepCurrTime;
        }
        else
            sourcesPlay[3] = false; 

            
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        stepCurrTime = TimeManager::Instance().GetTime();
        if (stepCurrTime - stepLastTime > 0.72) {
            sourcesPlay[3] = true;
            stepLastTime = stepCurrTime;
        }
        else
            sourcesPlay[3] = false;
        heatherAnimation = 2;
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, -0.06));
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        heatherAnimation = 3;
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, 0.165));
        stepCurrTime = TimeManager::Instance().GetTime();
        if (stepCurrTime - stepLastTime > 0.42) {
            sourcesPlay[3] = true;
            stepLastTime = stepCurrTime;
        }
        else
            sourcesPlay[3] = false;
    }
    //if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && enableChange == 1) {
    //    enableChange = 0;
    //    heatherAnimation = 4;
    //    TimeManager::Instance().resetStartTime();
    //}
    //else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
    //    enableChange = 1;

    else if (modelSelected == 1) {
        heatherAnimation = 0;
    }
        

    if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        modelMatrixWendigo = glm::rotate(modelMatrixWendigo, 0.02f, glm::vec3(0, 1, 0));
    else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        modelMatrixWendigo = glm::rotate(modelMatrixWendigo, -0.02f, glm::vec3(0, 1, 0));
    if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        //modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(0.0, 0.0, 0.06));
        wendigoRunning = 1;
    else
        wendigoRunning = 0;

    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //    matrixModelGate = glm::translate(matrixModelGate,  glm::vec3(0, 0, -0.01));
    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //    matrixModelGate = glm::translate(matrixModelGate, glm::vec3(0, 0, 0.01));
    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //    matrixModelGate = glm::translate(matrixModelGate, glm::vec3(0.01, 0, 0));
    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //    matrixModelGate = glm::translate(matrixModelGate, glm::vec3(-0.01, 0, 0));
    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    matrixModelGate = glm::rotate(matrixModelGate, 0.02f, glm::vec3(0, 1, 0));
    //if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    matrixModelGate = glm::rotate(matrixModelGate, -0.02f, glm::vec3(0, 1, 0));

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        fogOn = true;
        enableFog = false;
    }


    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        fogOn = false;
        enableFog = false;
    }

    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
        enableFog = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS) {
        showWall = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS) {
        showWall = false;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        gateAnimation = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        gateAnimation = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        gateAnimation = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        TimeManager::Instance().resetStartTime();

        unlockGate = true;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        unlockGate = false;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        pickUp = true;
    }
    else {
        pickUp = false;
    }


    glfwPollEvents();
    return continueApplication;
}

void applicationLoop() {
    bool psi = true;

    matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));
    modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(-73.4897, 0.0, 75.0));
    modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(84.2281, 0.0, 91.2608));

/**********************************************************/

    glm::vec3 lightPos = glm::vec3(3.5, 10, 11);

    shadowBox = new ShadowBox(-lightPos, camera.get(), 70, 0.1, 60.0f);

    /*****************************************
    Variables de tiempo
    ******************************************/

    lastTime = TimeManager::Instance().GetTime();
    stepLastTime = TimeManager::Instance().GetTime();

    currTimeParticlesAnimationFire = lastTime;
    lastTimeParticlesAnimationFire = lastTime;

    //gateCurrTime = TimeManager::Instance().GetTime();
    gateLastTime = TimeManager::Instance().GetTime();
    
   
    while (psi) {
        currTime = TimeManager::Instance().GetTime();
        if (currTime - lastTime < 0.016666667) {
            glfwPollEvents();
            continue;
        }
        
        lastTime = currTime;
        TimeManager::Instance().CalculateFrameRate(true);
        deltaTime = TimeManager::Instance().DeltaTime;
        psi = processInput(true);

        std::map<std::string, bool> collisionDetection;

        /*screenView -= 0.1;
        if (screenView < 50.0) screenView = 50.0;*/
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        glm::vec3 axis;
        glm::vec3 target;
        float angleTarget;

        if (modelSelected == 1) {
            axis = glm::axis(glm::quat_cast(modelMatrixHeather));
            angleTarget = glm::angle(glm::quat_cast(modelMatrixHeather));
            target = modelMatrixHeather[3];
            target.x += 0.8;
            target.y += 4;
        }

        glm::mat4 view;
        if (cameraType) {
            FPcamera->setPosition(glm::vec3(modelMatrixHeather * glm::vec4(0, 3.75, 0.75, 1)));
            //FPcamera->setFront(modelMatrixHeather[2]);
            view = FPcamera->getViewMatrix();
            
        }
        else {
            if (std::isnan(angleTarget))
                angleTarget = 0.0;
            if (axis.y < 0)
                angleTarget = -angleTarget;
            camera->setCameraTarget(target);
            camera->setAngleTarget(angleTarget);
            camera->updateCamera();
            view = camera->getViewMatrix();
        }

        shadowBox->update(screenWidth, screenHeight);
        glm::vec3 centerBox = shadowBox->getCenter();
        glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
        glm::mat4 lightSpaceMatrix;
        lightProjection[0][0] = 2.0 / shadowBox->getWidth();
        lightProjection[1][1] = 2.0 / shadowBox->getHeight();
        lightProjection[2][2] = -2.0 / shadowBox->getLength();
        lightProjection[3][3] = 1.0;
        lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0, 1, 0));
        lightSpaceMatrix = lightProjection * lightView;

        //Matriz de proyeccion del shadow mapping
        //glm::mat4 lightProjection, lightView;
        //glm::mat4 lightSpaceMatrix;
        //float near_plane = 0.1f;
        //float far_plane = 20.0f;
        //lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
        //lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
        //lightSpaceMatrix = lightProjection * lightView;
        shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));
        shaderDepth2.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

        // Settea la matriz de vista y projection al shader con solo color
        shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
        shader.setMatrix4("view", 1, false, glm::value_ptr(view));

        // Settea la matriz de vista y projection al shader con skybox
        shaderSkybox.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderSkybox.setMatrix4("view", 1, false,
            glm::value_ptr(glm::mat4(glm::mat3(view))));
        // Settea la matriz de vista y projection al shader con multiples luces
        shaderMulLighting.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderMulLighting.setMatrix4("view", 1, false,
            glm::value_ptr(view));
        shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
            glm::value_ptr(lightSpaceMatrix));
        // Settea la matriz de vista y projection al shader del terreno
        shaderTerrain.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderTerrain.setMatrix4("view", 1, false,
            glm::value_ptr(view));
        shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
            glm::value_ptr(lightSpaceMatrix));
        // Settea la matriz de vista y projection al shader de instancing
        shaderPrueba.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderPrueba.setMatrix4("view", 1, false,
            glm::value_ptr(view));
        shaderPrueba.setMatrix4("lightSpaceMatrix", 1, false,
            glm::value_ptr(lightSpaceMatrix));
        // Settea la matriz de vista y projection al shader de particulas de fuego
        shaderParticlesFire.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderParticlesFire.setMatrix4("view", 1, false,
            glm::value_ptr(view));

        /*******************************************
        * Propiedades de la neblina
        *******************************************/
        glm::vec3 fogColor = glm::vec3(0.15, 0.15, 0.15);
        shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(fogColor));
        shaderPrueba.setVectorFloat3("fogColor", glm::value_ptr(fogColor));
        shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(fogColor));
        shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(fogColor));

        shaderMulLighting.setFloat("density", nieblaD);
        shaderPrueba.setFloat("density", nieblaD);
        shaderTerrain.setFloat("density", nieblaD);
        shaderSkybox.setFloat("lowerLimit", 0.0);
        shaderSkybox.setFloat("upperLimit", nieblaLL);

        if (fogOn) {
            nieblaLL += 5.5 / 1800;
            nieblaD += 0.08 / 1800;
            if (nieblaLL > 5.5) nieblaLL = 5.5;
            if (nieblaD > 0.08) nieblaD = 0.08;
        }
        
        if (!fogOn) {
            nieblaLL = 0.0;
            nieblaD = 0.0001;
        }
        if (enableFog) {
            nieblaD = 0.025;
            nieblaLL = 4.5;
        }

        shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(accel));
        
        if (accelUp) {
            accel.y += 0.7;
            if (accel.y > 15) {
                accelUp = false;
            }
        }
        else {
            accel.y -= 0.7;
            if (accel.y < -9) {
                accelUp = true;
            }
        }

        /*******************************************
         * Propiedades Luz direccional
         *******************************************/
        shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
        shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.15, 0.15, 0.16)));
        shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.26)));
        shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.1, -0.5, -1.0)));

        /*******************************************
         * Propiedades Luz direccional Terrain
         *******************************************/
        shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
        shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.15, 0.15, 0.16)));
        shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.26)));
        shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.1, -0.5, -1.0)));

        shaderPrueba.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
        shaderPrueba.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.15, 0.15, 0.16)));
        shaderPrueba.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.26)));
        shaderPrueba.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderPrueba.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.1, -0.5, -1.0)));

        /*******************************************
         * Propiedades SpotLights
         *******************************************/
        shaderMulLighting.setInt("spotLightCount", 1);
        shaderTerrain.setInt("spotLightCount", 1);

        shaderPrueba.setInt("spotLightCount", 1);

        /*******************************************
         * Propiedades PointLights
         *******************************************/
        shaderMulLighting.setInt("pointLightCount", 0);
        shaderTerrain.setInt("pointLightCount", 0);

        shaderPrueba.setInt("pointLightCount", 0);

        /*******************************************
         *Paso 1: Se dibuja la escena desde el punto de vista de la luz
         *******************************************/

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Renderizado de la escena desde la luz
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        prepareDepthScene();
        renderScene(false);
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        /*******************************************
         *Paso 2: Renderizacion de los objetos
         *******************************************/

        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prepareScene();
        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        shaderMulLighting.setInt("shadowMap", 10);
        shaderPrueba.setInt("shadowMap", 10);
        shaderTerrain.setInt("shadowMap", 10);

        /*******************************************
         * Skybox
         *******************************************/
        GLint oldCullFaceMode;
        GLint oldDepthFuncMode;
        // deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
        glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
        shaderSkybox.setFloat("skybox", 0);
        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);
        glActiveTexture(GL_TEXTURE0);
        skyboxSphere.render();
        glCullFace(oldCullFaceMode);
        glDepthFunc(oldDepthFuncMode);
        renderScene();

        /**********************
        Colisiones
        **********************/

        if (!openGate) {
            AbstractModel::OBB gateCollider00;
            glm::mat4 modelMatrixCollidergate = glm::mat4(1.0f);
            modelMatrixCollidergate = glm::translate(modelMatrixCollidergate, glm::vec3(-40.1863, 0, -53.1113));
            modelMatrixCollidergate = glm::rotate(modelMatrixCollidergate, glm::radians(3.0f), glm::vec3(0, 1, 0));
            gateCollider00.u = glm::quat_cast(modelMatrixCollidergate);
            modelMatrixCollidergate = glm::scale(modelMatrixCollidergate, glm::vec3(0.261412, 3, 59.01047));
            //modelMatrixCollidergate = glm::translate(modelMatrixCollidergate, modelGate.getObb().c);
            gateCollider00.c = glm::vec3(modelMatrixCollidergate[3]);
            gateCollider00.e = modelGate.getObb().e * glm::vec3(0.161412, 3, 59.01047);
            addOrUpdateColliders(collidersOBB, "gate", gateCollider00, modelMatrixCollidergate);
        }

        if (modelMatrixHeather[3][0] > -30 && healtPicks < 3) {
            wendigoAttack = true;
            wendigoRun = true;
            openGate = false;
        }

        if (healtPicks == 3) {
            openGate = true;
            collidersOBB.erase("gate");
            wendigoAttack = false;
            wendigoRun = false;
            wendigoRender = false;
        }

        if (heatherHealth < 0) {
            wendigoAttack = false;
            wendigoRun = false;
            wendigoHit = false;
            modelMatrixHeather[3] = glm::vec4(-73.4897, 0.0, 75.0, 1.0);
            openGate = true;
            collidersOBB.erase("gate");
            heatherHealth = 100;
        }

        //Heater
        AbstractModel::OBB heatherCollider;
        glm::mat4 modelMatrixColliderHeather = glm::mat4(modelMatrixHeather);
        //Antes de escalar la matriz se debe de obtener la orientación
        heatherCollider.u = glm::quat_cast(modelMatrixColliderHeather);
        modelMatrixColliderHeather = glm::scale(modelMatrixColliderHeather, glm::vec3(0.025, 0.025, 0.025));
        modelMatrixColliderHeather = glm::translate(modelMatrixColliderHeather, heatherAnimate.getObb().c);
        heatherCollider.c = modelMatrixColliderHeather[3];
        //heatherCollider.e = heatherAnimate.getObb().e * glm::vec3(0.025, 0.025, 0.025) * glm::vec3(10.6, 40.9, 40.2);
        heatherCollider.e = heatherAnimate.getObb().e * glm::vec3(0.5, 4.9, 0.5);
        addOrUpdateColliders(collidersOBB, "heather", heatherCollider, modelMatrixHeather);

        //Wendigo
        AbstractModel::OBB wendigoCollider;
        glm::mat4 modelMatrixColliderWendigo = glm::mat4(modelMatrixWendigo);
        //Antes de escalar la matriz se debe de obtener la orientación
        wendigoCollider.u = glm::quat_cast(modelMatrixColliderWendigo);
        modelMatrixColliderWendigo = glm::translate(modelMatrixColliderWendigo, wendigoAnimate.getObb().c);
        modelMatrixColliderWendigo = glm::translate(modelMatrixColliderWendigo, glm::vec3(0, 1.7, 0));
        modelMatrixColliderWendigo = glm::scale(modelMatrixColliderWendigo, glm::vec3(0.035, 0.035, 0.035));
        wendigoCollider.c = modelMatrixColliderWendigo[3] ;
        //heatherCollider.e = heatherAnimate.getObb().e * glm::vec3(0.025, 0.025, 0.025) * glm::vec3(10.6, 40.9, 40.2);
        wendigoCollider.e = wendigoAnimate.getObb().e * glm::vec3(1, 2.8, 2);
        addOrUpdateColliders(collidersOBB, "wendigo", wendigoCollider, modelMatrixWendigo);
       
        float healthH;
        for (int i = 0; i < healthPosition.size(); i++) {
            if (healthRender[i]) {
                AbstractModel::OBB healthCollider;
                glm::mat4 modelMatrixColliderHealth = glm::mat4(1.0);
                healthH = terrain.getHeightTerrain(healthPosition[i].x, healthPosition[i].z) + healthPosition[i].y;
                modelMatrixColliderHealth = glm::translate(modelMatrixColliderHealth, glm::vec3(healthPosition[i].x, healthH, healthPosition[i].z));
                modelMatrixColliderHealth = glm::rotate(modelMatrixColliderHealth, glm::radians(healthOrientation[i].y), glm::vec3(0, 1, 0));
                modelMatrixColliderHealth = glm::rotate(modelMatrixColliderHealth, glm::radians(healthOrientation[i].x), glm::vec3(1, 0, 0));

                //modelMatrixColliderHealth = glm::rotate(modelMatrixColliderHealth, glm::radians(healthOrientation[i].z), glm::vec3(0, 0, 1));
                healthCollider.u = glm::quat_cast(modelMatrixColliderHealth);
                modelMatrixColliderHealth = glm::scale(modelMatrixColliderHealth, glm::vec3(0.35, 0.35, 0.35));
                modelMatrixColliderHealth = glm::translate(modelMatrixColliderHealth, modelHealth.getObb().c);
                healthCollider.c = modelMatrixColliderHealth[3];
                healthCollider.e = modelHealth.getObb().e * glm::vec3(0.35, 0.35, 0.35);
                addOrUpdateColliders(collidersOBB, "health-" + std::to_string(i), healthCollider, modelMatrixColliderHealth);
            }
        }


        /*******************************************
         * Render de colliders
         *******************************************/
        if (showWall) {
            for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
                collidersOBB.begin(); it != collidersOBB.end(); it++) {
                glm::mat4 matrixCollider = glm::mat4(1.0);
                matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
                matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
                matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
                boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
                boxCollider.enableWireMode();
                boxCollider.render(matrixCollider);
            }
        }

        /********************************************
        Para realizar las pruebas de colisión
        ********************************************/

        for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it = collidersOBB.begin();
            it != collidersOBB.end(); it++) {
            bool isCollision = false;
            for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt = collidersOBB.begin();
                jt != collidersOBB.end(); jt++) {
                if (it != jt && testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
                    if (it->first.compare("heather") == 0) {
                        //std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                        for (int i = 0; i < healthPosition.size(); i++) {
                            std::string item = "health-" + std::to_string(i);
                            if (it->first.compare("heather") == 0 && jt->first.compare(item) == 0 && pickUp) {
                                healthRender[i] = false;
                                collidersOBB.erase(item);
                                sourcesPlay[4] = true;
                                heatherHealth += 20;
                                healtPicks += 1;
     
                            }
                        }
                    }

                    if (it->first.compare("wendigo") == 0 && jt->first.compare("heather") == 0) {
                        wendigoAttack = false;
                        wendigoHit = true;
                        heatherAnimation = 4;
                        heatherHealth -= 10;
                        sourcesPlay[5] = true;
                        //std::cout << heatherHealth << std::endl;
                        
                    }

                    isCollision = true;
                }
            }
            for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.begin();
                jt != collidersSBB.end(); jt++) {
                if (testSphereOBox(std::get<0>(jt->second), std::get<0>(it->second))) {
                    std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                    isCollision = true;
                    addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
                }
            }
            addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
        }

        for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it = collidersSBB.begin();
            it != collidersSBB.end(); it++) {
            bool isCollision = false;
            for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.begin();
                jt != collidersSBB.end(); jt++) {
                if (it != jt && testSphereSphereIntersection(std::get<0>(it->second), std::get<0>(jt->second))) {
                    std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                    isCollision = true;
                }
            }
            addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
        }

        std::map<std::string, bool>::iterator colIt;
        for (colIt = collisionDetection.begin(); colIt != collisionDetection.end(); colIt++) {
            std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it = collidersOBB.find(colIt->first);
            std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.find(colIt->first);
            if (jt != collidersSBB.end()) {
                if (!colIt->second)
                    addOrUpdateColliders(collidersSBB, jt->first);
            }
            if (it != collidersOBB.end())
                if (!colIt->second)
                    addOrUpdateColliders(collidersOBB, it->first);
                else {
                   if (it->first.compare("heather") == 0)
                        modelMatrixHeather = std::get<1>(it->second);
                }
        }
        //std::cout << healtPicks << std::endl;


        /*******************************************
         * State machines
         *******************************************/

        glfwSwapBuffers(window);

        /****************************+
         * Open AL sound data
         */
   /*     source0Pos[0] = matrixModelTree00[3].x;
        source0Pos[1] = matrixModelTree00[3].y;
        source0Pos[2] = matrixModelTree00[3].z;*/
        alSourcefv(source[0], AL_POSITION, source0Pos);

        alSourcefv(source[1], AL_POSITION, source0Pos);

        //source2Pos[0] = modelMatrixDart[3].x;
        //source2Pos[1] = modelMatrixDart[3].y;
        //source2Pos[2] = modelMatrixDart[3].z;
        alSourcefv(source[2], AL_POSITION, source2Pos);

        source3Pos[0] = modelMatrixHeather[3].x;
        source3Pos[1] = modelMatrixHeather[3].y;
        source3Pos[2] = modelMatrixHeather[3].z;
        alSourcefv(source[3], AL_POSITION, source3Pos);

        source4Pos[0] = modelMatrixHeather[3].x;
        source4Pos[1] = modelMatrixHeather[3].y;
        source4Pos[2] = modelMatrixHeather[3].z;
        alSourcefv(source[4], AL_POSITION, source4Pos);

        source5Pos[0] = modelMatrixHeather[3].x;
        source5Pos[1] = modelMatrixHeather[3].y;
        source5Pos[2] = modelMatrixHeather[3].z;
        alSourcefv(source[5], AL_POSITION, source5Pos);

        source6Pos[0] = modelMatrixHeather[3].x;
        source6Pos[1] = modelMatrixHeather[3].y;
        source6Pos[2] = modelMatrixHeather[3].z;
        alSourcefv(source[6], AL_POSITION, source6Pos);

        // Listener for the Thris person camera
        listenerPos[0] = modelMatrixHeather[3].x;
        listenerPos[1] = modelMatrixHeather[3].y;
        listenerPos[2] = modelMatrixHeather[3].z;
        alListenerfv(AL_POSITION, listenerPos);

        glm::vec3 upModel = glm::normalize(modelMatrixHeather[1]);
        glm::vec3 frontModel = glm::normalize(modelMatrixHeather[2]);

        listenerOri[0] = frontModel.x;
        listenerOri[1] = frontModel.y;
        listenerOri[2] = frontModel.z;
        listenerOri[3] = upModel.x;
        listenerOri[4] = upModel.y;
        listenerOri[5] = upModel.z;

        // Listener for the First person camera
        /*listenerPos[0] = camera->getPosition().x;
        listenerPos[1] = camera->getPosition().y;
        listenerPos[2] = camera->getPosition().z;
        alListenerfv(AL_POSITION, listenerPos);
        listenerOri[0] = camera->getFront().x;
        listenerOri[1] = camera->getFront().y;
        listenerOri[2] = camera->getFront().z;
        listenerOri[3] = camera->getUp().x;
        listenerOri[4] = camera->getUp().y;
        listenerOri[5] = camera->getUp().z;*/
        alListenerfv(AL_ORIENTATION, listenerOri);

        for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
            if (sourcesPlay[i]) {
                sourcesPlay[i] = false;
                alSourcePlay(source[i]);
            }
        }
    }
}

void prepareScene() {
    skyboxSphere.setShader(&shaderSkybox);
   
    modelRock.setShader(&shaderMulLighting);
    modelGate.setShader(&shaderMulLighting);
    modelGate2.setShader(&shaderMulLighting);
    modelFence00.setShader(&shaderMulLighting);
    modelFence01.setShader(&shaderMulLighting);
    modelFence02.setShader(&shaderMulLighting);
    modelFence03.setShader(&shaderMulLighting);
    modelFence04.setShader(&shaderMulLighting);

    //modelTree00.loadModel("../models/tree2/tree_mango_var01.obj");
    modelTree00.setShader(&shaderMulLighting);

    modelTree01.setShader(&shaderMulLighting);

    modelTree02.setShader(&shaderMulLighting);

    modelPine00.setShader(&shaderPrueba);
    
    modelPine01.setShader(&shaderPrueba);

    modelStump.setShader(&shaderMulLighting);

    modelLog00.setShader(&shaderMulLighting);

    modelRock00.setShader(&shaderMulLighting);

    modelRock01.setShader(&shaderMulLighting);

    modelHealth.setShader(&shaderMulLighting);

    modelGrass.setShader(&shaderPrueba);

    terrain.setShader(&shaderTerrain);

    //Wendigo
    wendigoAnimate.setShader(&shaderMulLighting);
    //Heather
    heatherAnimate.setShader(&shaderMulLighting);

}


void prepareDepthScene() {
    skyboxSphere.setShader(&shaderDepth);

    modelRock.setShader(&shaderDepth);
    modelGate.setShader(&shaderDepth);
    modelGate2.setShader(&shaderDepth);
    modelFence00.setShader(&shaderDepth);
    modelFence01.setShader(&shaderDepth);
    modelFence02.setShader(&shaderDepth);
    modelFence03.setShader(&shaderDepth);
    modelFence04.setShader(&shaderDepth);

    //modelTree00.loadModel("../models/tree2/tree_mango_var01.obj");
    modelTree00.setShader(&shaderDepth);

    modelTree01.setShader(&shaderDepth);

    modelTree02.setShader(&shaderDepth);

    modelPine00.setShader(&shaderDepth2);
    
    modelPine01.setShader(&shaderDepth2);

    modelStump.setShader(&shaderDepth);

    modelLog00.setShader(&shaderDepth);

    modelRock00.setShader(&shaderDepth);

    modelRock01.setShader(&shaderDepth);

    modelGrass.setShader(&shaderDepth2);

    modelHealth.setShader(&shaderDepth);

    terrain.setShader(&shaderDepth);

    //Wendigo
    wendigoAnimate.setShader(&shaderDepth);

    heatherAnimate.setShader(&shaderDepth);

}

void renderScene(bool renderParticles) {
    /*******************************************
    * Terrain Cesped
    *******************************************/
    // Se activa la textura del background
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
    shaderTerrain.setInt("backgroundTexture", 0);
    //Se activa la textura de la tierra
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
    shaderTerrain.setInt("rTexture", 1);
    //Se activa la textura de flores
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
    shaderTerrain.setInt("gTexture", 2);
    //Se activa la textura del camino
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
    shaderTerrain.setInt("bTexture", 4);
    //Se activa la textura del mapa de mezcla
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
    shaderTerrain.setInt("blendMapTexture", 5);

    shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(30, 30)));
    terrain.render();
    shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
    glBindTexture(GL_TEXTURE_2D, 0);

    /*******************************************
     * Custom objects obj
     *******************************************/
     //Rock render
    matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
    modelRock.render(matrixModelRock);

    for (int i = 0; i < fence00Position.size(); i++) {
        fenceHeight = terrain.getHeightTerrain(fence00Position[i].x, fence00Position[i].y);
        modelFence00.setPosition(glm::vec3(fence00Position[i].x, fenceHeight, fence00Position[i].y));
        modelFence00.setScale(fenceScale);
        modelFence00.setOrientation(glm::vec3(0, fence00Orientation[i], 0));
        modelFence00.render(matrixModelFence00);
    }

    for (int i = 0; i < fence01Position.size(); i++) {
        fenceHeight = terrain.getHeightTerrain(fence01Position[i].x, fence01Position[i].y);
        modelFence01.setPosition(glm::vec3(fence01Position[i].x, fenceHeight, fence01Position[i].y));
        modelFence01.setScale(fenceScale);
        modelFence01.setOrientation(glm::vec3(0, fence01Orientation[i], 0));
        modelFence01.render(matrixModelFence01);
    }

    for (int i = 0; i < fence02Position.size(); i++) {
        fenceHeight = terrain.getHeightTerrain(fence02Position[i].x, fence02Position[i].y);
        modelFence02.setPosition(glm::vec3(fence02Position[i].x, fenceHeight, fence02Position[i].y));
        modelFence02.setScale(fenceScale);
        modelFence02.setOrientation(glm::vec3(0, fence02Orientation[i], 0));
        modelFence02.render(matrixModelFence02);
    }

    for (int i = 0; i < fence03Position.size(); i++) {
        fenceHeight = terrain.getHeightTerrain(fence03Position[i].x, fence03Position[i].y);
        modelFence03.setPosition(glm::vec3(fence03Position[i].x, fenceHeight, fence03Position[i].y));
        modelFence03.setScale(fenceScale);
        modelFence03.setOrientation(glm::vec3(0, fence03Orientation[i], 0));
        modelFence03.render(matrixModelFence03);
    }

    for (int i = 0; i < fence04Position.size(); i++) {
        fenceHeight = terrain.getHeightTerrain(fence04Position[i].x, fence04Position[i].y);
        modelFence04.setPosition(glm::vec3(fence04Position[i].x, fenceHeight, fence04Position[i].y));
        modelFence04.setScale(fenceScale);
        modelFence04.setOrientation(glm::vec3(0, fence04Orientation[i], 0));
        modelFence04.render(matrixModelFence04);
    }

    for (int i = 0; i < tree00Position.size(); i++) {
        glDisable(GL_CULL_FACE);
        treeHeight = terrain.getHeightTerrain(tree00Position[i].x, tree00Position[i].y);
        modelTree00.setPosition(glm::vec3(tree00Position[i].x, 0, tree00Position[i].y));
        modelTree00.setScale(tree00Scale[i]);
        modelTree00.setOrientation(glm::vec3(0, tree00Orientation[i], 0));
        modelTree00.render(matrixModelTree00);
        glEnable(GL_CULL_FACE);
    }

    for (int i = 0; i < tree01Position.size(); i++) {
        glDisable(GL_CULL_FACE);
        treeHeight = terrain.getHeightTerrain(tree01Position[i].x, tree01Position[i].y);
        modelTree01.setPosition(glm::vec3(tree01Position[i].x, 0, tree01Position[i].y));
        modelTree01.setScale(tree01Scale[i]);
        modelTree01.setOrientation(glm::vec3(0, tree01Orientation[i], 0));
        modelTree01.render(matrixModelTree00);
        glEnable(GL_CULL_FACE);
    }

    /*for (int i = 0; i < tree02Position.size(); i++) {
        glDisable(GL_CULL_FACE);
        treeHeight = terrain.getHeightTerrain(tree02Position[i].x, tree02Position[i].y);
        modelTree02.setPosition(glm::vec3(tree02Position[i].x, treeHeight + 0.5, tree02Position[i].y));
        modelTree02.setScale(tree02Scale[i]);
        modelTree02.setOrientation(glm::vec3(0, tree02Orientation[i], 0));
        modelTree02.render(matrixModelTree00);
        glEnable(GL_CULL_FACE);
    }*/

    for (int i = 0; i < log00Position.size(); i++) {
        logH = terrain.getHeightTerrain(log00Position[i].x, log00Position[i].z);
        modelLog00.setPosition(glm::vec3(log00Position[i].x, logH + log00Position[i].y, log00Position[i].z));
        modelLog00.setScale(log00Scale[i]);
        modelLog00.setOrientation(log00Orientation[i]);
        modelLog00.render(matrixModelLog00);
    }

    float stumpH = terrain.getHeightTerrain(13.2114, 71.083);
    modelStump.setPosition(glm::vec3(13.2114, stumpH - 0.05, 71.083));
    modelStump.setScale(glm::vec3(27.0));
    modelStump.setOrientation(glm::vec3(0, -24.3, 0));
    modelStump.render(matrixModelStump);

    float rockH = terrain.getHeightTerrain(-2.35956, 55.6855);
    modelRock00.setPosition(glm::vec3(-2.35956, rockH - 0.5, 55.6855));
    modelRock00.setScale(glm::vec3(1));
    modelRock00.setOrientation(glm::vec3(0, 0, 0));
    modelRock00.render(matrixModelRock00);

    rockH = terrain.getHeightTerrain(-2.33666, 58.1415);
    modelRock00.setPosition(glm::vec3(-2.33666, rockH - 0.5, 58.1415));
    modelRock00.setScale(glm::vec3(1));
    modelRock00.setOrientation(glm::vec3(90 - 49.4727, 0, 0));
    modelRock00.render(matrixModelRock00);

    rockH = terrain.getHeightTerrain(-2.02599, 60.2344);
    modelRock00.setPosition(glm::vec3(-2.02599, rockH - 0.5, 60.2344));
    modelRock00.setScale(glm::vec3(1));
    modelRock00.setOrientation(glm::vec3(90 - 49.4727, -95.596, 0));
    modelRock00.render(matrixModelRock00);

    for (int i = 0; i < rock00Position.size(); i++) {
        rockH = terrain.getHeightTerrain(rock00Position[i].x, rock00Position[i].z);
        modelRock00.setPosition(glm::vec3(rock00Position[i].x, rock00Position[i].y, rock00Position[i].z));
        modelRock00.setScale(rock00Scale[i]);
        modelRock00.setOrientation(rock00Orientation[i]);
        modelRock00.render(matrixModelRock00);
    }

    rockH = terrain.getHeightTerrain(42.8546, 11.7888);
    modelRock01.setPosition(glm::vec3(42.8546, rockH - 0.5, 11.7888));
    modelRock01.setScale(glm::vec3(1.18227));
    modelRock01.setOrientation(glm::vec3(0, 173.309, 0));
    modelRock01.render(matrixModelRock01);

    float healthH;
    for (int i = 0; i < healthPosition.size(); i++) {
        if (healthRender[i]) {
            healthH = terrain.getHeightTerrain(healthPosition[i].x, healthPosition[i].z);
            modelHealth.setPosition(glm::vec3(healthPosition[i].x, healthH + healthPosition[i].y, healthPosition[i].z));
            modelHealth.setScale(glm::vec3(0.35));
            modelHealth.setOrientation(healthOrientation[i]);
            modelHealth.render();
        }
    }
    
    glDisable(GL_CULL_FACE);
    modelPine01.renderInstance(glm::mat4(1.0f), pine01Amount);
    glEnable(GL_CULL_FACE);

    glDisable(GL_CULL_FACE);
    modelPine00.renderInstance(glm::mat4(1.0f), pine00Amount);
    glEnable(GL_CULL_FACE);
    
    glDisable(GL_CULL_FACE);
        modelGrass.renderInstance(matrixModelGrass, amount);
    glEnable(GL_CULL_FACE);
    
    // Forze to enable the unit texture to 0 always ----------------- IMPORTANT
    glActiveTexture(GL_TEXTURE0);

    /*******************************************
     * Custom Anim objects obj
     *******************************************/

    float gateH = terrain.getHeightTerrain(matrixModelGate[3][0], matrixModelGate[3][2]);
    //glm::mat4 matrixModelGate2 = glm::scale(matrixModelGate, glm::vec3(0.0001, 0.0001, 0.0001));
    //modelGate.setPosition(glm::vec3(-35.15625, 0, -50.78125));
    modelGate.setPosition(glm::vec3(-40.18627, gateH, -53.11125));
    //modelGate.setScale(glm::vec3(4.4, 4.5, 4.5));
    modelGate.setOrientation(glm::vec3(0, -85, 0));
    if (openGate)
        gateAnimation = 2;
    else
        gateAnimation = 0;
    modelGate.setAnimationIndex(gateAnimation);
    modelGate.render(matrixModelGate);

   //Heather
    modelMatrixHeather = glm::rotate(modelMatrixHeather, orixH, glm::vec3(0, 1, 0));
    modelMatrixHeather[3][1] = terrain.getHeightTerrain(modelMatrixHeather[3][0], modelMatrixHeather[3][2]);
    modelMatrixHeatherBody = glm::mat4(modelMatrixHeather);
    modelMatrixHeatherBody = glm::scale(modelMatrixHeatherBody, glm::vec3(0.025, 0.025, 0.025));
    heatherAnimate.setAnimationIndex(heatherAnimation);
    heatherAnimate.render(modelMatrixHeatherBody);

    //glm::vec3 spotLightPosition = glm::vec3(modelMatrixHeather * glm::vec4(0.15, 3.22043, 0.75, 1));
    //lightPos = spotLightPosition;
    glm::vec3 spotLightOrientation = glm::normalize(glm::vec3(modelMatrixHeather[2]));
    spotLightOrientation.y -= 0.25;
    glm::mat4 temp = modelMatrixHeather;
    temp[3] = modelMatrixHeather * glm::vec4(0.15, 3.22043, 0.75, 1);
    glm::mat4 matSpoth = glm::rotate(temp, oriz, glm::vec3(1.0f, 0, 0.0f));
    matSpoth = glm::rotate(matSpoth, orix, glm::vec3(0.0f, 1.0f, 0.0f));
    matSpoth = glm::translate(matSpoth, glm::vec3(0, 1.0, 0));

    /*shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(matSpoth[3]));
    shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(matSpoth[2]));*/

    //Para el shader de modelos
    shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.35, 0.35, 0.3)));
    shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.2)));
    shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
    //shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
    //shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
    shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(matSpoth[3]));
    shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(matSpoth[2]));
    //Vectores de atenuacion
    shaderMulLighting.setFloat("spotLights[0].constant", 0.25);
    shaderMulLighting.setFloat("spotLights[0].linear", 0.015);
    shaderMulLighting.setFloat("spotLights[0].quadratic", 0.0008);
    //Angulo
    shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
    shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));
    //Para el shader del terrenos
    shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.35, 0.35, 0.3)));
    shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.2)));
    shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
    //shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
    ////shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(glm::vec3(0.0, -0.4, 1.0)));
    //shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
    shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(matSpoth[3]));
    shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(matSpoth[2]));
    //Vectores de atenuacion
    shaderTerrain.setFloat("spotLights[0].constant", 0.25);
    shaderTerrain.setFloat("spotLights[0].linear", 0.015);
    shaderTerrain.setFloat("spotLights[0].quadratic", 0.0008);
    //Angulo
    shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
    shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));

    shaderPrueba.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.35, 0.35, 0.3)));
    shaderPrueba.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.25, 0.25, 0.2)));
    shaderPrueba.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
    //shaderPrueba.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
    //shaderPrueba.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
    shaderPrueba.setVectorFloat3("spotLights[0].position", glm::value_ptr(matSpoth[3]));
    shaderPrueba.setVectorFloat3("spotLights[0].direction", glm::value_ptr(matSpoth[2]));
    //Vectores de atenuacion
    shaderPrueba.setFloat("spotLights[0].constant", 0.25);
    shaderPrueba.setFloat("spotLights[0].linear", 0.015);
    shaderPrueba.setFloat("spotLights[0].quadratic", 0.0008);
    //Angulo
    shaderPrueba.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
    shaderPrueba.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));


    modelMatrixWendigo[3][1] = terrain.getHeightTerrain(modelMatrixWendigo[3][0], modelMatrixWendigo[3][2]);
    /*glm::vec3 wendigoNormal = terrain.getNormalTerrain(modelMatrixWendigo[3][0], modelMatrixWendigo[3][2]);

    glm::vec3 xaxis = glm::vec3(modelMatrixWendigo[0]);
    glm::vec3 zaxis = glm::normalize(glm::cross(xaxis, wendigoNormal));
    modelMatrixWendigo[1] = glm::vec4(wendigoNormal, 0.0);
    modelMatrixWendigo[2] = glm::vec4(zaxis, 0.0);*/


    //aqui
    if (wendigoAttack && !wendigoHit) {
        glm::vec3 tempz = glm::normalize(glm::vec3(modelMatrixHeather[3][0], 0.0, modelMatrixHeather[3][2]) - glm::vec3(modelMatrixWendigo[3][0], 0.0, modelMatrixWendigo[3][2]));
        glm::vec3 tempx = glm::normalize(glm::cross(glm::vec3(0, 1, 0), tempz));

        modelMatrixWendigo[0] = glm::vec4(tempx, 0.0);
        modelMatrixWendigo[2] = glm::vec4(tempz, 0.0);

    }

    if (wendigoRun) {
        modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(0.0, 0.0, 0.15));
        wendigoAnimate.setAnimationIndex(0);
        if (modelMatrixWendigo[3][0] < -95 || modelMatrixWendigo[3][0]  > 95 ||
            modelMatrixWendigo[3][2] < -95 || modelMatrixWendigo[3][2] > 95) {
            wendigoAttack = true;
            wendigoHit = false;
        }
    }


    glm::mat4 modelMatrixWendigoBody = glm::mat4(modelMatrixWendigo);
    
    modelMatrixWendigoBody = glm::scale(modelMatrixWendigoBody, glm::vec3(0.035, 0.035, 0.035));
    //if (wendigoAttack) {

    //}
    //else
    //    wendigoAnimate.setAnimationIndex(1);
    if (wendigoRender){
        wendigoAnimate.render(modelMatrixWendigoBody);
    }

    /**********
     * Update the position with alpha objects
     */
     // Update the aircraft
    blendingUnsorted.find("fire")->second = glm::vec3(modelMatrixWendigo[3]);


    /**********
     * Sorter with alpha objects
     */
    std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
    std::map<std::string, glm::vec3>::iterator itblend;
    for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
        float distanceFromView = glm::length(camera->getPosition() - itblend->second);
        blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
    }

    /**********
     * Render de las transparencias
     */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
        if (renderParticles && it->second.first.compare("fire") == 0) {
            /**********
             * Init Render particles systems
             */
            lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
            currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

            shaderParticlesFire.setInt("Pass", 1);
            shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
            shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_1D, texId);
            glEnable(GL_RASTERIZER_DISCARD);
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
            glBeginTransformFeedback(GL_POINTS);
            glBindVertexArray(particleArray[1 - drawBuf]);
            glVertexAttribDivisor(0, 0);
            glVertexAttribDivisor(1, 0);
            glVertexAttribDivisor(2, 0);
            glDrawArrays(GL_POINTS, 0, nParticlesFire);
            glEndTransformFeedback();
            glDisable(GL_RASTERIZER_DISCARD);

            shaderParticlesFire.setInt("Pass", 2);
            glm::mat4 modelFireParticles = glm::mat4(1.0);
            modelFireParticles = modelMatrixWendigo;
            //modelFireParticles = glm::translate(modelFireParticles, glm::vec3(fx, fy, fz));
            modelFireParticles = glm::translate(modelFireParticles, glm::vec3(0.0, 2.58 + accel.y * 0.01, 2.75));

            shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

            shaderParticlesFire.turnOn();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
            glDepthMask(GL_FALSE);
            glBindVertexArray(particleArray[drawBuf]);
            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);
            drawBuf = 1 - drawBuf;
            shaderParticlesFire.turnOff();

            /****************************+
             * Open AL sound data
             */
            //source1Pos[0] = modelFireParticles[3].x;
            //source1Pos[1] = modelFireParticles[3].y;
            //source1Pos[2] = modelFireParticles[3].z;
            //alSourcefv(source[1], AL_POSITION, source1Pos);

            /**********
             * End Render particles systems
             */
        }

    }
    glEnable(GL_CULL_FACE);

}

int main(int argc, char **argv) {
    init(1280, 720, "Window GLFW", false);
    //init(1920, 1080, "Window GLFW", false);
    applicationLoop();
    destroy();
    return 1;
}
