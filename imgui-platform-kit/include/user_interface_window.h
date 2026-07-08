#pragma once

/**
 * @file user_interface_window.h
 * @brief Abstract base class for user-defined windows and global window flags.
 *
 * Subclass imgui_kit::UserInterfaceWindow and override render() to create a
 * custom window, then register it with UserInterface::addWindow<T>().
 */

#include <string>

#include "imgui.h"

#if defined(_WIN32)
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#elif defined(__linux__) || defined(__APPLE__)
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

#include "implot.h"
#include "implot_internal.h"

#include <imgui-node-editor/imgui_node_editor.h>
namespace ImNodeEditor = ax::NodeEditor;

namespace imgui_kit
{
	/// ImGui window flags shared by every window managed by the kit.
	extern ImGuiWindowFlags g_globalWindowFlags;
	/**
	 * @brief Returns the window flags currently applied to every kit-managed window.
	 * @return The current global ImGui window flags.
	 */
	ImGuiWindowFlags getGlobalWindowFlags();
	/**
	 * @brief Replaces the global window flags.
	 * @param flags New set of ImGui window flags.
	 */
	void setGlobalWindowFlags(ImGuiWindowFlags flags);
	/**
	 * @brief Adds flags to the global window flags (bitwise OR).
	 * @param flags ImGui window flags to add.
	 */
	void addGlobalWindowFlags(ImGuiWindowFlags flags);
	/**
	 * @brief Removes flags from the global window flags.
	 * @param flags ImGui window flags to remove.
	 */
	void removeGlobalWindowFlags(ImGuiWindowFlags flags);

	/**
	 * @brief Base type for parameters passed to UserInterfaceWindow subclasses.
	 *
	 * Intentionally empty; derived structs are expected to define the member
	 * variables and functions relevant to their respective windows.
	 */
	struct UserInterfaceWindowParameters final
	{
		// This struct is intentionally left empty.
		// Derived structs are expected to define specific member variables
		// and functions relevant to their respective elements.
		UserInterfaceWindowParameters() = default;
		~UserInterfaceWindowParameters() = default;
	};

	/**
	 * @brief Abstract base class for all windows rendered by the kit.
	 *
	 * This is the library's extension point: derive from it, override
	 * render() with your ImGui draw calls, and register the window with
	 * UserInterface::addWindow<T>(). render() is invoked once per frame while
	 * the interface is running.
	 *
	 * Instances are non-copyable and non-movable; they are owned by the
	 * UserInterface after registration.
	 *
	 * @see TemplateWindow for a minimal example, LogWindow for a full one.
	 */
	class UserInterfaceWindow
	{
	public:
		UserInterfaceWindow() = default;
		/**
		 * @brief Renders the window contents. Called once per frame.
		 *
		 * Implementations typically wrap their draw calls in
		 * ImGui::Begin()/ImGui::End().
		 */
		virtual void render() = 0;
		virtual ~UserInterfaceWindow() = default;

		UserInterfaceWindow(const UserInterfaceWindow&) = delete;
		UserInterfaceWindow& operator=(const UserInterfaceWindow&) = delete;
		UserInterfaceWindow(UserInterfaceWindow&&) = delete;
		UserInterfaceWindow& operator=(UserInterfaceWindow&&) = delete;
	};
}
