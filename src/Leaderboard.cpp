// Leaderboard.cpp
#include "Leaderboard.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

wxString Leaderboard::escapeJsonString(const wxString& s)
{
    wxString out;
    for (size_t i = 0; i < s.length(); ++i) {
        wxChar c = s[i];
        switch (c) {
        case '\"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b"; break;
        case '\f': out += "\\f"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            if (c < 0x20) {
                wxString hex;
                hex.Printf("\\u%04x", (int)c);
                out += hex;
            } else {
                out += c;
            }
        }
    }
    return out;
}

void Leaderboard::SaveResultToJson(const wxString& name, double timeSeconds, int levelIndex)
{
    const char* filename = "assets/leaderboard.json";
    

    std::ostringstream entry;
    entry << "{";
    entry << "\"name\":\"" << std::string(escapeJsonString(name).mb_str()) << "\",";
    entry.setf(std::ios::fixed); entry << std::setprecision(3);
    entry << "\"time\":" << timeSeconds << ",";
    entry << "\"level\":" << levelIndex + 1;
    entry << "}";

    std::ifstream inFile(filename);
    if (!inFile.good()) {
        wxLogMessage("Failed to open leaderboard.json in assets folder!");
    }
    std::string content;
    if (inFile.good()) {
        std::ostringstream ss;
        ss << inFile.rdbuf();
        content = ss.str();
    }
    inFile.close();

    std::ofstream outFile(filename, std::ios::out | std::ios::trunc);
    if (content.empty()) {
        outFile << "[" << entry.str() << "]\n";
    } else {
        size_t pos = content.find_last_of(']');
        if (pos == std::string::npos) {
            outFile << "[" << entry.str() << "]\n";
        } else {
            std::string head = content.substr(0, pos);
            while (!head.empty() && isspace((unsigned char)head.back())) head.pop_back();
            if (head.back() == '[')
                outFile << head << entry.str() << "]";
            else
                outFile << head << "," << entry.str() << "]";
            outFile << "\n";
        }
    }
    outFile.close();
}

std::vector<LeaderboardEntry> Leaderboard::ReadLeaderboardEntries(int filterLevel)
{
    filterLevel += 1; // adjust for storage
    std::ifstream inFile("assets/leaderboard.json");
    if (!inFile.good()) return {};

    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    std::vector<LeaderboardEntry> entries;
    size_t pos = 0;
    while ((pos = content.find("{", pos)) != std::string::npos) {
        size_t end = content.find("}", pos);
        if (end == std::string::npos) break;

        std::string entryStr = content.substr(pos + 1, end - pos - 1);
        LeaderboardEntry e;
        e.name = "unknown";
        e.time = 0;
        e.level = 0;

        // parse name
        size_t n1 = entryStr.find("\"name\":\"");
        if (n1 != std::string::npos) {
            n1 += 8;
            size_t n2 = entryStr.find("\"", n1);
            if (n2 != std::string::npos) e.name = wxString(entryStr.substr(n1, n2 - n1));
        }

        // parse time
        size_t t1 = entryStr.find("\"time\":");
        if (t1 != std::string::npos) {
            t1 += 7;
            size_t t2 = entryStr.find(",", t1);
            if (t2 == std::string::npos) t2 = entryStr.length();
            e.time = std::stod(entryStr.substr(t1, t2 - t1));
        }

        // parse level
        size_t l1 = entryStr.find("\"level\":");
        if (l1 != std::string::npos) {
            l1 += 8;
            size_t l2 = entryStr.find(",", l1);
            if (l2 == std::string::npos) l2 = entryStr.length();
            e.level = std::stoi(entryStr.substr(l1, l2 - l1));
        }

        if (filterLevel == -1 || e.level == filterLevel)
            entries.push_back(e);

        pos = end + 1;
    }

    // sort ascending by time
    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
        return a.time < b.time;
    });

    return entries;
}


wxString Leaderboard::ReadLeaderboardDisplay(int levelIndex, const wxString& currentPlayer)
{
    auto entries = ReadLeaderboardEntries(levelIndex);

    if (entries.empty()) return "No entries yet.\n";

    wxString display;
    display << wxString::Format("Leaderboard for Level %d:\n\n", levelIndex + 1);

    for (const auto& e : entries) {
        if (e.name == currentPlayer) {
            // Mark current player with a special marker (we’ll color it later in the control)
            display << wxString::Format("[YOU] %s : %.3f sec\n", e.name, e.time);
        } else {
            display << wxString::Format("%s : %.3f sec\n", e.name, e.time);
        }
    }

    return display;
}
