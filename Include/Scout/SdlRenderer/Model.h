#pragma once

#include <vector>

#include <Scout/Math.h>

namespace Scout
{
    class GraphicsEngine_SDL;

    class Model_SDL
    {
    public:
        /*
        @vertices Unindexed vertex positions of triangles composing this mesh.
        */
        Model_SDL(const std::vector<glm::vec3>& vertices, const glm::vec4 modelColor);

        void Draw(GraphicsEngine_SDL& graphicalEngine, const glm::mat4& viewMat, const glm::mat4& projMat, const glm::mat4& modelMat) const;

    private:
        std::vector<glm::vec4> vertices_{};
        glm::vec4 modelColor_ = COLOR_CLEAR;
    };
}