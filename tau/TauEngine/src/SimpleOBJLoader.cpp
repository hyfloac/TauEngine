#pragma warning(push, 0)
#include <cstdio>
#include <cstring>
#pragma warning(pop)
#include <maths/Maths.hpp>
#include <model/OBJLoader.hpp>
#include <memory>
#include "VFS.hpp"

namespace objl
{
    float Vector3::magnitude() const noexcept
    {
        return sqrtf(this->magnitudeSquared());
    }

    float Vector3::midAngleCos(const Vector3& other) const noexcept
    {
        return this->dot(other) * rSqrt(this->magnitudeSquared() * other.magnitudeSquared());
    }

    Vector3 Vector3::projection(const Vector3& other) const noexcept
    {
        const Vector3 bn = other * rSqrt(this->magnitudeSquared());
        return bn * this->dot(other);
    }

    namespace algorithm
    {
        bool strEmpty(const char* str) noexcept { return !(str && strlen(str)); }

        bool strEmpty(const char* str, const size_t len) noexcept { return !(str && len); }

        size_t strFindLastNotOf(const char* str, const char* chars) noexcept
        {
            if(!str || !chars) { return 0; }

            size_t ret = 0;

            while(*chars != '\0')
            {
                const char* fnd = strrchr(str, *chars);

                if(!fnd) { return strlen(str) - 1; }

                const size_t offset = fnd - str;

                if(offset > ret) { ret = offset; }

                ++chars;
            }

            return ret;
        }

        const char* subString(const char* str, const size_t count) noexcept
        {
            char* retStr = new(std::nothrow) char[count + 1];
            if(!retStr) { return nullptr; }
            retStr[count] = '\0';
            strncpy_s(retStr, count + 1, str, count);
            return retStr;
        }

        const char* subString(const char* str, const size_t start, const size_t count) noexcept
        {
            char* retStr = new(std::nothrow) char[count + 1];
            if(!retStr) { return nullptr; }
            retStr[count] = '\0';
            strncpy_s(retStr, count + 1, str + start, count);
            return retStr;
        }

        bool sameSide(const Vector3& RESTRICT p1, const Vector3& RESTRICT p2, const Vector3& RESTRICT a, const Vector3& RESTRICT b) noexcept
        {
            const Vector3 ba = b - a;

            const Vector3 p1a = p1 - a;
            const Vector3 p2a = p2 - a;

            const Vector3 cp1 = ba.cross(p1a);
            const Vector3 cp2 = ba.cross(p2a);

            return cp1.dot(cp2) >= 0;
        }

        Vector3 genTriNormal(const Vector3& RESTRICT t1, const Vector3& RESTRICT t2, const Vector3& RESTRICT t3) noexcept
        {
            const Vector3 u = t2 - t1;
            const Vector3 v = t3 - t1;

            return u.cross(v);
        }

        bool inTriangle(const Vector3& RESTRICT point, const Vector3& RESTRICT tri1, const Vector3& RESTRICT tri2, const Vector3& RESTRICT tri3) noexcept
        {
            // Test to see if it is within an infinite prism that the triangle outlines.
            const bool withinTriPrism = sameSide(point, tri1, tri2, tri3) && 
                                        sameSide(point, tri2, tri1, tri3) && 
                                        sameSide(point, tri3, tri1, tri2);

            // If it isn't it will never be on the triangle
            if(!withinTriPrism) { return false; }

            const Vector3 n = genTriNormal(tri1, tri2, tri3);

            // If the distance from the triangle to the point is 0
            //	it lies on the triangle
            return point.projection(n).magnitudeSquared() <= 0.0001f;
        }

        void split(const std::string& RESTRICT in, std::vector<std::string>& RESTRICT out, const std::string& RESTRICT token) noexcept
        {
            out.clear();

            std::string temp;

            const size_t inSize = in.size();
            const size_t tokenSize = token.size();
            const size_t tokenSize1 = tokenSize + 1;

            for(size_t i = 0; i < inSize; ++i)
            {
                std::string test = in.substr(i, tokenSize);

                if(test == token)
                {
                    if(!temp.empty())
                    {
                        out.push_back(temp);
                        temp.clear();
                        i += tokenSize1;
                    }
                    else { out.emplace_back(""); }
                }
                else if(i + tokenSize >= inSize)
                {
                    temp += in.substr(i, tokenSize);
                    out.push_back(temp);
                    break;
                }
                else { temp += in[i]; }
            }
        }

        void split(const char* RESTRICT in, std::vector<std::string>& RESTRICT out, const char token) noexcept
        {
            out.clear();

            const size_t len = strlen(in);

            char* temp = new(std::nothrow) char[len + 1];
            if(!temp) { return; }
            char* tempIndex = temp;

            while(*in != '\0')
            {
                const char test = *in;

                if(test == token)
                {
                    if(tempIndex != temp)
                    {
                        *tempIndex = '\0';
                        out.emplace_back(temp);
                        tempIndex = temp;
                    }
                    else { out.emplace_back(""); }
                }
                else 
                { 
                    *tempIndex = test;
                    ++tempIndex;
                }

                ++in;
            }

            *tempIndex = '\0';
            out.emplace_back(temp);

            delete[] temp;
        }

        const char* tail(const char* in) noexcept
        {
            const size_t inLen = strlen(in);

            if(!strEmpty(in, inLen))
            {
                const size_t cTokenStart = strspn(in, " \t");
                const size_t cSpaceStart = strcspn(in + cTokenStart, "  \t") + cTokenStart;
                const size_t cTailStart = strspn(in + cSpaceStart, " \t") + cSpaceStart;
                const size_t cTailEnd = strFindLastNotOf(in, " \t");

                if(cTailStart != inLen)
                {
                    if(cTailEnd != inLen)
                    {
                        return subString(in, cTailStart, cTailEnd - cTailStart + 1);
                    }
                    return subString(in, cTailStart);
                }
            }

            return nullptr;
        }

        const char* firstToken(const char* in) noexcept
        {
            const size_t inLen = strlen(in);

            if(!strEmpty(in, inLen))
            {
                const size_t cTokenStart = strspn(in, " \t");
                const size_t cTokenEnd = strcspn(in + cTokenStart, "  \t") + cTokenStart;

                if(cTokenStart != inLen)
                {
                    if(cTokenEnd != inLen)
                    {
                        return subString(in, cTokenStart, cTokenEnd - cTokenStart);
                    }
                    return subString(in, cTokenStart);
                }
            }

            return nullptr;
        }

        template<typename _T>
        inline const _T& getElement(const std::vector<_T>& RESTRICT elements, const std::string& RESTRICT index)
        {
            i32 idx = std::stoi(index);
            if(idx < 0) { idx = static_cast<i32>(elements.size() + idx); }
            else { --idx; }
            return elements[idx];
        }
    }

    void Loader::genVerticesFromRawOBJ(std::vector<Vertex>& RESTRICT vertices, const std::vector<Vector3>& RESTRICT positions, const std::vector<Vector2>& RESTRICT textureCoords, const std::vector<Vector3>& RESTRICT normals, const char* currentLine) noexcept
    {
        std::vector<std::string> faceSplit, vertexSplit;
        Vertex vertex;
        const char* cTail = algorithm::tail(currentLine);
        algorithm::split(cTail, faceSplit, ' ');
        delete[] cTail;

        bool noNormal = false;

        enum VerticeTypes : u8
        {
            Pos = 0,
            PosTex,
            PosTexNorm,
            PosNorm
        };

        // For every given vertex do this
        for(const std::string& faceElm : faceSplit)
        {
            // See What type the vertex is.
            VerticeTypes verticeType = static_cast<VerticeTypes>(-1);

            algorithm::split(faceElm.c_str(), vertexSplit, '/');

            switch(vertexSplit.size())
            {
                case 1:
                    verticeType = Pos;
                    break;
                case 2:
                    verticeType = PosTex;
                    break;
                case 3:
                    if(!vertexSplit[1].empty()) { verticeType = PosTexNorm; }
                    else { verticeType = PosNorm; }
                    break;
                default: break;
            }

            // Calculate and store the vertex
            switch(verticeType)
            {
                case Pos: // P
                    vertex.position = algorithm::getElement(positions, vertexSplit[0]);
                    vertex.textureCoordinate = Vector2();
                    noNormal = true;
                    vertices.push_back(vertex);
                    break;
                case PosTex: // P/T
                    vertex.position = algorithm::getElement(positions, vertexSplit[0]);
                    vertex.textureCoordinate = algorithm::getElement(textureCoords, vertexSplit[1]);
                    noNormal = true;
                    vertices.push_back(vertex);
                    break;
                case PosNorm: // P//N
                    vertex.position = algorithm::getElement(positions, vertexSplit[0]);
                    vertex.textureCoordinate = Vector2();
                    vertex.normal = algorithm::getElement(normals, vertexSplit[2]);
                    vertices.push_back(vertex);
                    break;
                case PosTexNorm: // P/T/N
                    vertex.position = algorithm::getElement(positions, vertexSplit[0]);
                    vertex.textureCoordinate = algorithm::getElement(textureCoords, vertexSplit[1]);
                    vertex.normal = algorithm::getElement(normals, vertexSplit[2]);
                    vertices.push_back(vertex);
                    break;
                default: break;
            }
        }

        // take care of missing normals
        // these may not be truly accurate but it is the 
        // best they get for not compiling a mesh with normals	
        if(noNormal)
        {
            const Vector3 a = vertices[0].position - vertices[1].position;
            const Vector3 b = vertices[2].position - vertices[1].position;

            const Vector3 normal = a.cross(b);

            for(Vertex& vertice : vertices)
            {
                vertice.normal = normal;
            }
        }
    }

    void Loader::vertexTriangulation(std::vector<u32>& RESTRICT indices, const std::vector<Vertex>& RESTRICT vertices) noexcept
    {
        // If there are 2 or less verts,
        // no triangle can be created,
        // so exit
        if(vertices.size() < 3) { return; }
        // If it is a triangle no need to calculate it
        if(vertices.size() == 3)
        {
            indices.push_back(0);
            indices.push_back(1);
            indices.push_back(2);
            return;
        }

        // Create a list of vertices
        std::vector<Vertex> tVerts = vertices;

        while(true)
        {
            // For every vertex
            for(u32 i = 0; i < tVerts.size(); ++i)
            {
                // pPrev = the previous vertex in the list
                Vertex pPrev;
                if(i == 0) { pPrev = tVerts[tVerts.size() - 1]; }
                else { pPrev = tVerts[i - 1]; }

                // pCur = the current vertex;
                Vertex pCur = tVerts[i];

                // pNext = the next vertex in the list
                Vertex pNext;
                if(i == tVerts.size() - 1) { pNext = tVerts[0]; }
                else { pNext = tVerts[i + 1]; }

                // Check to see if there are only 3 verts left
                // if so this is the last triangle
                if(tVerts.size() == 3)
                {
                    // Create a triangle from pCur, pPrev, pNext
                    for(u32 j = 0; j < tVerts.size(); ++j)
                    {
                        if(vertices[j].position ==  pCur.position) { indices.push_back(j); }
                        if(vertices[j].position == pPrev.position) { indices.push_back(j); }
                        if(vertices[j].position == pNext.position) { indices.push_back(j); }
                    }

                    tVerts.clear();
                    break;
                }

                if(tVerts.size() == 4)
                {
                    // Create a triangle from pCur, pPrev, pNext
                    for(u32 j = 0; j < vertices.size(); ++j)
                    {
                        if(vertices[j].position ==  pCur.position) { indices.push_back(j); }
                        if(vertices[j].position == pPrev.position) { indices.push_back(j); }
                        if(vertices[j].position == pNext.position) { indices.push_back(j); }
                    }

                    Vector3 tempVec;
                    for(auto& vertex : tVerts)
                    {
                        if(vertex.position !=  pCur.position &&
                           vertex.position != pPrev.position &&
                           vertex.position != pNext.position)
                        {
                            tempVec = vertex.position;
                            break;
                        }
                    }

                    // Create a triangle from pCur, pPrev, pNext
                    for(u32 j = 0; j < vertices.size(); ++j)
                    {
                        if(vertices[j].position == pPrev.position) { indices.push_back(j); }
                        if(vertices[j].position == pNext.position) { indices.push_back(j); }
                        if(vertices[j].position == tempVec)        { indices.push_back(j); }
                    }

                    tVerts.clear();
                    break;
                }

                // If Vertex is not an interior vertex
                const Vector3 prevCur = pPrev.position - pCur.position;
                const Vector3 nextCur = pNext.position - pCur.position;

                const float angleCos = prevCur.midAngleCos(nextCur);
                if(angleCos <= -1.0f || angleCos >= 1.0f) { continue; }

                // If any vertices are within this triangle
                bool inTri = false;
                for(const auto& vertice : vertices)
                {
                    if(algorithm::inTriangle(vertice.position, pPrev.position, pCur.position, pNext.position) && 
                       vertice.position != pPrev.position && 
                       vertice.position !=  pCur.position && 
                       vertice.position != pNext.position)
                    {
                        inTri = true;
                        break;
                    }
                }

                if(inTri) { continue; }

                // Create a triangle from pCur, pPrev, pNext
                for(u32 j = 0; j < vertices.size(); ++j)
                {
                    if(vertices[j].position ==  pCur.position) { indices.push_back(j); }
                    if(vertices[j].position == pPrev.position) { indices.push_back(j); }
                    if(vertices[j].position == pNext.position) { indices.push_back(j); }
                }

                // Delete pCur from the list
                for(u32 j = 0; j < tVerts.size(); ++j)
                {
                    if(tVerts[j].position == pCur.position)
                    {
                        tVerts.erase(tVerts.begin() + j);
                        break;
                    }
                }

                // reset i to the start
                // -1 since loop will add 1 to it
                i = static_cast<u32>(-1);
            }

            // if no triangles were created
            if(indices.empty() || tVerts.empty()) { break; }
        }
    }

    bool Loader::loadMaterials(const char* path) noexcept
    {
        const size_t pathLen = strlen(path);

        if(!(pathLen > 4 && path[pathLen - 4] == '.' && path[pathLen - 3] == 'm' && path[pathLen - 2] == 't' && path[pathLen - 1] == 'l')) { return false; }

        FILE* cFile;
        if (fopen_s(&cFile, path, "r")) { return false; }

        Material tempMaterial;

        bool listening = false;

        { // Block used to destruct the line buffer.
            char cCurrentLine[256];

            while(fgets(cCurrentLine, sizeof(cCurrentLine), cFile))
            {
                if(cCurrentLine[0] == '\0' ||
                   cCurrentLine[0] == '\r' ||
                   cCurrentLine[0] == '\n' ||
                   cCurrentLine[0] == '#') { continue; }

                const size_t lineLen = strlen(cCurrentLine);

                if(cCurrentLine[lineLen - 1] == '\n') { cCurrentLine[lineLen - 1] = '\0'; }

                const std::string currentLine = cCurrentLine;

                // std::string sFirstToken = algorithm::firstToken(currentLine);
                const char* sFirstToken = algorithm::firstToken(cCurrentLine);

                if(algorithm::strEmpty(sFirstToken, lineLen)) { continue; }

                const char* clTail = algorithm::tail(cCurrentLine);
                // std::string tokenTail = clTail;
                // delete[] clTail;

                // new material and material name
                if(strcmp(sFirstToken, "newmtl") == 0)
                {
                    if(!listening) { listening = true; }
                    else
                    {
                        // Generate the material

                        // Push Back loaded Material
                        _materials.push_back(tempMaterial);

                        // Clear Loaded Material
                        tempMaterial = Material();
                    }

                    if(currentLine.size() > 7) { tempMaterial.name = clTail; }
                    else { tempMaterial.name = "none"; }
                }
                else if(sFirstToken[0] == 'K' && strlen(sFirstToken) == 2)
                {
                    std::vector<std::string> temp;
                    algorithm::split(clTail, temp, ' ');

                    if(temp.size() != 3) { continue; }

                    Vector3* vp = nullptr;

                    if(sFirstToken[1] == 'a') { vp = &tempMaterial.Ka; } // Ambient Color
                    else if(sFirstToken[1] == 'd') { vp = &tempMaterial.Kd; } // Diffuse Color
                    else if(sFirstToken[1] == 's') { vp = &tempMaterial.Ks; } // Specular Color

                    if(vp)
                    {
                        vp->x() = std::stof(temp[0]);
                        vp->y() = std::stof(temp[1]);
                        vp->z() = std::stof(temp[2]);
                    }
                }
                // Specular Exponent
                else if(strcmp(sFirstToken, "Ns") == 0) { tempMaterial.Ns = std::stof(clTail); }
                // Optical Density
                else if(strcmp(sFirstToken, "Ni") == 0) { tempMaterial.Ni = std::stof(clTail); }
                // Dissolve
                else if(strcmp(sFirstToken, "d") == 0) { tempMaterial.d = std::stof(clTail); }
                // Illumination
                else if(strcmp(sFirstToken, "illum") == 0) { tempMaterial.illum = std::stoi(clTail); }
                // Ambient Texture Map
                else if(strcmp(sFirstToken, "map_Ka") == 0) { tempMaterial.map_Ka = clTail; }
                // Diffuse Texture Map
                else if(strcmp(sFirstToken, "map_Kd") == 0) { tempMaterial.map_Kd = clTail; }
                // Specular Texture Map
                else if(strcmp(sFirstToken, "map_Ks") == 0) { tempMaterial.map_Ks = clTail; }
                // Specular Highlight Map
                else if(strcmp(sFirstToken, "map_Ns") == 0) { tempMaterial.map_Ns = clTail; }
                // Alpha Texture Map
                else if(strcmp(sFirstToken, "map_d") == 0) { tempMaterial.map_d = clTail; }
                // Bump Map
                else if(strcmp(sFirstToken, "map_Bump") == 0 ||
                        strcmp(sFirstToken, "map_bump") == 0 ||
                        strcmp(sFirstToken, "bump") == 0)
                { tempMaterial.map_bump = clTail; }

                delete[] clTail;
                delete[] sFirstToken;
            }
        }

        // Deal with last material

        _materials.push_back(tempMaterial);

        // Test to see if anything was loaded
        // If not return false
        return !_materials.empty();
    }

    bool Loader::loadFile(const char* path) noexcept
    {
        const size_t pathLen = strlen(path);
        if(!(pathLen > 4 && path[pathLen - 4] == '.' && path[pathLen - 3] == 'o' && path[pathLen - 2] == 'b' && path[pathLen - 1] == 'j')) { return false; }

        VFS::Container physPath = VFS::Instance().resolvePath(path);

        if(physPath.path.length() == 0)
        {
            return false;
        }

        path = physPath.path.c_str();

        FILE* cFile;
        if(fopen_s(&cFile, path, "r")) { return false; }

        _meshes.clear();
        _vertices.clear();
        _indices.clear();

        std::vector<Vector3> Positions;
        std::vector<Vector2> TCoords;
        std::vector<Vector3> Normals;

        std::vector<Vertex> Vertices;
        std::vector<u32> Indices;

        std::vector<std::string> MeshMatNames;

        std::string meshName;

        Mesh tempMesh;

        { // Block used to destruct the line buffer.
            char currentLine[256];

            while(fgets(currentLine, sizeof(currentLine), cFile))
            {
                if(currentLine[0] == '\0' ||
                   currentLine[0] == '\r' ||
                   currentLine[0] == '\n' ||
                   currentLine[0] == '#') { continue; }

                const size_t lastChar = strlen(currentLine) - 1;
                if(currentLine[lastChar] == '\n') { currentLine[lastChar] = '\0'; }

                const char* sFirstToken = algorithm::firstToken(currentLine);
                const size_t sFTLen = strlen(sFirstToken);

                if(algorithm::strEmpty(sFirstToken, sFTLen)) { continue; }
                const char cFirstToken = sFirstToken[0];

                // Generate a Mesh Object or Prepare for an object to be created
                if(cFirstToken == 'o' ||
                   cFirstToken == 'g')
                {
                    const char* cTmpMeshName = algorithm::tail(currentLine);

                    // Generate the mesh to put into the array

                    if(!Indices.empty() && !Vertices.empty())
                    {
                        // Create Mesh
                        tempMesh = Mesh(Vertices, Indices);
                        tempMesh.name = meshName;

                        // Insert Mesh
                        _meshes.push_back(tempMesh);

                        // Cleanup
                        Vertices.clear();
                        Indices.clear();
                        meshName.clear();
                    }

                    if(!algorithm::strEmpty(cTmpMeshName)) { meshName = cTmpMeshName; }
                    else { meshName = "unnamed"; }

                    delete[] cTmpMeshName;
                } // Generate a Vertex Position
                else if(cFirstToken == 'v')
                {
                    const char* clTail = algorithm::tail(currentLine);
                    if(sFTLen > 1)
                    {
                        const char cSecondToken = sFirstToken[1];
                        // Generate a Vertex Texture Coordinate
                        if(cSecondToken == 't')
                        {
                            std::vector<std::string> stex;
                            Vector2 vtex;
                            algorithm::split(clTail, stex, ' ');

                            vtex.x() = std::stof(stex[0]);
                            vtex.y() = std::stof(stex[1]);

                            TCoords.push_back(vtex);
                        } // Generate a Vertex Normal;
                        else if(cSecondToken == 'n')
                        {
                            std::vector<std::string> snor;
                            Vector3 vnor;
                            algorithm::split(clTail, snor, ' ');

                            vnor.x() = std::stof(snor[0]);
                            vnor.y() = std::stof(snor[1]);
                            vnor.z() = std::stof(snor[2]);

                            Normals.push_back(vnor);
                        }
                    }
                    else
                    {
                        std::vector<std::string> spos;
                        Vector3 vpos;
                        algorithm::split(clTail, spos, ' ');

                        vpos.x() = std::stof(spos[0]);
                        vpos.y() = std::stof(spos[1]);
                        vpos.z() = std::stof(spos[2]);

                        Positions.push_back(vpos);
                    }
                    delete[] clTail;
                } // Generate a Face (vertices & indices)
                else if(cFirstToken == 'f')
                {
                    // Generate the vertices
                    std::vector<Vertex> vVerts;
                    genVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, currentLine);

                    // Subtract Vertices
                    for(const Vertex& vVert : vVerts)
                    {
                        Vertices.push_back(vVert);

                        _vertices.push_back(vVert);
                    }

                    std::vector<u32> iIndices;

                    vertexTriangulation(iIndices, vVerts);

                    // Subtract Indices
                    for(u32 iIndice : iIndices)
                    {
                        u32 indNum = static_cast<u32>(Vertices.size() - vVerts.size()) + iIndice;
                        Indices.push_back(indNum);

                        indNum = static_cast<u32>(_vertices.size() - vVerts.size()) + iIndice;
                        _indices.push_back(indNum);
                    }
                } // Get Mesh Material Name
                else if(strcmp(sFirstToken, "usemtl") == 0)
                {
                    const char* clTail = algorithm::tail(currentLine);
                    MeshMatNames.emplace_back(clTail);
                    delete[] clTail;

                    // Create new Mesh, if Material changes within a group
                    if(!Indices.empty() && !Vertices.empty())
                    {
                        // Create Mesh
                        tempMesh = Mesh(Vertices, Indices);
                        tempMesh.name = meshName + "_2";

                        // Insert Mesh
                        _meshes.push_back(tempMesh);

                        // Cleanup
                        Vertices.clear();
                        Indices.clear();
                    }
                } // Load Materials
                else if(strcmp(sFirstToken, "mtllib") == 0)
                {
                    // Generate LoadedMaterial

                    // Generate a path to the material file
                    std::vector<std::string> temp;
                    algorithm::split(path, temp, '/');

                    std::string pathToMat;

                    const size_t tSize = temp.size() - 1;
                    if(tSize >= 0)
                    {
                        for(size_t i = 0; i < tSize; ++i)
                        {
                            pathToMat += temp[i] + "/";
                        }
                    }

                    const char* clTail = algorithm::tail(currentLine);
                    pathToMat += clTail;
                    delete[] clTail;

                    // Load Materials
                    loadMaterials(pathToMat.c_str());
                }

                delete[] sFirstToken;
            }
        }

        fclose(cFile);

        // Deal with last mesh

        if(!Indices.empty() && !Vertices.empty())
        {
            // Create Mesh
            tempMesh = Mesh(Vertices, Indices);
            tempMesh.name = meshName;

            // Insert Mesh
            _meshes.push_back(tempMesh);
        }

        // Set Materials for each Mesh
        for(u32 i = 0; i < MeshMatNames.size(); ++i)
        {
            std::string matName = MeshMatNames[i];

            // Find corresponding material name in loaded materials
            // when found copy material variables into mesh material
            for(auto& _material : _materials)
            {
                if(_material.name == matName)
                {
                    _meshes[i].material = _material;
                    break;
                }
            }
        }

        return !(_meshes.empty() && _vertices.empty() && _indices.empty());
    }
}
