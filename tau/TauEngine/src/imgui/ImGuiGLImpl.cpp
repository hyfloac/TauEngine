#include <imgui/ImGuiGLImpl.hpp>

bool ImGui_ImplGL_Init() noexcept { return false; }

void ImGui_ImplGL_Render(ImDrawData* drawData) noexcept { }

void ImGui_ImplGL_Destroy() noexcept { }

// #pragma warning(push, 0)
// #include <imgui/ImGuiGLImpl.hpp>
// // #include <gl/GLProgram.hpp>
// // #include <gl/GLShader.hpp>
// #include "shader/IShaderProgram.hpp"
// #include "shader/IShader.hpp"
// #include "shader/IUniform.hpp"
// #include <file/FileUtils.hpp>
// #pragma warning(pop)
//
// static Ref<IShaderProgram> _shader;
// // static GLProgram* _program;
// // static GLShader*  _vertexShader;
// // static GLShader*  _fragmentShader;
//
// static i32 projectionMatrixUniformLoc = -1;
// static i32 textureSamplerUniformLoc = -1;
//
// // static GLuint fontTexture = static_cast<GLuint>(-1);
// // static GLuint vboHandle = static_cast<GLuint>(-1);
// // static GLuint elementsHandle = static_cast<GLuint>(-1);
//
// static void loadFontsAndTextures() noexcept;
//
// bool ImGui_ImplGL_Init() noexcept
// {
//     // _shader = IShaderProgram::create()
//     // _program = new GLProgram;
//     // _vertexShader = new GLShader(ShaderType::VERTEX, "exe|resources/imgui_vertex.glsl", _program);
//     // _fragmentShader = new GLShader(ShaderType::FRAGMENT, "exe|resources/imgui_fragment.glsl", _program);
//
//     const char* vertexSrc = R"(
//     #version 330 core
//
//     layout(location = 0) in vec2 pos;
//     layout(location = 1) in vec2 uv;
//     layout(location = 2) in vec4 color;
//
//     out vec2 f_uv;
//     out vec4 f_color;
//
//     uniform mat4 projectionMatrix;
//
//     void main(void)
//     {
//         f_uv = uv;
//         f_color = color;
//         gl_Position = projectionMatrix * vec4(pos.xy, 0.0, 1.0);
//     }
//     )";
//
//     const char* fragmentSrc = R"(
//     #version 330 core
//
//     in vec2 f_uv;
//     in vec4 f_color;
//
//     layout(location = 0) out vec4 fragColor;
//
//     uniform sampler2D textureSampler;
//
//     void main(void)
//     {
//         fragColor = f_color * texture(textureSampler, f_uv.st);
//     }
//     )";
//
//     size_t size = std::strlen(vertexSrc);
//     char* src = reinterpret_cast<char*>(calloc(size + 1, 1));
//     if(!src) { return false; }
//     (void) std::memcpy(src, vertexSrc, size);
//
//     // loadResourceFile(IDR_IMGUI_VERTEX_SHADER, TEXT_FILE, &size, &src);
//     _vertexShader->loadShader(src);
//
//     // free(src);
//
//     size = std::strlen(fragmentSrc);
//     src = reinterpret_cast<char*>(calloc(size + 1, 1));
//     if(!src) { return false; }
//     (void) std::memcpy(src, fragmentSrc, size);
//
//     // loadResourceFile(IDR_IMGUI_FRAGMENT_SHADER, TEXT_FILE, &size, &src);
//     _fragmentShader->loadShader(src);
//
//     // free(src);
//
//     _program->linkAndValidate();
//
//     projectionMatrixUniformLoc = _vertexShader->createUniform("projectionMatrix");
//     textureSamplerUniformLoc = _fragmentShader->createUniform("textureSampler");
//
//     glGenBuffers(1, &vboHandle);
//     glGenBuffers(1, &elementsHandle);
//
//     loadFontsAndTextures();
//
//     return true;
// }
//
// void ImGui_ImplGL_Render(ImDrawData* drawData) noexcept
// {
//     const GLsizei fbWidth  = static_cast<GLsizei>(drawData->DisplaySize.x * drawData->FramebufferScale.x);
//     const GLsizei fbHeight = static_cast<GLsizei>(drawData->DisplaySize.y * drawData->FramebufferScale.y);
//     if(fbWidth <= 0 || fbHeight <= 0) { return; }
//
//     const bool clipOriginLowerLeft = true;
//
//
//     glEnable(GL_BLEND);
//     glBlendEquation(GL_FUNC_ADD);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//     glDisable(GL_CULL_FACE);
//     glDisable(GL_DEPTH_TEST);
//
//     glEnable(GL_SCISSOR_TEST);
//
//     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//
//     glViewport(0, 0, fbWidth, fbHeight);
//
//     const float L = drawData->DisplayPos.x;
//     const float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
//     // const float xs = drawData->DisplaySize.x;
//
//     const float T = drawData->DisplayPos.y;
//     const float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
//     // const float ys = drawData->DisplaySize.y;
//
//     const float ortho_projection[4][4] = 
//     {
//         { 2.0f/(R-L),  0.0f,         0.0f, 0.0f },
//         { 0.0f,        2.0f/(T-B),   0.0f, 0.0f },
//         { 0.0f,        0.0f,        -1.0f, 0.0f },
//         { (R+L)/(L-R), (T+B)/(B-T),  0.0f, 1.0f }
//     };
//     
//     // const float ortho_projection0[4][4] = 
//     // {
//     //     { 2.0f/(xs),   0.0f,         0.0f, 0.0f },
//     //     { 0.0f,        2.0f/(-ys),   0.0f, 0.0f },
//     //     { 0.0f,        0.0f,        -1.0f, 0.0f },
//     //     { (R+L)/(-xs), (T+B)/(ys),   0.0f, 1.0f }
//     // };
//
//
//     glActiveTexture(GL_TEXTURE0);
//     _program->activate();
//     glUniformMatrix4fv(projectionMatrixUniformLoc, 1, GL_FALSE, &ortho_projection[0][0]);
//     _fragmentShader->setUniform(textureSamplerUniformLoc, 0);
//     glBindSampler(0, 0);
//
//     GLuint vaoHandle = 0;
//     glGenVertexArrays(1, &vaoHandle);
//     glBindVertexArray(vaoHandle);
//     glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(0, 2, GL_FLOAT,         GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, pos)));
//     glVertexAttribPointer(1, 2, GL_FLOAT,         GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, uv)));
//     glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(ImDrawVert), reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, col)));
//
//     const ImVec2 clipOff = drawData->DisplayPos;
//     const ImVec2 clipScale = drawData->FramebufferScale;
//
//     for(u32 i = 0; i < static_cast<u32>(drawData->CmdListsCount); ++i)
//     {
//         const ImDrawList* cmdList = drawData->CmdLists[i];
//         size_t idxBufferOffset = 0;
//
//         glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
//         glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmdList->VtxBuffer.Size * sizeof(ImDrawVert)), reinterpret_cast<const GLvoid*>(cmdList->VtxBuffer.Data), GL_STREAM_DRAW);
//
//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsHandle);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmdList->IdxBuffer.Size * sizeof(ImDrawIdx)), reinterpret_cast<const GLvoid*>(cmdList->IdxBuffer.Data), GL_STREAM_DRAW);
//
//         for(u32 j = 0; j < static_cast<u32>(cmdList->CmdBuffer.Size); ++j)
//         {
//             const ImDrawCmd* cmd = &(cmdList->CmdBuffer[j]);
//
//             if(cmd->UserCallback)
//             {
//                 cmd->UserCallback(cmdList, cmd);
//             }
//             else
//             {
//                 ImVec4 clipRect;
//                 clipRect.x = (cmd->ClipRect.x - clipOff.x) * clipScale.x;
//                 clipRect.y = (cmd->ClipRect.y - clipOff.y) * clipScale.y;
//                 clipRect.z = (cmd->ClipRect.z - clipOff.x) * clipScale.x;
//                 clipRect.w = (cmd->ClipRect.w - clipOff.y) * clipScale.y;
//
//                 if(clipRect.x < fbWidth  && 
//                    clipRect.y < fbHeight && 
//                    clipRect.z >= 0.0f    && 
//                    clipRect.w >= 0.0f)
//                 {
//                     if(clipOriginLowerLeft)
//                     {
//                         glScissor(static_cast<i32>(clipRect.x), static_cast<i32>(fbHeight - clipRect.w), static_cast<i32>(clipRect.z - clipRect.x), static_cast<i32>(clipRect.w - clipRect.y));
//                     }
//                     else
//                     {
//                         glScissor(static_cast<i32>(clipRect.x), static_cast<i32>(clipRect.y), static_cast<i32>(clipRect.z), static_cast<i32>(clipRect.w));
//                     }
//
//                     glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<intptr_t>(cmd->TextureId)));
//                     glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cmd->ElemCount), sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, reinterpret_cast<void*>(idxBufferOffset));
//                 }
//             }
//
//             idxBufferOffset += cmd->ElemCount * sizeof(ImDrawIdx);
//         }
//     }
//
//     glBindTexture(GL_TEXTURE_2D, 0);
//
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//     glDisableVertexAttribArray(2);
//     glDisableVertexAttribArray(1);
//     glDisableVertexAttribArray(0);
//
//     glBindVertexArray(0);
//     glDeleteVertexArrays(1, &vaoHandle);
//
//     GLProgram::deactivate();
// }
//
// void ImGui_ImplGL_Destroy() noexcept
// {
//     delete _fragmentShader;
//     delete _vertexShader;
//     delete _program;
// }
//
// static void loadFontsAndTextures() noexcept
// {
//     ImGuiIO& io = ImGui::GetIO();
//
//     u8* pixels;
//     i32 width;
//     i32 height;
//
//     io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
//
//     glGenTextures(1, &fontTexture);
//
//     glBindTexture(GL_TEXTURE_2D, fontTexture);
//
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//     glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//
//     io.Fonts->TexID = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(fontTexture));
//
//     glBindTexture(GL_TEXTURE_2D, 0);
// }
