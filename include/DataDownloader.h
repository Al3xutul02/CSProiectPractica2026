#pragma once
#include <string>

class DataDownloader {
private:
    std::string ftpUrl;
    std::string username;
    std::string password;

    // Callbacks și helpers interne pentru cURL și decompresie zlib
    static size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* stream);
    bool decompressGz(const std::string& gzFilePath, const std::string& outFilePath);

public:
    DataDownloader(const std::string& user, const std::string& pass);

    // Descarcă o arhivă .gz și o salvează local direct dezarhivată (.nc)
    bool downloadFile(const std::string& remoteFileName, const std::string& localSavePath);

    // Generează secvența de fișiere din 15 în 15 min pentru un interval orar fix
    void downloadPeriod(int year, int month, int day, int startHour, int endHour);
};