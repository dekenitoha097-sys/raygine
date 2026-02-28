/**
 * @file AmbientAnimator.hpp
 * @brief Ambient sprite animator for automatic looping animations (grass, water, etc.)
 */
#ifndef AMBIENT_ANIMATOR_HPP
#define AMBIENT_ANIMATOR_HPP

#include "raylib.h"
#include <vector>
#include <string>

/**
 * @class AmbientAnimator
 * @brief Automatic looping sprite animator for ambient animations
 * 
 * This class is designed for animations that play automatically without user input,
 * such as grass swaying, water ripples, fire, clouds, etc.
 */
class AmbientAnimator {
private:
    Texture2D spriteSheet;         // The sprite sheet texture
    
    int sheetWidth;               // Total width of sprite sheet
    int sheetHeight;              // Total height of sprite sheet
    int spriteWidth;              // Width of each individual sprite
    int spriteHeight;             // Height of each individual sprite
    
    int columns;                  // Number of columns in sprite sheet
    int rows;                     // Number of rows in sprite sheet
    
    // Animation sequence - can be any arbitrary sequence of frames
    std::vector<Rectangle> frames;  // Frame sequence
    int currentFrame;              // Current frame index
    float currentFrameTime;        // Time accumulator for frame switching
    float frameDuration;           // Duration per frame in seconds
    
    Vector2 position;              // Current position on screen
    float scale;                   // Scale factor for rendering
    float rotation;                // Rotation angle
    Color tint;                    // Color tint
    
    bool isPlaying;                // Whether animation is playing
    bool loop;                     // Whether animation loops
    bool autoStart;                // Whether to start automatically

public:
    /**
     * @brief Constructor with sprite sheet path
     * @param spriteSheetPath Path to the sprite sheet image file
     * @param spriteWidth Width of each individual sprite
     * @param spriteHeight Height of each individual sprite
     */
    AmbientAnimator(const char* spriteSheetPath, int spriteWidth, int spriteHeight);
    
    /**
     * @brief Constructor with existing texture
     * @param texture Existing raylib texture
     * @param spriteWidth Width of each individual sprite
     * @param spriteHeight Height of each individual sprite
     */
    AmbientAnimator(Texture2D texture, int spriteWidth, int spriteHeight);
    
    /**
     * @brief Destructor
     */
    ~AmbientAnimator();
    
    /**
     * @brief Set animation frames from a row (left to right)
     * @param row Row index in sprite sheet
     * @param frameCount Number of frames to use
     */
    void SetAnimationRow(int row, int frameCount);
    
    /**
     * @brief Set animation frames from a column (top to bottom)
     * @param col Column index in sprite sheet
     * @param frameCount Number of frames to use
     */
    void SetAnimationColumn(int col, int frameCount);
    
    /**
     * @brief Set custom animation sequence with specific frame positions
     * @param frameRects Vector of rectangles defining each frame position
     */
    void SetCustomAnimation(const std::vector<Rectangle>& frameRects);
    
    /**
     * @brief Set custom animation from frame indices
     * @param indices Vector of (row, column) pairs for each frame
     */
    void SetAnimationFromIndices(const std::vector<std::pair<int, int>>& indices);
    
    /**
     * @brief Set the frame duration
     * @param duration Duration of each frame in seconds
     */
    void SetFrameDuration(float duration);
    
    /**
     * @brief Set whether animation should loop
     * @param loop True to loop, false to play once
     */
    void SetLoop(bool loop);
    
    /**
     * @brief Set whether animation starts automatically
     * @param autoStart True to start automatically
     */
    void SetAutoStart(bool autoStart);
    
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
     * @brief Start the animation
     */
    void Play();
    
    /**
     * @brief Pause the animation
     */
    void Pause();
    
    /**
     * @brief Stop and reset animation
     */
    void Stop();
    
    /**
     * @brief Reset to first frame
     */
    void Reset();
    
    /**
     * @brief Get current position
     * @return Current position vector
     */
    Vector2 GetPosition() const;
    
    /**
     * @brief Check if animation is playing
     * @return True if playing
     */
    bool IsPlaying() const;
    
    /**
     * @brief Check if animation is finished (one-shot)
     * @return True if finished playing
     */
    bool IsFinished() const;
    
    /**
     * @brief Get current frame index
     * @return Current frame index
     */
    int GetCurrentFrame() const;
    
    /**
     * @brief Get total number of frames
     * @return Total frame count
     */
    int GetFrameCount() const;
    
    /**
     * @brief Update animation (call this every frame)
     * @param deltaTime Time since last frame in seconds
     */
    void Update(float deltaTime);
    
    /**
     * @brief Draw the current sprite frame
     */
    void Draw() const;
    
    /**
     * @brief Draw at custom position
     * @param pos Position to draw at
     */
    void Draw(Vector2 pos) const;
    
    /**
     * @brief Draw with custom parameters
     * @param pos Position to draw at
     * @param scale Scale factor
     * @param tint Color tint
     */
    void Draw(Vector2 pos, float scale, Color tint) const;
    
    /**
     * @brief Draw multiple instances in a row
     * @param count Number of instances
     * @param spacing Spacing between instances
     */
    void DrawTiled(int count, float spacing) const;
    
    /**
     * @brief Get the bounding box
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
     * @brief Calculate columns and rows
     */
    void CalculateGrid();
    
    /**
     * @brief Advance to next frame
     */
    void NextFrame();
};

#endif // AMBIENT_ANIMATOR_HPP
