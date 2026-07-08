#include <catch2/catch_test_macros.hpp>
#include "test_helpers.h"
#include "themes.h"
#include "log_window.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

// Smoke tests proving the shared fixtures work. The real coverage suites
// (test_themes_apply, test_log_window_render, ...) build on these fixtures.

TEST_CASE("Fixture: ImGuiContextFixture supports theme application", "[fixture]")
{
    ImGuiContextFixture fixture;

    const ImVec4 before = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    applyCherryTheme();
    const ImVec4 after = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    const bool changed = before.x != after.x || before.y != after.y ||
                         before.z != after.z || before.w != after.w;
    REQUIRE(changed);
}

TEST_CASE("Fixture: ImGuiFrameFixture supports widget rendering", "[fixture]")
{
    ImGuiFrameFixture fixture;

    LogWindow::clearLogs();
    LogWindow::addLog(colours::White, "[INFO] fixture smoke test");

    ImGuiFrameFixture::runFrame([] {
        ImGui::Begin("fixture smoke");
        LogWindow::renderContent();
        ImGui::End();
    });

    REQUIRE(LogWindow::getLogCount() == 1);
    LogWindow::clearLogs();
}
