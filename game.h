#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "CircleCollider.h"
#include "shader.h"
#include "game_object.h"
#include "timer.h"
#include "Bullet.h"
#include "EnemyGameObject.h"
namespace game {

    // A class for holding the main game objects
    class Game {

        bool playerAlive;
        Timer collisionTimer;
        bool collisionTimerStarted;
        bool gamePaused;
        int shieldCollections = 0;
        int heartCollections = 0;
        int paintCollections = 0;
       

    public:
        // Constructor and destructor
        Game(void);
        ~Game();

        // Call Init() before calling any other method
        // Initialize graphics libraries and main window
        void Init(void);

        // Set up the game (scene, game objects, etc.)
        void Setup(void);

        // Run the game (keep the game active)
        void MainLoop(void);

    private:
        Timer timer;
        // Main window: pointer to the GLFW window structure
        GLFWwindow* window_;

        // Sprite geometry
        Geometry* sprite_;

        // Particle geometry
        Geometry* particles_;

        // Shader for rendering sprites in the scene
        Shader sprite_shader_;

        // Shader for rendering particles
        Shader particle_shader_;

        // References to textures
        // This needs to be a pointer
        GLuint* tex_;

        // List of game objects
        std::vector<GameObject*> game_objects_;

        // Keep track of time
        double current_time_;

        // Callback for when the window is resized
        static void ResizeCallback(GLFWwindow* window, int width, int height);

        // Set a specific texture
        void SetTexture(GLuint w, const char* fname);

        // Load all textures
        void SetAllTextures();

        // Handle user input
        void HandleControls(double delta_time);
        
        // Enemy Creation
        void EnemyCreator(glm::vec3 playerpos);

        // Item Creation
        void ItemCreator(glm::vec3 playerpos);

        // Update all the game objects
        void Update(double delta_time);

        // Does the appropriate actions when a collision is detected
        void HandleCollision(GameObject* object1, GameObject* Object2, bool hitOrHeal);

        // Does the appropriate actions when an item is collected
        void HandleCollection(GameObject* object1, GameObject* Object2);

        // Takes care of background scrolling
        void HandleBackground(GameObject* player, GameObject* tile);

        // Finds the closest enemy to the player
        EnemyGameObject* findClosestEnemy(GameObject* player);

        // Render the game world
        void Render(void);

        void BladeTransform(const glm::vec3&);

        // Information HUD for player
        void HUD(int hp);

    }; // class Game

} // namespace game

#endif // GAME_H_
