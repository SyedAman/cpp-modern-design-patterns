#include <stdexcept>
#include <iostream>

struct Document {};

class IMulftiFunctionPrinter
{
public:
    virtual void print(Document& doc) const = 0;
    virtual void scan(Document& doc) const = 0;
    virtual void fax(Document& doc) const = 0;
};

// Violation of interface segregation principle as this class is forced to implement all the methods of IMultiFunctionPrinter.
class ScannerWithMissingFunctionality : public IMulftiFunctionPrinter
{
public:
    void print(Document& doc) const override
    {
        throw std::runtime_error("Not supported");
    }
    void scan(Document& doc) const override
    {
        std::cout << "Scanning document" << std::endl;
    }
    void fax(Document& doc) const override
    {
        throw std::runtime_error("Not supported");
    }
};

class IScanner
{
public:
    virtual void scan(Document& doc) const = 0;
};

class IPrinter
{
public:
    virtual void print(Document& doc) const = 0;
};

class IFax
{
public:
    virtual void fax(Document& doc) const = 0;
};

// Compliant with interface segregation principle as it only inherits the methods it needs.
class ProperScanner : public IScanner
{
public:
    void scan(Document& doc) const override
    {
        std::cout << "Scanning document" << std::endl;
    }
};

class IMachine : public IScanner, public IPrinter {};

// Decorator pattern.
class Machine : public IMachine
{
    IPrinter& Printer;
    IScanner& Scanner;

    Machine(IPrinter& printer, IScanner& scanner)
        : Printer(printer), Scanner(scanner)
    {}

    void print(Document& doc) const override
    {
        Printer.print(doc);
    }

    void scan(Document& doc) const override
    {
        Scanner.scan(doc);
    }
};


int main()
{
    Document doc;
    ProperScanner scanner = ProperScanner();
    scanner.scan(doc);
    return 0;
}