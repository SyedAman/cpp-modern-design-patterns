#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct Journal
{
    std::string title;
    std::vector<std::string> entries;

    Journal() = default;

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

    friend std::istream& operator>>(std::istream& is, Journal& journal)
    {
        std::string title;
        is >> title;
        // Get the part after the colon in the first line.
        title = title.substr(title.find(':') + 1);
        // Ignore the newline character after the colon.
        // title.pop_back();
        journal.title = title;

        std::string line;
        while (std::getline(is, line))
        {
            journal.entries.push_back(line);
        }
        return is;
    }

    void save(const std::string& filename)
    {
        std::ofstream ofs(filename);
        ofs << *this;
    }

    void load(const std::string& filename)
    {
        std::ifstream ifs(filename);
        ifs >> *this;
    }
};

struct BetterJournal
{
    std::string title;
    std::vector<std::string> entries;

    BetterJournal() = default;

    BetterJournal(const std::string& title) : title(title) {}

    void add_entry(const std::string& entry)
    {
        static int count = 1;
        entries.push_back(std::to_string(count++) + ": " + entry);
    }

    friend std::ostream& operator<<(std::ostream& os, const BetterJournal& journal)
    {
        os << "Title: " << journal.title << "\n";
        for (const std::string& entry : journal.entries)
        {
            os << entry << "\n";
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, BetterJournal& journal)
    {
        // Get the part after the colon in the first line as title.
        std::string title;
        is >> title;
        journal.title = title.substr(title.find(':') + 1);

        std::string line;
        while (std::getline(is, line))
        {
            journal.entries.push_back(line);
        }
        return is;
    }
};

struct PersistenceManager
{
    static void save(const BetterJournal& journal, const std::string& filename)
    {
        std::ofstream ofs(filename);
        ofs << journal;
    }

    static void load(BetterJournal& journal, const std::string& filename)
    {
        std::ifstream ifs(filename);
        ifs >> journal;
    }
};

int main()
{
    BetterJournal journal("Dear Diary");
    journal.add_entry("I ate a bug");
    journal.add_entry("I cried today");
    std::cout << journal << std::endl;

    PersistenceManager::save(journal, "diary.txt");

    BetterJournal journal2;
    PersistenceManager::load(journal2, "diary.txt");
    std::cout << journal2 << std::endl;

    return 0;
}