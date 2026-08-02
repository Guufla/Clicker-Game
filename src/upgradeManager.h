#ifndef UPGRADE_MANAGER_H
#define UPGRADE_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <RmlUi/Core.h>

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
    
    int price = 0;
    int quantity = 0;
    int index = 0;
    
    
};

class UpgradeManager
{
public:
    std::vector<Upgrade> genUpg; // Make a list of all the possible bubble upgrades and their templates
    std::vector<Upgrade> clickUpg; // Make a list of all the possible bubble upgrades and their templates
    std::vector<Upgrade> bubbleUpg; // Make a list of all the possible bubble upgrades and their templates
    
    
    
    
    UpgradeManager();
    
    void Start();
    void Update(float dt);
    
    
    bool InitializeDataModel(Rml::Context* context);
    

    void AddUpgrade(const Upgrade& upgrade);
    void AddUpgrade(std::string name,
                    std::string description, 
                    std::string imgPath, 
                    int price, 
                    int quantity, 
                    int index);

    void LoadGeneralUpgrades();

    void BuyUpgrade(int index);

private:
    Json::Value data;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
    Rml::DataModelHandle modelHandle;
    
    std::vector<std::function<void()>> generalUpgCallbacks; // List of callbacks for general upgrades
    std::vector<std::function<void()>> clickUpgCallbacks; // List of callbacks for click upgrades
    std::vector<std::function<void()>> bubbleUpgCallbacks; // List of callbacks for bubble upgrades
    
    
    void BuyGeneralUpgrade(
        Rml::DataModelHandle model,
        Rml::Event& event,
        const Rml::VariantList& arguments
    );

    void BuyClickUpgrade(
        Rml::DataModelHandle model,
        Rml::Event& event,
        const Rml::VariantList& arguments
    );

    void BuyBubbleUpgrade(
        Rml::DataModelHandle model,
        Rml::Event& event,
        const Rml::VariantList& arguments
    );
    
    void AdditiveUpgrade();
    void MultiplierUpgrade();
    
    
};



#endif