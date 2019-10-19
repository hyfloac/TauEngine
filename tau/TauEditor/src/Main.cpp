// #pragma warning(push, 0)
// #include <spdlog/sinks/stdout_color_sinks.h>
// #include <imgui/imgui.h>
// #include <Utils.hpp>
// #include <GL/glew.h>
// #include <GL/wglew.h>
// #include <TauEngine.hpp>
// #include <system/Window.hpp>
// #include <gl/GLUtils.hpp>
// #include <shader/GLShader.hpp>
// #include <maths/Matrix4x4f.hpp>
// #include <maths/Vector3f.hpp>
// #include <maths/Maths.hpp>
// #include <model/OBJLoader.hpp>
// #include <model/RenderableObject.hpp>
// #include <console/ConsoleHandler.hpp>
// #include <texture/FITextureLoader.hpp>
// #include <CoreFuncs.h>
// #include <RenderingPipeline.hpp>
// #include <TextHandler.hpp>
// #include <Windows.h>
// #include <Camera.hpp>
// #include <events/Event.hpp>
// #include <events/WindowEvent.hpp>
// #include "VFS.hpp"
// #include "Win32File.hpp"
// #pragma warning(pop)
//
// std::shared_ptr<spdlog::logger> clientLogger;
//
// bool setupDebugCallback() noexcept;
// void GLAPIENTRY openGLDebugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept;
// void onMouseMove(const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, Window* window) noexcept;
//
// void onWindowEvent(void*, WindowEvent& e) noexcept;
//
// i32 debugCommand(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept;
//
// void update(float fixedDelta);
// void render(float delta);
//
// void __cdecl setupParams(RenderingPipeline&, Window&, void*) noexcept;
//
// struct RenderData final
// {
//     Window& window;
//     std::vector<Ref<RenderableObject>>& renderableObjects;
//     GLProgram* glProgram;
//     IShader* vertexShader;
//     IShader* pixelShader;
//     Matrix4x4f perspectiveProjMatrix;
//     Matrix4x4f& viewMatrix;
//     Matrix4x4f modelMatrix;
//     ITexture* texID;
//     ITexture* overlayID;
//
//     i32 compoundMatrixUniformLoc;
//     i32 projectionMatrixUniformLoc;
//     i32 cameraViewMatrixUniformLoc;
//     i32 modelViewMatrixUniformLoc;
//     i32 samplerLoc;
//     i32 overlayLoc;
//
//     Camera camera;
//     RenderingPipeline* rp;
//     TextHandler& th;
//     Matrix4x4f orthoProjMatrix;
// };
//
// static RenderData* renderData;
//
// static ConsoleHandler ch;
//
// static GLProgram* baseProgram;
//
// class StartupRenderer
// {
// private:
//     struct TextContainer
//     {
//         const char* str;
//         int yPos;
//     };
//     std::vector<TextContainer> texts;
//     int index;
// public:
//     StartupRenderer() : index(0) { }
//
//     void addText(const char* str)
//     {
//         texts.push_back({ str, (++index) * 15 });
//     }
//
//     void render(Window& window, TextHandler& th, const Matrix4x4f& ortho)
//     {
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//         for(TextContainer tc : texts)
//         {
//             th.renderText(tc.str, 0.0f, static_cast<float>(window.height() - tc.yPos), 0.35f, Vector3f(1.0f, 1.0f, 1.0f), ortho);
//         }
//
//         window.swapBuffers();
//     }
// };
//
// int EXPORT initGame() noexcept
// {
// #ifndef TAU_DLL_LINK
//     tauMain();
// #endif
//
//     clientLogger = spdlog::stdout_color_mt("TauEditor");
//     clientLogger->set_level(spdlog::level::trace);
//     clientLogger->set_pattern("%^[%H:%M:%S:%e] [%n] [%l]%$: %v");
//     
//     clientLogger->info("Starting Tau Engine Editor.");
//
//     VFS::Instance().mountDynamic("TERes", "E:/TauEngine/tau/TauEditor/resources", Win32FileLoader::Instance());
//
//     Window window(800, 600, "Tau Editor");
// #if !defined(TAU_PRODUCTION)
//     ContextSettings& contextSettings = window.contextSettings();
//     contextSettings.debug = true;
// #endif
//     window.createWindow();
//
//     clientLogger->debug("Window Created.");
//
//     window.showWindow();
//
//     clientLogger->debug("Window Active & Visible.");
//
//     if(!window.createContext())
//     {
//         return -1;
//     }
//
//     clientLogger->debug("Graphics Context Created.");
//
//     window.makeContextCurrent();
//
//     if(WGLEW_EXT_swap_control)
//     {
//         wglSwapIntervalEXT(0);
//     }
//
//     clientLogger->debug("Graphics Context Made Current.");
//
//     window.setEventHandler(onWindowEvent);
//
//     clientLogger->debug("Window Event Handlers Setup.");
//
//     setupDebugCallback();
//
//     clientLogger->debug("OpenGL Debug Error Callback Setup.");
//
//     // TextHandler th(R"(E:\TauEngine\tau\TauEditor\resources\TextVertexShader.glsl)", R"(E:\TauEngine\tau\TauEditor\resources\TextFragmentShader.glsl)");
//     TextHandler th("|TERes/TextVertexShader.glsl", "|TERes/TextFragmentShader.glsl");
//     FT_Error thError = th.init();
//     if(thError)
//     {
//         clientLogger->error("FreeType Initialization Error: {0}", thError);
//     }
//     thError = th.loadTTFFile("|TERes/MonoConsole.ttf");
//     if(thError)
//     {
//         clientLogger->error("FreeType TTF Loading Error: {0}", thError);
//     }
//     th.generateBitmapCharacters();
//     th.finishLoad();
//
//     const Matrix4f ortho = Matrix4f::orthographic(0.0f, static_cast<float>(window.width()), 0.0f, static_cast<float>(window.height()));
//
//     glEnable(GL_BLEND);
//     // glDisable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//     StartupRenderer* sr = new StartupRenderer;
//
// #ifndef _PRODUCTION
//   #define RENDER_TEXT(__STR) sr->addText(__STR); sr->render(window, th, ortho);
// #else
//   #define RENDER_TEXT(__STR)
// #endif
//
//     RENDER_TEXT("Orthographic Matrix Created.");
//
//     enableGLDepthTest();
//
//     RENDER_TEXT("Depth Test Enabled.");
//
//     enableGLCullFace();
//     glCullFace(GL_BACK);
//
//     RENDER_TEXT("Back Face Culling Enabled.");
//
//     glFrontFace(GL_CW);
//
//     RENDER_TEXT("Triangle Unwinding Setup.");
//
//     ShowCursor(FALSE);
//
//     RENDER_TEXT("Cursor Disabled.");
//
//     setGLBlend(true);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     RENDER_TEXT("Blending Enabled.");
//
//     objl::Loader loader;
//
//     bool loaded = loader.loadFile("|TERes/nanosuit/nanosuit.obj");
//
//     RENDER_TEXT("NanoSuit OBJ Loaded.");
//
//     std::vector<std::shared_ptr<RenderableObject>> objects;
//
//     if(loaded)
//     {
//         for(const objl::Mesh& mesh : loader.meshes())
//         {
//             objects.emplace_back(std::make_shared<RenderableObject>(mesh));
//         }
//     }
//
//     RENDER_TEXT("NanoSuit Meshes Loaded.");
//
//     GLProgram* glProgram = new GLProgram;
//     IShader* vertexShader = new GLShader(ShaderType::VERTEX, "|TERes/SimpleVertexShader.glsl", glProgram);
//     IShader* pixelShader = new GLShader(ShaderType::PIXEL, "|TERes/SimpleFragmentShader.glsl", glProgram);
//
//     baseProgram = glProgram;
//
//     RENDER_TEXT("Shaders Instantiated.");
//
//     vertexShader->loadShader();
//
//     RENDER_TEXT("Vertex Shader Loaded.");
//
//     pixelShader->loadShader();
//
//     RENDER_TEXT("Pixel Shader Loaded.");
//
//     glProgram->linkAndValidate();
//
//     RENDER_TEXT("Shaders Linked & Validated.");
//
//     Matrix4x4f perspectiveProjMatrix = Matrix4x4f::perspective(DEG_2_RAD(90), (float) window.width() / window.height(), 0.0001f, 1000.0f);
//     // Matrix4x4f perspectiveProjMatrix = Matrix4x4f::orthographic(window.width(), 0.0f, window.height(), 0.0f, 0.001f, 1000.0f);
//     // Matrix4x4f perspectiveProjMatrix = Matrix4x4f::orthographic(0.0f, window.width(), 0.0f, window.height(), 0.001f, 1000.0f);
//     // Matrix4x4f perspectiveProjMatrix = Matrix4x4f::orthographic(window.width(), 0.0f, 0.0f, window.height(), 0.001f, 1000.0f);
//     // Matrix4x4f perspectiveProjMatrix = Matrix4x4f::infDepthPerspective(DEG_2_RAD(90), 800.0f / 600.0f);
//
//     RENDER_TEXT("Perspective Matrix Created.");
//
//     Matrix4x4f viewMatrix;
//     Matrix4x4f modelMatrix;
//
//     viewMatrix.view({ 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f);
//     modelMatrix[3][2] = 5.0f;
//     modelMatrix.setTranslation({0.0f, -0.1f, 0.0f});
//
//     RENDER_TEXT("ViewMatrix & ModelMatrix Created.");
//
//     const i32 compoundMatrixUniformLoc = vertexShader->createUniform("compoundMatrix");
//     const i32 projectionMatrixUniformLoc = vertexShader->createUniform("projectionMatrix");
//     const i32 cameraViewMatrixUniformLoc = vertexShader->createUniform("cameraViewMatrix");
//     const i32 modelViewMatrixUniformLoc  = vertexShader->createUniform("modelViewMatrix");
//     // const i32 projectionMatrixUniformLoc = 0;
//     // const i32 cameraViewMatrixUniformLoc = 0;
//     // const i32 modelViewMatrixUniformLoc  = 0;
//     const i32 samplerLoc = pixelShader->createUniform("textureSampler");
//     const i32 overlayLoc = pixelShader->createUniform("textureOverlay");
//
//     RENDER_TEXT("Uniform Locations Loaded.");
//
//     TextureLoadError texError;
//
//     ITexture* textureID = loadTexture("|TERes/TestTexture.png", true, &texError);
//
//     if(textureID == null)
//     {
//         clientLogger->error("Texture Failed to Load");
//     }
//
//     if(texError != TextureLoadError::NONE)
//     {
//         clientLogger->error("Error Code: {0}", (u32) texError);
//     }
//
//     ITexture* overlayID = loadTexture("|TERes/Overlay.png", true, &texError);
//
//     if(overlayID == null)
//     {
//         clientLogger->error("Overlay Texture Failed to Load");
//     }
//
//     if(texError != TextureLoadError::NONE)
//     {
//         clientLogger->error("Error Code: {0}", static_cast<u32>(texError));
//     }
//
//     RENDER_TEXT("Textures Loaded.");
//
//     // ImGui::CreateContext();
//     //
//     // ImGui_ImplGL_Init();
//
//     RenderingPipeline rp(window, setupParams, nullptr, true);
//
//     RenderData rd = { window, objects, glProgram, vertexShader, pixelShader, perspectiveProjMatrix, viewMatrix, modelMatrix, textureID, overlayID, compoundMatrixUniformLoc, projectionMatrixUniformLoc, cameraViewMatrixUniformLoc, modelViewMatrixUniformLoc, samplerLoc, overlayLoc, Camera(), &rp, th, ortho };
//
//     renderData = &rd;
//
//     RENDER_TEXT("Render Data Created.");
//
//     ch.addCommand(dc::printBooleanAliasesName, dc::printBooleanAliasesUsage, dc::printBooleanAliases);
//     ch.addCommand(dc::addBooleanAliasName, dc::addBooleanAliasUsage, dc::addBooleanAlias);
//     ch.addCommand(dc::removeBooleanAliasName, dc::removeBooleanAliasUsage, dc::removeBooleanAlias);
//     ch.addCommand(dc::exitName, dc::exitUsage, dc::exit);
//     ch.addCommand(dc::parseIntName, dc::parseIntUsage, dc::parseInt);
//     ch.addCommand("debug", "debug", debugCommand);
//
//     RENDER_TEXT("Commands Setup.");
//
//     // ImGui::NewFrame();
//     //
//     // {
//     //     ImGui::Begin("Test Window");
//     //     
//     //     ImGui::Text("Random Text");
//     //     ImGui::Text("Random Float: %f", 3.141592653589793);
//     //     
//     //     ImGui::End();
//     //     
//     //     ImGui::Render();
//     // }
//
//
//     glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
//
//     RENDER_TEXT("Background Color Setup.");
//     RENDER_TEXT("Setup Complete.");
//     RENDER_TEXT("Activating Game Loop.");
//     delete sr;
//
//     tauGameLoop(32, update, render, [](u32 ups, u32 fps)
//     {
//         char buf[256];
//         snprintf(buf, 256, "Tau Editor: UPS / FPS: %u / %u", ups, fps);
//         renderData->window.setTitle(buf);
//     });
//     
//     // ImGui_ImplGL_Destroy();
//
//     return 0;
// }
//
// int EXPORT finalizeGame() noexcept
// {
//     stopDebugOutput();
//     // ImGui_ImplGL_Destroy();
//
//     return 0;
// }
//
// GameLoopData EXPORT getGameLoopData() noexcept
// {
//     return { 32, update, render, 1 };
// }
//
// i32 debugCommand(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
// {
//     UNUSED4(commandName, args, argCount, consoleHandler);
//
//     clientLogger->info("Debug!!!");
//
//     return 0;
// }
//
// static bool lockMouse = true;
// static bool activeWindow = true;
//
// static bool inCommand = false;
//
// static volatile i32 changeCursorState = 0;
//
// static void moveCamera(float fixedDelta)
// {
//     if(inCommand) { return; }
//
//     renderData->camera.update(fixedDelta);
//
//     // const float offset = (fixedDelta / 1000.0f) * 1.0f;
//     //
//     // float forwardOffset = 0.0f;
//     // float perpOffset = 0.0f;
//     //
//     // if(GetKeyState(0x44) & 0x8000) /* d */ { perpOffset += offset; }
//     // if(GetKeyState(0x41) & 0x8000) /* a */ { perpOffset -= offset; }
//     // if(GetKeyState(0x57) & 0x8000) /* w */ { forwardOffset += offset; }
//     // if(GetKeyState(0x53) & 0x8000) /* s */ { forwardOffset -= offset; }
//     // if(GetKeyState(VK_SPACE) & 0x8000)     { pos.y() += offset; }
//     // if(GetKeyState(VK_SHIFT) & 0x8000)     { pos.y() -= offset; }
//     if(GetKeyState(0x45) & 0x8000) /* e */ { renderData->camera.pos(Vector3f(0.0f, 0.0f, 0.0f)); }
//     if(GetKeyState(0x51) & 0x8000) /* q */ { renderData->camera.set(0.0f, 0.0f); }
//     if(GetKeyState(VK_INSERT) & 0x8000)    { glWireframe(); }
//     if(GetKeyState(VK_HOME)   & 0x8000)    { glFilled(); }
//     if(GetKeyState(VK_DELETE) & 0x8000)    { glVertices(); }
//
//     // const float pitchRad = DEG_2_RAD_F(pitch);
//     // const float yawRad   = DEG_2_RAD_F(yaw);
//     //
//     // SinCos<float> pitchSC = fastSinCosR(pitchRad);
//     // SinCos<float> yawSC = fastSinCosR(yawRad);
//     //
//     // if(forwardOffset != 0.0f)
//     // {
//     //     pos.x() -= forwardOffset * yawSC.sin;
//     //     pos.z() += forwardOffset * yawSC.cos;
//     // }
//     //
//     // if(perpOffset != 0.0f)
//     // {
//     //     pos.z() += perpOffset * yawSC.sin;
//     //     pos.x() += perpOffset * yawSC.cos;
//     // }
//     //
//     // pitchSC.negate();
//     // yawSC.negate();
//     //
//     // renderData->viewMatrix.fps(pos, pitchSC, yawSC);
//     // renderData->viewMatrix.fps(pos, pitchRad, yawRad);
// }
//
// static void rotateCamera(float delta)
// {
//     if(lockMouse && activeWindow && !inCommand)
//     {
//         i32 mouseX, mouseY;
//         renderData->window.getMousePos(mouseX, mouseY);
//
//         const u32 midPointX = renderData->window.width() / 2;
//         const u32 midPointY = renderData->window.height() / 2;
//
//         const i32 xDif = midPointX - mouseX;
//         const i32 yDif = midPointY - mouseY;
//
//         if(xDif != 0 && yDif != 0)
//         {
//             renderData->camera.updateRotation(delta, xDif, yDif);
//             renderData->window.setMousePos(renderData->window.width() / 2, renderData->window.height() / 2);
//         }
//     }
// }
//
// static void rotCube()
// {
//     if(inCommand) { return; }
//
//     static Vector3f rot(0.0f, 0.0f, 0.0f);
//
//     if(GetKeyState(VK_LEFT)  & 0x8000) { rot.y() -= 0.03f; }
//     if(GetKeyState(VK_UP)    & 0x8000) { rot.x() -= 0.03f; }
//     if(GetKeyState(VK_RIGHT) & 0x8000) { rot.y() += 0.03f; }
//     if(GetKeyState(VK_DOWN)  & 0x8000) { rot.x() += 0.03f; }
//
//     renderData->modelMatrix.transformation(rot, 0.1f);
// }
//
// void update(float fixedDelta)
// {
//     rotCube();
//     // rotateCamera(fixedDelta);
//     moveCamera(fixedDelta);
// }
//
// void render(float delta)
// {
//     // printf("Elapsed: %f\n", delta);
//     RenderingPipeline* rp = renderData->rp;
//     const Camera& cam = renderData->camera;
//
//     if(changeCursorState == 1)
//     {
//         ShowCursor(TRUE);
//     }
//     else if(changeCursorState == -1)
//     {
//         ShowCursor(FALSE);
//     }
//     changeCursorState = 0;
//     
//     rotateCamera(delta);
//
//     rp->pushGLClearBuffers(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     
//     rp->pushActivateShaderProgram(renderData->glProgram->programId());
//
//     // Matrix4x4f tmpMat = renderData->perspectiveProjMatrix;
//     // tmpMat.mulSIMD_SSE3(renderData->viewMatrix);
//     // tmpMat.mulSIMD_SSE3(renderData->modelMatrix);
//     Matrix4x4f tmpMat = renderData->camera.viewMatrix();
//     tmpMat.mulSIMD_SSE3(renderData->perspectiveProjMatrix);
//
//     rp->pushLoadUni(renderData->compoundMatrixUniformLoc, tmpMat);
//     rp->pushLoadUni(renderData->projectionMatrixUniformLoc, renderData->perspectiveProjMatrix);
//     rp->pushLoadUni(renderData->cameraViewMatrixUniformLoc, renderData->camera.viewMatrix());
//     rp->pushLoadUni(renderData->modelViewMatrixUniformLoc, renderData->modelMatrix);
//     rp->pushLoadUni(renderData->samplerLoc, 0);
//     rp->pushLoadUni(renderData->overlayLoc, 1);
//     
//     rp->pushBindTexture(renderData->texID, 0);
//     rp->pushBindTexture(renderData->overlayID, 1);
//     for(const Ref<RenderableObject>& ro : renderData->renderableObjects)
//     {
//         ro->preRender(*rp);
//         ro->render(*rp);
//         ro->postRender(*rp);
//     }
//     rp->pushUnbindTexture(renderData->texID, 0);
//     rp->pushUnbindTexture(renderData->overlayID, 1);
//     rp->pushActivateShaderProgram(0);
//     
//     char posString[128];
//     snprintf(posString, 128, "Pos(%.4f, %.4f, %.4f)", cam.pos().x(), cam.pos().y(), cam.pos().z());
//     rp->pushRenderText(&renderData->th, posString, 0.0f, static_cast<float>(renderData->window.height() - 15), 0.35f, Vector3f(0.9f, 0.9f, 0.2f), renderData->orthoProjMatrix);
//     snprintf(posString, 128, "Camera Pitch: %.4f, Camera Yaw: %.4f", cam.pitch(), cam.yaw());
//     rp->pushRenderText(&renderData->th, posString, 0.0f, static_cast<float>(renderData->window.height() - 30), 0.35f, Vector3f(0.9f, 0.9f, 0.2f), renderData->orthoProjMatrix);
//     snprintf(posString, 128, "Window: %dx%d", renderData->window.width(), renderData->window.height());
//     rp->pushRenderText(&renderData->th, posString, 0.0f, static_cast<float>(renderData->window.height() - 45), 0.35f, Vector3f(0.9f, 0.9f, 0.2f), renderData->orthoProjMatrix);
//
//     // rp->pushImGuiRender(ImGui::GetDrawData());
//
//     rp->pushFinishRender();
//     // ImGui_ImplGL_Render(ImGui::GetDrawData());
//
//     // renderData->window.swapBuffers();
// }
//
// bool setupDebugCallback() noexcept
// {
// #ifdef _DEBUG
//     setupDebugMessageCallback(openGLDebugErrorCallback, null, true);
//     return true;
// #else
//     return false;
// #endif
// }
//
// void onMouseMove(const MouseFlags mouseFlags, const u32 xPos, const u32 yPos, Window* window) noexcept
// {
//     UNUSED4(mouseFlags, xPos, yPos, window);
// }
//
// void GLAPIENTRY openGLDebugErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) noexcept
// {
//     UNUSED2(length, userParam);
//     UNUSED2(id, message);
//
// #define STR_CASE(__ENUM, __STR) case __ENUM: str = __STR; break
// #define DEFAULT_CASE default: str = "Unknown"; break
//
//     clientLogger->error("--OpenGL Error Callback--");
//     clientLogger->error("Message: {0}", message);
//     const char* str;
//     switch(source)
//     {
//         STR_CASE(GL_DEBUG_SOURCE_API, "API");
//         STR_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System");
//         STR_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler");
//         STR_CASE(GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party");
//         STR_CASE(GL_DEBUG_SOURCE_APPLICATION, "Application");
//         STR_CASE(GL_DEBUG_SOURCE_OTHER, "Other");
//         DEFAULT_CASE;
//     }
//     clientLogger->error("Source [0x{0:X}] {1}", source, str);
//
//     switch(type)
//     {
//         STR_CASE(GL_DEBUG_TYPE_ERROR, "Error");
//         STR_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior");
//         STR_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behavior");
//         STR_CASE(GL_DEBUG_TYPE_PORTABILITY, "Portability");
//         STR_CASE(GL_DEBUG_TYPE_PERFORMANCE, "Performance");
//         STR_CASE(GL_DEBUG_TYPE_OTHER, "Other");
//         STR_CASE(GL_DEBUG_TYPE_MARKER, "Marker");
//         DEFAULT_CASE;
//     }
//     clientLogger->error("Type: [0x{0:X}] {1}", type, str);
//     clientLogger->error("Id: 0x{0:X} // {0:d}", id);
//
//     switch(severity)
//     {
//         STR_CASE(GL_DEBUG_SEVERITY_MEDIUM, "Medium");
//         STR_CASE(GL_DEBUG_SEVERITY_HIGH, "High");
//         STR_CASE(GL_DEBUG_SEVERITY_LOW, "Low");
//         STR_CASE(GL_DEBUG_SEVERITY_NOTIFICATION, "Notification");
//         DEFAULT_CASE;
//     }
//     clientLogger->error("Severity: [0x{0:X}] {1}", severity, str);
//     clientLogger->error("-------------------------");
// #if _DEBUG && 0
//     if(severity != GL_DEBUG_SEVERITY_NOTIFICATION && renderData && renderData->rp)
//     {
//         FILE* fileData = fopen("CommandBuffer.txt", "w");
//         renderData->rp->dumpCommandBufferToFile(fileData);
//         fclose(fileData);
//
//         // char* fileName = new char[1024];
//         // GetFullPathNameA("CommandBuffer.txt", 1024, fileName, NULL);
//         // fprintf(stderr, "Command Dump: %s\n", fileName);
//         // delete[] fileName;
//         __debugbreak();
//     }
// #else
//     // getchar();
// #endif
//
// #undef DEFAULT_CASE
// #undef STR_CASE
// }
//
// static std::string commandBuffer;
//
// bool onKeyPress(WindowKeyEvent& e) noexcept
// {
//     if(e.event() == KeyboardEvent::KE_KEY_PRESSED)
//     {
//         // clientLogger->debug("Key Pressed: {}", key);
//         if(e.key() == Key::Esc) // Esc
//         {
//             clientLogger->debug("Escape Pressed, Exiting.");
//             tauExit(0);
//         }
//         else if(e.key() == Key::Alt) // Alt
//         {
//             lockMouse = !lockMouse;
//             if(lockMouse)
//             {
//                 clientLogger->debug("Locking Mouse Position.");
//
//                 changeCursorState = -1;
//             }
//             else
//             {
//                 clientLogger->debug("Unlocking Mouse.");
//
//                 changeCursorState = 1;
//             }
//         }
//         else if(e.key() == Key::Enter) // Enter
//         {
//             printf("\n");
//             ch.runCommand(commandBuffer.c_str());
//             commandBuffer = "";
//             inCommand = false;
//             lockMouse = false;
//             changeCursorState = -1;
//             // setEngineLoggerLevel(spdlog::level::level_enum::debug);
//             clientLogger->set_level(spdlog::level::level_enum::debug);
//         }
//         else if(e.key() == Key::BackSpace) // Backspace
//         {
//             if(commandBuffer.length() == 0)
//             {
//                 printf("\n");
//                 inCommand = false;
//                 // setEngineLoggerLevel(spdlog::level::level_enum::debug);
//                 clientLogger->set_level(spdlog::level::level_enum::debug);
//             }
//             else
//             {
//                 commandBuffer.pop_back();
//                 printf("\r/%s ", commandBuffer.c_str());
//             }
//         }
//     }
//     else if(e.event() == KeyboardEvent::KE_KEY_RELEASED)
//     {
//         // clientLogger->debug("Key Released: {}", key);
//     }
//     else if(e.event() == KeyboardEvent::KE_KEY_HELD)
//     {
//         // clientLogger->debug("Key Held: {}", key);
//     }
//
//     return false;
// }
//
// bool onCharPress(WindowAsciiKeyEvent& e) noexcept
// {
//     // Window* window = e.window();
//     // wchar_t _c16 = e.wideChar();
//     char c = e.c();
//
//     // clientLogger->debug("Character Pressed: '{}'", c);
//
//     if(inCommand)
//     {
//         commandBuffer += c;
//
//         printf("\r/%s", commandBuffer.c_str());
//     }
//     else if(c == '/')
//     {
//         inCommand = true;
//         lockMouse = false;
//         changeCursorState = 1;
//         // setEngineLoggerLevel(spdlog::level::level_enum::off);
//         clientLogger->set_level(spdlog::level::level_enum::off);
//         printf("/");
//     }
//     else if(c == '[' || c == ']')
//     {
//         renderData->rp->takeControlOfContext();
//
//         delete renderData->pixelShader;
//         delete renderData->vertexShader;
//         delete renderData->glProgram;
//
//         GLProgram* glProgram = new GLProgram;
//         GLShader* vertexShader;
//         GLShader* pixelShader;
//         if(c == '[')
//         {
//             vertexShader = new GLShader(ShaderType::VERTEX, "|TERes/DebugVertexShader.glsl", glProgram);
//             pixelShader = new GLShader(ShaderType::PIXEL, "|TERes/DebugFragmentShader.glsl", glProgram);
//         }
//         else
//         {
//             vertexShader = new GLShader(ShaderType::VERTEX, "|TERes/SimpleVertexShader.glsl", glProgram);
//             pixelShader = new GLShader(ShaderType::PIXEL, "|TERes/SimpleFragmentShader.glsl", glProgram);
//         }
//
//         baseProgram = glProgram;
//
//         vertexShader->loadShader();
//         pixelShader->loadShader();
//         glProgram->linkAndValidate();
//
//         const i32 projectionMatrixUniformLoc = vertexShader->createUniform("projectionMatrix");
//         const i32 cameraViewMatrixUniformLoc = vertexShader->createUniform("cameraViewMatrix");
//         const i32 modelViewMatrixUniformLoc  = vertexShader->createUniform("modelViewMatrix");
//         const i32 samplerLoc = pixelShader->createUniform("textureSampler");
//         const i32 overlayLoc = pixelShader->createUniform("textureOverlay");
//
//         renderData->glProgram = glProgram;
//         renderData->vertexShader = vertexShader;
//         renderData->pixelShader = pixelShader;
//
//         renderData->projectionMatrixUniformLoc = projectionMatrixUniformLoc;
//         renderData->cameraViewMatrixUniformLoc = cameraViewMatrixUniformLoc;
//         renderData->modelViewMatrixUniformLoc = modelViewMatrixUniformLoc;
//         renderData->samplerLoc = samplerLoc;
//         renderData->overlayLoc = overlayLoc;
//
//         renderData->rp->returnControlOfContext();
//     }
//     else if(c == 'v')
//     {
//         static bool vsync = false;
//
//         if(WGLEW_EXT_swap_control)
//         {
//             renderData->rp->takeControlOfContext();
//             vsync = !vsync;
//             if(vsync)
//             {
//                 wglSwapIntervalEXT(1);
//             }
//             else
//             {
//                 wglSwapIntervalEXT(0);
//             }
//             renderData->rp->returnControlOfContext();
//         }
//     }
//
//     return false;
// }
//
// bool onWindowResize(WindowResizeEvent& e) noexcept
// {
//     if(renderData->rp)
//     {
//         renderData->rp->pushResizeViewport(0, 0, e.newWidth(), e.newHeight());
//     }
//     else
//     {
//         renderData->window.updateViewPort(0, 0, e.newWidth(), e.newHeight());
//     }
//
//     renderData->perspectiveProjMatrix = Matrix4x4f::perspective(DEG_2_RAD(90), static_cast<float>(e.newWidth()) / static_cast<float>(e.newHeight()), 0.0001f, 1000.0f);
//     renderData->orthoProjMatrix = Matrix4f::orthographic(0.0f, static_cast<float>(e.newWidth()), 0.0f, static_cast<float>(e.newHeight()));
//
//     return false;
// }
//
// bool onWindowActive(WindowActiveEvent& e) noexcept
// {
//     activeWindow = e.active();
//     return false;
// }
//
// void onWindowEvent(void*, WindowEvent& e) noexcept
// {
//     EventDispatcher dispatcher(e);
//     dispatcher.dispatch<WindowResizeEvent>(&onWindowResize);
//     dispatcher.dispatch<WindowActiveEvent>(&onWindowActive);
//     dispatcher.dispatch<WindowKeyEvent>(&onKeyPress);
//     dispatcher.dispatch<WindowAsciiKeyEvent>(&onCharPress);
// }
//
// void __cdecl setupParams(RenderingPipeline&, Window&, void*) noexcept
// {
//     glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
//     
//     enableGLDepthTest();
//     
//     enableGLCullFace();
//     glCullFace(GL_BACK);
//     
//     glFrontFace(GL_CW);
//     
//     setGLBlend(true);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// }
