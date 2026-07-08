#include <catch2/catch_test_macros.hpp>

#include "test_helpers.h"
#include "themes.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

// NOTE: showImGuiKitThemeSelector()'s theme-selection branch
// (BeginCombo -> Selectable -> themes[theme]()) only fires when the combo is
// open and an item is clicked, which requires simulated mouse/keyboard input
// unavailable in a headless ImGui::NewFrame()-only context. These tests
// therefore cover every line reachable without that interaction: the window
// wrapper, the combo header, and the closed-combo path.

static bool colorsEqual(const ImVec4& a, const ImVec4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

TEST_CASE("ThemeSelector: renders with nullptr open flag", "[themeselector][render]")
{
    ImGuiFrameFixture fixture;

    ImGuiFrameFixture::runFrame([] {
        showImGuiKitThemeSelector(nullptr);
    });

    SUCCEED("renders without crashing");
}

TEST_CASE("ThemeSelector: renders with an open flag and leaves it untouched", "[themeselector][render]")
{
    ImGuiFrameFixture fixture;

    bool open = true;
    ImGuiFrameFixture::runFrame([&] {
        showImGuiKitThemeSelector(&open);
    });

    CHECK(open == true);
}

TEST_CASE("ThemeSelector: stable across multiple consecutive frames", "[themeselector][render]")
{
    ImGuiFrameFixture fixture;

    bool open = true;
    for (int i = 0; i < 3; ++i)
    {
        ImGuiFrameFixture::runFrame([&] {
            showImGuiKitThemeSelector(&open);
        });
    }

    CHECK(open == true);
}

TEST_CASE("ThemeSelector: rendering does not change the currently applied style", "[themeselector][render]")
{
    ImGuiFrameFixture fixture;

    applyCherryTheme();
    const ImVec4 before = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    ImGuiFrameFixture::runFrame([] {
        showImGuiKitThemeSelector(nullptr);
    });
    ImGuiFrameFixture::runFrame([] {
        showImGuiKitThemeSelector(nullptr);
    });

    const ImVec4 after = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    CHECK(colorsEqual(before, after));
}

TEST_CASE("ThemeSelector: coexists with other windows in the same frame", "[themeselector][render]")
{
    ImGuiFrameFixture fixture;

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("Other Window");
        ImGui::Text("Unrelated content");
        ImGui::End();

        showImGuiKitThemeSelector(nullptr);
    });

    SUCCEED("both windows render in the same frame without crashing");
}
