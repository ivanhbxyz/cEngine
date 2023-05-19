#include "platform/platform.h"

#if KPLATFORM_WINDOWS

#include "core/logger.h"

#include <windows.h>
#include <windowsx.h>

typedef struct internal_state {
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state;


// Clock
static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 platform_startup (
    platform_state *plat_state,
    const char *application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
        plat_state->internal_state = malloc(sizeof(internal_state));

        internal_state *state = (internal_state *)plat_state->internal_state;

        state->h_instance = GetModuleHandleA(0);

        HICON icon = LoadIcon(state->h_instance, IDI_APPLICATION);
        WNDCLASSA wc;
        memset(&wc, 0, sizeof(wc));
        wc.style = CS_DBLCLKS; // captures double-clicks
        wc.lpfnWndProc = win32_process_message; // pointer to windows procedure
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = state->h_instance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszClassName = "kohi_window_class";

        if(!RegisterClassA(&wc)) {
            MessageBox(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
            return FALSE;
        }

        // Create window
        u32 client_x = x;
        u32 client_y = y;
        u32 client_width = width;
        u32 client_height = height;

        u32 window_x = client_x;
        u32 window_y = client_y;
        u32 window_width = client_width;
        u32 window_height = client_height;

        u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        u32 window_ex_style = WS_EX_APPWINDOW;

        window_style |= WS_MAXIMIZEBOX;
        window_style |= WS_MINIMIZEBOX;
        window_style |= WS_THICKFRAME;

        RECT border_rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        window_x += border_rect.left;
        window_y += border_rect.top;

        // Grow by the size of the OS border.
        window_width += border_rect.right - border_rect.left;
        window_height += border_rect.bottom - border_rect.top;


        // handle will return what?
        HWND handle = CreateWindowExA (
            window_ex_style, "kohi_window_class", application_name,
            window_style, window_x, window_y, window_width, window_height,
            0, 0, state->h_instance, 0);
        
        if(handle == 0) {
            MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

            KFATAL("Window creation failed!");
            return FALSE;

        } else {
            state->hwnd = handle;
        }

        // Show the window
        b32 should_activate = 1; // TODO: If the window should not accept input
        i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE

        ShowWindow(state->hwnd, show_window_command_flags);

        // clock setup
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clock_frequency = 1.0/(f64)frequency.QuadPart;
        QueryPerformanceCounter(&start_time);

        return TRUE;
    }

    void platform_shutdown(platform_state *plat_state) {

        internal_state *state = (internal_state *) plat_state->internal_state;

        if(state->hwnd) {
            DestroyWindow(state->hwnd);
            state->hwnd = 0;
        }

    }

    b8 platform_pump_messages(platform_state* plat_state) {
        MSG message;

        while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        return TRUE;
    }

    void *platform_allocate(u64 size, b8 aligned) {
        return malloc(size);
    }

    void platform_free(void *block, b8 aligned) {
        free(block);
    }

    void *platform_zero_memory(void *block, u64 size) {
        return memset(block, 0, size);
    }

    void *platform_copy_memory(void *dest, const void *source, u64 size) {
        return memcpy(dest, source, size);
    }

    void *platform_set_memory(void *dest, i32 value, u64 size) {
        return memset(dest, value, size);
    }

    
void platform_console_write(const char* message, u8 colour){
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Fatal, error, info, debug, trace
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;

    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Fatal, error, info, debug, trace
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);
    
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;

    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return(f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch(msg) {
        case WM_ERASEBKGND:
            // Notify the OS that erasing will be handled by the application to prevent flicker
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            //RECT r;
            //GetClientRect(hwnd, &r);
            //u32 width = r.right - r.left;
            //u32 height = r.bottom - r.top;
            // Todo: fire an even for window resizing
        }break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // key pressed / release
            b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);

        } break;
        case WM_MOUSEMOVE : {

        }break;
        case WM_MOUSEWHEEL : {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if(z_delta != 0) {
                // Flatten the input to an OS independent
                z_delta = (z_delta < 0) ? -1 : 1;
            }
        } break;
    }
}
    
    #endif