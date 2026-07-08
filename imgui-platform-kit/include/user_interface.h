#pragma once

/**
 * @file user_interface.h
 * @brief Public entry point of the library; includes the platform-specific
 * imgui_kit::UserInterface implementation.
 *
 * This is the only header an application needs to include. It dispatches at
 * compile time to the backend for the target platform:
 * - Windows: Win32 + DirectX 12 (win32/win32_dx12_user_interface.h)
 * - macOS: GLFW + OpenGL 3 (macos/glfw_opengl3_user_interface.h)
 * - Linux: GLFW + OpenGL 3 (linux/glfw_opengl3_user_interface.h)
 *
 * All backends expose the same public imgui_kit::UserInterface API
 * (initialize(), render(), shutdown(), isShutdownRequested(), addWindow()).
 */

#if defined(_WIN32)
#include "win32/win32_dx12_user_interface.h"
#elif defined(__APPLE__)
#include "macos/glfw_opengl3_user_interface.h"
#elif defined(__linux__)
#include "linux/glfw_opengl3_user_interface.h"
#endif
