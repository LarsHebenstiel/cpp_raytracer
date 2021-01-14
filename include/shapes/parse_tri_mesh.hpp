#ifndef TRI_MESH_PARSE_H
#define TRI_MESH_PARSE_H

#include "utility.hpp"
#include "triangle.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <string>

std::vector<shared_ptr<triangle>> build_mesh(const std::string& filename, shared_ptr<material> mat_ptr, std::ostream &log) {
    std::vector<shared_ptr<triangle>> res;

    int nTriangles = 0;
    int nVertices = 0;
    std::vector<point3> vertices;
    std::vector<int> vertexIndices;
    std::vector<vec3> normals;
    std::vector<vec3> vert_uv;

    bool parseFailed = false;
    try {
        log << "[Parse .obj] Starting parsing\n";

        log << "\tAttempting to open file named \"" << filename << "\"\n";
        std::ifstream ifs(filename);

        if (!ifs) {
            log << "\tError: Input file stream opening failed\n";
            log << "\tPossibly file \"" << filename << "\" doesn't exist\n";
        } else {
            log << "\tInput file stream opened sucessfully\n";
        }

        log << std::flush;

        std::string line;
        std::string space(" ");
        std::string slash("/");
        int lineNum = 0;
        bool firstFace = true;
        uint8_t faceSyntaxType = 0; //type: 0 -> only vert, 1-> vert+tex, 2-> vert+norm, 3-> all

        //while we have data to read
        while (std::getline(ifs, line)) {
            lineNum++;
            if (line.length() < 1 || line[0] == '\n' || line[0] == '\r') {
                continue;
            }

            if (line[0] == '#') { //this line is a comment
                //don't do anything, but also don't log an error
            } else if (line[0] == 'v') { //vertex
                std::vector<std::string> parts = split(line, space);

                //missing dimensions
                if (parts.size() < 4) {
                    log << "\tMissing arguments for vertex on line " << lineNum << ": " << line << "\n";
                    // if vertices are missing don't stop parsing yet
                    // but we also know we cannot construct the mesh
                    parseFailed = true;
                    continue;
                }

                //determine which vert instruction
                if (line[1] == ' ') { //true vertex
                    std::istringstream ss(line.substr(2));
                    Float x, y, z;
                    bool ill_formed = false;
                    if (!(ss >> x)) {
                        ill_formed = true;
                        log << "\tError creating vertex on line " << lineNum << ":\n";
                        log << line.substr(2) << "\n";
                        log << "\t\tCould not parse argument 1\n";
                    }
                    if (!(ss >> y)) {
                        if (!ill_formed) {
                            log << "\tError creating vertex on line " << lineNum << ":\n";
                            ill_formed = true;
                        }
                        log << "\t\tCould not parse argument 2\n";
                    }
                    if (!(ss >> z)) {
                        if (!ill_formed) {
                            log << "\tError creating vertex on line " << lineNum << ":\n";
                            ill_formed = true;
                        }
                        log << "\t\tCould not parse argument 3\n";
                    }
                    if (ill_formed) {
                        parseFailed = true;
                        log << "\t\tfrom \"" << line << "\t\n";
                    } else {
                        vec3 tmp(x, y, z);
                        vertices.push_back(tmp);
                        nVertices++;
                    }
                } else if (line[1] == 'n') { //vertex normal
                    //TODO: Add vert normal to data
                } else if (line[1] == 't') { //tex-coord
                    //TODO: Add vert tex-coord to data
                } else {
                    log << "\tIncorrect vertex syntax on line " << lineNum << ": " << line << "\n";
                    continue;
                }
            } else if (line[0] == 'f') { //face
                std::vector<std::string> parts = split(line, space);

                //missing dimensions
                if (parts.size() < 4) {
                    parseFailed = true;
                    std::cerr << "\tMissing arguments for face on line " << lineNum << ": " << line << "\n";
                    continue;
                }
                
                uint8_t types[3];
                for (size_t i = 0; i < 3; i++) {
                    types[i] = count(parts[i + 1], '/');

                    if (types[i] == 2) {
                        if (parts[i + 1].find("//") == std::string::npos) {
                            types[i] = 3;
                        }
                    }
                }

                //cross check results between themselves assuming first argument is correct
                bool ill_formed = false;
                if (types[1] != types[0]) {
                    parseFailed = true;
                    ill_formed = true;
                    log << "\tError: On line " << lineNum << ":\n";
                    log << "\t\tIn \"" << line << "\"\n";
                    log << "\t\tArgument syntax specified in \"" << parts[2] << "\" does not match \"" << parts[1] << "\"\n";
                }
                if (types[2] != types[0]) {
                    parseFailed = true;
                    if (!ill_formed) {
                        log << "\tError: On line " << lineNum << ":\n";
                        log << "\t\tIn \"" << line << "\"\n";
                        ill_formed = true;
                    }
                    log << "\t\tArgument syntax specified in \"" << parts[3] << "\" does not match \"" << parts[1] << "\"\n";
                }
                if (ill_formed) {
                    continue;
                }

                // if we get to here we know syntax for the line agrees with itself
                // now assume this is document syntax
                if (firstFace) {
                    firstFace = false;
                    faceSyntaxType = types[1];
                } else {
                    ill_formed = false;
                    for (size_t i = 0; i < 3; i++) {
                        if (types[i] != faceSyntaxType) {
                            parseFailed = true;
                            log << "\tError: On line " << lineNum << ":\n";
                            log << "\t\tArgument syntax of \"" << line << "\" does not match detected file syntax\n";

                            //once we print error once, stop
                            break;
                        }
                    }
                }

                //TODO: Add face to data ... properly
                parts[1] = parts[1].substr(0, parts[1].find('/'));
                parts[2] = parts[2].substr(0, parts[2].find('/'));
                parts[3] = parts[3].substr(0, parts[3].find('/'));
                std::istringstream ss(parts[1] + " " + parts[2] + " " + parts[3]);
                Float x, y, z;
                ill_formed = false;
                if (!(ss >> x)) {
                    ill_formed = true;
                    log << "\tError creating face on line " << lineNum << ":\n";
                    log << line.substr(2) << "\n";
                    log << "\t\tCould not parse argument 1\n";
                }
                if (!(ss >> y)) {
                    if (!ill_formed) {
                        log << "\tError creating face on line " << lineNum << ":\n";
                        ill_formed = true;
                    }
                    log << "\t\tCould not parse argument 2\n";
                }
                if (!(ss >> z)) {
                    if (!ill_formed) {
                        log << "\tError creating face on line " << lineNum << ":\n";
                        ill_formed = true;
                    }
                    log << "\t\tCould not parse argument 3\n";
                }
                if (ill_formed) {
                    parseFailed = true;
                    log << "\t\tfrom \"" << line << "\t\n";
                } else {
                    //we use these values to index an array
                    vertexIndices.push_back(x - 1);
                    vertexIndices.push_back(y - 1);
                    vertexIndices.push_back(z - 1);
                    nTriangles++;
                }

            } else { //unknown/undef
                log << "\tUndefined control sequence on line " << lineNum << ": " << line << "\n";
            }
        }

        ifs.close();
    } catch(const std::exception& e) {
        parseFailed = true;
        log << "\tAn unrecoverable error has occurred:\n";
        log << "\t" << e.what();
    }

    if (parseFailed) {
        log << "\tTriangle mesh construction failed due to errors\n";
        log << std::flush;
    } else {
        log << "\tTriangle mesh construction starting\n";
        log << std::flush;
        //construct the mesh
        auto mesh = make_shared<TriangleMesh>(nTriangles, &(vertexIndices[0]), nVertices, &(vertices[0]), nullptr, mat_ptr);
        for (int i = 0; i < nTriangles; i++) {
            res.push_back(make_shared<triangle>(mesh, i));
        }
        log << "\tTriangle mesh construction complete\n";
    }

    log << "[/Parse .obj] Parsing complete\n\n";

    log << std::flush;

    return res;
}

#endif //TRI_MESH_PARSE_H