#include <string>
#include <vector>
#include <iostream>

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product
{
    std::string name;
    Color color;
    Size size;
};

struct ProductFilter
{
    virtual ~ProductFilter() = default;

    static std::vector<Product*> by_color(const std::vector<Product*> products, Color color)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->color == color)
                result.push_back(p);
        return result;
    }

    static std::vector<Product*> by_size(const std::vector<Product*> products, Size size)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->size == size)
                result.push_back(p);
        return result;
    }

    static std::vector<Product*> by_size_and_color(const std::vector<Product*> products, Size size, Color color)
    {
        std::vector<Product*> result;
        for (auto& p : products)
            if (p->size == size && p->color == color)
                result.push_back(p);
        return result;
    }
};

template <typename T> struct Criteria
{
    virtual bool is_satisfied(const T&) const = 0;
};

template <typename T> struct Filter
{
    virtual std::vector<T*> by_criteria(const std::vector<T*> items, Criteria<T>& criteria) const
    {
        std::vector<T*> result;
        for (auto& item : items)
            if (criteria.is_satisfied(*item))
                result.push_back(item);
        return result;
    }
};

struct ProductColorCriteria : Criteria<Product>
{
    Color color;
    ProductColorCriteria(Color color) : color(color) {}
    bool is_satisfied(const Product& product) const override
    {
        return product.color == color;
    }
};

struct ProductSizeCriteria : Criteria<Product>
{
    Size size;
    ProductSizeCriteria(Size size) : size(size) {}
    bool is_satisfied(const Product& product) const override
    {
        return product.size == size;
    }
};

int main()
{
    Product bread{"Bread", Color::red, Size::small};
    Product milk{"Milk", Color::green, Size::small};
    Product water{"Water", Color::blue, Size::large};
    Product coffee{"Coffee", Color::green, Size::medium};
    Product tea{"Tea", Color::green, Size::small};
    Product juice{"Juice", Color::blue, Size::medium};
    Product cherry{"Cherries", Color::red, Size::large};
    std::vector<Product*> products{&bread, &milk, &water, &coffee, &tea, &juice, &cherry};

    // auto green_products = ProductFilter::by_color(products, Color::green);
    // for (auto& p : green_products)
    //     std::cout << p->name << std::endl;

    // auto large_products = ProductFilter::by_size(products, Size::large);
    // for (auto& p : large_products)
    //     std::cout << p->name << std::endl;

    // auto green_large_products = ProductFilter::by_size_and_color(products, Size::small, Color::green);
    // for (auto& p : green_large_products)
    //     std::cout << p->name << std::endl;

    ProductColorCriteria productColorCriteria = ProductColorCriteria(Color::green);
    auto green_products = Filter<Product>().by_criteria(products, productColorCriteria);
    for (auto& p : green_products)
        std::cout << p->name << std::endl;

    return 0;
}