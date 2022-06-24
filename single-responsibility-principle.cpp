#include <iostream>
#include <string>
#include <vector>
#include <fstream>

struct Journal
{
    std::string title;
    std::vector<std::string> entries;

    Journal(const std::string& title) : title(title) {}

    void add_entry(const std::string& entry)
    {
        static int count = 1;
        entries.push_back(std::to_string(count++) + ": " + entry);
    }

    friend std::ostream& operator<<(std::ostream& os, const Journal& journal)
    {
        os << "Title: " << journal.title << "\n";
        for (const std::string& entry : journal.entries)
        {
            os << entry << "\n";
        }
        return os;
    }
};

struct PersistenceManager
{
    static void save(const Journal& journal, const std::string& filename)
    {
        std::ofstream ofs(filename);
        ofs << journal;
    }
};

int main()
{
    Journal journal("Dear Diary");
    journal.add_entry("I ate a bug");
    journal.add_entry("I cried today");
    std::cout << journal << std::endl;

    PersistenceManager::save(journal, "diary.txt");

    return 0;
}