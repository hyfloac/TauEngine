#pragma warning(push, 0)
#include <cstdio>
#include <cmath>
#pragma warning(pop)
#include <model/OBJLoader.hpp>
// #include <MathDefines.hpp>

namespace objl
{
    Vector2::Vector2() noexcept
        : _x(0.0f), _y(0.0f)
    { }

    Vector2::Vector2(const float x, const float y) noexcept
        : _x(x), _y(y)
    { }

    Vector3::Vector3() noexcept
        : _x(0.0f), _y(0.0f), _z(0.0f)
    { }

    Vector3::Vector3(const float x, const float y, const float z) noexcept
        : _x(x), _y(y), _z(z)
    { }

    Vector3 Vector3::cross(const Vector3& other) const noexcept
    {
        return Vector3(this->_y * other._z - this->_z * other._y,
                       this->_z * other._x - this->_x * other._z,
                       this->_x * other._y - this->_y * other._x);
    }

    float Vector3::magnitude() const noexcept
    {
        return sqrtf(this->magnitudeSquared());
    }

    static float fastInverseSqrt(float x) noexcept
    {
        const float halfX = x * 0.5f;
        i32 i = *(i32*)&x;

        i = 0x5F3759DF - (i >> 1);
        const float y = *(float*)&i;

        return y * (1.5f - (halfX * y * y));
    }

    float Vector3::midAngleCos(const Vector3& other) const noexcept
    {
        return this->dot(other) * fastInverseSqrt(this->magnitudeSquared() * other.magnitudeSquared());
    }

    Vector3 Vector3::projection(const Vector3& other) const noexcept
    {
        const Vector3 bn = other * fastInverseSqrt(this->magnitudeSquared());
        return bn * this->dot(other);
    }

    Material::Material() noexcept
        : name(""), 
          Ns(0.0f), 
          Ni(0.0f),
          d(0.0f),
          illum(0)
    { }

    Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<u32> _indices) noexcept
        : vertices(_vertices), indices(std::move(_indices))
    { }

    namespace algorithm
    {
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

            const u32 inSize = static_cast<u32>(in.size());
            const u32 tokenSize = static_cast<u32>(token.size());
            const u32 tokenSize1 = static_cast<u32>(token.size() + 1);

            for(u32 i = 0; i < inSize; ++i)
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

        void split(const std::string& RESTRICT in, std::vector<std::string>& RESTRICT out, const char token) noexcept
        {
            out.clear();

            std::string temp;

            for(char test : in)
            {
                if(test == token)
                {
                    if(!temp.empty())
                    {
                        out.push_back(temp);
                        temp.clear();
                    }
                    else { out.emplace_back(""); }
                }
                else { temp += test; }
            }

            out.push_back(temp);
        }

        std::string tail(const std::string& in) noexcept
        {
            if(!in.empty())
            {
                const size_t token_start = in.find_first_not_of(" \t");
                const size_t space_start = in.find_first_of(" \t", token_start);
                const size_t tail_start = in.find_first_not_of(" \t", space_start);
                const size_t tail_end = in.find_last_not_of(" \t");

                if (tail_start != std::string::npos)
                {
                    if (tail_end != std::string::npos)
                    {
                        return in.substr(tail_start, tail_end - tail_start + 1);
                    }

                    return in.substr(tail_start);
                }
            }

            return "";
        }

        std::string firstToken(const std::string& in) noexcept
        {
            if(!in.empty())
            {
                const size_t token_start = in.find_first_not_of(" \t");
                const size_t token_end = in.find_first_of(" \t", token_start);

                if(token_start != std::string::npos)
                {
                    if(token_end != std::string::npos)
                    {
                        return in.substr(token_start, token_end - token_start);
                    }

                    return in.substr(token_start);
                }
            }

            return "";
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

    void Loader::genVerticesFromRawOBJ(std::vector<Vertex>& RESTRICT vertices, const std::vector<Vector3>& RESTRICT positions, const std::vector<Vector2>& RESTRICT textureCoords, const std::vector<Vector3>& RESTRICT normals, std::string currentLine) noexcept
    {
        std::vector<std::string> faceSplit, vertexSplit;
        Vertex vertex;
        algorithm::split(algorithm::tail(currentLine), faceSplit, ' ');

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

            algorithm::split(faceElm, vertexSplit, '/');

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

    bool Loader::loadMaterials(const std::string& path) noexcept
    {
        if(path.substr(path.size() - 4, 4) != ".mtl") { return false; }

        FILE* cFile;
        if (fopen_s(&cFile, path.c_str(), "r")) { return false; }

        Material tempMaterial;

        bool listening = false;

        char cCurrentLine[256];

        while(fgets(cCurrentLine, sizeof(cCurrentLine), cFile))
        {
            if(cCurrentLine[0] == '\0' ||
               cCurrentLine[0] == '\r' ||
               cCurrentLine[0] == '\n' ||
               cCurrentLine[0] == '#') { continue; }

            const u32 lastChar = static_cast<u32>(strlen(cCurrentLine) - 1);
            if (cCurrentLine[lastChar] == '\n') { cCurrentLine[lastChar] = '\0'; }

            const std::string currentLine = cCurrentLine;

            std::string sFirstToken = algorithm::firstToken(currentLine);

            if (sFirstToken.empty()) { continue; }

            std::string tokenTail = algorithm::tail(currentLine);

            // new material and material name
            if(sFirstToken == "newmtl")
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

                if(currentLine.size() > 7) { tempMaterial.name = tokenTail; }
                else { tempMaterial.name = "none"; }
            }
            else if(sFirstToken[0] == 'K' && sFirstToken.size() == 2)
            {
                std::vector<std::string> temp;
                algorithm::split(tokenTail, temp, ' ');

                if(temp.size() != 3) { continue; }

                Vector3* vp = nullptr;

                if(     sFirstToken[1] == 'a') { vp = &tempMaterial.Ka; } // Ambient Color
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
            else if(sFirstToken == "Ns") { tempMaterial.Ns = std::stof(tokenTail); }
            // Optical Density
            else if(sFirstToken == "Ni") { tempMaterial.Ni = std::stof(tokenTail); }
            // Dissolve
            else if(sFirstToken == "d") { tempMaterial.d = std::stof(tokenTail); }
            // Illumination
            else if(sFirstToken == "illum") { tempMaterial.illum = std::stoi(tokenTail); }
            // Ambient Texture Map
            else if(sFirstToken == "map_Ka") { tempMaterial.map_Ka = tokenTail; }
            // Diffuse Texture Map
            else if(sFirstToken == "map_Kd") { tempMaterial.map_Kd = tokenTail; }
            // Specular Texture Map
            else if(sFirstToken == "map_Ks") { tempMaterial.map_Ks = tokenTail; }
            // Specular Highlight Map
            else if(sFirstToken == "map_Ns") { tempMaterial.map_Ns = tokenTail; }
            // Alpha Texture Map
            else if(sFirstToken == "map_d") { tempMaterial.map_d = tokenTail; }
            // Bump Map
            else if(sFirstToken == "map_Bump" ||
                    sFirstToken == "map_bump" ||
                    sFirstToken == "bump") { tempMaterial.map_bump = tokenTail; }
        }

        // Deal with last material

        _materials.push_back(tempMaterial);

        // Test to see if anything was loaded
        // If not return false
        return !_materials.empty();
    }

    bool Loader::loadFile(const std::string& path) noexcept
    {
        if(path.substr(path.size() - 4, 4) != ".obj") { return false; }

        FILE* cFile;
        if(fopen_s(&cFile, path.c_str(), "r")) { return false; }

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

        char cCurrentLine[256];

        while(fgets(cCurrentLine, sizeof(cCurrentLine), cFile))
        {
            if(cCurrentLine[0] == '\0' ||
               cCurrentLine[0] == '\r' ||
               cCurrentLine[0] == '\n' ||
               cCurrentLine[0] == '#') { continue; }

            const u32 lastChar = static_cast<u32>(strlen(cCurrentLine) - 1);
            if(cCurrentLine[lastChar] == '\n') { cCurrentLine[lastChar] = '\0'; }

            const std::string currentLine = cCurrentLine;

            std::string sFirstToken = algorithm::firstToken(currentLine);
            
            if(sFirstToken.empty()) { continue; }
            const char cFirstToken = sFirstToken[0];

            // Generate a Mesh Object or Prepare for an object to be created
            if(cFirstToken == 'o' ||
               cFirstToken == 'g')
            {
                std::string tmpMeshName = algorithm::tail(currentLine);

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

                    if (!tmpMeshName.empty()) { meshName = tmpMeshName; }
                    else { meshName = "unnamed"; }
                }
                else
                {
                    if (!tmpMeshName.empty()) { meshName = tmpMeshName; }
                    else { meshName = "unnamed"; }
                }
            } // Generate a Vertex Position
            else if(cFirstToken == 'v')
            {
                if(sFirstToken.size() > 1)
                {
                    const char cSecondToken = sFirstToken[1];
                    // Generate a Vertex Texture Coordinate
                    if(cSecondToken == 't')
                    {
                        std::vector<std::string> stex;
                        Vector2 vtex;
                        algorithm::split(algorithm::tail(currentLine), stex, ' ');

                        vtex.x() = std::stof(stex[0]);
                        vtex.y() = std::stof(stex[1]);

                        TCoords.push_back(vtex);
                    } // Generate a Vertex Normal;
                    else if(cSecondToken == 'n')
                    {
                        std::vector<std::string> snor;
                        Vector3 vnor;
                        algorithm::split(algorithm::tail(currentLine), snor, ' ');

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
                    algorithm::split(algorithm::tail(currentLine), spos, ' ');

                    vpos.x() = std::stof(spos[0]);
                    vpos.y() = std::stof(spos[1]);
                    vpos.z() = std::stof(spos[2]);

                    Positions.push_back(vpos);
                }
            } // Generate a Face (vertices & indices)
            else if(cFirstToken == 'f')
            {
                // Generate the vertices
                std::vector<Vertex> vVerts;
                genVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, currentLine);

                // Add Vertices
                for(const auto& vVert : vVerts)
                {
                    Vertices.push_back(vVert);

                    _vertices.push_back(vVert);
                }

                std::vector<unsigned int> iIndices;

                vertexTriangulation(iIndices, vVerts);

                // Add Indices
                for(u32 iIndice : iIndices)
                {
                    u32 indNum = (u32)(Vertices.size() - vVerts.size()) + iIndice;
                    Indices.push_back(indNum);

                    indNum = (u32)(_vertices.size() - vVerts.size()) + iIndice;
                    _indices.push_back(indNum);
                }
            } // Get Mesh Material Name
            else if(sFirstToken == "usemtl")
            {
                MeshMatNames.push_back(algorithm::tail(currentLine));

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
            else if(sFirstToken == "mtllib")
            {
                // Generate LoadedMaterial

                // Generate a path to the material file
                std::vector<std::string> temp;
                algorithm::split(path, temp, '/');

                std::string pathToMat;

                const u32 tSize = static_cast<u32>(temp.size() - 1);
                if(tSize >= 0)
                {
                    for(u32 i = 0; i < tSize; ++i)
                    {
                        pathToMat += temp[i] + "/";
                    }
                }

                pathToMat += algorithm::tail(currentLine);

                // Load Materials
                loadMaterials(pathToMat);
            }
        }

        // Deal with last mesh

        if(!Indices.empty() && !Vertices.empty())
        {
            // Create Mesh
            tempMesh = Mesh(Vertices, Indices);
            tempMesh.name = meshName;

            // Insert Mesh
            _meshes.push_back(tempMesh);
        }

        // file.close();
        fclose(cFile);

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
