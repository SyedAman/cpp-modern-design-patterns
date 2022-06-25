#include <string>
#include <vector>
#include <iostream>

template <typename T> struct Criteria;
// template <typename T> struct AndCriteria : public Criteria<T>;

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

template <typename T> struct AndCriteria : public Criteria<T>
{
    const Criteria<T>& left;
    const Criteria<T>& right;

    AndCriteria(const Criteria<T>& left, const Criteria<T>& right)
        : left(left), right(right)
    {}

    bool is_satisfied(const T& product) const override
    {
        return left.is_satisfied(product) && right.is_satisfied(product);
    }
};

template <typename T> struct Criteria
{
    virtual bool is_satisfied(const T&) const = 0;

    AndCriteria<T> operator && (const Criteria& other) const
    {
        return AndCriteria<T>(*this, other);
    } 
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

struct BetterProductFilter : Filter<Product>
{
    std::vector<Product*> by_criteria(const std::vector<Product*> items, Criteria<Product>& criteria) const override
    {
        return Filter<Product>::by_criteria(items, criteria);
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

    std::cout << "Filtering without OCP..." << std::endl;
    std::vector<Product*> green_products = ProductFilter::by_color(products, Color::green);
    for (Product* p : green_products)
        std::cout << p->name << " is green." << std::endl;

    std::vector<Product*> large_products = ProductFilter::by_size(products, Size::large);
    for (Product* p : large_products)
        std::cout << p->name << " is large." << std::endl;

    std::vector<Product*> green_large_products = ProductFilter::by_size_and_color(products, Size::small, Color::green);
    for (Product* p : green_large_products)
        std::cout << p->name << " is green and large." << std::endl;

    std::cout << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    std::cout << "Filtering with OCP..." << std::endl;
    BetterProductFilter bf;
    ProductColorCriteria productColorCriteria = ProductColorCriteria(Color::green);
    green_products = bf.by_criteria(products, productColorCriteria);
    for (Product* p : green_products)
        std::cout << p->name << " is green." << std::endl;

    ProductSizeCriteria productSizeCriteria = ProductSizeCriteria(Size::large);
    large_products = bf.by_criteria(products, productSizeCriteria);
    for (Product* p : large_products)
        std::cout << p->name << " is large." << std::endl;
    
    AndCriteria<Product> productSizeAndColorCriteria = ProductSizeCriteria(Size::small) && ProductColorCriteria(Color::green);
    green_large_products = bf.by_criteria(products, productSizeAndColorCriteria);
    for (Product* p : green_large_products)
        std::cout << p->name << " is green and large." << std::endl;

    return 0;
}