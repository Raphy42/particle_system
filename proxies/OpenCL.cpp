//
// Created by Raphaël Dantzer on 18/10/16.
//

#include <vector>
#include <OpenCL/cl_gl.h>
#include <OpenGL/OpenGL.h>
#include "OpenCL.h"
#include "../utils/FileLogger.h"

static std::vector<std::string> cl_errors = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "UNDEFINED",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE"
};

inline void cl_checkStatus(cl_int status, const char *caller)
{
    if (status == CL_SUCCESS)
        return;
    const unsigned long errorCount = cl_errors.size();
    const int index = -status;
    std::string error = cl_errors[index];
    error.append(": ");
    FLOG_CRITICAL(error + caller);
}

#define STATUS(s)    (cl_checkStatus(_status, s))

void Proxy::OpenCL::PreInit() {
    _status = clGetPlatformIDs(0, nullptr, &_platformCount);
    STATUS("clGetPlatformIDs");
    _platforms = new cl_platform_id[_platformCount];
    _status = clGetPlatformIDs(_platformCount, _platforms, NULL);
    STATUS("clGetPlatformIDs");
}

#if defined(__APPLE__)
void Proxy::OpenCL::Init() {
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);

        cl_context_properties properties[] = {
                CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                (cl_context_properties)kCGLShareGroup,
                0
        };
        _context = clCreateContext(properties, 0, nullptr, nullptr, nullptr, &_status);
        STATUS("clCreateContext");
        _status = clGetGLContextInfoAPPLE(_context, kCGLContext, CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE,
                                sizeof(_device), &_device, nullptr);
        STATUS("clGetGLContextInfoAPPLE");
}
#elif defined(__linux__)

#include <CL/cl_gl.h>
#include <GL/glx.h>

void Proxy::OpenCL::Init() {
    cl_context_properties       properties[] = {
                CL_CONTEXT_PLATFORM, (cl_context_properties)_platforms[0],
                CL_GL_CONTEXT_KHR,   (cl_context_properties)glXGetCurrentContext(),
                CL_GLX_DISPLAY_KHR,  (cl_context_properties)glXGetCurrentDisplay(),
                0
        };
        cl_device_id devices[32];
        size_t size;
        _status = clGetGLContextInfoKHR(properties, CL_DEVICES_FOR_GL_CONTEXT_KHR,
            32 * sizeof(cl_device_id), devices, &size);
        _context = clCreateContext(properties, 0, devices, NULL, 0, 0);
}
#else
#error "Cant init OpenCL as it is not yet implemented for other platforms: test with #define LEGACY_SALE in main"
#endif

void Proxy::OpenCL::PostInit() {
    _queue = clCreateCommandQueue(_context, _device, 0, &_status);
    STATUS("clCreateCommandQueue");
}

Proxy::OpenCL::OpenCL() : _kernels(new std::unordered_map<std::string, cl_kernel>) {
    PreInit();
    Init();
    PostInit();
}

Proxy::OpenCL::~OpenCL() {
//    clReleaseCommandQueue(_queue);
//    clReleaseContext(_context);
    //todo fix SIGABRT on destructor call
}

static void getProgramBuildError(cl_program program, cl_device_id device)
{
    size_t log_size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
    char log[log_size];

    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, nullptr);
    clReleaseProgram(program);
    FLOG_CRITICAL(log);
}

void Proxy::OpenCL::CreateKernelFromFile(const char *filename, const char *kernel_name) {
    std::ifstream t(filename);
    std::string str;

    if (!t.is_open())
        FLOG_CRITICAL("Failed to open file");

    t.seekg(0, std::ios::end);
    str.reserve(static_cast<unsigned long>(t.tellg()));
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    const char *source = str.c_str();
    const size_t length = str.length();

    _program = clCreateProgramWithSource(_context, 1, &source, &length, &_status);
    STATUS("clCreateProgramWithSource");
    _status = clBuildProgram(_program, 1, &_device, NULL, NULL, NULL);
    if (_status != CL_SUCCESS)
        getProgramBuildError(_program, _device);
    STATUS("clBuildProgram");

    cl_kernel kernel;
    kernel = clCreateKernel(_program, kernel_name, &_status);
    STATUS("clCreateKernel");
    _kernels->emplace(kernel_name, kernel);
}

void Proxy::OpenCL::CreateKernels(std::vector<const char *> kernel_name)
{
    for (auto it : kernel_name)
        CreateKernelFromProgram(it);
}

void Proxy::OpenCL::CreateKernelFromProgram(const char *kernel_name)
{
    cl_kernel kernel;
    kernel = clCreateKernel(_program, kernel_name, &_status);
    STATUS("clCreateKernel");
    _kernels->emplace(kernel_name, kernel);
}

cl_mem Proxy::OpenCL::CreateBufferFromVBO(cl_GLuint vbo, cl_mem_flags permission)
{
    cl_mem buffer = clCreateFromGLBuffer(_context, permission, vbo, &_status);
    STATUS("clCreateFromGLBuffer");
    return buffer;
}

cl_mem Proxy::OpenCL::CreateBuffer(size_t size, cl_mem_flags permission, void *host_ptr) {
    cl_mem buffer = clCreateBuffer(_context, permission, size, host_ptr, &_status);
    STATUS("clCreateBuffer");
    return buffer;
}


const cl_command_queue Proxy::OpenCL::getQueue() const {
    return _queue;
}

const void Proxy::OpenCL::getStatus(cl_int status, const char *caller) {
    cl_checkStatus(status, caller);
}

const cl_kernel Proxy::OpenCL::getKernel(std::string kernel) const {
    return _kernels->at(kernel);
}

