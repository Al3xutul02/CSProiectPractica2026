#include "DataDownloader.h"
#include <iostream>
#include <cstdio>
#include <curl/curl.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <zlib.h>

DataDownloader::DataDownloader(const std::string& user, const std::string& pass)
    : ftpUrl("ftp://ftphsaf.meteoam.it/"), username(user), password(pass) {}

size_t DataDownloader::writeCallback(void* ptr, size_t size, size_t nmemb, void* stream) {
    return fwrite(ptr, size, nmemb, (FILE*)stream);
}

bool DataDownloader::decompressGz(const std::string& gzFilePath, const std::string& outFilePath) {
    gzFile inFile = gzopen(gzFilePath.c_str(), "rb");
    if (!inFile) return false;

    FILE* outFile = fopen(outFilePath.c_str(), "wb");
    if (!outFile) {
        gzclose(inFile);
        return false;
    }

    char buffer[131072];
    int bytesRead = 0;
    while ((bytesRead = gzread(inFile, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytesRead, outFile);
    }

    fclose(outFile);
    gzclose(inFile);
    return (bytesRead >= 0);
}

bool DataDownloader::downloadFile(const std::string& remoteFileName, const std::string& localSavePath) {
    // Scoatem extensia .gz pentru a verifica și genera fișierul final .nc
    std::string unzippedPath = localSavePath.substr(0, localSavePath.find_last_of('.'));

    // Dacă fișierul e deja extras, sărim peste download
    std::ifstream checkFinalFile(unzippedPath);
    if (checkFinalFile.good()) {
        std::cout << "[SKIP] " << unzippedPath << " exista deja local." << std::endl;
        return true;
    }
    checkFinalFile.close();

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string fullUrl = ftpUrl + "/" + remoteFileName;
    FILE* fpage = fopen(localSavePath.c_str(), "wb");
    if (!fpage) {
        curl_easy_cleanup(curl);
        return false;
    }

    // Configurare cURL stabilă pentru descărcări succesive rapide pe FTP
    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpage);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 45L);

    std::cout << "[FTP] Descarcare: " << localSavePath << " ... " << std::flush;
    CURLcode res = curl_easy_perform(curl);
    fclose(fpage);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cout << "ESUAT" << std::endl;
        std::remove(localSavePath.c_str());
        return false;
    }

    std::cout << "OK -> [ZLIB] Extract ... " << std::flush;
    if (decompressGz(localSavePath, unzippedPath)) {
        std::cout << "DONE" << std::endl;
        std::remove(localSavePath.c_str()); // Păstrăm doar .nc-ul ca să economisim spațiu
        return true;
    } else {
        std::cout << "EROARE_DECOMPRESIE" << std::endl;
        return false;
    }
}

void DataDownloader::downloadPeriod(int year, int month, int day, int startHour, int endHour) {
    std::cout << "\n[BATCH] Pornire descarcari pentru: " << day << "/" << month << "/" << year << std::endl;
    int minutes[] = {0, 15, 30, 45};

    for (int hour = startHour; hour <= endHour; ++hour) {
        for (int min : minutes) {
            if (hour == endHour && min > 0) break;

            // Construim denumirea standardizată cerută de formatul H60
            std::stringstream ss;
            ss << "h60_" << year
               << std::setw(2) << std::setfill('0') << month
               << std::setw(2) << std::setfill('0') << day << "_"
               << std::setw(2) << std::setfill('0') << hour
               << std::setw(2) << std::setfill('0') << min << "_fdk.nc.gz";

            std::string fileName = ss.str();
            std::string remotePath = "h60/h60_cur_mon_data/" + fileName;

            downloadFile(remotePath, fileName);
        }
    }
}