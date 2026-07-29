

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

