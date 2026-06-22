#include <iostream>
#include <fstream>
#include <string>
#include <netcdf>
#include "DataDownloader.h"

int main() {
    std::cout << "=== MODUL ACHIZITIE DATE ===" << std::endl;

    std::string user, pass;
    std::ifstream configFile("config.txt");

    if (!configFile.is_open()) {
        std::cerr << "[EROARE] Nu s-a gasit config.txt in radacina proiectului!" << std::endl;
        std::cerr << "Creeaza fisierul si pune email-ul pe prima linie si parola pe a doua." << std::endl;
        return 1;
    }

    std::getline(configFile, user);
    std::getline(configFile, pass);
    configFile.close();

    if (user.empty() || pass.empty()) {
        std::cerr << "[EROARE] config.txt este gol sau incomplet!" << std::endl;
        return 1;
    }

    std::cout << "[AUTH] Date incarcate pentru: " << user << std::endl;

    DataDownloader downloader(user, pass);

    downloader.downloadPeriod(2026, 6, 13, 22, 23);

    std::cout << "\n[FINISH] Proces finalizat cu succes." << std::endl;
    return 0;
}