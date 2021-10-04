#include <string>

#include "renderer/gl.h"
#include "imgui_gl.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

using namespace ImGuiGl;

static HWND         g_hWnd;
static double       g_Time = 0.0f;
static bool         g_MousePressed[3] = { false, false, false };
static float        g_MouseWheel = 0.0f;
static GLuint       g_FontTexture = 0;
static GLuint       g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;
static bool         g_CursorShown = true;

static void RenderDrawLists(ImDrawData *drawData) {
    ImGuiIO &io = ImGui::GetIO();
    auto fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    auto fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0|| fb_height == 0) return;

    drawData->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    // setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    const float orthoProjection[4][4] = {
            { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
            { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
            { 0.0f,                  0.0f,                  -1.0f, 0.0f },
            {-1.0f,                  1.0f,                   0.0f, 1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &orthoProjection[0][0]);
    glBindVertexArray(g_VaoHandle);

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList *cmdList = drawData->CmdLists[n];
        const ImDrawIdx *idxBufferOffset = nullptr;

        glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmdList->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmdList->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmdList->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmdI = 0; cmdI < cmdList->CmdBuffer.Size; cmdI++) {
            const ImDrawCmd *pcmd = &cmdList->CmdBuffer[cmdI];
            if (pcmd->UserCallback != nullptr) {
                pcmd->UserCallback(cmdList, pcmd);
            } else {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idxBufferOffset);
            }
            idxBufferOffset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glActiveTexture(last_active_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

static void SetClipboardText(void *userData, const char *text) {
    auto charCount = strlen(text);
    auto *object = GlobalAlloc(GMEM_MOVEABLE, charCount * sizeof(char));
    auto *buffer = static_cast<char*>(GlobalLock(object));
    strcpy(buffer, text);

    GlobalUnlock(object);
    OpenClipboard((HWND) userData);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, object);
    CloseClipboard();
}

static const char *GetClipboardText(void *userData) {
    OpenClipboard((HWND) userData);

    HANDLE hData = GetClipboardData(CF_TEXT);
    auto *pszText = static_cast<char*>(GlobalLock(hData));
    auto *str = new char[strlen(pszText) + 1];
    strcpy(str, pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return str;
}

static bool CreateFontsTexture() {
    // build texture atlas
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // upload texture to graphics system
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // store identifier
    io.Fonts->TexID = (void*)(intptr_t)g_FontTexture;

    return true;
}

static void ShowCursorOnce(bool visible) {
    if (visible != g_CursorShown) {
        ShowCursor(visible ? TRUE : FALSE);
        g_CursorShown = visible;
    }
}

bool ImGuiGl::CreateDeviceObjects() {
    const char *vertexShader =
            "#version 330\n"
                    "uniform mat4 ProjMtx;\n"
                    "in vec2 Position;\n"
                    "in vec2 UV;\n"
                    "in vec4 Color;\n"
                    "out vec2 Frag_UV;\n"
                    "out vec4 Frag_Color;\n"
                    "void main()\n"
                    "{\n"
                    "	Frag_UV = UV;\n"
                    "	Frag_Color = Color;\n"
                    "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                    "}\n";

    const char *fragmentShader =
            "#version 330\n"
                    "uniform sampler2D Texture;\n"
                    "in vec2 Frag_UV;\n"
                    "in vec4 Frag_Color;\n"
                    "out vec4 Out_Color;\n"
                    "void main()\n"
                    "{\n"
                    "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
                    "}\n";

    g_ShaderHandle = glCreateProgram();
    g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
    g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(g_VertHandle, 1, &vertexShader, nullptr);
    glShaderSource(g_FragHandle, 1, &fragmentShader, nullptr);
    glCompileShader(g_VertHandle);
    glCompileShader(g_FragHandle);
    glAttachShader(g_ShaderHandle, g_VertHandle);
    glAttachShader(g_ShaderHandle, g_FragHandle);
    glLinkProgram(g_ShaderHandle);

    g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
    g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
    g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
    g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

    glGenBuffers(1, &g_VboHandle);
    glGenBuffers(1, &g_ElementsHandle);

    glGenVertexArrays(1, &g_VaoHandle);
    glBindVertexArray(g_VaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
    glEnableVertexAttribArray((GLuint)g_AttribLocationPosition);
    glEnableVertexAttribArray((GLuint)g_AttribLocationUV);
    glEnableVertexAttribArray((GLuint)g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer((GLuint)g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer((GLuint)g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer((GLuint)g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

    CreateFontsTexture();

    return true;
}

void ImGuiGl::InvalidateDeviceObjects() {
    if (g_VaoHandle != 0u) glDeleteVertexArrays(1, &g_VaoHandle);
    if (g_VboHandle != 0u) glDeleteBuffers(1, &g_VboHandle);
    if (g_ElementsHandle != 0u) glDeleteBuffers(1, &g_ElementsHandle);
    g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

    if (g_ShaderHandle != 0u && g_VertHandle != 0u) glDetachShader(g_ShaderHandle, g_VertHandle);
    if (g_VertHandle != 0u) glDeleteShader(g_VertHandle);
    g_VertHandle = 0;

    if (g_ShaderHandle != 0u && g_FragHandle != 0u) glDetachShader(g_ShaderHandle, g_FragHandle);
    if (g_FragHandle != 0u) glDeleteShader(g_FragHandle);
    g_FragHandle = 0;

    if (g_ShaderHandle != 0u) glDeleteProgram(g_ShaderHandle);
    g_ShaderHandle = 0;

    if (g_FontTexture != 0u)
    {
        glDeleteTextures(1, &g_FontTexture);
        ImGui::GetIO().Fonts->TexID = nullptr;
        g_FontTexture = 0;
    }
}

void ImGuiGl::MouseDownCallback(int button) {
    g_MousePressed[button] = true;
}

void ImGuiGl::ScrollCallback(double xOffset, double yOffset) {
    g_MouseWheel += (float) yOffset;
}

void ImGuiGl::KeyCallback(int key, bool press) {
    ImGuiIO &io = ImGui::GetIO();
    io.KeysDown[key] = press;

    io.KeyCtrl = io.KeysDown[VK_CONTROL];
    io.KeyShift = io.KeysDown[VK_SHIFT];
    io.KeyAlt = io.KeysDown[VK_MENU];
    //io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGuiGl::CharCallback(unsigned int c) {
    ImGuiIO &io = ImGui::GetIO();
    if (c > 0 && c < 0x10000) {
        io.AddInputCharacter((unsigned short)c);
    }
}

bool ImGuiGl::Init(HWND hWnd) {
    g_hWnd = hWnd;

    ImGuiIO &io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = 0x41;
    io.KeyMap[ImGuiKey_C] = 0x43;
    io.KeyMap[ImGuiKey_V] = 0x56;
    io.KeyMap[ImGuiKey_X] = 0x58;
    io.KeyMap[ImGuiKey_Y] = 0x59;
    io.KeyMap[ImGuiKey_Z] = 0x5A;

    io.RenderDrawListsFn = RenderDrawLists;
    io.SetClipboardTextFn = SetClipboardText;
    io.GetClipboardTextFn = GetClipboardText;
    io.ClipboardUserData = hWnd;
    io.ImeWindowHandle = hWnd;

    // use segoeui
    //io.Fonts->AddFontFromFileTTF("segoeui.ttf", 16);

    return true;
}

void ImGuiGl::Shutdown() {
    InvalidateDeviceObjects();
    ImGui::Shutdown();
}

void ImGuiGl::NewFrame() {
    if (g_FontTexture == 0u) CreateDeviceObjects();

    ImGuiIO &io = ImGui::GetIO();

    // setup display size
    RECT area{};
    GetClientRect(g_hWnd, &area);
    int w = area.right, h = area.bottom;

    io.DisplaySize = ImVec2((float)w, (float)h);
    io.DisplayFramebufferScale = ImVec2((float)(w > 0), (float)(h > 0));

    // setup time step
    auto currentTime = (double)timeGetTime() / 1000;
    auto deltaTime = (float)(currentTime - g_Time);
    io.DeltaTime = deltaTime > 0.0 ? deltaTime : 1.0f / 60.0f;
    g_Time = currentTime;

    const int keyMap[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };

    // setup inputs
    POINT cursorPos{};
    GetCursorPos(&cursorPos);
    ScreenToClient(g_hWnd, &cursorPos);
    io.MousePos = ImVec2((float)cursorPos.x, (float)cursorPos.y);

    for (int i = 0; i < 3; i++) {
        io.MouseDown[i] = g_MousePressed[i] || GetAsyncKeyState(keyMap[i]) != 0;
        g_MousePressed[i] = false;
    }

    io.MouseWheel = g_MouseWheel;
    g_MouseWheel = 0.0f;

    ShowCursorOnce(!io.MouseDrawCursor);

    if (g_CursorShown) {
        auto setCursor = ImGui::GetMouseCursor();
        ShowCursorOnce(setCursor != ImGuiMouseCursor_None);

        switch((ImGuiMouseCursor_)setCursor) {
            case ImGuiMouseCursor_Arrow: SetCursor(LoadCursor(NULL, IDC_ARROW)); break;
            case ImGuiMouseCursor_TextInput: SetCursor(LoadCursor(NULL, IDC_IBEAM)); break;
            case ImGuiMouseCursor_Move: SetCursor(LoadCursor(NULL, IDC_SIZEALL)); break;
            case ImGuiMouseCursor_ResizeNS: SetCursor(LoadCursor(NULL, IDC_SIZENS)); break;
            case ImGuiMouseCursor_ResizeEW: SetCursor(LoadCursor(NULL, IDC_SIZEWE)); break;
            case ImGuiMouseCursor_ResizeNESW: SetCursor(LoadCursor(NULL, IDC_SIZENESW)); break;
            case ImGuiMouseCursor_ResizeNWSE: SetCursor(LoadCursor(NULL, IDC_SIZENWSE)); break;
        }
    }

    // start the frame
    ImGui::NewFrame();
}
