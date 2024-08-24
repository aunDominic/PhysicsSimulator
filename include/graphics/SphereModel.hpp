#pragma once
#include "GraphicsModel.hpp"
namespace aun{
class SphereModel : public GraphicsModel{
    private:
        float radius;
        int sectors;
        int stacks;
        void createSphere();
    public:
        SphereModel(float radius, int sectors, int stacks);
        SphereModel();
};
}