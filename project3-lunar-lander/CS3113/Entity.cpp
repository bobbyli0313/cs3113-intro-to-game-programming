#include "Entity.h"

Entity::Entity() : mPosition {0.0f, 0.0f}, mMovement {0.0f, 0.0f},
                   mVelocity {0.0f, 0.0f}, mAcceleration {0.0f, 0.0f},
                   mScale {DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions {DEFAULT_SIZE, DEFAULT_SIZE},
                   mTexture {NULL}, mTextureType {SINGLE}, mAngle {0.0f},
                   mSpriteSheetDimensions {}, mDirection {LEFT},
                   mAnimationAtlas {{}}, mAnimationIndices {}, mFrameSpeed {0},
                   mEntityType {NONE}, mEnergy {0}, mChickenStatus {INGAME} { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
        EntityType entityType) : mPosition {position}, mVelocity {0.0f, 0.0f},
        mAcceleration {0.0f, 0.0f}, mScale {scale}, mMovement {0.0f, 0.0f},
        mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)},
        mTextureType {SINGLE}, mDirection {LEFT}, mAnimationAtlas {{}},
        mAnimationIndices {}, mFrameSpeed {0}, mSpeed {DEFAULT_SPEED},
        mAngle {0.0f}, mEntityType {entityType}, mEnergy {DEFAULT_ENERGY}, 
        mChickenStatus {INGAME} { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<Direction,
        std::vector<int>> animationAtlas, EntityType entityType) :
        mPosition {position}, mVelocity {0.0f, 0.0f},
        mAcceleration {0.0f, 0.0f}, mMovement { 0.0f, 0.0f }, mScale {scale},
        mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)},
        mTextureType {ATLAS}, mSpriteSheetDimensions {spriteSheetDimensions},
        mAnimationAtlas {animationAtlas}, mDirection {LEFT},
        mAnimationIndices {animationAtlas.at(LEFT)},
        mFrameSpeed {DEFAULT_FRAME_SPEED}, mAngle { 0.0f },
        mSpeed {DEFAULT_SPEED}, mEntityType {entityType}, 
        mEnergy {DEFAULT_ENERGY}, mChickenStatus {INGAME} { }

Entity::~Entity() { UnloadTexture(mTexture); };

void Entity::checkCollisionY(Entity *collidableEntities, int 
    collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) -
                              (collidableEntity->mColliderDimensions.y / 2.0f));

            if (mVelocity.y > 0)
            {
                mPosition.y -= yOverlap;
                mVelocity.y  = 0;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0)
            {
                mPosition.y += yOverlap;
                mVelocity.y  = 0;
                mIsCollidingTop = true;
            }

            if (collidableEntity->mEntityType == WATER || 
                collidableEntity->mEntityType == PLANE) {
                // Losing condition
                mEntityStatus = INACTIVE;
                mChickenStatus = LOSE;
            } else if (collidableEntity->mEntityType == GRASS) {
                // Winning condition
                mEntityStatus = INACTIVE;
                mChickenStatus = WIN;
            }
        }
    }
}

void Entity::checkCollisionX(Entity *collidableEntities, int 
collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];

        if (isColliding(collidableEntity))
        {
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - 
            (collidableEntity->mColliderDimensions.y / 2.0f));

            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance - (mColliderDimensions.x / 2.0f) - 
            (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x -= xOverlap;
                mVelocity.x  = 0;
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x += xOverlap;
                mVelocity.x  = 0;
                mIsCollidingLeft = true;
            }
        }
    }
}

bool Entity::isColliding(Entity *other) const
{
    if (!other->isActive() || other == this) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) -
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) -
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    // Make frame speed dependent on velocity
    float velocity = fabs(mVelocity.x) + fabs(mVelocity.y);
    mFrameSpeed = velocity / 10.0f;

    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond)
    {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

void Entity::update(float deltaTime, Entity *water, int waterCount, Entity 
*grass, int grassCount, Entity *plane, int planeCount)
{
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == PLANE) {
        mPosition.x += mMovement.x * 2.0f;
        if (mPosition.x > 1300.0f) { // Right limit
            // Rotate by 180 and change direction
            mAngle = 180.0f;
            moveLeft();
        } else if (mPosition.x < 100.0f) { // Left limit
            mAngle = 0.0f;
            moveRight();
        }
    }

    resetColliderFlags();

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(water, waterCount);
    checkCollisionY(grass, grassCount);
    checkCollisionY(plane, planeCount);

    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(water, waterCount);
    checkCollisionX(grass, grassCount);
    checkCollisionX(plane, planeCount);

    // Prevents player from going too high
    if (mPosition.y < -100.0f) {
        mEntityStatus = INACTIVE;
        mChickenStatus = LOSE;
    }

    if (mTextureType == ATLAS)
        animate(deltaTime);
}

void Entity::render()
{
    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            textureArea = {
                0.0f, 0.0f,
                static_cast<float>(mTexture.width),
                static_cast<float>(mTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture,
                mAnimationIndices[mCurrentFrameIndex],
                mSpriteSheetDimensions.x,
                mSpriteSheetDimensions.y
            );

        default: break;
    }

    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    DrawTexturePro(
        mTexture,
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );

}

void Entity::displayCollider()
{
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,
        mPosition.y - mColliderDimensions.y / 2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };

    DrawRectangleLines(
        colliderBox.x,
        colliderBox.y,
        colliderBox.width,
        colliderBox.height,
        GREEN
    );
}
