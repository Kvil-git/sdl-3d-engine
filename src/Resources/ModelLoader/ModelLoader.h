#ifndef MODELLOADER_H
#define MODELLOADER_H


#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../../Core/Geometry/Material.h"
#include "../../Core/Geometry/Model.h"
#include "../../Core/Utilities/StringFunctions.h"
#include "../../Core/Utilities/OutputFunctions.h"



template <typename ComponentType>
class ModelLoader {
    private:
        using Triangle3 = Polygon3D<ComponentType, 3>;
        using Quadrilateral = Polygon3D<ComponentType, 4>;
        using NGon = NGon3D<ComponentType>;

        using Vertex3 = Vertex3<ComponentType>;
        using Model = Model<ComponentType>;
        using Material = Material<ComponentType>;
        using Vector3 = Vector<ComponentType, 3>;
        using Vector2 = Vector<ComponentType, 2>;

    public:
        std::vector<Model> models;
        std::vector<Material> materials;
        std::vector<Triangle3> triangles;
        std::vector<Quadrilateral> quadrilaterals;
        std::vector<NGon> ngons;
        
        ModelLoader(){};

        bool LoadFromObj(std::string filepath) {
            if (EndsWith(filepath, ".obj") == false) {
                std::cout << "Error: File is not a .obj file." << std::endl;
                return false;
            }

            std::ifstream file(filepath);

            if (file.is_open() == false) {
                std::cout << "Error: Could not open file." << std::endl;
                return false;
            }

            triangles.clear();
            models.clear();
            materials.clear();

            std::vector<Vector3> verticePositions, verticeNormals;
            std::vector<Vector2> verticeTextureCoordinates;
            
            std::string line;
            while (std::getline(file, line)) {
                std::vector<std::string> lineWords = Split(line, " ");
                if(lineWords[0] == "mtllib") {
                    std::string materialPath = "assets/models/" + Split(line, " ")[1];
                    { //print material file
                    
                    std::cout << "material path: "<<  materialPath << std::endl;
                    std::ifstream materialFile(materialPath);

                    if (materialFile.is_open() == false) {
                        std::cout << "Error: Could not open material file." << std::endl;
                        continue;
                    }
                    std::cout<<"\n\n\nOpening a material file\n\n\n";

                    Material newMaterial;
                    while(std::getline(materialFile, line)) {
                        if(line.empty() or line[0] == ' ') continue;
                        
                        std::vector<std::string> lineWords = Split(line, " ");
                        
                        if(lineWords[0] == "newmtl") newMaterial.name = lineWords[1];
                        else if(lineWords[0] == "Ka") newMaterial.ambientColor = StringToVector<ComponentType, 3> (line, 1);
                        else if(lineWords[0] == "Kd") newMaterial.diffuseColor = StringToVector<ComponentType, 3> (line, 1);
                        else if(lineWords[0] == "Ks") newMaterial.specularColor = StringToVector<ComponentType, 3> (line, 1);
                        else if(lineWords[0] == "Ns") newMaterial.specularExponent = std::stod(lineWords[1]);
                        else if(lineWords[0] == "d") newMaterial.dissolve = std::stod(lineWords[1]); //dissolve
                        else if(lineWords[0] == "Tr") newMaterial.dissolve = 1.0f - std::stod(lineWords[1]); //Transparency is inverted dissolve, so we revert it back
                        else if(lineWords[0] == "Ni") newMaterial.opticalDensity = std::stod(lineWords[1]);
                        else if(lineWords[0] == "illum") newMaterial.illumination = std::stoi(lineWords[1]);
                        else if(lineWords[0] == "map_Ka") newMaterial.ambientColorMap = lineWords[1];
                        else if(lineWords[0] == "map_Kd") newMaterial.diffuseColorMap = lineWords[1];
                        else if(lineWords[0] == "map_Ks") newMaterial.specularColorMap = lineWords[1];
                        else if(lineWords[0] == "map_Ns") newMaterial.specularExponentMap = lineWords[1];
                        else if(lineWords[0] == "map_d") newMaterial.dissolveMap = lineWords[1];
                        else if(lineWords[0] == "map_bump") newMaterial.bumpMap = lineWords[1];
                        else if(lineWords[0] == "map_Bump") newMaterial.bumpMap = lineWords[1];
                        else if(lineWords[0] == "bump") newMaterial.bumpMap = lineWords[1];
                        else if(lineWords[0] == "Bump") newMaterial.bumpMap = lineWords[1];
                    }

                    std::cout<<"\n\n\nClosing a material file\n\n\n";
                    materialFile.close();
                    materials.push_back(newMaterial);
                    //std::cout << newMaterial << std::endl;
                    }
                }
                else if(lineWords[0] == "v") {
                    size_t size = lineWords.size();
                    Vector3 position = StringToVector<ComponentType, 3>(line, 1);
                    if(size == 5) { // if the vector is homogeneous we divide it by the w component
                        position /= stod(lineWords[4]);
                    }
                    verticePositions.push_back(position);
                }
                else if(lineWords[0] == "vt") {
                    Vector2 textureCoordinate = StringToVector<ComponentType, 2>(line, 1);
                    verticeTextureCoordinates.push_back(textureCoordinate);
                }
                else if(lineWords[0] == "vn") {
                    Vector3 normal = StringToVector<ComponentType, 3>(line, 1);
                    verticeNormals.push_back(normal.Unit());
                }
                else if(lineWords[0] == "f") {
                    size_t vertexCount = lineWords.size() - 1;
                    Vertex3 polygonVertices[vertexCount];
                    for(int i=1; i <= vertexCount; i++) {
                        std::vector<std::string> faceWords = Split(lineWords[i], "/");
                        polygonVertices[i-1].position = verticePositions[std::stoi(faceWords[0]) - 1];
                        if(faceWords.size() < 2) continue;
                        if(faceWords.size() == 2 or
                        (faceWords.size() == 3 and faceWords[1].empty() == false)){
                            polygonVertices[i-1].textureCoordinates = verticeTextureCoordinates[std::stoi(faceWords[1]) - 1];
                        }
                        if(faceWords.size() < 3) continue;
                        polygonVertices[i-1].normal = verticeNormals[std::stoi(faceWords[2]) - 1];
                    }
                    switch(vertexCount){
                        case 3: triangles.push_back(Triangle3(polygonVertices)); break;
                        case 4: quadrilaterals.push_back(Quadrilateral(polygonVertices)); break;
                        default:
                            ngons.push_back(
                                NGon(polygonVertices, vertexCount)
                            );
                            break;
                    }
                }
            }

            // for(int i=0; i < triangles.size(); i++) {
            //     std::cout<<triangles[i]<<std::endl;
            // }

            // for(int i=0; i < quadrilaterals.size(); i++) {
            //     std::cout<<quadrilaterals[i]<<std::endl;
            // }

            // for(int i=0; i < ngons.size(); i++) {
            //     std::cout<<ngons[i]<<std::endl;
            // }

            file.close();
            return true;
        }
};

#endif