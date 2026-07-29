#ifndef UPGRADE_MANAGER_H
#define UPGRADE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <learnopengl/filesystem.h>
#include <iostream>
#include <queue>

#include <json/json.h>
#include <fstream>

struct Upgrade {
    std::string id;
    std::string name;
    std::string description;
    std::string imgPath;
    int price;
    int quantity;
    int index;
};

class UpgradeManager
{
public:
    std::vector<Upgrade> Upgrades; // Make a list of all the possible bubble upgrades and their templates
    
    
    UpgradeManager();
    
    void Start();
    
    void Update(float dt);

private:
    Json::Value data;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
};



#endif