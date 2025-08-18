#pragma once

#include <string>
#include <vector>

// Forward declaration to avoid circular include dependency
class Circuit;

class FileManager {
public:
    FileManager();
    ~FileManager();

    // تابع برای ذخیره کردن وضعیت فعلی مدار در یک فایل
    void saveCircuit(const Circuit* circuit, const std::string& filename) const;
    void loadCircuit(Circuit* circuit, const std::string& filename);
    void listFiles() const;
    void loadCircuitByIndex(Circuit* circuit, int index);

private:
    std::vector<std::string> lastListedFiles;
};