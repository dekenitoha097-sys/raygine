/**
 * @file AmbientAnimator.cpp
 * @brief Implementation of AmbientAnimator class
 */
#include "AmbientAnimator.hpp"

AmbientAnimator::AmbientAnimator(const char* spriteSheetPath, int spriteWidth, int spriteHeight) 
    : spriteWidth(spriteWidth), spriteHeight(spriteHeight),
      scale(1.0f), rotation(0.0f), currentFrame(0), currentFrameTime(0.0f),
      frameDuration(0.1f), isPlaying(false), loop(true), autoStart(true)
{
    spriteSheet = LoadTexture(spriteSheetPath);
    Init(spriteWidth, spriteHeight);
}

AmbientAnimator::AmbientAnimator(Texture2D texture, int spriteWidth, int spriteHeight)
    : spriteSheet(texture), spriteWidth(spriteWidth), spriteHeight(spriteHeight),
      scale(1.0f), rotation(0.0f), currentFrame(0), currentFrameTime(0.0f),
      frameDuration(0.1f), isPlaying(false), loop(true), autoStart(true)
{
    Init(spriteWidth, spriteHeight);
}

void AmbientAnimator::Init(int spriteWidth, int spriteHeight) {
    sheetWidth = spriteSheet.width;
    sheetHeight = spriteSheet.height;
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;
    
    position = { 0.0f, 0.0f };
    tint = WHITE;
    
    CalculateGrid();
    
    if (autoStart) {
        isPlaying = true;
    }
}

void AmbientAnimator::CalculateGrid() {
    columns = sheetWidth / spriteWidth;
    rows = sheetHeight / spriteHeight;
}

AmbientAnimator::~AmbientAnimator() {
    if (spriteSheet.id > 0) {
        UnloadTexture(spriteSheet);
    }
}

void AmbientAnimator::SetAnimationRow(int row, int frameCount) {
    frames.clear();
    
    for (int i = 0; i < frameCount && i < columns; i++) {
        Rectangle frame;
        frame.x = (float)(i * spriteWidth);
        frame.y = (float)(row * spriteHeight);
        frame.width = (float)spriteWidth;
        frame.height = (float)spriteHeight;
        frames.push_back(frame);
    }
    
    if (autoStart) {
        isPlaying = true;
    }
}

void AmbientAnimator::SetAnimationColumn(int col, int frameCount) {
    frames.clear();
    
    for (int i = 0; i < frameCount && i < rows; i++) {
        Rectangle frame;
        frame.x = (float)(col * spriteWidth);
        frame.y = (float)(i * spriteHeight);
        frame.width = (float)spriteWidth;
        frame.height = (float)spriteHeight;
        frames.push_back(frame);
    }
    
    if (autoStart) {
        isPlaying = true;
    }
}

void AmbientAnimator::SetCustomAnimation(const std::vector<Rectangle>& frameRects) {
    frames = frameRects;
    
    if (autoStart) {
        isPlaying = true;
    }
}

void AmbientAnimator::SetAnimationFromIndices(const std::vector<std::pair<int, int>>& indices) {
    frames.clear();
    
    for (const auto& index : indices) {
        int row = index.first;
        int col = index.second;
        
        if (row >= 0 && row < rows && col >= 0 && col < columns) {
            Rectangle frame;
            frame.x = (float)(col * spriteWidth);
            frame.y = (float)(row * spriteHeight);
            frame.width = (float)spriteWidth;
            frame.height = (float)spriteHeight;
            frames.push_back(frame);
        }
    }
    
    if (autoStart) {
        isPlaying = true;
    }
}

void AmbientAnimator::SetFrameDuration(float duration) {
    frameDuration = duration;
}

void AmbientAnimator::SetLoop(bool loop) {
    this->loop = loop;
}

void AmbientAnimator::SetAutoStart(bool autoStart) {
    this->autoStart = autoStart;
    if (autoStart && frames.size() > 0) {
        isPlaying = true;
    }
}

void AmbientAnimator::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void AmbientAnimator::SetPosition(Vector2 pos) {
    position = pos;
}

void AmbientAnimator::SetScale(float scale) {
    this->scale = scale;
}

void AmbientAnimator::SetRotation(float degrees) {
    this->rotation = degrees;
}

void AmbientAnimator::SetTint(Color tint) {
    this->tint = tint;
}

void AmbientAnimator::Play() {
    if (frames.size() > 0) {
        isPlaying = true;
    }
}

void AmbientAnimator::Pause() {
    isPlaying = false;
}

void AmbientAnimator::Stop() {
    isPlaying = false;
    currentFrame = 0;
    currentFrameTime = 0.0f;
}

void AmbientAnimator::Reset() {
    currentFrame = 0;
    currentFrameTime = 0.0f;
}

Vector2 AmbientAnimator::GetPosition() const {
    return position;
}

bool AmbientAnimator::IsPlaying() const {
    return isPlaying;
}

bool AmbientAnimator::IsFinished() const {
    return !loop && currentFrame >= (int)frames.size() - 1;
}

int AmbientAnimator::GetCurrentFrame() const {
    return currentFrame;
}

int AmbientAnimator::GetFrameCount() const {
    return (int)frames.size();
}

void AmbientAnimator::Update(float deltaTime) {
    if (!isPlaying || frames.size() == 0) {
        return;
    }
    
    currentFrameTime += deltaTime;
    
    if (currentFrameTime >= frameDuration) {
        currentFrameTime -= frameDuration;
        NextFrame();
    }
}

void AmbientAnimator::NextFrame() {
    currentFrame++;
    
    if (currentFrame >= (int)frames.size()) {
        if (loop) {
            currentFrame = 0;
        } else {
            currentFrame = (int)frames.size() - 1;
            isPlaying = false;
        }
    }
}

void AmbientAnimator::Draw() const {
    Draw(position, scale, tint);
}

void AmbientAnimator::Draw(Vector2 pos) const {
    Draw(pos, scale, tint);
}

void AmbientAnimator::Draw(Vector2 pos, float scale, Color tint) const {
    if (frames.size() == 0) {
        return;
    }
    
    Rectangle sourceRect = frames[currentFrame];
    
    Rectangle destRect = {
        pos.x,
        pos.y,
        sourceRect.width * scale,
        sourceRect.height * scale
    };
    
    Vector2 origin = { 0.0f, 0.0f };
    
    DrawTexturePro(spriteSheet, sourceRect, destRect, origin, rotation, tint);
}

void AmbientAnimator::DrawTiled(int count, float spacing) const {
    for (int i = 0; i < count; i++) {
        Vector2 pos = { position.x + i * (spriteWidth * scale + spacing), position.y };
        Draw(pos);
    }
}

Rectangle AmbientAnimator::GetBounds() const {
    return {
        position.x,
        position.y,
        (float)spriteWidth * scale,
        (float)spriteHeight * scale
    };
}

Vector2 AmbientAnimator::GetDimensions() const {
    return { (float)spriteWidth, (float)spriteHeight };
}
