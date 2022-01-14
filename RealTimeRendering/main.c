#include "Settings.h"
#include "Window.h"
#include "Camera.h"
#include "Matrix.h"
#include "Vector.h"
#include "Scene.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Shader.h"
#include "Tools.h"
#include "Mesh.h"
#include "Material.h"
#include "objectActions.h"

int main(int argc, char *argv[])
{
    Window *window = NULL;
    Renderer *renderer = NULL;
    Scene *scene = NULL;
    Mesh *mesh = NULL;

    // Initialise la SDL et crée la fenêtre
    int exitStatus = Settings_InitSDL();
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    window = Window_New();
    if (!window) goto ERROR_LABEL;

    renderer = Window_getRenderer(window);

    g_time = Timer_New();
    if (!g_time) goto ERROR_LABEL;

    // Crée la scène
    scene = Scene_New(window);
    if (!scene) goto ERROR_LABEL;

    //mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/Jaxy", "Jaxy.obj");
    mesh = Scene_CreateMeshFromOBJ(scene, "../Obj/CaptainToad", "CaptainToad.obj");
    if (!mesh) goto ERROR_LABEL;

    // Arbre de scène
    Object *root   = Scene_GetRoot(scene);
    Camera *camera = Scene_GetCamera(scene);

    // Crée l'objet
    Object *object = Scene_CreateObject(scene, sizeof(Object));
    if (!object) goto ERROR_LABEL;

    exitStatus = Object_Init(object, scene, Mat4_Identity, root);
    if (exitStatus != EXIT_SUCCESS) goto ERROR_LABEL;

    Object_SetMesh(object, mesh);

    // Calcule une échelle normalisée pour l'objet
    Vec3 meshCenter = mesh->m_center;
    Vec3 meshMin = mesh->m_min;
    Vec3 meshMax = mesh->m_max;
    float xSize = fabsf(meshMax.x - meshMin.x);
    float ySize = fabsf(meshMax.y - meshMin.y);
    float zSize = fabsf(meshMax.z - meshMin.z);
    float objectSize = fmaxf(xSize, fmaxf(ySize, zSize));
    float scale = 3.0f / objectSize;
    Mat4 finalScale = Mat4_GetScaleMatrix(scale);
    float scaleY = 0.f;

    // Centre l'objet en (0,0,0) et applique l'échelle
    Mat4 objectTransform = Mat4_Identity;
    objectTransform = Mat4_GetTranslationMatrix(Vec3_Neg(mesh->m_center));
    objectTransform = Mat4_MulMM(finalScale, objectTransform);
    Object_SetLocalTransform(object, objectTransform);

    // Lancement du temps global
    Timer_Start(g_time);

    // Paramètre initiaux de la caméra
    float camDistance = 10.f;
    float angleY = 0.0f;

    float fpsAccu = 0.0f;
    int frameCount = 0;

    bool objectJump = false;

    float rotatingSpeed = 0.f;
    bool rotating = true;

    bool quit = false;

    // variables pour le saut 
    float jumpVelocity = 5.f;
    float gravity = -9.81f;
    Mat4 toadTransform = objectTransform;
    Vec4 toadPosition = objectTransform.lines[0];
    float nextYPos = toadPosition.y;
    bool splosh = false;
    Timer* sploshTime = Timer_New();
    while (!quit)
    {
        SDL_Event evt;
        SDL_Scancode scanCode;
        // Met à jour le temps global
        Timer_Update(g_time);
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                scanCode = evt.key.keysym.scancode;
                if (evt.key.repeat)
                    break;

                switch (scanCode)
                {
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    Scene_SetWireframe(scene, !Scene_GetWireframe(scene));
                    break;
                case SDL_SCANCODE_KP_PLUS:
                    scene->m_lightColor = Vec3_Add(scene->m_lightColor, Vec3_One);
                    break;
                case SDL_SCANCODE_KP_MINUS:
                    scene->m_lightColor = Vec3_Sub(scene->m_lightColor, Vec3_One);
                    if (scene->m_lightColor.x < 0.f || scene->m_lightColor.y < 0.f || scene->m_lightColor.z < 0.f)
                        scene->m_lightColor = Vec3_Zero;
                    break;
                case SDL_SCANCODE_KP_0:
                    changeLightReturn(ALBEDO);
                    break;
                case SDL_SCANCODE_KP_1:
                    changeLightReturn(DIFFUSAL);
                    break;
                case SDL_SCANCODE_KP_2:
                    changeLightReturn(BLINNPHONG);
                    break;
                case SDL_SCANCODE_RIGHT:
                    rotatingSpeed += 1.f;
                    break;
                case SDL_SCANCODE_LEFT:
                    rotatingSpeed -= 1.f;
                    break;
                case SDL_SCANCODE_UP:
                    objectJump = true;
                    break;
                case SDL_SCANCODE_DOWN:
                    rotatingSpeed = 0.f;
                    break;
                case SDL_SCANCODE_P:
                    rotating = rotating == true ? false : true;
                    break;
                case SDL_SCANCODE_C:
                    camDistance = 10.f;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEWHEEL:
                if (evt.wheel.y > 0)
                    camDistance -= 0.5f;
                else
                    camDistance += 0.5f;
            default:
                break;
            }
        }

        // Calcule la rotation de la caméra
        angleY -= rotating ? rotatingSpeed * (360.f / 40.f * Timer_GetDelta(g_time)) : 0.f;

        // Calcule la matrice locale de la caméra
        Mat4 cameraModel = Mat4_Identity;
        cameraModel = Mat4_MulMM(
            Mat4_GetTranslationMatrix(Vec3_Set(0.f, 0.f, camDistance)),
            cameraModel
        );
        cameraModel = Mat4_MulMM(
            Mat4_GetYRotationMatrix(angleY),
            cameraModel
        );

        // Applique la matrice locale de la caméra
        Object_SetTransform(((Object *)camera), Scene_GetRoot(scene), cameraModel);

        // Calcule le rendu de la scène dans un buffer
        Scene_Render(scene);

        // Met à jour le rendu (affiche le buffer précédent)
        Renderer_Update(renderer);

        // Calcule les FPS
        fpsAccu += Timer_GetDelta(g_time);
        frameCount++;
        if (fpsAccu > 1.0f)
        {
            printf("FPS = %.1f\n", (float)frameCount / fpsAccu);
            fpsAccu = 0.0f;
            frameCount = 0;
        }
        Timer_GetDelta(g_time);
        // Calcule à chaque frame la nouvelle position du Toad durant le saut
        if (objectJump)
        {
            float delta = Timer_GetDelta(g_time);
            nextYPos += jumpVelocity * delta;
            jumpVelocity += gravity * delta;
            if (nextYPos < toadPosition.y)
            {
                scaleY = -1.f;
                Timer_Start(sploshTime);
                jumpVelocity = 5.f;
                splosh = true;
                objectJump = false;
                nextYPos = 0.f;
            }
        }
        if (splosh)
            Timer_Update(sploshTime);
        if (Timer_GetElapsed(sploshTime) > 2.f)
        {
            scaleY = 0.f;
            splosh = false;
        }

        float currentScaleY = Mat4_GetScaleMatrix(scale).data[1][1] + scaleY;
        finalScale.data[1][1] = currentScaleY;
        objectTransform = Mat4_GetTranslationMatrix(Vec3_Neg(mesh->m_center));
        objectTransform = Mat4_MulMM(finalScale, objectTransform);

        Object_SetLocalTransform(object, Mat4_MulMM(objectTransform, Mat4_GetTranslationMatrix(Vec3_Set(0.f, nextYPos, 0.f))));
    }

    Scene_Free(scene);
    Timer_Free(g_time);
    Window_Free(window);

    Settings_QuitSDL();

    return EXIT_SUCCESS;

ERROR_LABEL:
    printf("ERROR - main()\n");
    assert(false);
    Scene_Free(scene);
    Timer_Free(g_time);
    Window_Free(window);
    return EXIT_FAILURE;
}