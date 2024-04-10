#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include "CircleCollider.h"
#include <path_config.h>
#include "timer.h"
#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "game.h"
#include "CollectableGameObject.h"
#include "EnemyGameObject.h"
#include "Bullet.h"
#include <random>
#include "particles.h"
#include "particle_system.h"
#include "glm/ext.hpp"

#define HIT true
#define HEAL false


namespace game {

    // Some configuration constants
    // They are written here as global variables, but ideally they should be loaded from a configuration file

    // Globals that define the OpenGL window and viewport
    const char* window_title_g = "Assignment4";
    const unsigned int window_width_g = 800;
    const unsigned int window_height_g = 600;
    const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);
    std::vector<GameObject*> background_objects_;
    GameObject* HUDObj;
    std::vector<Bullet*> bullet_objects_;
    std::vector<Bullet*> enemy_bullets;
    int bulletcount = 0;
    int enemybulletcount = 0;
    GameObject* blade;
    int enemycount = 10;
    int itemcount = 10;

    


    // Directory with game resources such as textures
    const std::string resources_directory_g = RESOURCES_DIRECTORY;

    Game::Game(void) :playerAlive(true)
    {

    }


    void Game::Init(void)
    {

        // Initialize the window management library (GLFW)
        if (!glfwInit()) {
            throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
        }

        // Set whether window can be resized
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // Create a window and its OpenGL context
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
        if (!window_) {
            glfwTerminate();
            throw(std::runtime_error(std::string("Could not create window")));
        }

        // Make the window's OpenGL context the current one
        glfwMakeContextCurrent(window_);

        // Initialize the GLEW library to access OpenGL extensions
        // Need to do it after initializing an OpenGL context
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char*)glewGetErrorString(err))));
        }

        // Set event callbacks
        glfwSetFramebufferSizeCallback(window_, ResizeCallback);

        // Initialize sprite geometry
        sprite_ = new Sprite();
        sprite_->CreateGeometry();

        // Initialize particle geometry
        particles_ = new Particles();
        particles_->CreateGeometry();

        // Initialize particle shader
        particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());



        // Initialize sprite shader
        sprite_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/sprite_fragment_shader.glsl")).c_str());

        // Initialize time
        current_time_ = 0.0;
    }


    Game::~Game()
    {
        // Free memory for all objects
        // Only need to delete objects that are not automatically freed
        delete sprite_;
        delete particles_;
        for (int i = 0; i < game_objects_.size(); i++) {
            delete game_objects_[i];
        }

        // Close window
        glfwDestroyWindow(window_);
        glfwTerminate();
    }


    void Game::Setup(void)
    {
        //random enemy location generation

        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 eng(rd()); // Seed the generator
        std::uniform_real_distribution<> distr(-30.0, 30.0); // Define the range
        // Setup the game world

        // Load textures
        SetAllTextures();

        // Setup the player object (position, texture, vertex count)
        // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
        game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[0]));
        float pi_over_two = glm::pi<float>() / 2.0f;
        game_objects_[0]->SetRotation(pi_over_two);

        // Setup other objects
        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[1]->SetRotation(pi_over_two);
        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[2]->SetRotation(pi_over_two);
        // new enemy ships
        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[3]->SetRotation(pi_over_two);
        game_objects_[3]->SetScale(glm::vec2(1.0f, 0.5f));//demonstrates the scale setting features

        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[4]->SetRotation(pi_over_two);

        game_objects_.push_back(new Speeder(glm::vec3(5.0f, 0.0f , 0.0f), sprite_, &sprite_shader_, tex_[4]));
        game_objects_[5]->SetRotation(pi_over_two);
        


        //add collectable game objects 
        game_objects_.push_back(new CollectableGameObject(glm::vec3(-3.0f, 3.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
        game_objects_[6]->SetRotation(pi_over_two);

        game_objects_.push_back(new CollectableGameObject(glm::vec3(-3.0f, 2.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
        game_objects_[7]->SetRotation(pi_over_two);

        game_objects_.push_back(new CollectableGameObject(glm::vec3(-3.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
        game_objects_[8]->SetRotation(pi_over_two);

        game_objects_.push_back(new CollectableGameObject(glm::vec3(-3.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
        game_objects_[9]->SetRotation(pi_over_two);

        game_objects_.push_back(new CollectableGameObject(glm::vec3(-3.0f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[6]));
        game_objects_[10]->SetRotation(pi_over_two);


        //add enemy game objects
        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[11]->SetRotation(pi_over_two);

        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[12]->SetRotation(pi_over_two);

        game_objects_.push_back(new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
        game_objects_[13]->SetRotation(pi_over_two);

        game_objects_.insert(game_objects_.begin()+1,new GameObject(glm::vec3(19.0f,1.0f,0.0f), sprite_, &sprite_shader_, tex_[9]));
        game_objects_[1]->SetRotation(pi_over_two);
        game_objects_[1]->toggleCollide();
        GameObject* blade = game_objects_[1];
        blade->SetPosition(game_objects_[0]->GetPosition());
        blade->SetScale(glm::vec2(3.0, 3.0));
        



        // Setup background
        // In this specific implementation, the background is always the
        // last object
        GameObject* background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background);
        background_objects_.push_back(background);
        background->toggleCollide();
        GameObject* background2 = new GameObject(glm::vec3(20.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background2->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background2);
        background_objects_.push_back(background2);
        background2->toggleCollide();
        GameObject* background3 = new GameObject(glm::vec3(-20.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background3->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background3);
        background_objects_.push_back(background3);
        background3->toggleCollide();
        GameObject* background4 = new GameObject(glm::vec3(0.0f, -20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background4->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background4);
        background_objects_.push_back(background4);
        background4->toggleCollide();
        GameObject* background5 = new GameObject(glm::vec3(0.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background5->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background5);
        background_objects_.push_back(background5);
        background5->toggleCollide();
        GameObject* background6 = new GameObject(glm::vec3(20.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background6->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background6);
        background_objects_.push_back(background6);
        background6->toggleCollide();
        GameObject* background7 = new GameObject(glm::vec3(-20.0f, 20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background7->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background7);
        background_objects_.push_back(background7);
        background7->toggleCollide();
        GameObject* background8 = new GameObject(glm::vec3(-20.0f, -20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background8->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background8);
        background_objects_.push_back(background8);
        background8->toggleCollide();
        GameObject* background9 = new GameObject(glm::vec3(20.0f, -20.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
        background9->SetScale(glm::vec2(20.0f, 20.0f));
        game_objects_.push_back(background9);
        background_objects_.push_back(background9);
        background9->toggleCollide();

        // Setup HUD
        GameObject* newHud = new GameObject(glm::vec3(-2.0f, 2.0f, 0.0f), sprite_, &sprite_shader_, tex_[17]);
        HUDObj = newHud;

        // Setup particle system
        GameObject* particles = new ParticleSystem(glm::vec3(-0.5f, 0.0f, 0.0f), particles_, &particle_shader_, tex_[10], game_objects_[0]);
        particles->SetScale(glm::vec2(0.1,0.1));
        particles->SetRotation(-pi_over_two);
        game_objects_.push_back(particles);

        

    }


    void Game::ResizeCallback(GLFWwindow* window, int width, int height)
    {

        // Set OpenGL viewport based on framebuffer width and height
        glViewport(0, 0, width, height);
    }


    void Game::SetTexture(GLuint w, const char* fname)
    {
        // Bind texture buffer
        glBindTexture(GL_TEXTURE_2D, w);

        // Load texture from a file to the buffer
        int width, height;
        unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
        if (!image) {
            std::cout << "Cannot load texture " << fname << std::endl;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Texture Filtering    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


    void Game::SetAllTextures(void)
    {
        // Load all textures that we will need
        // Declare all the textures here
        const char* texture[] = { "/textures/player.png", "/textures/bomber.png", "/textures/enemyship_blue.png", "/textures/stars2.png","/textures/speeder.png",
            "/textures/ImmunePlayer.png","/textures/ShieldItem.png", "/textures/explosion0.png","/textures/bullet.png","/textures/blade.png","/textures/orb.png","/textures/heart.png", 
            "/textures/paintBucket.png","/textures/shooter.png", "/textures/HP0.png", "/textures/HP1.png", "/textures/HP2.png", "/textures/HP3.png"};
        // Get number of declared textures
        int num_textures = sizeof(texture) / sizeof(char*);
        // Allocate a buffer for all texture references
        tex_ = new GLuint[num_textures];
        glGenTextures(num_textures, tex_);
        // Load each texture 
        for (int i = 0; i < num_textures; i++) {
            SetTexture(tex_[i], (resources_directory_g + std::string(texture[i])).c_str());
        }
        // Set first texture in the array as default
        glBindTexture(GL_TEXTURE_2D, tex_[0]);
    }


    void Game::MainLoop(void)
    {
        // Loop while the user did not close the window
        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(window_)) {
            if (game_objects_[0]->boomTime()) {
                std::cout << "Game Over" << std::endl;
                glfwSetWindowShouldClose(window_, true);
                break;
            }
            // Calculate delta time
            double current_time = glfwGetTime();
            double delta_time = current_time - last_time;
            last_time = current_time;

            // Update window events like input handling
            glfwPollEvents();

            // Handle user input
            // Handle user input
            HandleControls(delta_time);

            // Update all the game objects
            Update(delta_time);

            // Render all the game objects
            Render();

            // Push buffer drawn in the background onto the display
            glfwSwapBuffers(window_);
        }


    }


    void Game::HandleControls(double delta_time)
    {
        // Get player game object
        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
        // Get background game objects
        

        // Get current position and angle
        glm::vec3 curpos = player->GetPosition();
        float angle = player->GetRotation();
        // Compute current bearing direction
        glm::vec3 dir = player->GetBearing();
        // Adjust motion increment and angle increment 
        // if translation or rotation is too slow
        float speed = delta_time * 100.0;
        float motion_increment = 0.001 * speed;
        float angle_increment = (glm::pi<float>() / 180.0f) * speed;

        glm::vec3 a = glm::vec3(0.0001f, 0.0001f, 0.0f);


        // Check for player input and make changes accordingly
        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            a = glm::vec3(0.0003, 0.0003, 0.0f) * dir;

            player->Accelerate(a);

        }
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            a = glm::vec3(-0.0003f, -0.0003f, 0.0f) * dir;
            player->Accelerate(a);
        }
        glm::vec3 up(0.0f, 0.0f, 1.0f);
        glm::vec3 right = glm::normalize(glm::cross(dir, up)); // Right direction relative to 'dir'


        if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
            // Correctly move to the right by applying acceleration perpendicular to the current direction

            a = right * 0.0003f;
            player->Accelerate(a);
        }
        if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
            // Move to the left by applying acceleration in the opposite direction of 'right'

            a = right * -0.0003f; // Adjust the magnitude as needed for the left movement
            player->Accelerate(a);
        }

        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            player->SetRotation(angle - angle_increment);
        }
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            player->SetRotation(angle + angle_increment);
        }
        if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
            player->SetPosition(curpos - motion_increment * player->GetRight());
        }
        if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
            player->SetPosition(curpos + motion_increment * player->GetRight());
        }
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window_, true);
        }
        if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
            PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
            if (player->shootready()) {
                bullet_objects_.push_back(new Bullet(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]));
                bullet_objects_[bulletcount]->SetScale(glm::vec2(0.5,0.5));
                bullet_objects_[bulletcount]->SetRotation(player->GetRotation());
                bullet_objects_[bulletcount]->SetVelocity(player->GetBearing());
                bulletcount++;
                player->startshoot();
            }
            
        }

    }

    




    void Game::Update(double delta_time) {
        current_time_ += delta_time;

        // Get a reference to the player object
        GameObject* player = game_objects_[0];
        GameObject* blade = game_objects_[1];

        EnemyCreator(player->GetPosition());
        ItemCreator(player->GetPosition());

       
        for (Bullet* obj : bullet_objects_)
        {
            obj->Update(delta_time);
            if (obj->lifeover()) {
                obj->SetTemporary(true);
                bulletcount--;
            }
        }
        for (Bullet* obj : enemy_bullets)
        {
            obj->Update(delta_time);
            if (obj->lifeover()) {
                obj->SetTemporary(true);
                enemybulletcount--;
            }
        }

        for (Bullet* bullet : bullet_objects_) {
            glm::vec3 rayOrigin = bullet->GetPosition();
            glm::vec3 rayDirection = bullet->GetBearing(); // Assuming velocity gives the ray direction

            for (GameObject* obj : game_objects_) {
                if (obj == game_objects_[0]) {
                    continue;
                }
                EnemyGameObject* target = dynamic_cast<EnemyGameObject*>(obj);
                if (target != nullptr) {
                    float collisionTime = bullet->FindCollisionTime(rayOrigin, rayDirection, bullet->GetVelocity(), target->GetPosition(), 0.45f, delta_time);
                    if (collisionTime >= 0.0) {
                        Shooter* shooter = dynamic_cast<Shooter*>(target);
                        if (shooter != nullptr) {
                            shooter->setShoot(false);
                        }
                        Speeder* speeder = dynamic_cast<Speeder*>(target);
                        if (speeder != nullptr) {
                            speeder->setShoot(false);
                        }
                        target->SetTex(tex_[7]);
                        target->toggleCollide();
                        target->boomStart();
                        bullet->SetTemporary(true);
                        bulletcount--;
                        break; // Optional: Stop checking collisions for this bullet
                    }
                }
            }
        }
        for (Bullet* bullet : enemy_bullets) {
            glm::vec3 rayOrigin = bullet->GetPosition();
            glm::vec3 rayDirection = bullet->GetBearing(); // Assuming velocity gives the ray direction

            PlayerGameObject* obj = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
                if (obj != nullptr) {
                    float collisionTime = bullet->FindCollisionTime(rayOrigin, rayDirection, bullet->GetVelocity(), obj->GetPosition(), 0.45f, delta_time);
                    if (collisionTime >= 0.0) {
                        std::cout << "PLAYERSHOT" << std::endl;
                        obj->SetTex(tex_[7]);
                        obj->toggleCollide();
                        obj->boomStart();
                        bullet->SetTemporary(true);
                        enemybulletcount--;
                        break; // Optional: Stop checking collisions for this bullet
                    }
                }
            }
       



        for (GameObject* obj : game_objects_) {
            
            EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(obj);

            Shooter* shooter = dynamic_cast<Shooter*>(obj);

            Speeder* speeder= dynamic_cast<Speeder*>(obj);


            if (shooter != nullptr) {


                glm::vec3 playerPosition = player->GetPosition();
                glm::vec3 shooterPosition = shooter->GetPosition();
                glm::vec3 distanceVector;

                distanceVector.x = std::abs(playerPosition.x - shooterPosition.x);
                distanceVector.y = std::abs(playerPosition.y - shooterPosition.y);


                if (distanceVector.x <= 5.0f && distanceVector.y <= 5.0f && shooter->BlindFinished()) {
                    shooter->SetTargetPosition(player->GetPosition());
                    shooter->setState(Shooter::State::Intercepting);
                    
                    if (shooter->shootDone() && shooter->canShoot()) {
                        enemy_bullets.push_back(new Bullet(shooter->GetPosition(), sprite_, &sprite_shader_, tex_[8]));
                        enemy_bullets[enemybulletcount]->SetScale(glm::vec2(0.5, 0.5));
                        enemy_bullets[enemybulletcount]->SetRotation(shooter->GetRotation());
                        enemy_bullets[enemybulletcount]->SetVelocity(shooter->GetBearing()/2.0f);
                        enemybulletcount++;

                    }
                    
                }
                else {
                    shooter->setState(Shooter::State::Patrolling);

                }
            }

            //


            if (speeder != nullptr) {


                glm::vec3 playerPosition = player->GetPosition();
                glm::vec3 shooterPosition = speeder->GetPosition();
                glm::vec3 distanceVector;

                distanceVector.x = std::abs(playerPosition.x - shooterPosition.x);
                distanceVector.y = std::abs(playerPosition.y - shooterPosition.y);


                if (distanceVector.x <= 5.0f && distanceVector.y <= 5.0f && speeder->BlindFinished()) {
                    speeder->SetTargetPosition(player->GetPosition());
                    speeder->setState(Speeder::State::Intercepting);

                    if (speeder->shootDone() && speeder->canShoot()) {
                        enemy_bullets.push_back(new Bullet(speeder->GetPosition(), sprite_, &sprite_shader_, tex_[8]));
                        enemy_bullets[enemybulletcount]->SetScale(glm::vec2(0.5, 0.5));
                        enemy_bullets[enemybulletcount]->SetRotation(speeder->GetRotation());
                        enemy_bullets[enemybulletcount]->SetVelocity(speeder->GetBearing() / 3.0f);
                        enemybulletcount++;

                    }

                }
                else {
                    speeder->setState(Speeder::State::Patrolling);

                }
            }

            if (enemy != nullptr) {

                // Calculate the distance between the player and this enemy
                glm::vec3 playerPosition = player->GetPosition();
                glm::vec3 enemyPosition = enemy->GetPosition();
                glm::vec3 distanceVector;

                distanceVector.x = std::abs(playerPosition.x - enemyPosition.x);
                distanceVector.y = std::abs(playerPosition.y - enemyPosition.y);


                // If the enemy is within 20.0f units of the player, switch its state to Intercepting
                if (distanceVector.x <= 5.0f && distanceVector.y <= 5.0f && enemy->BlindFinished()) {

                    enemy->SetTargetPosition(player->GetPosition());
                    enemy->setState(EnemyGameObject::State::Intercepting);
                }
                else {
                    enemy->setState(EnemyGameObject::State::Patrolling);

                }
            }
            for (int i = 0; i < game_objects_.size(); i++) {
                GameObject* current_game_object = game_objects_[i];
                if (current_game_object->boomTime() == true) {
                    enemycount++;
                    
                    current_game_object->SetTemporary(true);

                    
                }
                if (current_game_object->invincibleDone()) {
                    current_game_object->onCollide();

                    current_game_object->SetTex(tex_[0]);
                }
                
                else {
                    // Update the current game object
                    current_game_object->Update(delta_time);

                    // Check for collision with other game objects
                    // Avoid testing the last object (background)
                    for (int j = i + 1; j < game_objects_.size() - 1; j++) {
                        GameObject* other_game_object = game_objects_[j];


                        // Use the collidesWith method for collision detection
                        if (current_game_object->GetCollider().collidesWith(other_game_object->GetCollider())) {

                            if (current_game_object == game_objects_[0] && other_game_object->isCollect()) {
                                HandleCollection(current_game_object, other_game_object);

                            }



                            else if (current_game_object->canCollide() == true && other_game_object->canCollide() == true) {
                                HandleCollision(current_game_object, other_game_object, HIT);
                            }
                        }
                    }
                }
            }

            if (!player->inTile(background_objects_[0])) {
                for (int i = 0; i < background_objects_.size(); ++i) {
                    if (player->inTile(background_objects_[i])) {
                        HandleBackground(game_objects_[0], background_objects_[i]);
                        break;
                    }
                }
            }


            







            // Remove temporary objects
            auto removeIt = std::remove_if(game_objects_.begin(), game_objects_.end(), [](GameObject* obj) {
                bool isTemp = obj->IsTemporary();
                if (isTemp) {
                    delete obj;  // Deallocate memory if the bullet is temporary
                }
                return isTemp;
                });
            game_objects_.erase(removeIt, game_objects_.end());

            auto removeIt2 = std::remove_if(bullet_objects_.begin(), bullet_objects_.end(), [](Bullet* obj) {
                bool isTemp = obj->IsTemporary();
                if (isTemp) {
                    delete obj;  // Deallocate memory if the bullet is temporary
                }
                return isTemp;
                });
            bullet_objects_.erase(removeIt2, bullet_objects_.end());

            auto removeIt3 = std::remove_if(enemy_bullets.begin(), enemy_bullets.end(), [](Bullet* obj) {
                bool isTemp = obj->IsTemporary();
                if (isTemp) {
                    delete obj;  // Deallocate memory if the bullet is temporary
                }
                return isTemp;
                });
            enemy_bullets.erase(removeIt3, enemy_bullets.end());
            

          

        }
    }


    void Game::HandleCollection(GameObject* Object1, GameObject* Object2) {
        Object2->toggleCollide();
        Object2->SetTemporary(true);
        itemcount++;

        if (Object2->GetTex() == tex_[6]) {
            shieldCollections++;
        }

        if (Object2->GetTex() == tex_[11]) {
            heartCollections++;
        }

        if (Object2->GetTex() == tex_[12]) {
            paintCollections++;
        }

        if (shieldCollections == 5) {
            shieldCollections = 0;

            Object1->invincibleStart();
            Object1->toggleCollide();
            Object1->SetTex(tex_[5]);
        }

        if (heartCollections == 5) {
            heartCollections = 0;
            HandleCollision(Object1, Object2, HEAL);
        }

        if (paintCollections == 5) {
            paintCollections = 0;
            findClosestEnemy(game_objects_[0])->BlindStart();

        }


    }



    void Game::HandleCollision(GameObject* object1, GameObject* object2, bool hitOrHeal) {
        // Define a local variable to count player collisions
        static int playerCollisions = 0;


        if ((object1 == game_objects_[0] || object2 == game_objects_[0]) && hitOrHeal == HIT) {
            // Check if either object1 or object2 is the player object
            playerCollisions++; // Increment the player collision counter


            // Check if the player has reached three collisions
            if (playerCollisions >= 3) {
                playerAlive = false;
                object1->SetTex(tex_[7]);

                //start player timer here
                object1->toggleCollide();
                object1->boomStart();
                object2->boomStart();
            }
        }

        if (hitOrHeal == HEAL && playerCollisions > 0) {
            playerCollisions--;
            HUD(3 - playerCollisions);
        }

        object2->SetTex(tex_[7]);
        object2->toggleCollide();
        object2->boomStart();

        //start timers here 




    }



    void Game::HandleBackground(GameObject* player, GameObject* tile) {

        GameObject* middleTile = background_objects_[0];
        glm::vec3 offset = middleTile->GetPosition() - tile->GetPosition();
        
        for (int i = 0; i < background_objects_.size(); i++) {
            background_objects_[i]->SetPosition(background_objects_[i]->GetPosition() - offset);
        }
    }



    void Game::Render(void) {
        // Clear background
        glClearColor(viewport_background_color_g.r,
            viewport_background_color_g.g,
            viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use aspect ratio to properly scale the window
        int width, height;
        glfwGetWindowSize(window_, &width, &height);
        glm::mat4 window_scale_matrix;
        if (width > height) {
            float aspect_ratio = ((float)width) / ((float)height);
            window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / aspect_ratio, 1.0f, 1.0f));
        }
        else {
            float aspect_ratio = ((float)height) / ((float)width);
            window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f / aspect_ratio, 1.0f));
        }

        // Set view to zoom out
        float camera_zoom = 0.15f; // Adjust zoom level as needed
        glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, 1.0f));

        // Get the player's current position
        glm::vec3 playerPosition = game_objects_[0]->GetPosition();

        // Create a translation matrix to follow the player's movement
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -playerPosition);

        // Combine all transformations: note the order of multiplication
        glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * translationMatrix;

        // Render all game objects
        
        for (int i = 0; i < bullet_objects_.size(); i++) {
            bullet_objects_[i]->Render(view_matrix, current_time_);
        }

        for (int i = 0; i < enemy_bullets.size(); i++) {
            enemy_bullets[i]->Render(view_matrix, current_time_);
        }

        game_objects_[1]->Render(view_matrix, current_time_, game_objects_[1]->BladeTransform(game_objects_[0]->GetPosition(), game_objects_[0]->GetAngle()));
        for (int i = 0; i < game_objects_.size(); i++) {
            game_objects_[i]->Render(view_matrix, current_time_);
        }

        HUDObj->Render(view_matrix, current_time_);
        
        
    }
    void Game::EnemyCreator(glm::vec3 playerpos) {
        while (enemycount > 0) {
            std::cout << "create enemy" << enemycount << std::endl;
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 eng(rd()); // Seed the generator
            std::uniform_real_distribution<> distr(-30.0 + playerpos.x, 30.0 + playerpos.x); // Define the range
            
            int type = distr(eng);

            if (type >= -30.0 && type < -10.0) {
                game_objects_.insert(game_objects_.end() - 10, new EnemyGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[1]));
            }
            else if (type >= -10.0 && type < 10.0) {
                game_objects_.insert(game_objects_.end() - 10, new Shooter(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[13]));
            }
            else if (type >= 10.0 && type <= 30.0) {
                game_objects_.insert(game_objects_.end() - 10, new Speeder(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[4]));
            }

            enemycount--;
        }
    }

    void Game::ItemCreator(glm::vec3 playerpos) {
        while (itemcount > 0) {
            std::cout << "create item" << itemcount << std::endl;
            std::random_device rd; // Obtain a random number from hardware
            std::mt19937 eng(rd()); // Seed the generator
            std::uniform_real_distribution<> distr(-30.0 + playerpos.x, 30.0 + playerpos.x);

            int type = distr(eng);

            if (type >= -30.0 && type < -10.0) {
                game_objects_.insert(game_objects_.end() - 10, new CollectableGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[6]));
            }
            else if (type >= -10.0 && type < 10.0) {
                game_objects_.insert(game_objects_.end() - 10, new CollectableGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[11]));
            }
            else if (type >= 10.0 && type <= 30.0) {
                game_objects_.insert(game_objects_.end() - 10, new CollectableGameObject(glm::vec3(distr(eng), distr(eng), 0.0f), sprite_, &sprite_shader_, tex_[12]));
            }   

            itemcount--;
        }
    }



    EnemyGameObject* Game::findClosestEnemy(GameObject* player) {
        EnemyGameObject* closestEnemy = nullptr;
        float shortestDistance = std::numeric_limits<float>::max();

        for (GameObject* obj : game_objects_) {
            EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(obj);

            // To make sure the object is an enemy type
            if (enemy != nullptr) {
                float distance = glm::distance(player->GetPosition(), enemy->GetPosition());

                    if (distance < shortestDistance) {
                        closestEnemy = enemy;
                        shortestDistance = distance;
                    }
            }
        }

        return closestEnemy;
    }

    void Game::HUD(int hp) {

        if (hp == 0) {
            HUDObj->SetTex(tex_[14]);
        }
        if (hp == 1) {
            HUDObj->SetTex(tex_[15]);
        }
        if (hp == 2) {
            HUDObj->SetTex(tex_[16]);
        }
        if (hp == 3) {
            HUDObj->SetTex(tex_[17]);
        }
    }

   

} // namespace game

