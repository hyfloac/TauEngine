// #include "gl/GLShaderBundleVisitor.hpp"
// #include "shader/bundle/ast/FileExprAST.hpp"
// #include "shader/bundle/ast/BlockExprAST.hpp"
// #include "shader/bundle/ast/ShaderIOBindingExprAST.hpp"
// #include "shader/IShader.hpp"
// #include "shader/IShaderProgram.hpp"
// #include "system/GraphicsInterface.hpp"
// #include <GL/glew.h>
//
// #include "gl/GLShaderProgram.hpp"
// #include <TUMaths.hpp>
//
// #include "shader/ShaderBindMap.hpp"
//
// static void fillUniformBinding(RefDynArray<u32>& arr, const ArrayList<BindPointUnion>& map, GLuint program) noexcept;
// static void fillUniformBlockBinding(RefDynArray<u32>& arr, const ArrayList<BindPointUnion>& map, GLuint program) noexcept;
//
// CPPRef<IShaderProgram> GLShaderBundleVisitor::getShader(IRenderingContext& ctx, IGraphicsInterface& gi) noexcept
// {
//     if(_builtShader)
//     {
//         return _builtShader;
//     }
//
//     CPPRef<IShader> vertex;
//     CPPRef<IShader> tessCtrl = null;
//     CPPRef<IShader> tessEval = null;
//     CPPRef<IShader> geometry = null;
//     CPPRef<IShader> pixel;
//
//     {
//         ShaderArgs args;
//         args.vfsMount = "__bundle__";
//         args.path = "/";
//
//         args.fileName = _vertex.fileName;
//         args.stage = EShader::Stage::Vertex;
//         vertex = gi.createShader().buildCPPRef(args, null);
//
//         args.fileName = _tessCtrl.fileName;
//         args.stage = EShader::Stage::TessellationControl;
//         if(_tessCtrl.fileName.length() > 0)
//         { tessCtrl = gi.createShader().buildCPPRef(args, null); }
//
//         args.fileName = _tessEval.fileName;
//         args.stage = EShader::Stage::TessellationEvaluation;
//         if(_tessEval.fileName.length() > 0)
//         { tessEval = gi.createShader().buildCPPRef(args, null); }
//
//         args.fileName = _geometry.fileName;
//         args.stage = EShader::Stage::Geometry;
//         if(_geometry.fileName.length() > 0)
//         { geometry = gi.createShader().buildCPPRef(args, null); }
//
//         args.fileName = _pixel.fileName;
//         args.stage = EShader::Stage::Pixel;
//         pixel = gi.createShader().buildCPPRef(args, null);
//     }
//
//     const CPPRef<IShaderProgram> shader = IShaderProgram::create(gi);
//     shader->setVertexShader(ctx, vertex);
//     shader->setTessellationControlShader(ctx, tessCtrl);
//     shader->setTessellationEvaluationShader(ctx, tessEval);
//     shader->setGeometryShader(ctx, geometry);
//     shader->setPixelShader(ctx, pixel);
//
//     _builtShader = shader;
//
//     const GLuint shaderProgram = RefCast<GLShaderProgram>(shader)->programID();
//
//     if(!glBindAttribLocation && GLEW_ARB_vertex_shader && glBindAttribLocationARB)
//     {
//         for(uSys i = 0; i < _vertex.ioPoints.count(); ++i)
//         {
//             if(_vertex.ioPoints[i].type == BindPointUnion::Str)
//             {
//                 glBindAttribLocationARB(shaderProgram, _vertex.ioPoints[i].bindPoint, _vertex.ioPoints[i].bindName);
//             }
//         }
//     }
//     else
//     {
//         for(uSys i = 0; i < _vertex.ioPoints.count(); ++i)
//         {
//             if(_vertex.ioPoints[i].type == BindPointUnion::Str)
//             {
//                 glBindAttribLocation(shaderProgram, _vertex.ioPoints[i].bindPoint, _vertex.ioPoints[i].bindName);
//             }
//         }
//     }
//
//     if(!glBindFragDataLocation && GLEW_EXT_gpu_shader4 && glBindFragDataLocationEXT)
//     {
//         for(uSys i = 0; i < _pixel.ioPoints.count(); ++i)
//         {
//             if(_pixel.ioPoints[i].type == BindPointUnion::Str)
//             {
//                 glBindFragDataLocationEXT(shaderProgram, _pixel.ioPoints[i].bindPoint, _pixel.ioPoints[i].bindName);
//             }
//         }
//     }
//     else
//     {
//         for(uSys i = 0; i < _pixel.ioPoints.count(); ++i)
//         {
//             if(_pixel.ioPoints[i].type == BindPointUnion::Str)
//             {
//                 glBindFragDataLocation(shaderProgram, _pixel.ioPoints[i].bindPoint, _pixel.ioPoints[i].bindName);
//             }
//         }
//     }
//
//     shader->link(ctx);
//
//     fillUniformBlockBinding(_vertexUniMap, _vertex.uniformPoints, shaderProgram);
//     fillUniformBlockBinding(_tessCtrlUniMap, _tessCtrl.uniformPoints, shaderProgram);
//     fillUniformBlockBinding(_tessEvalUniMap, _tessEval.uniformPoints, shaderProgram);
//     fillUniformBlockBinding(_geometryUniMap, _geometry.uniformPoints, shaderProgram);
//     fillUniformBlockBinding(_pixelUniMap, _pixel.uniformPoints, shaderProgram);
//
//     fillUniformBinding(_vertexTexMap, _vertex.texturePoints, shaderProgram);
//     fillUniformBinding(_tessCtrlTexMap, _tessCtrl.texturePoints, shaderProgram);
//     fillUniformBinding(_tessEvalTexMap, _tessEval.texturePoints, shaderProgram);
//     fillUniformBinding(_geometryTexMap, _geometry.texturePoints, shaderProgram);
//     fillUniformBinding(_pixelTexMap, _pixel.texturePoints, shaderProgram);
//
//     return shader;
// }
//
// static void fillUniformBinding(RefDynArray<u32>& arr, const ArrayList<BindPointUnion>& map, const GLuint program) noexcept
// {
//     if(map.count() == 0)
//     {
//         return;
//     }
//
//     u32 maxIndex = 0;
//
//     for(uSys i = 0; i < map.count(); ++i)
//     {
//         maxIndex = maxT(maxIndex, map[i].bindPoint);
//     }
//     arr = RefDynArray<u32>(maxIndex + 1);
//     if(!glGetUniformLocation && GLEW_ARB_shader_objects && glGetUniformLocationARB)
//     {
//         for(uSys i = 0; i < map.count(); ++i)
//         {
//             const auto& bind = map[i];
//
//             const uSys index = bind.bindPoint;
//
//             if(bind.type == BindPointUnion::Number)
//             {
//                 arr[index] = bind.mapPoint;
//             }
//             else if(bind.type == BindPointUnion::Str)
//             {
//                 arr[index] = glGetUniformLocationARB(program, bind.bindName);
//             }
//         }
//     }
//     else
//     {
//         for(uSys i = 0; i < map.count(); ++i)
//         {
//             const auto& bind = map[i];
//
//             const uSys index = bind.bindPoint;
//
//             if(bind.type == BindPointUnion::Number)
//             {
//                 arr[index] = bind.mapPoint;
//             }
//             else if(bind.type == BindPointUnion::Str)
//             {
//                 arr[index] = glGetUniformLocation(program, bind.bindName);
//             }
//         }
//     }
// }
//
// static void fillUniformBlockBinding(RefDynArray<u32>& arr, const ArrayList<BindPointUnion>& map, const GLuint program) noexcept
// {
//     if(map.count() == 0)
//     {
//         return;
//     }
//
//     u32 maxIndex = 0;
//
//     for(uSys i = 0; i < map.count(); ++i)
//     {
//         maxIndex = maxT(maxIndex, map[i].bindPoint);
//     }
//     arr = RefDynArray<u32>(maxIndex + 1);
//     for(uSys i = 0; i < map.count(); ++i)
//     {
//         const auto& bind = map[i];
//
//         const uSys index = bind.bindPoint;
//
//         if(bind.type == BindPointUnion::Number)
//         {
//             arr[index] = bind.mapPoint;
//         }
//         else if(bind.type == BindPointUnion::Str)
//         {
//             arr[index] = glGetUniformBlockIndex(program, bind.bindName);
//             glUniformBlockBinding(program, arr[index], arr[index]);
//         }
//     }
//
// }
//
// ShaderBindMap GLShaderBundleVisitor::getBindMap() noexcept
// {
//     return ShaderBindMap(_vertexUniMap, _tessCtrlUniMap, _tessEvalUniMap, _geometryUniMap, _pixelUniMap, _vertexTexMap, _tessCtrlTexMap, _tessEvalTexMap, _geometryTexMap, _pixelTexMap);
// }
//
// void GLShaderBundleVisitor::visit(const ExprAST* expr) noexcept
// {
//     IShaderBundleVisitor::visit(expr);
// }
//
// void GLShaderBundleVisitor::visit(const FileExprAST& expr) noexcept
// {
//     switch(_currentStage)
//     {
//         case EShader::Stage::Vertex:
//             _vertex.fileName = expr.filePath();
//             break;
//         case EShader::Stage::TessellationControl:
//             _tessCtrl.fileName = expr.filePath();
//             break;
//         case EShader::Stage::TessellationEvaluation:
//             _tessEval.fileName = expr.filePath();
//             break;
//         case EShader::Stage::Geometry:
//             _geometry.fileName = expr.filePath();
//             break;
//         case EShader::Stage::Pixel:
//             _pixel.fileName = expr.filePath();
//             break;
//         default: break;
//     }
//
//     visitNext(expr);
// }
//
// void GLShaderBundleVisitor::visit(const TypedBlockExprAST& expr) noexcept
// {
//     switch(expr.type())
//     {
//         case BlockType::Vertex:
//             _currentStage = EShader::Stage::Vertex;
//             break;
//         case BlockType::TessellationControl:
//             _currentStage = EShader::Stage::TessellationControl;
//             break;
//         case BlockType::TessellationEvaluation:
//             _currentStage = EShader::Stage::TessellationEvaluation;
//             break;
//         case BlockType::Geometry:
//             _currentStage = EShader::Stage::Geometry;
//             break;
//         case BlockType::Pixel:
//             _currentStage = EShader::Stage::Pixel;
//             break;
//         case BlockType::Uniforms:
//             _bindingBlock = BindBlock::Uniforms;
//             break;
//         case BlockType::Textures:
//             _bindingBlock = BindBlock::Textures;
//             break;
//         case BlockType::Inputs:
//             _bindingBlock = BindBlock::Inputs;
//             break;
//         case BlockType::Outputs:
//             _bindingBlock = BindBlock::Outputs;
//             break;
//         default: break;
//     }
//
//     visit(expr.container().get());
//     visitNext(expr);
// }
//
// void GLShaderBundleVisitor::visit(const ShaderIOMapPointExprAST& expr) noexcept
// {
//     switch(_currentStage)
//     {
//         case EShader::Stage::Vertex:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _vertex.uniformPoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 case BindBlock::Textures:
//                     _vertex.texturePoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::TessellationControl:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _tessCtrl.uniformPoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 case BindBlock::Textures:
//                     _tessCtrl.texturePoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::TessellationEvaluation:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _tessEval.uniformPoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 case BindBlock::Textures:
//                     _tessEval.texturePoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::Geometry:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _geometry.uniformPoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 case BindBlock::Textures:
//                     _geometry.texturePoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::Pixel:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _pixel.uniformPoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 case BindBlock::Textures:
//                     _pixel.texturePoints.emplace(expr.bindPoint(), expr.shaderBind());
//                     break;
//                 default: break;
//             }
//             break;
//         default: break;
//     }
//
//     visitNext(expr);
// }
//
// void GLShaderBundleVisitor::visit(const ShaderIOBindPointExprAST& expr) noexcept
// {
//     switch(_currentStage)
//     {
//         case EShader::Stage::Vertex:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Inputs:
//                     _vertex.ioPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Uniforms:
//                     _vertex.uniformPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Textures:
//                     _vertex.texturePoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::TessellationControl:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _tessCtrl.uniformPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Textures:
//                     _tessCtrl.texturePoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::TessellationEvaluation:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _tessEval.uniformPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Textures:
//                     _tessEval.texturePoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::Geometry:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Uniforms:
//                     _geometry.uniformPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Textures:
//                     _geometry.texturePoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 default: break;
//             }
//             break;
//         case EShader::Stage::Pixel:
//             switch(_bindingBlock)
//             {
//                 case BindBlock::Outputs:
//                     _pixel.ioPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Uniforms:
//                     _pixel.uniformPoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 case BindBlock::Textures:
//                     _pixel.texturePoints.emplace(expr.bindPoint(), expr.uniformName());
//                     break;
//                 default: break;
//             }
//             break;
//         default: break;
//     }
//
//     visitNext(expr);
// }
