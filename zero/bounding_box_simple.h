/*

The logic here is to check all of the conditions where
a collision *can't* happen, then return the opposite.

Those conditions are:

Left edge of [test] is further right than the right edge of [this].
Right edge of [test] is further left than the left edge of [this].
Top edge of [test] is below the bottom edge of [this].
Bottom edge of [test] is above the top edge of [this].

This is equivalent to:

!(this->right() < test.left() ||
this->left() > test.right() ||
this->bottom() < test.top() ||
this->top() > test.bottom())

To make the operations simpler and more SIMD friendly,
that logic can be converted to this (bitwise & is intentional):

this->right() >= test.left() &
-this->left() >= -test.right() &
this->bottom() >= test.top() &
-this->top() >= -test.bottom()

*/

#include <utility>

#define EdgeCollisionsDoNotCount

enum MovementDirection {
    Left,
    Right,
    Up,
    Down,
#ifdef AllowDiagonals
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
#endif
};


struct Float2 {
    float x;
    float y;
};

// This alignas enables a few more
// SIMD instruction types to be used
struct alignas(16) BoundingBox {
    Float2 position;
    Float2 size;
    
#ifdef CenterXY
    // Only addition is used as an FMA optimization
    inline constexpr float left() const {
        return this->position.x + this->size.x * -0.5f;
    }
    inline constexpr float right() const {
        return this->position.x + this->size.x * 0.5f;
    }
    inline constexpr float top() const {
        return this->position.y + this->size.y * -0.5f;
    }
    inline constexpr float bottom() const {
        return this->position.y + this->size.y * 0.5f;
    }
#elifdef TopLeftXY
    inline constexpr float left() const {
        return this->position.x;
    }
    inline constexpr float right() const {
        return this->position.x + this->size.x;
    }
    inline constexpr float top() const {
        return this->position.y;
    }
    inline constexpr float bottom() const {
        return this->position.y + this->size.y;
    }
#else
    static_assert(false);
#endif

private:
    // Logic shared between boxes/points
    static inline constexpr bool test_intersect_impl(const float (&boxA)[4], const float (&boxB)[4]) {
        // Edge collisions are usually not wanted
        // since it counts two boxes "sliding past each other"
        // as a collision, but I've included the code for reference.
#ifdef EdgeCollisionsCount
        return boxA[0] >= boxB[0] &
               boxA[1] >= boxB[1] &
               boxA[2] >= boxB[2] &
               boxA[3] >= boxB[3];
#elifdef EdgeCollisionsDoNotCount
        return boxA[0] > boxB[0] &
               boxA[1] > boxB[1] &
               boxA[2] > boxB[2] &
               boxA[3] > boxB[3];
#else
        static_assert(false);
#endif
    }
public:

    // Collision with a 2D box
    inline constexpr bool test_intersect(const BoundingBox& test_box) const {
        float boxA[4] = {
            this->right(), this->bottom(),
            -this->left(), -this->top(),
        };
        float boxB[4] = {
            test_box.left(), test_box.top(),
            -test_box.right(), -test_box.bottom(),
        };

        return test_intersect_impl(boxA, boxB);
    }

    // Collision with a 2D point
    inline constexpr bool test_intersect(const Float2& test_point) const {
        float boxA[4] = {
            this->right(), this->bottom(),
            -this->left(), -this->top(),
        };
        float boxB[4] = {
            test_point.x, test_point.y,
            -test_point.x, -test_point.y,
        };

        return test_intersect_impl(boxA, boxB);
    }
    
    inline constexpr bool handle_wall_collision(const BoundingBox& wall_box, MovementDirection direction) {
        bool ret = false;
        switch (direction) {
#ifdef AllowDiagonals
            case UpLeft:
                if (float diff = wall_box.bottom() - this->top();
                    diff > 0.0f && diff < this->size.y + wall_box.size.y
                ) {
                    this->position.y += diff;
                    ret = true;
                }
                [[fallthrough]];
#endif
            case Left:
                if (float diff = wall_box.right() - this->left();
                    diff > 0.0f && diff < this->size.x + wall_box.size.x
                ) {
                    this->position.x += diff;
                    ret = true;
                }
                return ret;
#ifdef AllowDiagonals
            case DownRight:
                if (float diff = this->bottom() - wall_box.top();
                    diff > 0.0f && diff < this->size.y + wall_box.size.y
                ) {
                    this->position.y -= diff;
                    ret = true;
                }
                [[fallthrough]];
#endif
            case Right:
                if (float diff = this->right() - wall_box.left();
                    diff > 0.0f && diff < this->size.x + wall_box.size.x
                ) {
                    this->position.x -= diff;
                    ret = true;
                }
                return ret;
#ifdef AllowDiagonals
            case UpRight:
                if (float diff = this->right() - wall_box.left();
                    diff > 0.0f && diff < this->size.x + wall_box.size.x
                ) {
                    this->position.x -= diff;
                    ret = true;
                }
                [[fallthrough]];
#endif
            case Up:
                if (float diff = wall_box.bottom() - this->top();
                    diff > 0.0f && diff < this->size.y + wall_box.size.y
                ) {
                    this->position.y += diff;
                    ret = true;
                }
                return ret;
#ifdef AllowDiagonals
            case DownLeft:
                if (float diff = wall_box.right() - this->left();
                    diff > 0.0f && diff < this->size.x + wall_box.size.x
                ) {
                    this->position.x += diff;
                    ret = true;
                }
                [[fallthrough]];
#endif
            case Down:
                if (float diff = this->bottom() - wall_box.top();
                    diff > 0.0f && diff < this->size.y + wall_box.size.y
                ) {
                    this->position.y -= diff;
                    ret = true;
                }
                return ret;
            default:
                unreachable();
        }
    }
};