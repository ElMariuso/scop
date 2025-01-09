#pragma once

// STD Libs
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

// OpenGL Libs
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// OpenGL Math Libs
#include <glm/glm.hpp>

// Custom Libs
#include <Vertex.hpp>

static std::vector<Vertex> loadOBJ(const std::string& filename)
{
    std::vector<glm::fvec3> vertex_positions;
    std::vector<glm::fvec2> vertex_texcoords;
    std::vector<glm::fvec3> vertex_normals;

    std::vector<GLint> vertex_position_indices;
    std::vector<GLint> vertex_texcoord_indices;
    std::vector<GLint> vertex_normal_indices;

    std::vector<Vertex> vertices;

    std::stringstream ss;
    std::ifstream in_file(filename);

    if (!in_file.is_open()) throw std::runtime_error("ERROR::OBJLOADER::Could not open file.");

    std::string line = "";
    std::string prefix = "";
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLint temp_glint = 0;

    while (std::getline(in_file, line))
    {
        // Get the prefix
        ss.clear();
        ss.str(line);
        ss >> prefix;

        // Ignore comments
        if (prefix == "#")
            continue;

        if (prefix == "v")
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_positions.push_back(temp_vec3);
        }
        else if (prefix == "vt")
        {
            ss >> temp_vec2.x >> temp_vec2.y;
            vertex_texcoords.push_back(temp_vec2);
        }
        else if (prefix == "vn")
        {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertex_normals.push_back(temp_vec3);
        }
        else if (prefix == "f")
        {
            int counter = 0;
            while (ss >> temp_glint)
            {
                if (counter == 0)
                    vertex_position_indices.push_back(temp_glint);
                else if (counter == 1)
                    vertex_texcoord_indices.push_back(temp_glint);
                else if (counter == 2)
                    vertex_normal_indices.push_back(temp_glint);

                if (ss.peek() == '/')
                {
                    ++counter;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ')
                {
                    ++counter;
                    ss.ignore(1, ' ');
                }

                if (counter > 2)
                    counter = 0;
            }
        }
    }

    // Build final vertex array (mesh)
    vertices.resize(vertex_position_indices.size(), Vertex());

    for (size_t i = 0; i < vertex_position_indices.size(); ++i)
    {
        if (vertex_position_indices[i] - 1 >= 0 && vertex_position_indices[i] - 1 < vertex_positions.size())
            vertices[i].position = vertex_positions[vertex_position_indices[i] - 1];
        else
            throw std::runtime_error("ERROR::OBJLOADER::Invalid or missing vertex position in OBJ file.");

        if (!vertex_texcoords.empty() && vertex_texcoord_indices[i] - 1 >= 0 && vertex_texcoord_indices[i] - 1 < vertex_texcoords.size())
            vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indices[i] - 1];
        else
            vertices[i].texcoord = glm::vec2(0.0f, 0.0f);

        if (!vertex_normals.empty() && vertex_normal_indices[i] - 1 >= 0 && vertex_normal_indices[i] - 1 < vertex_normals.size())
            vertices[i].normal = vertex_normals[vertex_normal_indices[i] - 1];
        else
            vertices[i].normal = glm::vec3(0.0f, 0.0f, 1.0f);

        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }

    std::cout << "vertex_positions.size(): " << vertex_positions.size() << "\n";
    std::cout << "vertex_texcoords.size(): " << vertex_texcoords.size() << "\n";
    std::cout << "vertex_normals.size(): " << vertex_normals.size() << "\n";
    std::cout << "vertex_position_indices.size(): " << vertex_position_indices.size() << "\n";

    std::cout << "OBJ file loaded" << "\n";
    return vertices;
}