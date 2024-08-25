#include <graphics/SphereModel.hpp>
#include <spdlog/spdlog.h>
namespace aun{
void SphereModel::createSphere() {
    vertices.clear();
    indices.clear();
    spdlog::debug("Creating vertices...");
    const float PI = 3.14159265358979f;
    const float sector_step = 2 * PI / sectors;
    const float stack_step = PI / stacks;

    for (int i = 0; i <= stacks; ++i) {
        float phi = PI / 2 - i * stack_step;
        float y = radius * sinf(phi);
        float xz = radius * cosf(phi);

        for (int j = 0; j <= sectors; ++j) {
            float theta = j * sector_step;
            float x = xz * cosf(theta);
            float z = xz * sinf(theta);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Calculate normal (normalized position)
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int first = (i * (sectors + 1)) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    spdlog::debug("Vertices created successfully.");

}
SphereModel::SphereModel(float radius, int sectors, int stacks) : radius(radius), sectors(sectors), stacks(stacks){
   createSphere();
   initBuffers(); 
}
SphereModel::SphereModel(){
    spdlog::debug("Constructing SphereModel...");
    radius = 3; sectors = 20; stacks = 15;
    createSphere();
    initBuffers(); 
}
}