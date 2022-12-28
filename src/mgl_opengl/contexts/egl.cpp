
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifdef MGL_OPENGL_EGL

#  include "mgl_core/log.hpp"
#  include "mgl_opengl/context.hpp"
#  include <dlfcn.h>

struct Display;

typedef unsigned int EGLenum;
typedef int EGLint;
typedef unsigned int EGLBoolean;
typedef Display* EGLNativeDisplayType;
typedef void* EGLConfig;
typedef void* EGLSurface;
typedef void* EGLContext;
typedef void* EGLDeviceEXT;
typedef void* EGLDisplay;

#  define EGL_DEFAULT_DISPLAY 0
#  define EGL_NO_CONTEXT 0
#  define EGL_NO_SURFACE 0
#  define EGL_NO_DISPLAY 0
#  define EGL_PBUFFER_BIT 0x0001
#  define EGL_WINDOW_BIT 0x0004
#  define EGL_RENDERABLE_TYPE 0x3040
#  define EGL_NONE 0x3038
#  define EGL_OPENGL_BIT 0x0008
#  define EGL_BLUE_SIZE 0x3022
#  define EGL_DEPTH_SIZE 0x3025
#  define EGL_RED_SIZE 0x3024
#  define EGL_GREEN_SIZE 0x3023
#  define EGL_SURFACE_TYPE 0x3033
#  define EGL_OPENGL_API 0x30A2
#  define EGL_WIDTH 0x3057
#  define EGL_HEIGHT 0x3056
#  define EGL_SUCCESS 0x3000
#  define EGL_CONTEXT_MAJOR_VERSION 0x3098
#  define EGL_CONTEXT_MINOR_VERSION 0x30FB
#  define EGL_CONTEXT_OPENGL_PROFILE_MASK 0x30FD
#  define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT 0x00000001
#  define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE 0x31B1
#  define EGL_PLATFORM_DEVICE_EXT 0x313F
#  define EGL_PLATFORM_WAYLAND_EXT 0x31D8
#  define EGL_PLATFORM_X11_EXT 0x31D5
#  define EGL_DRAW 0x3059
#  define EGL_READ 0x305A

typedef EGLint (*m_eglGetErrorProc)();
typedef EGLDisplay (*m_eglGetDisplayProc)(EGLNativeDisplayType);
typedef EGLBoolean (*m_eglInitializeProc)(EGLDisplay, EGLint*, EGLint*);
typedef EGLBoolean (*m_eglChooseConfigProc)(EGLDisplay, const EGLint*, EGLConfig*, EGLint, EGLint*);
typedef EGLBoolean (*m_eglBindAPIProc)(EGLenum);
typedef EGLContext (*m_eglCreateContextProc)(EGLDisplay, EGLConfig, EGLContext, const EGLint*);
typedef EGLBoolean (*m_eglDestroyContextProc)(EGLDisplay, EGLContext);
typedef EGLBoolean (*m_eglMakeCurrentProc)(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
typedef void (*(*m_eglGetProcAddressProc)(const char*))();
typedef EGLBoolean (*m_eglQueryDevicesEXTProc)(EGLint, EGLDeviceEXT*, EGLint*);
typedef EGLDisplay (*m_eglGetPlatformDisplayEXTProc)(EGLenum, void*, const EGLint*);
typedef EGLContext (*m_eglGetCurrentContextProc)(void);
typedef EGLSurface (*m_eglGetCurrentSurfaceProc)(EGLint readdraw);
typedef EGLDisplay (*m_eglGetCurrentDisplayProc)(void);

struct EGLContextData
{
  void* libgl;
  void* libegl;
  EGLContext ctx;
  EGLDisplay dpy;
  EGLConfig cfg;
  EGLSurface wnd;

  int standalone;

  m_eglGetErrorProc m_eglGetError;
  m_eglGetDisplayProc m_eglGetDisplay;
  m_eglInitializeProc m_eglInitialize;
  m_eglChooseConfigProc m_eglChooseConfig;
  m_eglBindAPIProc m_eglBindAPI;
  m_eglCreateContextProc m_eglCreateContext;
  m_eglDestroyContextProc m_eglDestroyContext;
  m_eglMakeCurrentProc m_eglMakeCurrent;
  m_eglGetProcAddressProc m_eglGetProcAddress;
  m_eglQueryDevicesEXTProc m_eglQueryDevicesEXT;
  m_eglGetPlatformDisplayEXTProc m_eglGetPlatformDisplayEXT;
  m_eglGetCurrentContextProc m_eglGetCurrentContext;
  m_eglGetCurrentSurfaceProc m_eglGetCurrentSurface;
  m_eglGetCurrentDisplayProc m_eglGetCurrentDisplay;
};

namespace mgl_opengl
{
  ContextEGL::ContextEGL(ContextMode::Enum mode, int required)
  {
    m_mode = mode;
    m_released = true;
    int device_index = 0;

    m_context = nullptr;

    auto res = new EGLContextData;

    res->libgl = dlopen("libGL.so", RTLD_LAZY);
    if(!res->libgl)
    {
      MGL_CORE_ERROR("'libGL.so' not loaded");
      delete res;
      return;
    }

    res->libegl = dlopen("libEGL.so", RTLD_LAZY);
    if(!res->libegl)
    {
      MGL_CORE_ERROR("'libEGL.so' not loaded");
      delete res;
      return;
    }

    res->m_eglGetError = (m_eglGetErrorProc)dlsym(res->libegl, "eglGetError");
    if(!res->m_eglGetError)
    {
      MGL_CORE_ERROR("eglGetError not found");
      delete res;
      return;
    }

    res->m_eglGetDisplay = (m_eglGetDisplayProc)dlsym(res->libegl, "eglGetDisplay");
    if(!res->m_eglGetDisplay)
    {
      MGL_CORE_ERROR("eglGetDisplay not found");
      delete res;
      return;
    }

    res->m_eglInitialize = (m_eglInitializeProc)dlsym(res->libegl, "eglInitialize");
    if(!res->m_eglInitialize)
    {
      MGL_CORE_ERROR("eglInitialize not found");
      delete res;
      return;
    }

    res->m_eglChooseConfig = (m_eglChooseConfigProc)dlsym(res->libegl, "eglChooseConfig");
    if(!res->m_eglChooseConfig)
    {
      MGL_CORE_ERROR("eglChooseConfig not found");
      delete res;
      return;
    }

    res->m_eglBindAPI = (m_eglBindAPIProc)dlsym(res->libegl, "eglBindAPI");
    if(!res->m_eglBindAPI)
    {
      MGL_CORE_ERROR("eglBindAPI not found");
      delete res;
      return;
    }

    res->m_eglCreateContext = (m_eglCreateContextProc)dlsym(res->libegl, "eglCreateContext");
    if(!res->m_eglCreateContext)
    {
      MGL_CORE_ERROR("eglCreateContext not found");
      delete res;
      return;
    }

    res->m_eglDestroyContext = (m_eglDestroyContextProc)dlsym(res->libegl, "eglDestroyContext");
    if(!res->m_eglDestroyContext)
    {
      MGL_CORE_ERROR("eglDestroyContext not found");
      delete res;
      return;
    }

    res->m_eglMakeCurrent = (m_eglMakeCurrentProc)dlsym(res->libegl, "eglMakeCurrent");
    if(!res->m_eglMakeCurrent)
    {
      MGL_CORE_ERROR("eglMakeCurrent not found");
      delete res;
      return;
    }

    res->m_eglGetProcAddress = (m_eglGetProcAddressProc)dlsym(res->libegl, "eglGetProcAddress");
    if(!res->m_eglGetProcAddress)
    {
      MGL_CORE_ERROR("eglGetProcAddress not found");
      delete res;
      return;
    }

    res->m_eglQueryDevicesEXT = (m_eglQueryDevicesEXTProc)res->m_eglGetProcAddress("eglQueryDevicesEXT");
    if(!res->m_eglQueryDevicesEXT)
    {
      MGL_CORE_ERROR("eglQueryDevicesEXT not found");
      delete res;
      return;
    }

    res->m_eglGetPlatformDisplayEXT = (m_eglGetPlatformDisplayEXTProc)res->m_eglGetProcAddress("eglGetPlatformDisplayEXT");
    if(!res->m_eglGetPlatformDisplayEXT)
    {
      MGL_CORE_ERROR("eglGetPlatformDisplayEXT not found");
      delete res;
      return;
    }

    res->m_eglGetCurrentDisplay = (m_eglGetCurrentDisplayProc)res->m_eglGetProcAddress("eglGetCurrentDisplay");
    if(!res->m_eglGetCurrentDisplay)
    {
      MGL_CORE_ERROR("eglGetCurrentDisplay not found");
      delete res;
      return;
    }

    res->m_eglGetCurrentContext = (m_eglGetCurrentContextProc)res->m_eglGetProcAddress("eglGetCurrentContext");
    if(!res->m_eglGetCurrentContext)
    {
      MGL_CORE_ERROR("eglGetCurrentContext not found");
      delete res;
      return;
    }

    res->m_eglGetCurrentSurface = (m_eglGetCurrentSurfaceProc)res->m_eglGetProcAddress("eglGetCurrentSurface");
    if(!res->m_eglGetCurrentSurface)
    {
      MGL_CORE_ERROR("eglGetCurrentSurfaceProc not found");
      delete res;
      return;
    }

    switch(mode)
    {
      case ContextMode::STANDALONE: {
        res->standalone = true;
        res->wnd = EGL_NO_SURFACE;

        EGLint num_devices;
        if(!res->m_eglQueryDevicesEXT(0, NULL, &num_devices))
        {
          MGL_CORE_ERROR("eglQueryDevicesEXT failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        if(device_index >= num_devices)
        {
          MGL_CORE_ERROR("requested device index %d, but found %d devices", device_index, num_devices);
          delete res;
          return;
        }

        EGLDeviceEXT* devices = (EGLDeviceEXT*)malloc(sizeof(EGLDeviceEXT) * num_devices);
        if(!res->m_eglQueryDevicesEXT(num_devices, devices, &num_devices))
        {
          MGL_CORE_ERROR("eglQueryDevicesEXT failed (0x%x)", res->m_eglGetError());
          free(devices);
          delete res;
          return;
        }
        EGLDeviceEXT device = devices[device_index];
        free(devices);

        res->dpy = res->m_eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, device, 0);
        if(res->dpy == EGL_NO_DISPLAY)
        {
          MGL_CORE_ERROR("eglGetPlatformDisplayEXT failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        EGLint major, minor;
        if(!res->m_eglInitialize(res->dpy, &major, &minor))
        {
          MGL_CORE_ERROR("eglInitialize failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        EGLint config_attribs[] = {
          EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8,
          EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,  EGL_NONE
        };

        EGLint num_configs = 0;
        if(!res->m_eglChooseConfig(res->dpy, config_attribs, &res->cfg, 1, &num_configs))
        {
          MGL_CORE_ERROR("eglChooseConfig failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        if(!res->m_eglBindAPI(EGL_OPENGL_API))
        {
          MGL_CORE_ERROR("eglBindAPI failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        int ctxattribs[] = {
          EGL_CONTEXT_MAJOR_VERSION,
          required / 100 % 10,
          EGL_CONTEXT_MINOR_VERSION,
          required / 10 % 10,
          EGL_CONTEXT_OPENGL_PROFILE_MASK,
          EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
          // EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, 1,
          EGL_NONE,
        };

        res->ctx = res->m_eglCreateContext(res->dpy, res->cfg, EGL_NO_CONTEXT, ctxattribs);
        if(!res->ctx)
        {
          MGL_CORE_ERROR("eglCreateContext failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        res->m_eglMakeCurrent(res->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, res->ctx);
      }
      break;
      case ContextMode::SHARE: {
        res->standalone = false;

        EGLContext ctx_share = res->m_eglGetCurrentContext();
        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) eglGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->wnd = res->m_eglGetCurrentSurface(EGL_DRAW);
        if(!res->wnd)
        {
          MGL_CORE_ERROR("(share) m_eglGetCurrentSurface failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        res->dpy = res->m_eglGetCurrentDisplay();
        if(res->dpy == EGL_NO_DISPLAY)
        {
          MGL_CORE_ERROR("eglGetCurrentDisplay failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        EGLint config_attribs[] = {
          EGL_SURFACE_TYPE,    EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24,
          EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_NONE
        };

        EGLint num_configs = 0;
        if(!res->m_eglChooseConfig(res->dpy, config_attribs, &res->cfg, 1, &num_configs))
        {
          MGL_CORE_ERROR("eglChooseConfig failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        if(!res->m_eglBindAPI(EGL_OPENGL_API))
        {
          MGL_CORE_ERROR("eglBindAPI failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        int ctxattribs[] = {
          EGL_CONTEXT_MAJOR_VERSION,
          required / 100 % 10,
          EGL_CONTEXT_MINOR_VERSION,
          required / 10 % 10,
          EGL_CONTEXT_OPENGL_PROFILE_MASK,
          EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
          // EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, 1,
          EGL_NONE,
        };

        res->ctx = res->m_eglCreateContext(res->dpy, res->cfg, ctx_share, ctxattribs);
        if(!res->ctx)
        {
          MGL_CORE_ERROR("eglCreateContext failed (0x%x)", res->m_eglGetError());
          delete res;
          return;
        }

        res->m_eglMakeCurrent(res->dpy, res->wnd, res->wnd, res->ctx);
      }
      break;
      default: {
        MGL_CORE_ERROR("Detect mode not supported");
        delete res;
        return;
      }
      break;
    }

    m_context = res;
    m_released = false;
  }

  ContextEGL::~ContextEGL()
  {
    if(m_context != nullptr)
      delete(EGLContextData*)m_context;
  }

  gl_function ContextEGL::load(const mgl_core::string& method)
  {
    if(!m_context)
      return nullptr;

    auto self = (EGLContextData*)m_context;

    void* proc = (void*)dlsym(self->libgl, method.c_str());
    if(!proc)
    {
      proc = (void*)self->m_eglGetProcAddress(method.c_str());
    }

    return proc;
  }

  void ContextEGL::enter()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    self->m_eglMakeCurrent(self->dpy, self->wnd, self->wnd, self->ctx);
  }

  void ContextEGL::exit()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    self->m_eglMakeCurrent(self->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  }

  void ContextEGL::release()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    self->m_eglDestroyContext(self->dpy, self->ctx);
    m_released = true;
  }

  bool ContextEGL::is_valid()
  {
    return m_context != nullptr && !m_released;
  }
} // namespace mgl_opengl
#endif