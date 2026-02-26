/**
 * @file SpriteAnimator.cpp
 * @brief Implementation of SpriteAnimator class
 */
#include <SpriteAnimator.hpp>
#include <algorithm>
#include <cctype>

namespace {
std::string NormalizeKeyName(std::string keyName) {
    std::transform(keyName.begin(), keyName.end(), keyName.begin(),
                   [](unsigned char c) { return (char)std::tolower(c); });
    if (keyName.rfind("key_", 0) == 0) {
        keyName = keyName.substr(4);
    }
    return keyName;
}

const std::unordered_map<std::string, KeyboardKey>& GetKeyboardKeyMap() {
    static const std::unordered_map<std::string, KeyboardKey> keyMap = {
        { "apostrophe", KEY_APOSTROPHE }, { "'", KEY_APOSTROPHE },
        { "comma", KEY_COMMA }, { ",", KEY_COMMA },
        { "minus", KEY_MINUS }, { "-", KEY_MINUS },
        { "period", KEY_PERIOD }, { ".", KEY_PERIOD },
        { "slash", KEY_SLASH }, { "/", KEY_SLASH },
        { "semicolon", KEY_SEMICOLON }, { ";", KEY_SEMICOLON },
        { "equal", KEY_EQUAL }, { "=", KEY_EQUAL },
        { "left_bracket", KEY_LEFT_BRACKET }, { "[", KEY_LEFT_BRACKET },
        { "backslash", KEY_BACKSLASH }, { "\\", KEY_BACKSLASH },
        { "right_bracket", KEY_RIGHT_BRACKET }, { "]", KEY_RIGHT_BRACKET },
        { "grave", KEY_GRAVE }, { "`", KEY_GRAVE },

        { "space", KEY_SPACE },
        { "escape", KEY_ESCAPE }, { "esc", KEY_ESCAPE },
        { "enter", KEY_ENTER }, { "return", KEY_ENTER },
        { "tab", KEY_TAB },
        { "backspace", KEY_BACKSPACE },
        { "insert", KEY_INSERT },
        { "delete", KEY_DELETE },
        { "right", KEY_RIGHT },
        { "left", KEY_LEFT },
        { "down", KEY_DOWN },
        { "up", KEY_UP },
        { "page_up", KEY_PAGE_UP }, { "pageup", KEY_PAGE_UP },
        { "page_down", KEY_PAGE_DOWN }, { "pagedown", KEY_PAGE_DOWN },
        { "home", KEY_HOME },
        { "end", KEY_END },
        { "caps_lock", KEY_CAPS_LOCK }, { "capslock", KEY_CAPS_LOCK },
        { "scroll_lock", KEY_SCROLL_LOCK }, { "scrolllock", KEY_SCROLL_LOCK },
        { "num_lock", KEY_NUM_LOCK }, { "numlock", KEY_NUM_LOCK },
        { "print_screen", KEY_PRINT_SCREEN }, { "printscreen", KEY_PRINT_SCREEN },
        { "pause", KEY_PAUSE },

        { "left_shift", KEY_LEFT_SHIFT }, { "lshift", KEY_LEFT_SHIFT }, { "shift", KEY_LEFT_SHIFT },
        { "left_control", KEY_LEFT_CONTROL }, { "left_ctrl", KEY_LEFT_CONTROL },
        { "lctrl", KEY_LEFT_CONTROL }, { "ctrl", KEY_LEFT_CONTROL }, { "control", KEY_LEFT_CONTROL },
        { "left_alt", KEY_LEFT_ALT }, { "lalt", KEY_LEFT_ALT }, { "alt", KEY_LEFT_ALT },
        { "left_super", KEY_LEFT_SUPER }, { "lsuper", KEY_LEFT_SUPER }, { "super", KEY_LEFT_SUPER },
        { "right_shift", KEY_RIGHT_SHIFT }, { "rshift", KEY_RIGHT_SHIFT },
        { "right_control", KEY_RIGHT_CONTROL }, { "right_ctrl", KEY_RIGHT_CONTROL }, { "rctrl", KEY_RIGHT_CONTROL },
        { "right_alt", KEY_RIGHT_ALT }, { "ralt", KEY_RIGHT_ALT },
        { "right_super", KEY_RIGHT_SUPER }, { "rsuper", KEY_RIGHT_SUPER },
        { "kb_menu", KEY_KB_MENU }, { "menu", KEY_KB_MENU },

        { "kp_decimal", KEY_KP_DECIMAL },
        { "kp_divide", KEY_KP_DIVIDE },
        { "kp_multiply", KEY_KP_MULTIPLY },
        { "kp_subtract", KEY_KP_SUBTRACT },
        { "kp_add", KEY_KP_ADD },
        { "kp_enter", KEY_KP_ENTER },
        { "kp_equal", KEY_KP_EQUAL },
        { "kp_0", KEY_KP_0 }, { "kp_1", KEY_KP_1 }, { "kp_2", KEY_KP_2 }, { "kp_3", KEY_KP_3 }, { "kp_4", KEY_KP_4 },
        { "kp_5", KEY_KP_5 }, { "kp_6", KEY_KP_6 }, { "kp_7", KEY_KP_7 }, { "kp_8", KEY_KP_8 }, { "kp_9", KEY_KP_9 },

        { "f1", KEY_F1 }, { "f2", KEY_F2 }, { "f3", KEY_F3 }, { "f4", KEY_F4 }, { "f5", KEY_F5 }, { "f6", KEY_F6 },
        { "f7", KEY_F7 }, { "f8", KEY_F8 }, { "f9", KEY_F9 }, { "f10", KEY_F10 }, { "f11", KEY_F11 }, { "f12", KEY_F12 }
    };
    return keyMap;
}
} // namespace

SpriteAnimator::SpriteAnimator(const char* spriteSheetPath, int spriteWidth, int spriteHeight) 
    : spriteWidth(spriteWidth), spriteHeight(spriteHeight),
      scale(1.0f), rotation(0.0f), currentFrame(0), currentFrameTime(0.0f),
      isAnimating(false), facingRight(true), useIdleAnimation(false), actionInProgress(false)
{
    spriteSheet = LoadTexture(spriteSheetPath);
    Init(spriteWidth, spriteHeight);
}

SpriteAnimator::SpriteAnimator(Texture2D texture, int spriteWidth, int spriteHeight)
    : spriteSheet(texture), spriteWidth(spriteWidth), spriteHeight(spriteHeight),
      scale(1.0f), rotation(0.0f), currentFrame(0), currentFrameTime(0.0f),
      isAnimating(false), facingRight(true), useIdleAnimation(false), actionInProgress(false)
{
    Init(spriteWidth, spriteHeight);
}

void SpriteAnimator::Init(int spriteWidth, int spriteHeight) {
    sheetWidth = spriteSheet.width;
    sheetHeight = spriteSheet.height;
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;
    
    position = { 0.0f, 0.0f };
    tint = WHITE;
    currentAnimation = "";
    idleAnimation = "";
    actionAnimation = "";
    animationBeforeAction = "";
    
    CalculateGrid();
}

void SpriteAnimator::CalculateGrid() {
    columns = sheetWidth / spriteWidth;
    rows = sheetHeight / spriteHeight;
}

SpriteAnimator::~SpriteAnimator() {
    // Only unload if texture was loaded by this class
    // (check if it's a valid managed texture)
    if (spriteSheet.id > 0) {
        UnloadTexture(spriteSheet);
    }
}

void SpriteAnimator::AddAnimation(const std::string& name, int row, int frameCount, 
                                   float frameDuration, bool loop) {
    Animation animation;
    animation.frameDuration = frameDuration;
    animation.loop = loop;
    
    // Create frames for this animation
    for (int i = 0; i < frameCount && i < columns; i++) {
        AnimationFrame frame;
        frame.x = i * spriteWidth;
        frame.y = row * spriteHeight;
        frame.width = spriteWidth;
        frame.height = spriteHeight;
        animation.frames.push_back(frame);
    }
    
    animations[name] = animation;

    KeyboardKey key;
    if (TryResolveAnimationKey(name, key)) {
        bool updated = false;
        for (auto& binding : animationKeyBindings) {
            if (binding.first == key) {
                binding.second = name;
                updated = true;
                break;
            }
        }
        if (!updated) {
            animationKeyBindings.push_back({ key, name });
        }
    }
    
    // If no current animation set, use this one
    if (currentAnimation.empty()) {
        SetAnimation(name);
    }
}

void SpriteAnimator::SetAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        if (currentAnimation != name) {
            currentAnimation = name;
            currentFrame = 0;
            currentFrameTime = 0.0f;
            isAnimating = true;
        }
    }
}

void SpriteAnimator::SetIdleAnimation(const std::string& name) {
    idleAnimation = name;
    useIdleAnimation = true;
}

void SpriteAnimator::ClearIdleAnimation() {
    idleAnimation.clear();
    useIdleAnimation = false;
}

void SpriteAnimator::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void SpriteAnimator::SetPosition(Vector2 pos) {
    position = pos;
}

void SpriteAnimator::SetScale(float scale) {
    this->scale = scale;
}

void SpriteAnimator::SetRotation(float degrees) {
    this->rotation = degrees;
}

void SpriteAnimator::SetTint(Color tint) {
    this->tint = tint;
}

void SpriteAnimator::SetFrameDuration(float duration) {
    auto it = animations.find(currentAnimation);
    if (it != animations.end()) {
        it->second.frameDuration = duration;
    }
}

Vector2 SpriteAnimator::GetPosition() const {
    return position;
}

std::string SpriteAnimator::GetCurrentAnimation() const {
    return currentAnimation;
}

bool SpriteAnimator::IsAnimating() const {
    return isAnimating;
}

bool SpriteAnimator::IsFacingRight() const {
    return facingRight;
}

void SpriteAnimator::Update(float deltaTime) {
    if (actionInProgress) {
        return;
    }
    UpdateWithKeys(deltaTime, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
}

void SpriteAnimator::UpdateWithKeys(float deltaTime, KeyboardKey upKey, KeyboardKey downKey, 
                                     KeyboardKey leftKey, KeyboardKey rightKey) {
    if (actionInProgress) {
        return;
    }

    bool moved = false;
    std::string newAnimation = currentAnimation;
    
    // Check keyboard input
    if (IsKeyDown(upKey)) {
        newAnimation = "up";
        moved = true;
    } else if (IsKeyDown(downKey)) {
        newAnimation = "down";
        moved = true;
    } else if (IsKeyDown(leftKey)) {
        newAnimation = "left";
        facingRight = false;
        moved = true;
    } else if (IsKeyDown(rightKey)) {
        newAnimation = "right";
        facingRight = true;
        moved = true;
    }

    // Support any bound keyboard key from AddAnimation("key_name", ...)
    if (!moved) {
        for (const auto& binding : animationKeyBindings) {
            if (IsKeyDown(binding.first)) {
                newAnimation = binding.second;
                moved = true;
                break;
            }
        }
    }
    
    // Only change animation if movement detected
    if (moved) {
        if (newAnimation == "left") {
            facingRight = false;
        } else if (newAnimation == "right") {
            facingRight = true;
        }

        // If animation name doesn't exist, try to find a matching one
        if (animations.find(newAnimation) == animations.end()) {
            // Try common variations
            if (newAnimation == "left" && animations.find("walk_left") != animations.end()) {
                newAnimation = "walk_left";
            } else if (newAnimation == "right" && animations.find("walk_right") != animations.end()) {
                newAnimation = "walk_right";
            } else if (newAnimation == "up" && animations.find("walk_up") != animations.end()) {
                newAnimation = "walk_up";
            } else if (newAnimation == "down" && animations.find("walk_down") != animations.end()) {
                newAnimation = "walk_down";
            } else if (animations.find("walk") != animations.end()) {
                newAnimation = "walk";
            }
        }
        
        SetAnimation(newAnimation);
        isAnimating = true;
    } else {
        // No movement - optionally play idle animation
        if (useIdleAnimation && animations.find(idleAnimation) != animations.end()) {
            SetAnimation(idleAnimation);
            isAnimating = true;
        } else {
            isAnimating = false;
            currentFrame = 0;
            currentFrameTime = 0.0f;
        }
    }
    
    // Update frame if animating
    AdvanceAnimation(deltaTime);
}

void SpriteAnimator::UpdateManual(float deltaTime, const std::string& animationName, bool isMoving) {
    if (actionInProgress) {
        return;
    }

    if (isMoving) {
        SetAnimation(animationName);
        isAnimating = true;
    } else {
        if (useIdleAnimation && animations.find(idleAnimation) != animations.end()) {
            SetAnimation(idleAnimation);
            isAnimating = true;
        } else {
            isAnimating = false;
            currentFrame = 0;
            currentFrameTime = 0.0f;
        }
    }

    AdvanceAnimation(deltaTime);
}

bool SpriteAnimator::UpdateAction(float deltaTime, KeyboardKey actionKey, const std::string& actionName) {
    bool actionCompleted = false;

    if (!actionInProgress && IsKeyPressed(actionKey)) {
        auto actionIt = animations.find(actionName);
        if (actionIt != animations.end() && !actionIt->second.frames.empty()) {
            actionInProgress = true;
            actionAnimation = actionName;
            animationBeforeAction = currentAnimation;
            SetAnimation(actionName);
            currentFrame = 0;
            currentFrameTime = 0.0f;
            isAnimating = true;
        }
    }

    if (!actionInProgress) {
        return false;
    }

    auto it = animations.find(actionAnimation);
    if (it == animations.end() || it->second.frames.empty()) {
        actionInProgress = false;
        actionAnimation.clear();
        return false;
    }

    currentFrameTime += deltaTime;

    while (currentFrameTime >= it->second.frameDuration) {
        currentFrameTime -= it->second.frameDuration;

        if (currentFrame < (int)it->second.frames.size() - 1) {
            currentFrame++;
        } else {
            actionCompleted = true;
            actionInProgress = false;
            actionAnimation.clear();
            currentFrameTime = 0.0f;

            if (useIdleAnimation && animations.find(idleAnimation) != animations.end()) {
                SetAnimation(idleAnimation);
            } else if (!animationBeforeAction.empty() && animations.find(animationBeforeAction) != animations.end()) {
                SetAnimation(animationBeforeAction);
            } else {
                isAnimating = false;
            }
            break;
        }
    }

    return actionCompleted;
}

void SpriteAnimator::NextFrame() {
    auto it = animations.find(currentAnimation);
    if (it != animations.end()) {
        auto& anim = it->second;
        currentFrame++;
        
        if (currentFrame >= (int)anim.frames.size()) {
            if (anim.loop) {
                currentFrame = 0;
            } else {
                currentFrame = (int)anim.frames.size() - 1;
            }
        }
    }
}

Rectangle SpriteAnimator::GetCurrentFrameRect() const {
    auto it = animations.find(currentAnimation);
    if (it != animations.end() && currentFrame < (int)it->second.frames.size()) {
        const auto& frame = it->second.frames[currentFrame];
        return { (float)frame.x, (float)frame.y, (float)frame.width, (float)frame.height };
    }
    
    // Default to first frame
    return { 0.0f, 0.0f, (float)spriteWidth, (float)spriteHeight };
}

void SpriteAnimator::AdvanceAnimation(float deltaTime) {
    if (!isAnimating) {
        return;
    }

    auto it = animations.find(currentAnimation);
    if (it == animations.end()) {
        return;
    }

    currentFrameTime += deltaTime;
    if (currentFrameTime >= it->second.frameDuration) {
        currentFrameTime -= it->second.frameDuration;
        NextFrame();
    }
}

bool SpriteAnimator::TryResolveAnimationKey(const std::string& animationName, KeyboardKey& outKey) const {
    std::string keyName = NormalizeKeyName(animationName);
    if (keyName.empty()) {
        return false;
    }

    if (keyName.size() == 1) {
        char c = keyName[0];
        if (c >= 'a' && c <= 'z') {
            outKey = (KeyboardKey)(KEY_A + (c - 'a'));
            return true;
        }
        if (c >= '0' && c <= '9') {
            outKey = (KeyboardKey)(KEY_ZERO + (c - '0'));
            return true;
        }
    }

    const auto& keyMap = GetKeyboardKeyMap();
    auto it = keyMap.find(keyName);
    if (it != keyMap.end()) {
        outKey = it->second;
        return true;
    }

    return false;
}

void SpriteAnimator::Draw() const {
    Draw(position, scale, tint);
}

void SpriteAnimator::Draw(float x, float y, float width, float height) const {
    Rectangle sourceRect = GetCurrentFrameRect();
    
    // Calculate scale factors based on desired dimensions
    float scaleX = width / sourceRect.width;
    float scaleY = height / sourceRect.height;
    
    // Handle flipping for left direction
    Rectangle destRect = {
        x + position.x,
        y + position.y,
        sourceRect.width * scaleX * (facingRight ? 1.0f : -1.0f),
        sourceRect.height * scaleY
    };
    
    Vector2 origin = { 0.0f, 0.0f };  // Draw from top-left
    
    DrawTexturePro(spriteSheet, sourceRect, destRect, origin, rotation, tint);
}

void SpriteAnimator::Draw(Vector2 pos, float scale, Color tint) const {
    Rectangle sourceRect = GetCurrentFrameRect();
    
    // Handle flipping for left direction
    Rectangle destRect = {
        pos.x,
        pos.y,
        sourceRect.width * scale * (facingRight ? 1.0f : -1.0f),
        sourceRect.height * scale
    };
    
    Vector2 origin = { 0.0f, 0.0f };  // Draw from top-left
    
    DrawTexturePro(spriteSheet, sourceRect, destRect, origin, rotation, tint);
}

Rectangle SpriteAnimator::GetBounds() const {
    return {
        position.x,
        position.y,
        (float)spriteWidth * scale,
        (float)spriteHeight * scale
    };
}

Vector2 SpriteAnimator::GetDimensions() const {
    return { (float)spriteWidth, (float)spriteHeight };
}
