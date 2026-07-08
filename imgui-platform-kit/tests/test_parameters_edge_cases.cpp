#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include "user_interface_parameters.h"

using namespace imgui_kit;
namespace fs = std::filesystem;

namespace
{
    // RAII guard: writes `content` to a unique temp file and removes it on
    // destruction, even if an assertion throws/fails mid-test.
    class TempIniFile
    {
    public:
        explicit TempIniFile(const std::string& name, const std::string& content)
            : path((fs::temp_directory_path() / name).string())
        {
            std::error_code ec;
            fs::remove(path, ec);
            std::ofstream out(path);
            out << content;
        }

        ~TempIniFile()
        {
            std::error_code ec;
            fs::remove(path, ec);
        }

        TempIniFile(const TempIniFile&) = delete;
        TempIniFile& operator=(const TempIniFile&) = delete;

        [[nodiscard]] const std::string& str() const { return path; }

    private:
        std::string path;
    };
}

// ─── load() throws on malformed numeric fields ────────────────────────────────

TEST_CASE("WindowParameters: load throws std::invalid_argument on malformed Width", "[parameters][window][io][edge]")
{
    // Pins current behavior: load() does not validate numeric fields; a
    // corrupt INI throws a raw std::invalid_argument out of std::stoi.
    TempIniFile ini("ipk_edge_bad_width.ini", "[Window Parameters]\nWidth=abc\n");
    WindowParameters p("App", 800, 600);
    CHECK_THROWS_AS(p.load(ini.str()), std::invalid_argument);
}

TEST_CASE("WindowParameters: load throws std::invalid_argument on malformed Height", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_bad_height.ini", "[Window Parameters]\nHeight=xyz\n");
    WindowParameters p("App", 800, 600);
    CHECK_THROWS_AS(p.load(ini.str()), std::invalid_argument);
}

TEST_CASE("WindowParameters: load throws std::invalid_argument on malformed StartPosX", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_bad_startposx.ini", "[Window Parameters]\nStartPosX=notanumber\n");
    WindowParameters p("App", 800, 600);
    CHECK_THROWS_AS(p.load(ini.str()), std::invalid_argument);
}

TEST_CASE("WindowParameters: load throws std::invalid_argument on malformed StartPosY", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_bad_startposy.ini", "[Window Parameters]\nStartPosY=??\n");
    WindowParameters p("App", 800, 600);
    CHECK_THROWS_AS(p.load(ini.str()), std::invalid_argument);
}

TEST_CASE("WindowParameters: load throws std::out_of_range on an overflowing value", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_overflow.ini", "[Window Parameters]\nWidth=99999999999999999999\n");
    WindowParameters p("App", 800, 600);
    CHECK_THROWS_AS(p.load(ini.str()), std::out_of_range);
}

// ─── partial / sparse files ────────────────────────────────────────────────────

TEST_CASE("WindowParameters: partial file only updates the keys present", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_partial.ini", "[Window Parameters]\nTitle=OnlyTitle\n");
    WindowParameters p("Original", 1280, 720, 5, 6);

    p.load(ini.str());

    CHECK(p.title == "OnlyTitle");
    CHECK(p.width == 1280);
    CHECK(p.height == 720);
    CHECK(p.startPosX == 5);
    CHECK(p.startPosY == 6);
}

TEST_CASE("WindowParameters: unknown keys are ignored", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_unknown_keys.ini",
        "[Window Parameters]\nNonsense=42\nDepth=3\nTitle=KnownTitle\n");
    WindowParameters p("Original", 1280, 720);

    REQUIRE_NOTHROW(p.load(ini.str()));
    CHECK(p.title == "KnownTitle");
    CHECK(p.width == 1280);
    CHECK(p.height == 720);
}

TEST_CASE("WindowParameters: empty file is a no-op on all fields", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_empty.ini", "");
    WindowParameters p("Original", 1280, 720, 5, 6);

    p.load(ini.str());

    CHECK(p.title == "Original");
    CHECK(p.width == 1280);
    CHECK(p.height == 720);
    CHECK(p.startPosX == 5);
    CHECK(p.startPosY == 6);
}

TEST_CASE("WindowParameters: the [Window Parameters] header line is ignored", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_header_only.ini", "[Window Parameters]\n");
    WindowParameters p("Original", 1280, 720, 5, 6);

    REQUIRE_NOTHROW(p.load(ini.str()));
    CHECK(p.title == "Original");
    CHECK(p.width == 1280);
}

// ─── title containing '=' and spaces round-trips ───────────────────────────────

TEST_CASE("WindowParameters: title containing spaces and '=' round-trips exactly", "[parameters][window][io][edge]")
{
    TempIniFile ini("ipk_edge_weird_title.ini", "");
    const std::string& path = ini.str();

    const WindowParameters saved("My = Weird == Title", 1280, 720, 1, 2);
    saved.save(path);

    WindowParameters loaded("Other", 640, 480);
    loaded.load(path);

    CHECK(loaded.title == "My = Weird == Title");
}

// ─── negative/zero values bypass validation on load() ──────────────────────────

TEST_CASE("WindowParameters: load stores negative values without validation", "[parameters][window][io][edge]")
{
    // The width/height > 0 validation only runs in the constructor's
    // monitor-fallback logic, not in load() itself — load() bypasses it.
    TempIniFile ini("ipk_edge_negative.ini", "[Window Parameters]\nWidth=-500\nHeight=-1\n");
    WindowParameters p("App", 800, 600);

    REQUIRE_NOTHROW(p.load(ini.str()));
    CHECK(p.width == -500);
    CHECK(p.height == -1);
}

// ─── save() determinism ─────────────────────────────────────────────────────────

static std::string readFile(const std::string& path)
{
    std::ifstream in(path);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

TEST_CASE("WindowParameters: save() is deterministic and writes exactly 6 lines", "[parameters][window][io][edge]")
{
    TempIniFile iniA("ipk_edge_det_a.ini", "");
    TempIniFile iniB("ipk_edge_det_b.ini", "");

    const WindowParameters p("Deterministic", 1024, 768, 3, 4);
    p.save(iniA.str());
    p.save(iniB.str());

    const std::string contentA = readFile(iniA.str());
    const std::string contentB = readFile(iniB.str());
    CHECK(contentA == contentB);

    const auto lineCount = std::count(contentA.begin(), contentA.end(), '\n');
    CHECK(lineCount == 6);
}

// ─── save() throws with message on unopenable path ────────────────────────────

TEST_CASE("WindowParameters: save() throws with a descriptive message on an unopenable path", "[parameters][window][io][edge]")
{
    const WindowParameters p("App", 800, 600);
    CHECK_THROWS_WITH(
        p.save("this/directory/does/not/exist/file.ini"),
        Catch::Matchers::ContainsSubstring("Could not open"));
}

// ─── UserInterfaceParameters::save() with a custom filename ───────────────────

TEST_CASE("UserInterfaceParameters: save() with a custom filename delegates to WindowParameters::save()", "[parameters][io][edge]")
{
    TempIniFile ini("ipk_edge_uiparams.ini", "");

    const UserInterfaceParameters params(WindowParameters("Delegated", 800, 600, 0, 0));
    params.save(ini.str());

    REQUIRE(fs::exists(ini.str()));
    const std::string content = readFile(ini.str());
    CHECK(content.rfind("[Window Parameters]", 0) == 0);
    CHECK(content.find("Title=Delegated") != std::string::npos);
}
