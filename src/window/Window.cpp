#include "mypch.h"
#include "Window.h"
#include "core/core.h"
#include "glm/glm/glm.hpp"
#include "events/Event.h"
#include "core/KeyCodes.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

namespace ALStore {

	Window::Window() {}
	Window::~Window() { glfwDestroyWindow(m_Window); }
	static void GLFWErrorCallback(int error, const char* description)
	{
		AL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	bool Window::init(int w, int h) {
		AL_CORE_INFO("Window init");
		if (!glfwInit()) AL_CORE_CRITICAL("GLFW FAILED");
		glfwSetErrorCallback(GLFWErrorCallback);
		m_Window = glfwCreateWindow(w,h, "ALAvery", NULL, NULL);
		if (!m_Window) { AL_CORE_CRITICAL("GLFW WINDOW CREATION FAILED"); return false; }
		m_Data.Title = "ALAvery"; // props.Title;
		m_Data.Width = 800; // props.Width;
		m_Data.Height = 600; // props.Height;

		//AL_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		glfwMakeContextCurrent(m_Window);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AL_CORE_ASSERT(status, "Failed to initialize Glad!");
		AL_CORE_INFO("OpenGL Info:");
		//AL_CORE_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		AL_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		AL_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		AL_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));
	
		GLint major, minor, txtSize, combunits, max[1];
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combunits);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, max);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &txtSize);
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		AL_CORE_INFO("  Max texture units: {0}", std::to_string(max[0]));
		AL_CORE_INFO("  Combined texture units: {0}", std::to_string(combunits));
		AL_CORE_INFO("  Max texture size: {0}", std::to_string(txtSize));

		AL_CORE_INFO("  GL Version(integer) : major {0}", std::to_string(major));
		AL_CORE_INFO("  GL Version(integer) : minor {0}", std::to_string(minor));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// tell GL to only draw onto a pixel if the shape is closer to the viewer
		//////////////////glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glfwSwapInterval(0); //SetVSync(true);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});
		return true;
	}
	void Window::showFPS(GLFWwindow* pWindow)
	{
		//float time = (float)glfwGetTime();
		//Timestep timestep = time - m_LastFrameTime;
		//m_LastFrameTime = time;
	   // HZ_CORE_INFO(time);
		// Measure speed
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		nbFrames++;
		if (delta >= 1.0) { // If last cout was more than 1 sec ago
			//cout << 1000.0 / double(nbFrames) << endl;

			double fps = double(nbFrames) / delta;

			std::stringstream ss;
			ss << "My game" << " [" << fps << " FPS]";

			glfwSetWindowTitle(pWindow, ss.str().c_str());

			nbFrames = 0;
			lastTime = currentTime;
		}
	}
	void Window::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		showFPS(m_Window);
	}
	void Window::OnShutdown() { glfwDestroyWindow(m_Window); glfwTerminate(); }
}