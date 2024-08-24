
#include <physics/Geometry.hpp>
namespace aun{
struct CollisionInfo {
    bool hasCollision;
    glm::vec3 contactPoint;
    glm::vec3 normal;
    float penetrationDepth;
};

class CollisionSystem {
public:
    static CollisionInfo checkCollision(const Geometry& a, const Geometry& b);
    // Implement specific collision checks for different Geometry combinations
};
}