#include <catch2/catch_test_macros.hpp>
#include <memory>

#include "test_helpers.h"
#include "template_window.h"
#include "user_interface_window.h"
#include "themes.h"
#include "log_window.h"
#include "implot.h"

using namespace imgui_kit;
using namespace imgui_kit::testing;

namespace
{
    // TemplateWindow::render() is private (only reachable through the
    // UserInterfaceWindow base, exactly as UserInterface itself calls it).
    std::unique_ptr<UserInterfaceWindow> makeTemplateWindow()
    {
        return std::make_unique<TemplateWindow>();
    }

    bool colorsEqual(const ImVec4& a, const ImVec4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    // template_window.cpp unconditionally calls ImPlot::ShowDemoWindow(), so
    // every test needs a live ImPlot context alongside the ImGui one. Created
    // after the ImGui context, destroyed before it dies.
    class ImPlotContextGuard
    {
    public:
        ImPlotContextGuard() { context = ImPlot::CreateContext(); }
        ~ImPlotContextGuard() { ImPlot::DestroyContext(context); }
        ImPlotContextGuard(const ImPlotContextGuard&) = delete;
        ImPlotContextGuard& operator=(const ImPlotContextGuard&) = delete;
    private:
        ImPlotContext* context;
    };
}

// NOTE: the "Hello, world!" button's LogWindow::addLog call and every
// checkbox's flag-toggle branch require simulated mouse input, unavailable
// headless. These tests cover every unconditional draw line (both windows'
// bodies, all 16 checkbox reads, the demo/style-editor/ImPlot-demo/theme
// selector calls) without ever clicking anything.

TEST_CASE("TemplateWindow: render executes headless without crashing", "[templatewindow][render]")
{
    ImGuiFrameFixture fixture;
    ImPlotContextGuard implot;
    LogWindow::clearLogs();

    auto window = makeTemplateWindow();
    ImGuiFrameFixture::runFrame([&] {
        window->render();
    });

    // The button was never clicked, so rendering must not have logged anything.
    CHECK(LogWindow::getLogCount() == 0);
    LogWindow::clearLogs();
}

TEST_CASE("TemplateWindow: render leaves global window flags untouched", "[templatewindow][render]")
{
    ImGuiFrameFixture fixture;
    ImPlotContextGuard implot;

    setGlobalWindowFlags(ImGuiWindowFlags_None);
    auto window = makeTemplateWindow();
    ImGuiFrameFixture::runFrame([&] {
        window->render();
    });
    CHECK(getGlobalWindowFlags() == ImGuiWindowFlags_None);

    setGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGuiFrameFixture::runFrame([&] {
        window->render();
    });
    CHECK(getGlobalWindowFlags() == (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove));

    setGlobalWindowFlags(ImGuiWindowFlags_None);
}

TEST_CASE("TemplateWindow: render is stable across multiple frames", "[templatewindow][render]")
{
    ImGuiFrameFixture fixture;
    ImPlotContextGuard implot;

    auto window = makeTemplateWindow();
    for (int i = 0; i < 3; ++i)
    {
        ImGuiFrameFixture::runFrame([&] {
            window->render();
        });
    }
    SUCCEED("static locals (showFlagsWindow, show) survive repeated render() calls");
}

TEST_CASE("TemplateWindow: two instances render safely in the same frame", "[templatewindow][render]")
{
    ImGuiFrameFixture fixture;
    ImPlotContextGuard implot;

    auto windowA = makeTemplateWindow();
    auto windowB = makeTemplateWindow();

    ImGuiFrameFixture::runFrame([&] {
        windowA->render();
        windowB->render();
    });

    SUCCEED("second Begin() of the same-named windows appends without crashing");
}

TEST_CASE("TemplateWindow: render does not change the currently applied theme", "[templatewindow][render]")
{
    ImGuiFrameFixture fixture;
    ImPlotContextGuard implot;

    applyCherryTheme();
    const ImVec4 before = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    auto window = makeTemplateWindow();
    ImGuiFrameFixture::runFrame([&] {
        window->render();
    });

    const ImVec4 after = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    CHECK(colorsEqual(before, after));
}
