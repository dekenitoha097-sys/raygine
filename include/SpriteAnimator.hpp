/**
 * @file SpriteAnimator.hpp
 * @brief Generic sprite animator class for raylib
 */
#ifndef SPRITE_ANIMATOR_HPP
#define SPRITE_ANIMATOR_HPP

#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <string>

/**
 * @struct AnimationFrame
 * @brief Represents a single frame of animation
 */
struct AnimationFrame {
    int x;      // X position on sprite sheet
    int y;      // Y position on sprite sheet
    int width;  // Width of the sprite
    int height; // Height of the sprite
};

/**
 * @struct Animation
 * @brief Contains all frames for a specific animation direction
 */
struct Animation {
    std::vector<AnimationFrame> frames;
    float frameDuration;          // Duration per frame in seconds
    bool loop;                    // Whether the animation loops
};

/**
 * @class SpriteAnimator
 * @brief Generic sprite animator that handles sprite sheet animations based on keyboard input
 */
class SpriteAnimator {
private:
    Texture2D spriteSheet;         // The sprite sheet texture
    
    int sheetWidth;                // Total width of sprite sheet
    int sheetHeight;              // Total height of sprite sheet
    int spriteWidth;              // Width of each individual sprite
    int spriteHeight;             // Height of each individual sprite
    
    int columns;                  // Number of columns in sprite sheet
    int rows;                     // Number of rows in sprite sheet
    
    std::unordered_map<std::string, Animation> animations;  // Animation map
    std::vector<std::pair<KeyboardKey, std::string>> animationKeyBindings; // Key -> animation name
    
    std::string currentAnimation;  // Current animation name
    std::string idleAnimation;     // Optional idle animation name
    int currentFrame;             // Current frame index
    float currentFrameTime;        // Time accumulator for frame switching
    
    Vector2 position;             // Current position on screen
    float scale;                  // Scale factor for rendering
    float rotation;               // Rotation angle
    Color tint;                   // Color tint
    
    bool isAnimating;             // Whether animation is currently playing
    bool facingRight;             // Direction facing (true = right, false = left)
    bool useIdleAnimation;        // Whether idle animation is enabled
    bool actionInProgress;        // Whether a one-shot action is currently playing
    std::string actionAnimation;  // Current action animation name
    std::string animationBeforeAction; // Animation to restore after action

public:
    /**
     * @brief Constructor with sprite sheet
     * @param spriteSheetPath Path to the sprite sheet image file
     * @param spriteWidth Width of each individual sprite
     * @param spriteHeight Height of each individual sprite
     */
    SpriteAnimator(const char* spriteSheetPath, int spriteWidth, int spriteHeight);
    
    /**
     * @brief Constructor with existing texture
     * @param texture Existing raylib texture
     * @param spriteWidth Width of each individual sprite
     * @param spriteHeight Height of each individual sprite
     */
    SpriteAnimator(Texture2D texture, int spriteWidth, int spriteHeight);
    
    /**
     * @brief Destructor
     */
    ~SpriteAnimator();
    
    /**
     * @brief Add an animation for a specific direction
     * @param name Animation name (e.g., "up", "a", "1", "space", "f1", "kp_0")
     * @param row Row index in sprite sheet for this animation
     * @param frameCount Number of frames in this animation
     * @param frameDuration Duration of each frame in seconds
     * @param loop Whether the animation should loop
     */
    void AddAnimation(const std::string& name, int row, int frameCount, 
                      float frameDuration = 0.1f, bool loop = true);
    
    /**
     * @brief Set the current animation
     * @param name Animation name to play
     */
    void SetAnimation(const std::string& name);

    /**
     * @brief Enable idle animation played when no movement key is pressed
     * @param name Animation name to use for idle state
     */
    void SetIdleAnimation(const std::string& name);

    /**
     * @brief Disable idle animation behavior
     */
    void ClearIdleAnimation();
    
    /**
     * @brief Set the position of the sprite
     * @param x X position
     * @param y Y position
     */
    void SetPosition(float x, float y);
    
    /**
     * @brief Set the position of the sprite (Vector2)
     * @param pos Position vector
     */
    void SetPosition(Vector2 pos);
    
    /**
     * @brief Set the scale factor
     * @param scale Scale value (1.0 = original size)
     */
    void SetScale(float scale);
    
    /**
     * @brief Set the rotation angle
     * @param degrees Rotation in degrees
     */
    void SetRotation(float degrees);
    
    /**
     * @brief Set the color tint
     * @param tint Color to tint the sprite
     */
    void SetTint(Color tint);
    
    /**
     * @brief Set the frame duration for current animation
     * @param duration Duration in seconds
     */
    void SetFrameDuration(float duration);
    
    /**
     * @brief Get current position
     * @return Current position vector
     */
    Vector2 GetPosition() const;
    
    /**
     * @brief Get current animation name
     * @return Current animation name
     */
    std::string GetCurrentAnimation() const;
    
    /**
     * @brief Check if animation is playing
     * @return True if animating
     */
    bool IsAnimating() const;
    
    /**
     * @brief Check if sprite is facing right
     * @return True if facing right
     */
    bool IsFacingRight() const;
    
    /**
     * @brief Update animation based on keyboard input
     * @param deltaTime Time since last frame in seconds
     * 
     * This method checks arrow keys and updates animation accordingly:
     * - UP: Play "up" animation
     * - DOWN: Play "down" animation
     * - LEFT: Play "left" animation, face left
     * - RIGHT: Play "right" animation, face right
     * - Also supports key-name animations (e.g., "a", "1", "space", "f1", "kp_0", etc.)
     */
    void Update(float deltaTime);
    
    /**
     * @brief Update with custom key bindings
     * @param deltaTime Time since last frame in seconds
     * @param upKey Key for up animation
     * @param downKey Key for down animation
     * @param leftKey Key for left animation
     * @param rightKey Key for right animation
     */
    void UpdateWithKeys(float deltaTime, KeyboardKey upKey, KeyboardKey downKey, 
                        KeyboardKey leftKey, KeyboardKey rightKey);
    
    /**
     * @brief Manually update animation (no keyboard input)
     * @param deltaTime Time since last frame in seconds
     * @param animationName Animation to play
     * @param isMoving Whether the character is moving
     */
    void UpdateManual(float deltaTime, const std::string& animationName, bool isMoving);

    /**
     * @brief Play a one-shot action animation when a key is pressed (e.g., attack)
     * @param deltaTime Time since last frame in seconds
     * @param actionKey Key that triggers the action
     * @param actionName Animation name used for the action
     * @return True when the action finishes on this frame, otherwise false
     */
    bool UpdateAction(float deltaTime, KeyboardKey actionKey, const std::string& actionName);
    
    /**
     * @brief Draw the current sprite frame
     */
    void Draw() const;
    
    /**
     * @brief Draw the sprite with explicit position and size
     * @param x X position to draw at
     * @param y Y position to draw at
     * @param width Width of the sprite
     * @param height Height of the sprite
     */
    void Draw(float x, float y, float width, float height) const;
    
    /**
     * @brief Draw the sprite with custom parameters
     * @param pos Position to draw at
     * @param scale Scale factor
     * @param tint Color tint
     */
    void Draw(Vector2 pos, float scale, Color tint) const;
    
    /**
     * @brief Get the bounding box of the current sprite
     * @return Rectangle representing the sprite bounds
     */
    Rectangle GetBounds() const;
    
    /**
     * @brief Get sprite dimensions
     * @return Vector2 with width and height
     */
    Vector2 GetDimensions() const;

private:
    /**
     * @brief Initialize common properties
     */
    void Init(int spriteWidth, int spriteHeight);
    
    /**
     * @brief Calculate columns and rows from sprite dimensions
     */
    void CalculateGrid();
    
    /**
     * @brief Advance to the next frame
     */
    void NextFrame();
    
    /**
     * @brief Get the current frame rectangle from sprite sheet
     * @return Rectangle for current frame
     */
    Rectangle GetCurrentFrameRect() const;

    /**
     * @brief Advance current animation based on delta time
     */
    void AdvanceAnimation(float deltaTime);

    /**
     * @brief Parse animation name as a key name and return matching KeyboardKey
     */
    bool TryResolveAnimationKey(const std::string& animationName, KeyboardKey& outKey) const;
};

#endif // SPRITE_ANIMATOR_HPP
