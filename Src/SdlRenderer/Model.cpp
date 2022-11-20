#include <Scout/SdlRenderer/Model.h>

#include <SDL.h>
#ifdef main
#undef main
#endif

#include <Scout/SdlRenderer/GraphicEngine.h>

namespace Scout
{
    Model_SDL::Model_SDL(const std::vector<glm::vec3>& vertices, const glm::vec4 modelColor) :
        modelColor_(modelColor)
    {
        if (vertices_.size() % 3 != 0) throw std::runtime_error("Model_SDL::Model_SDL: vertices does not contain triangle data.");

        vertices_.resize(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertices_[i] = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
        }
    }

    void Model_SDL::Draw(GraphicsEngine_SDL& graphicalEngine, const glm::mat4& viewMat, const glm::mat4& projMat, const glm::mat4& modelMat) const
    {
        auto* pSdlRenderer = graphicalEngine.GetSdlRenderer();
        const float viewportWidth = (float)graphicalEngine.GetViewportWidth();
        const float viewportHeight = (float)graphicalEngine.GetViewportHeight();
        const auto transformation = projMat * glm::inverse(viewMat) * modelMat;
        SDL_SetRenderDrawColor(pSdlRenderer, (Uint8)(modelColor_.r * 255.f), (Uint8)(modelColor_.g * 255.0f), (Uint8)(modelColor_.b * 255.0f), (Uint8)(modelColor_.a * 255.0f));

        const auto len = vertices_.size();
        for (size_t i = 0; i < len; i += 3)
        {
            const auto pt0_clip = transformation * glm::vec4(vertices_[i].x, vertices_[i].y, vertices_[i].z, 1.0f);
            const auto pt1_clip = transformation * glm::vec4(vertices_[i + 1].x, vertices_[i + 1].y, vertices_[i + 1].z, 1.0f);
            const auto pt2_clip = transformation * glm::vec4(vertices_[i + 2].x, vertices_[i + 2].y, vertices_[i + 2].z, 1.0f);

            const glm::vec2 pt0_screenspace = graphicalEngine.ClipSpaceToScreenSpace({pt0_clip.x / pt0_clip.w, pt0_clip.y / pt0_clip.w});
            const glm::vec2 pt1_screenspace = graphicalEngine.ClipSpaceToScreenSpace({pt1_clip.x / pt1_clip.w, pt1_clip.y / pt1_clip.w});
            const glm::vec2 pt2_screenspace = graphicalEngine.ClipSpaceToScreenSpace({pt2_clip.x / pt2_clip.w, pt2_clip.y / pt2_clip.w});

            SDL_RenderDrawLine(
                pSdlRenderer,
                (int)(pt0_screenspace.x),
                (int)(pt0_screenspace.y),
                (int)(pt1_screenspace.x),
                (int)(pt1_screenspace.y));
            SDL_RenderDrawLine(
                pSdlRenderer,
                (int)(pt1_screenspace.x),
                (int)(pt1_screenspace.y),
                (int)(pt2_screenspace.x),
                (int)(pt2_screenspace.y));
            SDL_RenderDrawLine(
                pSdlRenderer,
                (int)(pt2_screenspace.x),
                (int)(pt2_screenspace.y),
                (int)(pt0_screenspace.x),
                (int)(pt0_screenspace.y));
        }
    }
}