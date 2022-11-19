#include <Scout/SdlRenderer/Model.h>

#include <SDL.h>
#ifdef main
#undef main
#endif

#include <Scout/SdlRenderer/GraphicEngine.h>

namespace Scout
{
    Model_SDL::Model_SDL(const std::vector<Vec3>& vertices, const Color modelColor) :
        modelColor_(modelColor)
    {
        if (vertices_.size() % 3 != 0) throw std::runtime_error("Model_SDL::Model_SDL: vertices does not contain triangle data.");

        vertices_.resize(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertices_[i] = { vertices[i].x, vertices[i].y, vertices[i].z, 1.0f };
        }
    }

    void Model_SDL::Draw(GraphicsEngine_SDL& graphicalEngine, const Mat4x4& viewMat, const Mat4x4& projMat, const Mat4x4& modelMat) const
    {
        auto* pSdlRenderer = graphicalEngine.GetSdlRenderer();
        const float viewportWidth = (float)graphicalEngine.GetViewportWidth();
        const float viewportHeight = (float)graphicalEngine.GetViewportHeight();
        const auto transformation = projMat * viewMat * modelMat;
        SDL_SetRenderDrawColor(pSdlRenderer, (Uint8)(modelColor_.r * 255.f), (Uint8)(modelColor_.g * 255.0f), (Uint8)(modelColor_.b * 255.0f), (Uint8)(modelColor_.a * 255.0f));

        const auto len = vertices_.size();
        for (size_t i = 0; i < len; i += 3)
        {
            // TODO: w component is supposed to be 1.0, not 0.0 for translations to work, debug it
            const auto pt0_udc = transformation * Vec4(vertices_[i].x, vertices_[i].y, vertices_[i].z, 1.0f);
            const auto pt1_udc = transformation * Vec4(vertices_[i + 1].x, vertices_[i + 1].y, vertices_[i + 1].z, 1.0f);
            const auto pt2_udc = transformation * Vec4(vertices_[i + 2].x, vertices_[i + 2].y, vertices_[i + 2].z, 1.0f);

            // To screen space following "Viewport transform" section of: https://www.khronos.org/opengl/wiki/Viewport_Transform note: adjusted to fit SDL's coordinate convention.
            const Vec2 pt0_screenspace =
            {
                viewportWidth * 0.5f * pt0_udc.x + viewportWidth * 0.5f,
                viewportHeight - (viewportHeight * 0.5f * pt0_udc.y + viewportHeight * 0.5f)
            };
            const Vec2 pt1_screenspace =
            {
                viewportWidth * 0.5f * pt1_udc.x + viewportWidth * 0.5f,
                viewportHeight - (viewportHeight * 0.5f * pt1_udc.y + viewportHeight * 0.5f)
            };
            const Vec2 pt2_screenspace =
            {
                viewportWidth * 0.5f * pt2_udc.x + viewportWidth * 0.5f,
                viewportHeight - (viewportHeight * 0.5f * pt2_udc.y + viewportHeight * 0.5f)
            };

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