

#include "upgradeManager.h"
#include "game.h"

extern Game BubbleBop;


UpgradeManager::UpgradeManager()
{
    std::string filePath = FileSystem::getPath("resources/upgradeData/Upgrades.json").c_str();
    std::ifstream file(filePath, std::ifstream::binary);
    
    Json::parseFromStream(readerBuilder,file,&data,&errs);
    
    
    generalUpgCallbacks.push_back([this]() { AdditiveUpgrade();});
    generalUpgCallbacks.push_back([this]() { MultiplierUpgrade();});
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
    
    constructor.BindEventCallback("buyGeneralUpgrade",
                                  &UpgradeManager::BuyGeneralUpgrade,
                                  this);
    constructor.BindEventCallback("buyBubbleUpgrade",
                                  &UpgradeManager::BuyBubbleUpgrade,
                                  this);
    constructor.BindEventCallback("buyClickUpgrade",
                                  &UpgradeManager::BuyClickUpgrade,
                                  this);
                     
        
    
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


void UpgradeManager::BuyGeneralUpgrade(Rml::DataModelHandle model,Rml::Event& event,const Rml::VariantList& arguments)
{
    if (arguments.empty())
        return;

    // Need to also get the price of the upgrade and increase the quantity on the document
    int index = arguments[0].Get<int>();
    
    // Based on this index you do the specific upgrade

    //std::cout << "Bought item at index: " << index << '\n';
    
    generalUpgCallbacks[index](); // Call the callback for the specific upgrade
}
void UpgradeManager::BuyBubbleUpgrade(Rml::DataModelHandle model,Rml::Event& event,const Rml::VariantList& arguments)
{
    if (arguments.empty())
    return;

    int index = arguments[0].Get<int>();
    
    // Based on this index you do the specific upgrade

    //std::cout << "Bought item at index: " << index << '\n';
    
    //bubbleUpgCallbacks[index](); // Call the callback for the specific upgrade
}
void UpgradeManager::BuyClickUpgrade(Rml::DataModelHandle model,Rml::Event& event,const Rml::VariantList& arguments)
{
    if (arguments.empty())
    return;

    int index = arguments[0].Get<int>();
    
    // Based on this index you do the specific upgrade

    //std::cout << "Bought item at index: " << index << '\n';
    
    //clickUpgCallbacks[index](); // Call the callback for the specific upgrade
}




void UpgradeManager::AdditiveUpgrade()
{
    BubbleBop.clickAdditive += 1.0f; // Increase the click additive by 1.0
    std::cout << "Click Additive increased to: " << BubbleBop.clickAdditive << std::endl;
}

void UpgradeManager::MultiplierUpgrade()
{
    BubbleBop.clickMultiplier += 1.0f; // Increase the click multiplier by 1.0
    std::cout << "Click Multiplier increased to: " << BubbleBop.clickMultiplier << std::endl;
}