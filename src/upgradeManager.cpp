

#include "upgradeManager.h"


UpgradeManager::UpgradeManager()
{
    std::string filePath = FileSystem::getPath("resources/upgradeData/Upgrades.json").c_str();
    std::ifstream file(filePath, std::ifstream::binary);
    
    Json::parseFromStream(readerBuilder,file,&data,&errs);
}

void UpgradeManager::Start()
{
    std::cout << data["general"][1]["id"].asString() << std::endl;
    std::cout << "Printed" << std::endl;
}

bool UpgradeManager::InitializeDataModel(Rml::Context* context)
{
    Rml::DataModelConstructor constructor = context->CreateDataModel("upgrade_model");
    if(!constructor)
    {
        std::cout << "constructor could not initialize" << std::endl;
        return false;
    }
    
    if(auto upgradeHandle = constructor.RegisterStruct<Upgrade>())
    {
        upgradeHandle.RegisterMember("name",&Upgrade::name);
        upgradeHandle.RegisterMember("description",&Upgrade::description);
        upgradeHandle.RegisterMember("image",&Upgrade::imgPath);
        upgradeHandle.RegisterMember("price",&Upgrade::price);
        upgradeHandle.RegisterMember("quantity",&Upgrade::quantity);
    }
    
    constructor.RegisterArray<std::vector<Upgrade>>();
    constructor.Bind("generalUpgrades", &genUpg);
    constructor.Bind("clickUpgrades", &clickUpg);
    constructor.Bind("bubbleUpgrades", &bubbleUpg);
    
    modelHandle = constructor.GetModelHandle();
    return true;
}

void UpgradeManager::AddUpgrade(std::string name,std::string description,std::string imgPath,int price,int quantity,int index)
{
    Upgrade curUpgrade;

    curUpgrade.id = name;
    curUpgrade.name = name;
    curUpgrade.description = description;
    curUpgrade.imgPath = FileSystem::getPath(imgPath).c_str();
    curUpgrade.price = price;
    curUpgrade.quantity = quantity;

    genUpg.push_back(std::move(curUpgrade));

    modelHandle.DirtyVariable("generalUpgrades");
    
}


void UpgradeManager::LoadGeneralUpgrades()
{
    genUpg.clear();

    for (const Json::Value& value : data["general"])
    {
        Upgrade upgrade;

        upgrade.id = value["id"].asString();
        upgrade.name = value["name"].asString();
        upgrade.description = value["description"].asString();
        upgrade.imgPath = value["image"].asString();
        upgrade.price = value["price"].asInt();
        upgrade.quantity = value["quantity"].asInt();

        genUpg.push_back(std::move(upgrade));
        modelHandle.DirtyVariable("generalUpgrades");
    }
    
    for (const Json::Value& value : data["clicking"])
    {
        Upgrade upgrade;

        upgrade.id = value["id"].asString();
        upgrade.name = value["name"].asString();
        upgrade.description = value["description"].asString();
        upgrade.imgPath = value["image"].asString();
        upgrade.price = value["price"].asInt();
        upgrade.quantity = value["quantity"].asInt();

        clickUpg.push_back(std::move(upgrade));
        modelHandle.DirtyVariable("clickUpgrades");
    }
    
    for (const Json::Value& value : data["bubbles"])
    {
        Upgrade upgrade;

        upgrade.id = value["id"].asString();
        upgrade.name = value["name"].asString();
        upgrade.description = value["description"].asString();
        upgrade.imgPath = value["image"].asString();
        upgrade.price = value["price"].asInt();
        upgrade.quantity = value["quantity"].asInt();

        bubbleUpg.push_back(std::move(upgrade));
        modelHandle.DirtyVariable("bubbleUpgrades");
    }
}
