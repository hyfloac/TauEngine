// #include <vector>
// #include <NumTypes.hpp>
// #include <Utils.hpp>
//
// struct Vertex
// {
//     float x, y, z;
// };
//
// struct TextureCoord
// {
//     float u, v;
// };
//
// typedef Vertex Normal;
//
// struct Indice
// {
//     u32 i, j, k;
// };
//
// struct Face
// {
//     Indice vertex, texture, normal;
// };
//
// static void continuePastWhitespace(const char*& str)
// {
//     while((*str == '\n' || *str == '\r' || *str == ' ' || *str == '\t') && *str != '\0')
//     {
//         ++str;
//     }
// }
//
// static i32 charsToWhitespace(const char* str)
// {
//     i32 count = 0;
//     while(*str != ' ' && *str != '\t' && *str != '\0')
//     {
//         ++str;
//         ++count;
//     }
//     if(*str == '\0') { return -1; }
//     return count;
// }
//
// static void continueToWhitespace(const char*& str)
// {
//     while(*str != '\n' && *str != '\r' && *str != ' ' && *str != '\t' && *str != '\0')
//     {
//         ++str;
//     }
// }
//
// static void continueToNextLine(const char*& str)
// {
//
//     while(*str != '\n' && *str != '\r' && *str != '\0')
//     {
//         ++str;
//     }
//     while((*str == '\n' || *str == '\r') && *str != '\0')
//     {
//         ++str;
//     }
// }
//
// static void continueToChar(const char*& str, char delim)
// {
//     while(*str != delim && *str != '\0')
//     {
//         ++str;
//     }
// }
//
// static i32 charsToChar(const char* str, char delim)
// {
//     i32 count = 0;
//     while(*str != delim && *str != '\0')
//     {
//         ++str;
//         ++count;
//     }
//     if(*str == '\0') { return -1; }
//     return count;
// }
//
// enum FaceType : u8
// {
//     VERTEX = 0,
//     VERTEX_TEXTURE,
//     VERTEX_NORMAL,
//     VERTEX_TEXTURE_NORMAL
// };
//
// void loadOBJ(const char* fileData) noexcept
// {
//     std::vector<Vertex>       vertices;
//     std::vector<Normal>       normals;
//     std::vector<TextureCoord> textures;
//     std::vector<Face>         indices;
//
//     FaceType faceType = static_cast<FaceType>(-1);
//
//     while(*fileData != '\0')
//     {
//         // Skipping past comments
//         if(*fileData == '#' || *fileData == ';')
//         {
//             do
//             {
//                 ++fileData;
//             } while(*fileData != '\n' && *fileData != '\r' && *fileData != '\0');
//
//             if(*fileData == '\0') { return; }
//         }
//
//         // Skipping past new lines
//         continuePastWhitespace(fileData);
//
//         if(*fileData == '\0') { return; }
//
//         switch(*fileData)
//         {
//             case 'v':
//                 ++fileData;
//                 switch(*fileData)
//                 {
//                     case 't': /* Texture Coord */
//                     {
//                         ++fileData;
//                         const float u = std::strtof(fileData, null);
//                         continueToWhitespace(fileData);
//                         if(*fileData == '\0') { return; }
//                         const float v = std::strtof(fileData, null);
//                         textures.push_back({ u, v });
//                     }
//                         break;
//                     case 'n': /* Normal */
//                     {
//                         ++fileData;
//                         const float xn = std::strtof(fileData, null);
//                         continueToWhitespace(fileData);
//                         if(*fileData == '\0') { return; }
//                         const float yn = std::strtof(fileData, null);
//                         continueToWhitespace(fileData);
//                         if(*fileData == '\0') { return; }
//                         const float zn = std::strtof(fileData, null);
//                         normals.push_back({ xn, yn, zn });
//                     }
//                         break;
//                     case ' ':  /* Plain Vertex */
//                     case '\t':
//                     {
//                         ++fileData;
//                         const float x = std::strtof(fileData, null);
//                         continueToWhitespace(fileData);
//                         if(*fileData == '\0') { return; }
//                         const float y = std::strtof(fileData, null);
//                         continueToWhitespace(fileData);
//                         if(*fileData == '\0') { return; }
//                         const float z = std::strtof(fileData, null);
//                         vertices.push_back({ x, y, z });
//                     }
//                     break;
//                 }
//                 break;
//             case 'f':
//             {
//                 ++fileData;
//                 Indice vertexIndice, textureIndice, normalIndice;
//
//                 const u32 xi = std::strtoul(fileData, nullptr, 10);
//                 if(xi == 0)
//                 {
//                     return;
//                 }
//                 if(faceType == static_cast<FaceType>(-1) && charsToChar(fileData, '/') == -1)
//                 {
//                     faceType = VERTEX;
//                 }
//                 else
//                 {
//                     continueToWhitespace(fileData);
//                     const i32 xi = std::atoi(fileData);
//                 }
//             }
//                 break;
//         }
//         continueToNextLine(fileData);
//     }
// }
