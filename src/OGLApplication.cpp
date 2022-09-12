/**
 * OGLApplication.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include "OGLApplication.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

int r_width=1920;
int r_height=1080;

OGLApplication* currentApplication = NULL;

OGLApplication& OGLApplication::getInstance() {
  if (currentApplication)
    return *currentApplication;
  else
    throw std::runtime_error("There is no current Application");
}

OGLApplication::OGLApplication()
    : _state(stateReady), title("Terrain Generator") {
  currentApplication = this;

  cout << "[Info] GLFW initialisation" << endl;

  // initialize the GLFW library
  if (!glfwInit()) {
    throw std::runtime_error("Couldn't init GLFW");
  }

  // setting the opengl version
  int major = 3;
  int minor = 2;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create the window
  _window = glfwCreateWindow(r_width, r_height, title.c_str(), NULL, NULL);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Couldn't create a window");
  }

  glfwMakeContextCurrent(_window);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error(string("Could initialize GLEW, error = ") +
                             (const char*)glewGetErrorString(err));
  }

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  cout << "Renderer: " << renderer << endl;
  cout << "OpenGL version supported " << version << endl;

  // opengl configuration
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  // set the window user pointer to this instance so that we can reference members in OGL callbacks
  glfwSetWindowUserPointer(_window, this);

  // setup mouse event listeners
  glfwSetCursorPosCallback(_window, +[](GLFWwindow* win, double x, double y){
    static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))->mouseMoved(win, x, y);
  });
  glfwSetWindowSizeCallback(_window, +[](GLFWwindow* win, int cx, int cy){
    static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))->windowSizeChanged(win, cx, cy);
  });

  // setup keyboard event listener
  glfwSetKeyCallback(_window, +[](GLFWwindow* win, int key, int scancode, int action, int mods){
    static_cast<OGLApplication*>(glfwGetWindowUserPointer(win))->handleKeyboardEvent(win, key, scancode, action, mods);
  });

  // setup monitor
  _primaryMonitor = glfwGetPrimaryMonitor();
  glfwGetWindowSize(_window, &_windowSize[0], &_windowSize[1]);
  glfwGetWindowPos(_window, &_windowSize[0], &_windowSize[1]);
  _updateViewport = true;
}

GLFWwindow* OGLApplication::getWindow() const {
  return _window;
}

void OGLApplication::exit() {
  _state = stateExit;
}

float OGLApplication::getFrameDeltaTime() const {
  return _deltaTime;
}

float OGLApplication::getTime() const {
  return _time;
}

void OGLApplication::run() {
  _state = stateRun;

  // Make the window's context current
  glfwMakeContextCurrent(_window);

  _time = glfwGetTime();

  while (_state == stateRun) {
    // compute new time and delta time
    float t = glfwGetTime();
    _deltaTime = t - _time;
    _time = t;

    if (_updateViewport) {
      glfwGetFramebufferSize(_window, &_viewportSize[0], &_viewportSize[1]);
      glViewport(0, 0, _viewportSize[0], _viewportSize[1]);
      _updateViewport = false;
    }

    // execute the frame code
    loop();

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(_window);

    // Pool and process events
    glfwPollEvents();
  }

  glfwTerminate();
}

void OGLApplication::windowSizeChanged(GLFWwindow* window, int cx, int cy) {
  void *ptr = glfwGetWindowUserPointer(window);
  if (OGLApplication *wndPtr = static_cast<OGLApplication*>(ptr)) {
    wndPtr->_resize(cx, cy);
  }
}

void OGLApplication::_resize(int cx, int cy) {
    _updateViewport = true;
}

bool OGLApplication::isFullScreen() {
  return glfwGetWindowMonitor(_window) != NULL;
}

void OGLApplication::setFullScreen(bool fullscreen) {
  if (isFullScreen() == fullscreen) {
    return;
  }

  if (fullscreen) {
    // backup window position and window size
    glfwGetWindowPos(_window, &_windowPosition[0], &_windowPosition[1]);
    glfwGetWindowSize(_window, &_windowSize[0], &_windowSize[1]);
    
    // get resolution of monitor
    const GLFWvidmode* mode = glfwGetVideoMode(_primaryMonitor);

    cout << "setting fullsreen" << endl;
    cout << "resolution: " << mode->width << "*" << mode->height << endl;

    // switch to full screen
    glfwSetWindowMonitor(_window, _primaryMonitor, 0, 0, mode->width, mode->height, 0);
  } else {
    cout << "unsetting fullsreen" << endl;
    // restore last window size and position
    glfwSetWindowMonitor(_window, NULL, _windowPosition[0], _windowPosition[1], _windowSize[0], _windowSize[1], 0);
  }

  _updateViewport = true;
}

void OGLApplication::loop() {
  cout << "[INFO] : loop" << endl;
}

int OGLApplication::getWidth() {
  return _windowSize[0];
}

int OGLApplication::getHeight() {
  return _windowSize[1];
}

float OGLApplication::getWindowRatio() {
  return float(_windowSize[0]) / float(_windowSize[1]);
}

void OGLApplication::mouseMoved(GLFWwindow * window, double x, double y) {
  cout << "[INFO] : mouseMoved to <" << x << "," << y << ">" << endl;
}

void OGLApplication::handleKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
  cout << "[INFO] : keyboard event, key = " << key << ", scancode = " << scancode << ", action = " << action << ", mods = " << mods << endl;
}