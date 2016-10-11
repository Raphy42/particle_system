//
// Created by Raphaël Dantzer on 07/10/16.
//

#include <fstream>
#include "SharedContext.h"

static bool     IsExtensionSupported(const char *support_str, const char *ext_string, size_t ext_buffer_size)
{
    size_t      offset = 0;
    const char *space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
    size_t      space_pos = space_substr ? space_substr - ext_string : 0;
    while (space_pos < ext_buffer_size)
    {
        if (strncmp(support_str, ext_string + offset, space_pos) == 0)
        {
            std::cerr << "Support found for extension: " << support_str << std::endl;
            return true;
        }
        offset = space_pos + 1;
        space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
        space_pos = space_substr ? space_substr - ext_string : 0;
    }
    return false;
}

OpenCL::SharedContext::SharedContext() : _context(nullptr)
{
    this->Init();
}


OpenCL::SharedContext::~SharedContext() {
//    clReleaseKernel(_kernel);
//    clReleaseProgram(_program);
    clReleaseContext(_context);
    free(_platforms);
    free(_deviceList);
}

void OpenCL::SharedContext::Init() {
#if defined(__APPLE__) || defined(MACOSX)
    static const char   *CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
    static const char   *CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif
    size_t              extension_size = 1024;
    char                *ext_string = (char *)malloc(extension_size);

    _clStatus = clGetPlatformIDs(0, NULL, &_numPlatforms);
    _platforms = (cl_platform_id *)(malloc(sizeof(cl_platform_id) * _numPlatforms));
    _clStatus = clGetPlatformIDs(_numPlatforms, _platforms, NULL);
    _clStatus = clGetDeviceIDs(_platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &_numDevices);
    _deviceList = (cl_device_id  *)(malloc(sizeof(cl_device_id) * _numDevices));
    _clStatus = clGetDeviceIDs(_platforms[0], CL_DEVICE_TYPE_GPU, _numDevices, _deviceList, NULL);
    OpenCL::getStatus(_clStatus, "clGetDeviceIDs");
    _clStatus = clGetDeviceInfo(_deviceList[0], CL_DEVICE_EXTENSIONS, extension_size, ext_string, &extension_size);
    OpenCL::getStatus(_clStatus, "clGetDeviceInfo");

    if (!IsExtensionSupported(CL_GL_SHARING_EXT, ext_string, extension_size))
        throw std::runtime_error(std::string("Extension support not found for ").append(CL_GL_SHARING_EXT));
    free(ext_string);
#if defined(__APPLE__) || defined(MAXOSX)
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

    cl_context_properties properties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties)kCGLShareGroup,
            0
    };

    _context = clCreateContext(properties, 0, 0, NULL, 0, &_clStatus);
    OpenCL::getStatus(_clStatus, "clCreateContext");
#elif defined(__linux__)
    cl_context_properties       properties[] = {
                CL_GL_CONTEXT_KHR, (cl_context_properties)glfwGetGLXContext(window),
                CL_GLX_DISPLAY_KHR, (cl_context_properties)glfwGetX11Display(),
                CL_CONTEXT_PLATFORM, (cl_context_properties)_platforms[0],
                0
        };
        cl_device_id devices[32];
        size_t size;
        clGetGLContextInfoKHR(properties, CL_DEVICES_FOR_GL_CONTEXT_KHR,
            32 * sizeof(cl_device_id), devices, &size);
        _context = clCreateContext(properties, devices, size / sizeof(cl_device_id), NULL, 0, 0);
#else
        cl_context_properties properties[] = {
           CL_GL_CONTEXT_KHR, (cl_context_properties)glfwGetWGLContext(window),
            CL_WGL_HDC_KHR, (cl_context_properties)GetDC(glfwGetWin32Window(window)),
            CL_CONTEXT_PLATFORM, (cl_context_properties)_platforms[0],
            0
        };
        cl_device_id devices[32];
        size_t size;
        clGetGLContextInfoKHR(properties, CL_DEVICES_FOR_GL_CONTEXT_KHR,
            32 * sizeof(cl_device_id), devices, &size);
        _context = clCreateContext(properties, devices, size / sizeof(cl_device_id), NULL, 0, 0);
#endif
    for(int i = 0; i < _numDevices; i++)
    {
        _queue = clCreateCommandQueue(_context, _deviceList[i], 0, &_clStatus);
        OpenCL::getStatus(_clStatus, "clCreateCommandQueue");
        if (_clStatus == CL_SUCCESS)
            break ;
    }
}

cl_context OpenCL::SharedContext::getContext() const {
    return _context;
}

cl_command_queue OpenCL::SharedContext::getQueue() const {
    return _queue;
}

static int getBuildStatus(cl_program program, cl_device_id device)
{
       cl_int logStatus;
       char * buildLog = NULL;
       size_t buildLogSize = 0;
       logStatus = clGetProgramBuildInfo(program,
         device,
         CL_PROGRAM_BUILD_LOG,
         buildLogSize,
         buildLog,
         &buildLogSize);
        OpenCL::getStatus(logStatus, "clGetProgramBuildInfo");
       if(logStatus != CL_SUCCESS)
       {
           std::cerr << "Error # "<< logStatus <<":: clGetProgramBuildInfo<CL_PROGRAM_BUILD_LOG>failed.";
           return (-1);
        }
       buildLog = (char*)malloc(buildLogSize);
       if(buildLog == NULL)
       {
           std::cerr << "Failed to allocate host memory.(buildLog)\n";
           return -1;
       }
       memset(buildLog, 0, buildLogSize);
       logStatus = clGetProgramBuildInfo(program,
         device,
         CL_PROGRAM_BUILD_LOG,
         buildLogSize,
         buildLog,
         NULL);
        OpenCL::getStatus(logStatus, "clGetProgramBuildInfo");
       if(logStatus != CL_SUCCESS)
       {
           std::cerr << "Error # "<< logStatus
               <<":: clGetProgramBuildInfo<CL_PROGRAM_BUILD_LOG>failed.";
        exit(1);
        }
    std::cerr << buildLog << std::endl;
    return (0);
}

void OpenCL::SharedContext::BindKernel(std::string filename)
{
    FILE    *f;
    long    size;
    char    *buffer;

    f = fopen(filename.c_str(), "r");
    if (!f)
        throw std::runtime_error("IO read");
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    buffer = (char *)malloc(sizeof(char) * size);
    fread(buffer, sizeof(char), (size_t)size, f);
    _program = clCreateProgramWithSource(_context, 1, (const char **)&buffer, (const size_t *)&size, &_clStatus);
    OpenCL::getStatus(_clStatus, "clCreateProgramWithSource");
    _clStatus = clBuildProgram(_program, _numDevices, _deviceList, NULL, NULL, NULL);
    OpenCL::getStatus(_clStatus, "clBuildProgram");
    for (int i = 0; i < _numDevices; i++)
    {
        cl_build_status status;
        clGetProgramBuildInfo(_program, _deviceList[i], CL_PROGRAM_BUILD_STATUS,
                              sizeof(status), &status, NULL);
        getBuildStatus(_program, _deviceList[i]);
        std::cerr << "Build status for device " << i << " = " << status << std::endl;
    }
    _kernel = clCreateKernel(_program, "random", &_clStatus);
    OpenCL::getStatus(_clStatus, "clCreateKernel");
}
