#include <physics/RigidBodies.hpp>
#include <physics/Sphere.hpp>

namespace aun{


    
Ball::Ball(){
    geometry = std::make_unique<Sphere>();
}
void Ball::moveUp(){
    applyForce(glm::vec3(1,0,0));
}

}