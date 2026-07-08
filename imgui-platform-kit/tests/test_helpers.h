#pragma once

// Shared test fixtures for tests that need a live ImGui context.
//
// ImGuiContextFixture — bare context, enough for anything that only touches
// ImGui::GetStyle() (theme appliers, StyleParameters::apply()).
//
// ImGuiFrameFixture — context plus everything required to run NewFrame()
// without any GPU backend (display size, font atlas). Use beginFrame()/
// endFrame() around widget-drawing code (LogWindow::renderContent(),
// showImGuiKitThemeSelector(), TemplateWindow::render(), ...).

#include "imgui.h"

namespace imgui_kit::testing
{
    // Exact (non-approximate) ImVec4 comparison — the values under test are
    // always literal source constants or copies of them, so bit-for-bit
    // equality is expected and any divergence is a real regression.
    inline bool colorsEqual(const ImVec4& a, const ImVec4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    class ImGuiContextFixture
    {
    public:
        ImGuiContextFixture()
        {
            context = ImGui::CreateContext();
            ImGui::SetCurrentContext(context);
            // Never write imgui.ini from tests.
            ImGui::GetIO().IniFilename = nullptr;
        }

        ~ImGuiContextFixture()
        {
            ImGui::DestroyContext(context);
        }

        ImGuiContextFixture(const ImGuiContextFixture&) = delete;
        ImGuiContextFixture& operator=(const ImGuiContextFixture&) = delete;
        ImGuiContextFixture(ImGuiContextFixture&&) = delete;
        ImGuiContextFixture& operator=(ImGuiContextFixture&&) = delete;

    private:
        ImGuiContext* context;
    };

    class ImGuiFrameFixture : public ImGuiContextFixture
    {
    public:
        ImGuiFrameFixture()
        {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(1280.0f, 720.0f);
            io.Fonts->AddFontDefault();
            // Build the font atlas on the CPU so NewFrame() works without a
            // rendering backend.
            unsigned char* pixels = nullptr;
            int width = 0;
            int height = 0;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        }

        // Begins an ImGui frame. Pair every call with endFrame().
        static void beginFrame()
        {
            ImGui::NewFrame();
        }

        // Ends the frame and generates (discarded) draw data.
        static void endFrame()
        {
            ImGui::Render();
        }

        // Convenience: runs `body` inside a single frame.
        template <typename Body>
        static void runFrame(Body&& body)
        {
            beginFrame();
            body();
            endFrame();
        }
    };
}
